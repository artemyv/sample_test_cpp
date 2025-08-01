#pragma once

#include "../Deps/dllhelper/dllhelper.hpp"
#include "IFace.h"

class ComponentWrapper
{
public:
	ComponentWrapper() = delete;
	ComponentWrapper(const ComponentWrapper&) = delete; // Copy constructor
	ComponentWrapper& operator=(const ComponentWrapper&) = delete; // Copy assignment
	ComponentWrapper(ComponentWrapper&&) = delete; // Move constructor
	ComponentWrapper& operator=(ComponentWrapper&&) = delete; // Move assignment
	~ComponentWrapper() = default;
	explicit ComponentWrapper(const wchar_t* path) :
		m_dll(path)
	{
		if(m_dll) {
			m_create = m_dll["CreateInstance"];
		}
	}
	IUnknown* CreateInstance() noexcept {
		if(m_create)
			return m_create();
		return nullptr;
	}
private:
	DllHelper m_dll;
	decltype(::CreateInstance)* m_create{};
};