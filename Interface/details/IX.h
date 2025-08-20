#pragma once

#include "IUnknownRe.h"

struct  IX : public IUnknownReplica
{
public:
    static constexpr auto iid = *uuids::uuid::from_string("32bb8320-b41b-11cf-a6bb-0080c7b2d682");
    virtual int32_t Fx( void) noexcept = 0;
};
