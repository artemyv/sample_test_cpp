#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "EncodingUtils.h"

#include <gsl/narrow>

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
