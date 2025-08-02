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
	template<typename Interface, typename F >
	HRESULT CallInterfaceMethod(F&& call_method_result) const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<Interface, &__uuidof(Interface)>> ptr(m_pIUnknown);
			if(ptr) {
				return  std::invoke(std::forward<F>(call_method_result), ptr.GetInterfacePtr());
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
		return CallInterfaceMethod<IX>([](IX* ptr) {return ptr->Fx(); });
	}
	HRESULT Fy() const noexcept
	{
		return CallInterfaceMethod<IY>([](IY* ptr) {return ptr->Fy(); });
	}
	HRESULT Fz() const noexcept
	{
		return CallInterfaceMethod<IZ>([](IZ* ptr) {return ptr->Fz(); });
	}
	HRESULT GetVersion(std::wstring& version) const noexcept
	{
		return CallInterfaceMethod<IX2>([&version](IX2* ptr) {
			BSTR result = nullptr;
			auto hr = ptr->GetVersion(&result);
			if(FAILED(hr)) {
				return hr;
			}
			_bstr_t bstrResult(result, false); 
			if(bstrResult.length() > 0)
			{
				version = std::wstring(bstrResult.operator const wchar_t *(), bstrResult.length());
				return S_OK;
			}
			return S_FALSE;
		});
	}

private:
	DllHelper m_dll;
	
	_com_ptr_t<_com_IIID<IUnknown, &IID_IUnknown>> m_pIUnknown{};
};