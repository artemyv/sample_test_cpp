#pragma once

#include "IUnknownRe.h"
namespace ComponentAPI
{
    struct  IY: public IUnknownReplica
    {
    public:
        static constexpr auto iid = *uuids::uuid::from_string("32bb8321-b41b-11cf-a6bb-0080c7b2d682");
        virtual int32_t Fy(void) noexcept = 0;
    protected:
        ~IY() = default;
    };
}
