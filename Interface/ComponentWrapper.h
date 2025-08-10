#pragma once

#include <dllhelper.hpp>
#include "details/IFace.h"
#include "InterfaceWrapperBase.h"

#include <string>
#include <system_error>
#include <filesystem>

extern "C" IUnknownReplica* CreateInstance();

namespace ComponentWrapper
{
	inline auto result_of(int32_t ret)
	{
		return std::make_error_code(static_cast<std::errc>(ret));
	}

	class ComponentWrapper
	{
	public:
		explicit ComponentWrapper(const std::filesystem::path& path):
			m_dll(path),
			m_create{m_dll[gsl::not_null<gsl::czstring>("CreateInstance")]},
			m_pIUnknown(m_create())
		{}
		explicit ComponentWrapper(auto p) = delete; // Prevent implicit conversion from other types
	private:
	public:
		template <typename F>
		std::error_code expect(F&& f) const noexcept
		{
			try {
				return f();
			}
			catch(const std::system_error& e) {
				return e.code();
			}
			catch(...) {
				return std::make_error_code(std::errc::not_supported);
			}
		}

		std::error_code Fx() const noexcept
		{
			return expect([&pI = m_pIUnknown]() {
				auto i = pI.QueryInterface<IX>();
				return i.CallInterfaceMethod([](IX* ptr) {
					return result_of(ptr->Fx());
				});
			});
		}
		std::error_code Fy() const noexcept
		{
			return expect([&pI = m_pIUnknown]() {
				auto i = pI.QueryInterface<IY>();
				return i.CallInterfaceMethod([](IY* ptr) {
					return result_of(ptr->Fy());
				});
			});
		}
		std::error_code GetVersion(std::string& version) const noexcept
		{
			return expect([&pI = m_pIUnknown, &version]() {
				auto i = pI.QueryInterface<IX2>();
				return i.CallInterfaceMethod([&version](IX2* ptr) {
					const char* result = nullptr;
					const auto ec = result_of(ptr->GetVersion(&result));
					if(ec) {
						return ec;
					}
					if(result) {
						version = std::string(result);
						ptr->FreeResult(result);
					}
					return std::error_code{};
				});
			});
		}
		std::error_code  GenerateRandomNumbers(size_t count, std::string& numbers_json) const noexcept
		{
			return expect([&pI = m_pIUnknown, count, &numbers_json]() {
				auto i = pI.QueryInterface<IRandom>();
				return i.CallInterfaceMethod([&numbers_json, count](IRandom* ptr) {
					const char* result = nullptr;
					const auto ec = result_of(ptr->GenerateRandomNumbers(count, &result));
					if(ec) {
						return ec;
					}
					if(result) {
						numbers_json = std::string(result);
						ptr->FreeResult(result);
					}
					return std::error_code{};
				});
			});
		}

	private:
		dll::Helper m_dll;
		dll::Fp<decltype(::CreateInstance)> m_create;
		InterfaceWrapperBase<IUnknownReplica> m_pIUnknown;
	};
}