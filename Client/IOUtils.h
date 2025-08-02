#pragma once

#include <source_location>
#include <string>
#include <string_view>

void trace(std::string_view msg, std::source_location loc = std::source_location::current());
std::wstring readName();
