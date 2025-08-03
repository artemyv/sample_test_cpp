#include "IOUtils.h"
#include <format>
#include <cstdio>
#include <iostream>

void trace(std::string_view msg, std::source_location loc)
{
	std::puts(std::format("Client 1:\t{:40} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
}

std::string readName()
{
	std::string name;
	std::puts("Enter the dll name[Component.dll]: ");
	std::getline(std::cin, name);
	if (!name.empty() && name.back() == L'\n') {
		name.pop_back();
	}
	if(name.empty()) {
		name = "Component.dll";
	}
	return name;
}