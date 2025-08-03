#pragma once

#include <dllhelper.hpp>
#include "IFace.h"
#include "Create.h"
#include <string>
#include <memory>
#include <system_error>
template<typename T>
struct ComReleaser
{
	void operator()(T* ptr) const noexcept
	{
		if(ptr) ptr->Release();
	}
};

class ComponentWrapper
{
public:
	template<typename T>
	using unique_com_ptr = std::unique_ptr<T, ComReleaser<T>>;

	ComponentWrapper() = delete;
	ComponentWrapper(const ComponentWrapper&) = delete; // Copy constructor
	ComponentWrapper& operator=(const ComponentWrapper&) = delete; // Copy assignment
	ComponentWrapper(ComponentWrapper&&) = delete; // Move constructor
	ComponentWrapper& operator=(ComponentWrapper&&) = delete; // Move assignment
	~ComponentWrapper() = default;
	explicit ComponentWrapper(const wchar_t* path) noexcept:
		m_dll(path)
	{
		if(!m_dll)
			return;

		decltype(::CreateInstance)* create = m_dll["CreateInstance"];
		if(!create)
			return;

		m_pIUnknown.reset(create());
	}
	operator bool() const noexcept
	{
		return m_pIUnknown.get() != nullptr;
	}
private:
	template<typename Interface, typename F >
	std::error_code CallInterfaceMethod(F&& call_method_result) const noexcept
	{
		if(!m_pIUnknown) {
			return std::make_error_code(std::errc::invalid_argument);
		}
		Interface* raw = nullptr;
		const std::error_code ec(m_pIUnknown->QueryInterface(&raw), std::system_category());
		if(!ec && raw) {
			unique_com_ptr<Interface> ptr{raw};
			return  std::invoke(std::forward<F>(call_method_result), ptr.get());
		}
		return ec;
	}
public:
	std::error_code Fx() const noexcept
	{
		return CallInterfaceMethod<IX>([](IX* ptr) 
			{
				if(ptr) 
					return std::error_code(ptr->Fx(), std::system_category()); 
				return std::make_error_code(std::errc::not_supported);
			});
	}
	std::error_code GetVersion(std::string& version) const noexcept
	{
		return CallInterfaceMethod<IX2>([&version](IX2* ptr) {
			const char* result = nullptr;
			if(!ptr) {
				return std::make_error_code(std::errc::invalid_argument);
			}
			const std::error_code ec(ptr->GetVersion(&result), std::system_category());
			if(ec) {
				return ec;
			}
			if(result)
			{
				version = std::string(result);
				ptr->FreeResult(result);
			}
			return std::error_code{};
		});
	}
	std::error_code  GenerateRandomNumbers(int count, std::string& numbers_json) const noexcept
	{
		return CallInterfaceMethod<IRandom>([&numbers_json, count](IRandom* ptr) {
			if(!ptr) {
				return std::make_error_code(std::errc::invalid_argument);
			}
			const char* result = nullptr;
			const std::error_code ec(ptr->GenerateRandomNumbers(count, &result), std::system_category());
			if(ec) {
				return ec;
			}
			if(result)
			{
				numbers_json = std::string(result);
				ptr->FreeResult(result);
			}
			return std::error_code{};
		});
	}

private:
	DllHelper m_dll;
	
	unique_com_ptr<IUnknownReplica> m_pIUnknown{};
};