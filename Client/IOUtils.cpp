#include "IOUtils.h"
#include <format>
#include <cstdio>
#include <iostream>

void trace(std::string_view msg, std::source_location loc)
{
	std::puts(std::format("Client 1:\t{:40} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
}

std::filesystem::path readDllPath()
{
	std::string name;
	std::puts("Enter the full dll path: ");
	std::getline(std::cin, name);
	if (!name.empty() && name.back() == L'\n') {
		name.pop_back();
	}
	if(name.empty()) {
		trace("Path cannot be empty");
		throw std::invalid_argument("Path cannot be empty");
	}
	if(name.length() > 2 && name.front() == L'"' && name.back() == L'"') {
		name = name.substr(1, name.length() - 2);
    }
	std::filesystem::path path{name};
	if(!std::filesystem::exists(path)) {
		trace("File does not exist");
		throw std::invalid_argument("File does not exist");
	}
	if(!std::filesystem::is_regular_file(path)) {
		trace("Path is not a regular file");
		throw std::invalid_argument("Path is not a regular file");
	}
	return path;
}