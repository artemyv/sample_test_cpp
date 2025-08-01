//
// Cmpnt1.cpp
// Компиляция: cl /LD Cmpnt1.cpp GUIDs.cpp UUID.lib Cmpnt1.def
//
#include <iostream>
#include <objbase.h>
#include "IFace.h"
#include <format>
#include <source_location>
namespace
{
	static void trace(const char* msg, std::source_location loc = std::source_location::current()) noexcept
	{
		try {
			std::puts(std::format("Component 1:\t{:30} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
		}
		catch(std::exception&) {
			//swallow exceptions from std::format
		}
	}
	//
	// Компонент
	//
	class CA: public IX, public IY
	{
		// Реализация IUnknown
		HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) override;
		ULONG __stdcall AddRef() override;
		ULONG __stdcall Release() override;
		// Реализация интерфейса IX
		void __stdcall Fx() noexcept override { trace("Fx"); }
		// Реализация интерфейса IY
		void __stdcall Fy() noexcept override  { trace("Fy"); }
	public:
		// Деструктор
		virtual ~CA() { trace("Destructing"); }
		CA() = default;
		CA(const CA&) = delete; // Запрет копирования
		CA& operator=(const CA&) = delete; // Запрет присваивания
		CA(CA&&) = delete; // Запрет перемещения
		CA& operator=(CA&&) = delete; // Запрет перемещения

	private:
		long m_cRef{0};
	};
	HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
	{
		if(ppv == nullptr) {
			trace("ppv is null");
			return E_POINTER;
		}
		if(iid == __uuidof(IUnknown)) {
			trace("return IUnknown");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == __uuidof(IX)) {
			trace("return IX");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == __uuidof(IY)) {
			trace("return IY");
			*ppv = static_cast<IY*>(this);
		}
		else {
			trace("not supported");
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
		static_cast<IUnknown*>(*ppv)->AddRef();
		return S_OK;
	}
	ULONG __stdcall CA::AddRef()
	{
		const auto res = InterlockedIncrement(&m_cRef);
		trace(std::format("AddRef: {}", res).c_str());
		return res;
	}
	ULONG __stdcall CA::Release()
	{
		const auto res = InterlockedDecrement(&m_cRef);
		trace(std::format("Release: {}", res).c_str());
		if(res == 0) {
			delete this;
			return 0;
		}
		return res;
	}
}
//
// Функция создания
//
extern "C" IUnknown * CreateInstance()
{
	IUnknown* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}