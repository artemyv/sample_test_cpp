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
	HRESULT Fx() const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try
		{
			_com_ptr_t<_com_IIID<IX, &__uuidof(IX)>> pIX(m_pIUnknown);
			if(pIX) {
				pIX->Fx();
				return S_OK;
			}
			return E_NOTIMPL;
		}
		catch (const _com_error& e)
		{
			return e.Error();
		}
	}
	HRESULT Fy() const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<IY, &__uuidof(IY)>> pIY(m_pIUnknown);
			if(pIY) {
				pIY->Fy();
				return S_OK;
			}
			return E_NOTIMPL;
		}
		catch(const _com_error& e) {
			return e.Error();
		}
	}
	HRESULT Fz() const noexcept
	{
		if(!m_pIUnknown) {
			return E_NOINTERFACE;
		}
		try {
			_com_ptr_t<_com_IIID<IZ, &__uuidof(IZ)>> pIZ(m_pIUnknown);
			if(pIZ) {
				pIZ->Fz();
				return S_OK;
			}
			return E_NOTIMPL;
		}
		catch(const _com_error& e) {
			return e.Error();
		}
	}
private:
	DllHelper m_dll;
	
	_com_ptr_t<_com_IIID<IUnknown, &IID_IUnknown>> m_pIUnknown{};
};