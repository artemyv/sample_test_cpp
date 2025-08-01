//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//
#include <string>
#include <IFace.h>
#include <ComponentWrapper.h>
#include <format>
#include <iostream>
#include <source_location>

namespace
{
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
	auto res = wrapper.Fx();
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