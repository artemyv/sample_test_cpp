//
// Client1.cpp
// Compilation: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//

#include <format>
#include <string>

#include <ComponentWrapper.h>
#include "IOUtils.h"

//
// Client1
//
int main()
{
    // Read component name
    trace("get IUnknown");
    try {
        std::filesystem::path p = readDllPath();
        ComponentWrapper::ComponentWrapper wrapper(p);

        trace("get IX");
        {
            std::string version{"Unknown"};
            const auto res = wrapper.GetVersion(version);
            if(res) {
                trace(std::format("GetVersion failed with error: {}", res.message()));
            }
            else {
                trace(std::format("GetVersion succeeded : {}", version));
            }
        }

        trace("get IX");
        {
            const auto res = wrapper.Fx();
            if(res) {
                trace(std::format("Fx failed with error: {}", res.message()));
            }
            else {
                trace("Fx succeeded");
            }
        }
        trace("get IY");
        {
            const auto res = wrapper.Fy();
            if(res) {
                trace(std::format("Fy failed with error: {}", res.message()));
            }
            else {
                trace("Fy succeeded");
            }
        }

        trace("get random numbers");
        {
            std::string numbers_json;
            const auto res = wrapper.GenerateRandomNumbers(25, numbers_json);
            if(res) {
                trace(std::format("GenerateRandomNumbers failed with error: {}", res.message()));
            }
            else {
                trace(numbers_json);
            }
        }
    }
    catch(const std::exception& e) {
        trace(std::format("Exception: {}", e.what()));
        return 1;
    }
    catch(...) {
        trace("Unknown exception");
        return 1;
	}

    return 0;
}