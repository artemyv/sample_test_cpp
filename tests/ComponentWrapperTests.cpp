#include <gtest/gtest.h>
#include <ComponentWrapper.h>
#include <string>
#include <filesystem>

class ComponentWrapperTest : public ::testing::Test {
protected:
    std::filesystem::path dllPath;

    void SetUp() override {
        // Set the path to your built Component.dll here
        dllPath = std::filesystem::absolute(COMPONENT_TARGET_NAME);
    }
};

TEST_F(ComponentWrapperTest, FxReturnsNoError) {
    ComponentWrapper::ComponentWrapper wrapper(dllPath);
    auto ec = wrapper.Fx();
    EXPECT_TRUE(!ec);
}

TEST_F(ComponentWrapperTest, GetVersionReturnsVersionString) {
    ComponentWrapper::ComponentWrapper wrapper(dllPath);
    std::string version;
    auto ec = wrapper.GetVersion(version);
    EXPECT_TRUE(!ec);
    EXPECT_FALSE(version.empty());
}

TEST_F(ComponentWrapperTest, GenerateRandomNumbersReturnsJson) {
    ComponentWrapper::ComponentWrapper wrapper(dllPath);
    std::string numbers_json;
    auto ec = wrapper.GenerateRandomNumbers(5, numbers_json);
    EXPECT_TRUE(!ec);
    EXPECT_NE(numbers_json.find("\"numbers\":"), std::string::npos);
}

TEST_F(ComponentWrapperTest, FyReturnsError) {
    ComponentWrapper::ComponentWrapper wrapper(dllPath);
    auto ec = wrapper.Fy();
    auto expected = std::make_error_code(std::errc::operation_not_supported);
    EXPECT_EQ(ec, expected);
}