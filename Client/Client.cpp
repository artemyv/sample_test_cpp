//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//
#include <ComponentWrapper.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <source_location>

#include <gsl/narrow>


namespace
{
	std::string wstringToUtf8(std::wstring_view wstr)
	{
		const auto in_size = gsl::narrow<int>(std::ssize(wstr));
		const int sizeNeeded{WideCharToMultiByte(CP_UTF8, 0, wstr.data(), in_size, nullptr, 0, nullptr, nullptr)};
		std::string utf8Str(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), in_size, utf8Str.data(), sizeNeeded, nullptr, nullptr);
		if(!utf8Str.empty() && utf8Str.back() == '\0') // Ensure null terminator is removed
			utf8Str.pop_back(); // Remove null terminator
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
	std::wstring version{L"Unknown"};
	auto res = wrapper.GetVersion(version);
	if(FAILED(res)) {
		trace(std::format("GetVersion failed with error: {:#10x}", forLog(res)).c_str());
	}
	else {
		trace(std::format("GetVersion succeeded : {}", wstringToUtf8(version)).c_str());
	}

	trace("get IX");
	res = wrapper.Fx();
	if (FAILED(res)) {
		trace(std::format("Fx failed with error: {:#10x}", forLog(res)).c_str());
	}
	else
	{
		trace("Fx succeeded");
	}
	trace("get IY");
	res = wrapper.Fy();
	if(FAILED(res)) {
		trace(std::format("Fy failed with error: {:#10x}", forLog(res)).c_str());
	}
	else {
		trace("Fy succeeded");
	}
	trace("get IZ");
	res = wrapper.Fz();
	if(FAILED(res)) {
		trace(std::format("Fz failed with error: {:#10x}", forLog(res)).c_str());
	}
	else {
		trace("Fz succeeded");
	}


	return 0;
}