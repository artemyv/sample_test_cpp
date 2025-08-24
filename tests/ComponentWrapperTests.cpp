#include <gtest/gtest.h>
#define private public           // Expose safecall for white-box test
#include <ComponentWrapper.h>
#undef private
#include <string>
#include <filesystem>
#include <dllhelper.hpp>
#include <details/IFace.h>

namespace ComponentWrapperTests
{
    class ComponentWrapperTest: public ::testing::Test
    {
        std::filesystem::path dllPath;

    protected:
        const std::filesystem::path& GetDllPath() const
        {
            return dllPath;
        }
        void SetUp() override
        {
            // Set the path to your built Component.dll here
            dllPath = std::filesystem::absolute(COMPONENT_TARGET_NAME);
        }
    };

    TEST_F(ComponentWrapperTest, FxReturnsNoError)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        auto ec = wrapper.Fx();
        EXPECT_TRUE(!ec);
    }

    TEST_F(ComponentWrapperTest, GetVersionReturnsVersionString)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        std::string version;
        auto ec = wrapper.GetVersion(version);
        EXPECT_TRUE(!ec);
        EXPECT_FALSE(version.empty());
    }

    TEST_F(ComponentWrapperTest, GenerateRandomNumbersReturnsJson)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        std::string numbers_json;
        auto ec = wrapper.GenerateRandomNumbers(5, numbers_json);
        EXPECT_TRUE(!ec);
        EXPECT_NE(numbers_json.find("\"numbers\":"), std::string::npos);
    }

    TEST_F(ComponentWrapperTest, FyReturnsError)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        auto ec = wrapper.Fy();
        auto expected = std::make_error_code(std::errc::operation_not_supported);
        EXPECT_EQ(ec, expected);
    }

    class BadComponentWrapperTest: public ::testing::Test
    {
        std::filesystem::path dllPath;

    protected:
        const std::filesystem::path& GetDllPath() const
        {
            return dllPath;
        }
        void SetUp() override
        {
            // Set the path to your built Component.dll here
            dllPath = std::filesystem::absolute(BAD_COMPONENT_TARGET_NAME);
        }
    };

    TEST_F(BadComponentWrapperTest, FxReturnsNoError)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        auto ec = wrapper.Fx();
        EXPECT_TRUE(!ec);
    }

    TEST_F(BadComponentWrapperTest, GetVersionReturnsVersionString)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        std::string version;
        auto ec = wrapper.GetVersion(version);
        auto expected = std::make_error_code(std::errc::not_enough_memory);
        EXPECT_EQ(ec, expected);
    }

    TEST_F(BadComponentWrapperTest, GenerateRandomNumbersReturnsJson)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        std::string numbers_json;
        auto ec = wrapper.GenerateRandomNumbers(5, numbers_json);
        auto expected = std::make_error_code(std::errc::not_enough_memory);
        EXPECT_EQ(ec, expected);
    }

    TEST_F(BadComponentWrapperTest, FyIsCalled)
    {
        ComponentWrapper::ComponentWrapper wrapper(GetDllPath());
        auto ec = wrapper.Fy();
        EXPECT_TRUE(!ec);
    }

    TEST(SafeCallTest, SafecallCatchesBadAlloc)
    {
        // Directly invoke private static safecall (temporarily made public via macro)
        auto ec = ComponentWrapper::ComponentWrapper::safecall([]() {
            throw std::bad_alloc();
            return 0;
            });
        auto expected = std::make_error_code(std::errc::not_enough_memory);
        EXPECT_EQ(ec, expected);
    }
}
namespace ComponentRawTest
{
    TEST(ComponentRawInterfaceTest, GetVersionWithNullPointerReturnsInvalidArgument)
    {
        // Load the real component DLL
        auto dllPath = std::filesystem::absolute(COMPONENT_TARGET_NAME);
        dll::Helper helper(dllPath);

        // Obtain factory and create root IUnknown
        dll::Fp<decltype(::CreateInstance)> create{helper[dll::procname_t("CreateInstance")]};
        ComponentAPI::IUnknownReplica* pRoot = create();
        ASSERT_NE(pRoot, nullptr);

        // Query IX2
        ComponentAPI::IX2* pIX2 = nullptr;
        int32_t qiRet = pRoot->QueryInterface<ComponentAPI::IX2>(&pIX2);
        ASSERT_EQ(qiRet, 0);
        ASSERT_NE(pIX2, nullptr);

        // Call GetVersion with nullptr to hit the guarded branch
        int32_t ret = pIX2->GetVersion(nullptr);

        // Convert returned int to std::error_code the same way wrapper does
        std::error_code ec = std::make_error_code(static_cast<std::errc>(ret));
        EXPECT_EQ(ec, std::make_error_code(std::errc::invalid_argument));

        // Release interfaces
        pIX2->Release();
        pRoot->Release();
    }
    TEST(ComponentRawInterfaceTest, GetRandomWithNullPointerReturnsInvalidArgument)
    {
        // Load the real component DLL
        auto dllPath = std::filesystem::absolute(COMPONENT_TARGET_NAME);
        dll::Helper helper(dllPath);

        // Obtain factory and create root IUnknown
        dll::Fp<decltype(::CreateInstance)> create{helper[dll::procname_t("CreateInstance")]};
        ComponentAPI::IUnknownReplica* pRoot = create();
        ASSERT_NE(pRoot, nullptr);

        // Query IRandom
        ComponentAPI::IRandom* pIRandom = nullptr;
        int32_t qiRet = pRoot->QueryInterface<ComponentAPI::IRandom>(&pIRandom);
        ASSERT_EQ(qiRet, 0);
        ASSERT_NE(pIRandom, nullptr);

        // Call GetVersion with nullptr to hit the guarded branch
        int32_t ret = pIRandom->GenerateRandomNumbers(0, nullptr);

        // Convert returned int to std::error_code the same way wrapper does
        std::error_code ec = std::make_error_code(static_cast<std::errc>(ret));
        EXPECT_EQ(ec, std::make_error_code(std::errc::invalid_argument));

        // Release interfaces
        pIRandom->Release();
        pRoot->Release();
    }

    TEST(ComponentRawInterfaceTest, QueryInterfaceTest)
    {
        // Load the real component DLL
        auto dllPath = std::filesystem::absolute(COMPONENT_TARGET_NAME);
        dll::Helper helper(dllPath);

        // Obtain factory and create root IUnknown
        dll::Fp<decltype(::CreateInstance)> create{helper[dll::procname_t("CreateInstance")]};
        ComponentAPI::IUnknownReplica* pRoot = create();
        ASSERT_NE(pRoot, nullptr);

        // Query IRandom
        ComponentAPI::IUnknownReplica* pIU = nullptr;
        int32_t qiRet = pRoot->QueryInterface<ComponentAPI::IUnknownReplica>(&pIU);
        ASSERT_EQ(qiRet, 0);
        ASSERT_EQ(pIU, pRoot);
        int32_t qiRet2 = pRoot->QueryInterface<ComponentAPI::IUnknownReplica>(nullptr);

        std::error_code ec = std::make_error_code(static_cast<std::errc>(qiRet2));
        EXPECT_EQ(ec, std::make_error_code(std::errc::invalid_argument));

        // Release interfaces
        pIU->Release();
        pRoot->Release();
    }
}