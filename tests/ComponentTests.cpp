#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <atomic>
#include <cstdlib>
#include <new>
#include <details/IFace.h>
// -------------------------
// Test-only allocation fault injection
// -------------------------
namespace
{
    struct FailAllocGuard
    {
        explicit FailAllocGuard(int failCount = 1)
        {
            g_failRemaining.store(failCount, std::memory_order_relaxed);
            g_failAlloc.store(true, std::memory_order_release);
        }
        ~FailAllocGuard()
        {
            g_failAlloc.store(false, std::memory_order_release);
            g_failRemaining.store(-1, std::memory_order_relaxed);
        }
        static std::atomic<bool>& fail()
        {
            return g_failAlloc;
        }
        static std::atomic<int>& remaining()
        {
            return g_failRemaining;
        }

    private:
        inline static std::atomic g_failAlloc{false};
        inline static std::atomic g_failRemaining{-1}; // -1 means unlimited while enabled
    };
    // Helper to decide whether to fail this allocation
    inline bool should_fail(std::size_t n) noexcept
    {
        if(!FailAllocGuard::fail().load(std::memory_order_relaxed)) return false;
        // Target small allocations (Component::GetVersion allocates length of version + 1 = 6)
        if(n <= 32) {
            int rem = FailAllocGuard::remaining().load(std::memory_order_relaxed);
            if(rem == -1 || rem > 0) {
                if(rem > 0) {
                    FailAllocGuard::remaining().store(rem - 1, std::memory_order_relaxed);
                }
                return true;
            }
        }
        return false;
    }
}

// Override global new/delete in test executable (affects shared lib allocations too)
void* operator new(std::size_t n)
{
    if(should_fail(n)) throw std::bad_alloc();
    if(void* p = std::malloc(n)) return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t n)
{
    if(should_fail(n)) throw std::bad_alloc();
    if(void* p = std::malloc(n)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void operator delete[](void* p, std::size_t) noexcept { std::free(p); }

extern "C" ComponentAPI::IUnknownReplica* CreateInstance();

namespace BadAllocTests
{
    class ComponentTest: public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            pRoot = CreateInstance();
            EXPECT_NE(pRoot, nullptr);
        }
        int32_t GetVersion(std::string& version)
        {
            // Query IX2
            if(pIX2 == nullptr) {
                pRoot->QueryInterface<ComponentAPI::IX2>(&pIX2);
            }
            EXPECT_NE(pIX2, nullptr);
            // Call GetVersion with nullptr to hit the guarded branch
            const char* result = nullptr;
            if(const auto ec = pIX2->GetVersion(&result); ec != 0) {
                std::fputs("GetVersion failed\n", stdout);

                return ec;
            }
            std::fputs("GetVersion succeeded ", stdout);
            if(result) {
                std::fputs(result, stdout);
                version = std::string(result);
                pIX2->FreeResult(result);
            }
            std::fputs("\n", stdout);
            return 0;
        }
        int32_t GenerateRandomNumbers(std::string& numbers)
        {
            // Query IX2
            if(pIRandom == nullptr) {
                int32_t qiRet = pRoot->QueryInterface<ComponentAPI::IRandom>(&pIRandom);
                EXPECT_EQ(qiRet, 0);
                EXPECT_NE(pIRandom, nullptr);
            }
            // Call GetVersion with nullptr to hit the guarded branch
            const char* result = nullptr;
            if(const auto ec = pIRandom->GenerateRandomNumbers(1u, &result); ec != 0) {
                return ec;
            }
            if(result) {
                numbers = std::string(result);
                pIRandom->FreeResult(result);
            }
            return 0;
        }

        void TearDown() override 
        {
            if(pRoot)
                pRoot->Release();
            if(pIX2)
                pIX2->Release();
            if(pIRandom)
                pIRandom->Release();
        }
    private:
        ComponentAPI::IUnknownReplica* pRoot = nullptr;
        ComponentAPI::IX2* pIX2 = nullptr;
        ComponentAPI::IRandom* pIRandom = nullptr;
    };


    TEST_F(ComponentTest, GetVersionBadAllocReturnsNotEnoughMemory)
    {
        std::string version = "unchanged";
        std::fputs("Calling GetVersion with limited memory conditions\n", stdout);

        auto ec = [&version, this]() {
            FailAllocGuard guard(/*failCount=*/1); // Fail first small allocation
            return GetVersion(version);
            }();

        EXPECT_EQ(ec, (decltype(ec))std::errc::not_enough_memory)
            << "Expected mapping of bad_alloc to not_enough_memory";
        EXPECT_EQ(version, "unchanged") << "Version string should not be modified on failure";

        // Sanity: subsequent call (without failing alloc) should succeed
        std::fputs("Calling GetVersion with standard memory conditions\n", stdout);
        std::string version2;
        auto ec2 = GetVersion(version2);
        EXPECT_TRUE(!ec2);
        EXPECT_FALSE(version2.empty());
    }
    TEST_F(ComponentTest, GetRandomBadAllocReturnsNotEnoughMemory)
    {
        std::string numbers = "unchanged";
        auto ec = [&numbers, this]() {
            FailAllocGuard guard(/*failCount=*/1); // Fail first small allocation
            return GenerateRandomNumbers(numbers);
            }();

        EXPECT_EQ(ec, (decltype(ec))std::errc::not_enough_memory)
            << "Expected mapping of bad_alloc to not_enough_memory";
        EXPECT_EQ(numbers, "unchanged") << "Numbers string should not be modified on failure";

        // Sanity: subsequent call (without failing alloc) should succeed
        std::string numbers2;
        auto ec2 = GenerateRandomNumbers(numbers2);
        EXPECT_TRUE(!ec2);
        EXPECT_FALSE(numbers2.empty());
    }
}