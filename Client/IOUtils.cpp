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
		throw std::runtime_error("Missing path");
	}
	std::filesystem::path path{name};
	if(!std::filesystem::exists(path)) {
		trace("File does not exist");
		throw std::runtime_error("Invalid path");
	}
	if(!std::filesystem::is_regular_file(path)) {
		trace("Path is not a regular file");
		throw std::runtime_error("Invalid file type");
	}
	return path;
}