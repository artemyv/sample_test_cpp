#pragma once
#include <string>
#include <unknwn.h> // Объявление IUnknown

IUnknown* CallCreateInstance(std::string name);