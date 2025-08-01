//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//
#include <string>
#include <IFace.h>
#include "Create.h"
#include <format>
#include <iostream>
#include <source_location>
#include <comdef.h>

static void trace(const char* msg, std::source_location loc = std::source_location::current()) 
{
	std::puts(std::format("Client 1:\t{:30} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
}
//
// Клиент1
//
int main()
{
	// Считать имя компонента

	std::string name;
	std::puts( "Enter the dll name[Component.dll]: ");
	std::getline(std::cin, name);
	if (!name.empty() && name.back() == '\n') name.pop_back();
	
	// Создать компонент вызовом функции CreateInstance из DLL
	trace("get IUnknown");
	{
		_com_ptr_t<_com_IIID<IUnknown, &IID_IUnknown>> pIUnknown(CallCreateInstance(name), false);
		if (!pIUnknown)
		{
			trace("CallCreateInstance failed");
			return 1;
		}
		{
			trace("get IX");
			_com_ptr_t<_com_IIID<IX, &__uuidof(IX)>> pIX(pIUnknown);

			if (pIX)
			{
				trace("got IX");
				pIX->Fx();
			}
			else
			{
				trace("failed to get IX");
			}
			if (pIX)
			{
				trace("get IY");
				_com_ptr_t<_com_IIID<IY, &__uuidof(IY)>> pIY(pIX);

				if (pIY) {
					trace("got IY");
					pIY->Fy();
				}
				else {
					trace("failed to get IY");
				}
				trace("release IY");
			}
			trace("release IX");
		}
		{
			trace("get IZ");
			_com_ptr_t<_com_IIID<IZ, &__uuidof(IZ)>> pIZ(pIUnknown);

			if(pIZ) {
				trace("got IZ");
				pIZ->Fz();
			}
			else {
				trace("failed to get IZ");
			}
			trace("release IZ");
		}
		trace("release IUnknown");
	}
	return 0;
}