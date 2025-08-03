//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//

#include <format>
#include <string>

#include <ComponentWrapper.h>
#include "IOUtils.h"

//
// Клиент1
//
int main()
{
	// Считать имя компонента

	std::string name = readName();
	trace("get IUnknown");
	ComponentWrapper wrapper(name.c_str());

	if(!wrapper) {
		std::string error;
		[[maybe_unused]]const auto err = wrapper.error_code(error);
		trace(std::format("CallCreateInstance failed: {}", error));
		return 1;
	}
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


	return 0;
}