//
// Create.cpp
//
#include <iostream>
#include "Create.h"
#include <IFace.h>

IUnknown* CallCreateInstance(std::string name)
{
	if (name.empty()) name = "Component.dll";
	// Загрузить в процесс динамическую библиотеку
	HINSTANCE hComponent = ::LoadLibraryA(name.c_str());
	if (hComponent == nullptr)
	{
		std::cout << "CallCreateInstance:\tFailed to load dll\n";
		return nullptr;
	}
	// Получить адрес функции CreateInstance
    auto create = reinterpret_cast<decltype(CreateInstance)*>(::GetProcAddress(hComponent, "CreateInstance"));
	if (create == nullptr)
	{
		std::cout << "CallCreateInstance:\tfailed to get proc address of CreateInstance\n";
		return nullptr;
	}
	return create();
}