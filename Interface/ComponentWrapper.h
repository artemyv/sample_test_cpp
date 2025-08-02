#pragma once

#include "../Deps/dllhelper/dllhelper.hpp"
#include "IFace.h"
#include "Create.h"
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
	template<typename Interface, typename ...Args, typename F = HRESULT(*)(HRESULT)>
	HRESULT CallInterfaceMethod(HRESULT(Interface::* method)(Args...), Args&& ...args, F handle_result = [](HRESULT hr) {return hr; }) const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<Interface, &__uuidof(Interface)>> ptr(m_pIUnknown);
			if(ptr) {
				auto result = (ptr->*method)(std::forward<Args>(args)...);
				return handle_result(result);
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
		return CallInterfaceMethod(&IX::Fx);
	}
	HRESULT Fy() const noexcept
	{
		return CallInterfaceMethod(&IY::Fy);
	}
	HRESULT Fz() const noexcept
	{
		return CallInterfaceMethod(&IZ::Fz);
	}
	HRESULT GetVersion(std::wstring& version) const noexcept
	{
		BSTR result = nullptr;
		return CallInterfaceMethod(&IX2::GetVersion, &result, [&result, &version](HRESULT hr) {
			if(FAILED(hr)) {
				return hr;
			}
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