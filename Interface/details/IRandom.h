#pragma once

#include "IUnknownRe.h"
namespace ComponentAPI
{
    struct IRandom: public IUnknownReplica
    {
    public:
        static constexpr auto iid = *uuids::uuid::from_string("32bb8324-b41b-11cf-a6bb-0080c7b2d682");
        virtual int32_t  GenerateRandomNumbers(size_t count, const char** numbers_json) noexcept = 0;
        virtual int32_t  FreeResult(char const* numbers_json) noexcept = 0;
    };
}
