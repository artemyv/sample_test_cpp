//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <source_location>

#include <gsl/narrow>

#include <ComponentWrapper.h>

namespace
{
	std::string wstringToUtf8(std::wstring_view wstr)
	{
		
		const int in_size = gsl::narrow<int>(std::ssize(wstr));
		const auto data_in{wstr.data()};
		if(in_size == 0 || data_in == nullptr) return {};

		const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, data_in, in_size, nullptr, 0, nullptr, nullptr);
		if(sizeNeeded <= 0) return {}; // fail-safe

		std::string utf8Str(sizeNeeded, '\0');
		const int written = WideCharToMultiByte(CP_UTF8, 0, data_in, in_size, utf8Str.data(), sizeNeeded, nullptr, nullptr);
		if(written <= 0) return {}; // fail-safe

		// Remove trailing null if present
		if(!utf8Str.empty() && utf8Str.back() == '\0')
			utf8Str.pop_back();

		return utf8Str;
	}

	constexpr unsigned long forLog(HRESULT hr) noexcept
	{
		return hr;
	}

	static void trace(const char* msg, std::source_location loc = std::source_location::current())
	{
		std::puts(std::format("Client 1:\t{:40} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
	}
}
//
// Клиент1
//
int main()
{
	// Считать имя компонента

	std::wstring name;
	std::puts( "Enter the dll name[Component.dll]: ");
	std::getline(std::wcin, name);
	if (!name.empty() && name.back() == '\n') name.pop_back();
	if(name.empty()) {
		name = L"Component.dll";
	}
	trace("get IUnknown");
	ComponentWrapper wrapper(name.c_str());

	if(!wrapper) {
		trace("CallCreateInstance failed");
		return 1;
	}
	trace("get IX");
	{
		std::wstring version{L"Unknown"};
		const auto res = wrapper.GetVersion(version);
		if(FAILED(res)) {
			trace(std::format("GetVersion failed with error: {:#10x}", forLog(res)).c_str());
		}
		else {
			trace(std::format("GetVersion succeeded : {}", wstringToUtf8(version)).c_str());
		}
	}

	trace("get IX");
	{
		const auto res = wrapper.Fx();
		if(FAILED(res)) {
			trace(std::format("Fx failed with error: {:#10x}", forLog(res)).c_str());
		}
		else {
			trace("Fx succeeded");
		}
	}
	trace("get IY");
	{
		const auto res = wrapper.Fy();
		if(FAILED(res)) {
			trace(std::format("Fy failed with error: {:#10x}", forLog(res)).c_str());
		}
		else {
			trace("Fy succeeded");
		}
	}
	trace("get IZ");
	{
		const auto res = wrapper.Fz();
		if(FAILED(res)) {
			trace(std::format("Fz failed with error: {:#10x}", forLog(res)).c_str());
		}
		else {
			trace("Fz succeeded");
		}
	}
	trace("get random numbers");
	{
		std::wstring numbers_json;
		const auto res = wrapper.GenerateRandomNumbers(25, numbers_json);
		if(FAILED(res)) {
			trace(std::format("GenerateRandomNumbers failed with error: {:#10x}", forLog(res)).c_str());
		}
		else {
			trace(wstringToUtf8(numbers_json).c_str());
		}
	}


	return 0;
}