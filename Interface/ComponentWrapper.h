#pragma once

#include "../Deps/dllhelper/dllhelper.hpp"
#include "IFace.h"
#include <comdef.h>
#include <string>

class ComponentWrapper
{
public:
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

		m_pIUnknown.Attach(create());
	}
	operator bool() const noexcept
	{
		return m_pIUnknown;
	}
private:
	template<typename Interface, typename Ret, typename F = HRESULT(*)(void)>
	HRESULT CallInterfaceMethod(Ret(Interface::* method)() const, F handle_result = []() {return S_OK; }) const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<Interface, &__uuidof(Interface)>> ptr(m_pIUnknown);
			if(ptr) {
				if constexpr(std::is_void_v<Ret>) {
					(ptr->*method)();
					return handle_result();
				}
				else {
					auto result = (ptr->*method)();
					return handle_result(result);
				}
			}
			return E_NOTIMPL;
		}
		catch(const _com_error& e) {
			return e.Error();
		}
		catch(std::bad_alloc&) {
			return E_OUTOFMEMORY;
		}
		catch(...) {
			return E_FAIL;
		}
	}
public:
	HRESULT Fx() const noexcept
	{
		return CallInterfaceMethod<IX, void>(&IX::Fx);
	}
	HRESULT Fy() const noexcept
	{
		return CallInterfaceMethod<IY, void>(&IY::Fy);
	}
	HRESULT Fz() const noexcept
	{
		return CallInterfaceMethod<IZ, void>(&IZ::Fz);
	}
	HRESULT GetVersion(std::wstring& version) const noexcept
	{
		return CallInterfaceMethod<IX2, BSTR>(&IX2::GetVersion, [&version](BSTR& result) {
			if(result && ::SysStringLen(result) > 0) {
				version = std::wstring(result, ::SysStringLen(result));
				::SysFreeString(result);
				return S_OK;
			}
			return S_FALSE;
		});
	}

private:
	DllHelper m_dll;
	
	_com_ptr_t<_com_IIID<IUnknown, &IID_IUnknown>> m_pIUnknown{};
};