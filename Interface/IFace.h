#pragma once

#include <uuid.h>

struct IUnknownReplica
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("00000000-0000-0000-C000-000000000046");
    virtual int  QueryInterface(
        /* [in] */ const uuids::uuid& riid,
        /* [iid_is][out] */ void ** ppvObject) noexcept = 0;

    virtual unsigned long  AddRef(void) noexcept = 0;

    virtual unsigned long  Release(void) noexcept = 0;

    template<class Q>
    int  QueryInterface(Q** pp) noexcept
    {
        return QueryInterface(Q::iid, (void**)pp);
    }
};
//MIDL_INTERFACE("32bb8320-b41b-11cf-a6bb-0080c7b2d682")
struct  IX : public IUnknownReplica
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("32bb8320-b41b-11cf-a6bb-0080c7b2d682");
    virtual int  Fx( void) noexcept = 0;
};
    
struct IX2 : public IX
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("32bb8323-b41b-11cf-a6bb-0080c7b2d682");
    virtual int  GetVersion(const char ** version) noexcept = 0;
	virtual int  FreeResult(const char *  version) noexcept = 0;
};
    
struct IRandom : public IUnknownReplica
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("32bb8324-b41b-11cf-a6bb-0080c7b2d682");
    virtual int  GenerateRandomNumbers( int count, const char** numbers_json) noexcept = 0;
    virtual int  FreeResult(char const* numbers_json) noexcept = 0;
};
