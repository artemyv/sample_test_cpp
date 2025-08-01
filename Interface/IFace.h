#pragma once

#include <unknwn.h>
//
// Iface.h
//
// Интерфейсы

interface __declspec(uuid("32bb8320-b41b-11cf-a6bb-0080c7b2d682")) IX : public IUnknown
{
	virtual void Fx() const = 0;
};
interface __declspec(uuid("32bb8323-b41b-11cf-a6bb-0080c7b2d682")) IX2 : public IX
{
	virtual BSTR GetVersion() const = 0;
};
interface __declspec(uuid("32bb8321-b41b-11cf-a6bb-0080c7b2d682")) IY : public IUnknown
{
	virtual void __stdcall Fy() const = 0;
};
interface __declspec(uuid("32bb8322-b41b-11cf-a6bb-0080c7b2d682")) IZ : public IUnknown
{
	virtual void __stdcall Fz() const = 0;
};

extern "C" IUnknown* CreateInstance();