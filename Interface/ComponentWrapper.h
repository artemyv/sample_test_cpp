#pragma once

#include <dllhelper.hpp>
#include "details/IFace.h"
#include "InterfaceWrapper.h"

#include <string>
#include <system_error>
#include <filesystem>

extern "C" ComponentAPI::IUnknownReplica* CreateInstance();

namespace ComponentWrapper
{
	class ComponentWrapper
	{
	public:
		explicit ComponentWrapper(const std::filesystem::path& path):
			m_dll(path),
			m_pIUnknown(CreateInstanceofUnknown(m_dll))
		{}
		explicit ComponentWrapper(auto p) = delete; // Prevent implicit conversion from other types
		std::error_code Fx() const noexcept
		{
			return safecall([&pI = m_pIUnknown]() {
				auto i = pI.QueryInterface<ComponentAPI::IX>();
				return i->Fx();
			});
		}
		std::error_code Fy() const noexcept
		{
			return safecall([&pI = m_pIUnknown]() {
				auto i = pI.QueryInterface<ComponentAPI::IY>();
				return i->Fy();
			});
		}
		std::error_code GetVersion(std::string& version) const noexcept
		{
			return safecall([&pI = m_pIUnknown, &version]() {
				auto i = pI.QueryInterface<ComponentAPI::IX2>();
				const char* result = nullptr;
				
				if(const auto ec = i->GetVersion(&result); ec != 0) {
					return ec;
				}
				if(result) {
					version = std::string(result);
					i->FreeResult(result);
				}
				return 0;
			});
		}
		std::error_code  GenerateRandomNumbers(size_t count, std::string& numbers_json) const noexcept
		{
			return safecall([&pI = m_pIUnknown, count, &numbers_json]() {
			auto i = pI.QueryInterface<ComponentAPI::IRandom>();
				const char* result = nullptr;
				
				if(const auto ec = i->GenerateRandomNumbers(count, &result); ec != 0) {
					return ec;
				}
				if(result) {
					numbers_json = std::string(result);
					i->FreeResult(result);
				}
				return 0;
			});
		}	
	protected:	
		// forward-declare result_of so safecall can use it (deduced return types must be declared before use)
		inline static auto result_of(int32_t ret)
		{
			return std::make_error_code(static_cast<std::errc>(ret));
		}		template <typename F>
		static std::error_code safecall(F&& f)
		{
			try {
				return result_of(f());
			}
			catch(const std::system_error& e) {
				return e.code();
			}
			catch(const std::bad_alloc&) {
				return std::make_error_code(std::errc::not_enough_memory);
			}
		}
	private:

		static ComponentAPI::IUnknownReplica* CreateInstanceofUnknown(const dll::Helper& helper)
		{
			dll::Fp<decltype(::CreateInstance)> create{helper[dll::procname_t("CreateInstance")]};
			return create();
		}


	private:
		dll::Helper m_dll; //we do not use this member after initalizing m_pIUnknown, but it will keep the DLL loaded
		InterfaceWrapper<ComponentAPI::IUnknownReplica> m_pIUnknown;
	};
}