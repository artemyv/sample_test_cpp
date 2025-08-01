#pragma once

#include <unknwn.h>
//
// Iface.h
//
// Интерфейсы

interface __declspec(uuid("32bb8320-b41b-11cf-a6bb-0080c7b2d682")) IX : public IUnknown
{
	virtual void Fx() = 0;
};
interface __declspec(uuid("32bb8321-b41b-11cf-a6bb-0080c7b2d682")) IY : public IUnknown
{
	virtual void __stdcall Fy() = 0;
};
interface __declspec(uuid("32bb8322-b41b-11cf-a6bb-0080c7b2d682")) IZ : public IUnknown
{
	virtual void __stdcall Fz() = 0;
};

extern "C" IUnknown* CreateInstance();