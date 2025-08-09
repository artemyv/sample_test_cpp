#pragma once

#include <uuid.h>

struct IUnknownReplica
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("00000000-0000-0000-C000-000000000046");
    virtual int32_t QueryInterface(
        /* [in] */ const uuids::uuid& riid,
        /* [iid_is][out] */ void ** ppvObject) noexcept = 0;

    virtual unsigned long  AddRef(void) noexcept = 0;

    virtual unsigned long  Release(void) noexcept = 0;

    template<class Q>
    int32_t QueryInterface(Q** pp) noexcept
    {
        return QueryInterface(Q::iid, (void**)pp);
    }
};
