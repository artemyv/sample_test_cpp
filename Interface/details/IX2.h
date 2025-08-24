#pragma once

#include "IX.h"
namespace ComponentAPI
{
    struct IX2: public IX
    {
    public:
        static constexpr auto iid = *uuids::uuid::from_string("32bb8323-b41b-11cf-a6bb-0080c7b2d682");
        virtual int32_t GetVersion(const char** version) noexcept = 0;
        virtual int32_t FreeResult(const char* version) noexcept = 0;
    protected:
        ~IX2() = default;
    };
}
