#pragma once

#include "../Deps/dllhelper/dllhelper.hpp"
#include "IFace.h"
#include <comdef.h>

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
	template<typename Interface, typename Method>
	HRESULT CallInterfaceMethod(Method method) const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<Interface, &__uuidof(Interface)>> ptr(m_pIUnknown);
			if(ptr) {
				(ptr->*method)();
				return S_OK;
			}
			return E_NOTIMPL;
		}
		catch(const _com_error& e) {
			return e.Error();
		}
	}
public:
	HRESULT Fx() const noexcept
	{
		return CallInterfaceMethod<IX>(&IX::Fx);
	}
	HRESULT Fy() const noexcept
	{
		return CallInterfaceMethod<IY>(&IY::Fy);
	}
	HRESULT Fz() const noexcept
	{
		return CallInterfaceMethod<IZ>(&IZ::Fz);
	}
private:
	DllHelper m_dll;
	
	_com_ptr_t<_com_IIID<IUnknown, &IID_IUnknown>> m_pIUnknown{};
};