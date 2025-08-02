#pragma once

#include "../Deps/dllhelper/dllhelper.hpp"
#include "IFace.h"
#include "Create.h"
#include <string>
#include <memory>

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
	HRESULT CallInterfaceMethod(F&& call_method_result) const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		Interface* raw = nullptr;
		const auto hr = m_pIUnknown->QueryInterface(&raw);
		if(SUCCEEDED(hr) && raw) {
			unique_com_ptr<Interface> ptr{raw};
			return  std::invoke(std::forward<F>(call_method_result), ptr.get());
		}
		return E_NOTIMPL;
	}
public:
	HRESULT Fx() const noexcept
	{
		return CallInterfaceMethod<IX>([](IX* ptr) {if(ptr) return ptr->Fx(); return E_NOTIMPL; });
	}
	HRESULT GetVersion(std::string& version) const noexcept
	{
		return CallInterfaceMethod<IX2>([&version](IX2* ptr) {
			const char* result = nullptr;
			if(!ptr) {
				return E_NOTIMPL;
			}
			const auto hr = ptr->GetVersion(&result);
			if(FAILED(hr)) {
				return hr;
			}
			if(result)
			{
				version = std::string(result);
				ptr->FreeResult(result);
				return S_OK;
			}
			return S_FALSE;
		});
	}
	HRESULT GenerateRandomNumbers(int count, std::string& numbers_json) const noexcept
	{
		return CallInterfaceMethod<IRandom>([&numbers_json, count](IRandom* ptr) {
			if(!ptr) {
				return E_NOTIMPL;
			}
			const char* result = nullptr;
			const auto hr = ptr->GenerateRandomNumbers(count, &result);
			if(FAILED(hr)) {
				return hr;
			}
			if(result)
			{
				numbers_json = std::string(result);
				ptr->FreeResult(result);
				return S_OK;
			}
			return S_FALSE;
		});
	}

private:
	DllHelper m_dll;
	
	unique_com_ptr<IUnknownReplica> m_pIUnknown{};
};