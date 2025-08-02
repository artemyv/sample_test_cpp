#pragma once

#include <guiddef.h>

using HRESULT = long;
struct __declspec(uuid("00000000-0000-0000-C000-000000000046")) IUnknownReplica
{
public:
    virtual HRESULT __stdcall QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void ** ppvObject) noexcept = 0;

    virtual unsigned long __stdcall AddRef(void) noexcept = 0;

    virtual unsigned long __stdcall Release(void) noexcept = 0;

    template<class Q>
    HRESULT __stdcall QueryInterface(Q** pp) noexcept
    {
        return QueryInterface(__uuidof(Q), (void**)pp);
    }
};
//MIDL_INTERFACE("32bb8320-b41b-11cf-a6bb-0080c7b2d682")
struct __declspec(uuid("32bb8320-b41b-11cf-a6bb-0080c7b2d682")) IX : public IUnknownReplica
{
public:
    virtual HRESULT __stdcall Fx( void) noexcept = 0;
};
    
struct __declspec(uuid("32bb8323-b41b-11cf-a6bb-0080c7b2d682")) IX2 : public IX
{
public:
    virtual HRESULT __stdcall GetVersion(const char ** version) noexcept = 0;
	virtual HRESULT __stdcall FreeResult(const char *  version) noexcept = 0;
};
    
struct __declspec(uuid("32bb8324-b41b-11cf-a6bb-0080c7b2d682")) IRandom : public IUnknownReplica
{
public:
    virtual HRESULT __stdcall GenerateRandomNumbers( int count, const char** numbers_json) noexcept = 0;
    virtual HRESULT __stdcall FreeResult(char const* numbers_json) noexcept = 0;
};
