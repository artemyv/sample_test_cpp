//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//

#include <format>
#include <string>

#include <ComponentWrapper.h>
#include "IOUtils.h"
#include "EncodingUtils.h"

namespace
{
	constexpr unsigned long forLog(HRESULT hr) noexcept
	{
		return hr;
	}
}
//
// Клиент1
//
int main()
{
	// Считать имя компонента

	std::wstring name = readName();
	trace("get IUnknown");
	ComponentWrapper wrapper(name.c_str());

	if(!wrapper) {
		trace("CallCreateInstance failed");
		return 1;
	}
	trace("get IX");
	{
		std::string version{"Unknown"};
		const auto res = wrapper.GetVersion(version);
		if(FAILED(res)) {
			trace(std::format("GetVersion failed with error: {:#10x}", forLog(res)));
		}
		else {
			trace(std::format("GetVersion succeeded : {}", version));
		}
	}

	trace("get IX");
	{
		const auto res = wrapper.Fx();
		if(FAILED(res)) {
			trace(std::format("Fx failed with error: {:#10x}", forLog(res)));
		}
		else {
			trace("Fx succeeded");
		}
	}
	trace("get random numbers");
	{
		std::string numbers_json;
		const auto res = wrapper.GenerateRandomNumbers(25, numbers_json);
		if(FAILED(res)) {
			trace(std::format("GenerateRandomNumbers failed with error: {:#10x}", forLog(res)));
		}
		else {
			trace(numbers_json);
		}
	}


	return 0;
}