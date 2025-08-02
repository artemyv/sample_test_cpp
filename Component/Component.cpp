//
// Cmpnt1.cpp
// Компиляция: cl /LD Cmpnt1.cpp GUIDs.cpp UUID.lib Cmpnt1.def
//
#include <cstdio>
#include <IFace.h>
#include <format>
#include <source_location>
#include <comutil.h>
#include <comdef.h> // For linking with right comsuppw.lib or comsuppwd.lib. (If you must compile with /Zc:wchar_t-, use comsupp.lib.) If you include the comdef.h header file, the correct library is specified for you. 
#include <random>
#include <string_view>
namespace
{
	static void trace(const char* msg, std::source_location loc = std::source_location::current()) noexcept
	{
		try {
			std::puts(std::format("Component 1:\t{:40} [{}:{}]", msg, loc.function_name(), loc.line()).c_str());
		}
		catch(std::exception&) {
			//swallow exceptions from std::format
		}
	}
	//
	// Компонент
	//
	class CA: public IX2, public IRandom
	{
		// Реализация IUnknown
		HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) noexcept override;
		ULONG __stdcall AddRef() noexcept override;
		ULONG __stdcall Release() noexcept override;
		// Реализация интерфейса IX
		HRESULT __stdcall Fx(void) noexcept override { trace("Fx"); return S_OK; }
		HRESULT __stdcall GetVersion(const char** version) noexcept override
		{
			if(version == nullptr) {
				trace("version is null");
				return E_POINTER; // Проверка на нулевой указатель
			}
			try {
				std::string sversion("1.0.0");

				auto version_ptr = std::make_unique<char[]>(sversion.size() + 1);
				std::memcpy(version_ptr.get(), sversion.c_str(), sversion.size()); // include null terminator
				*version = version_ptr.release(); // Передаем владение указателем вызывающему коду

				return S_OK;
			}
			catch(const std::bad_alloc& e) {
				trace(std::format("Exception: {}", e.what()).c_str());
				return E_OUTOFMEMORY; 
			}
		}

		// Реализация интерфейса IY
		HRESULT __stdcall GenerateRandomNumbers( int count, const char** numbers_json)  noexcept override
		{
			if(numbers_json == nullptr) {
				trace("numbers_json is null");
				return E_POINTER; // Проверка на нулевой указатель
			}

			std::random_device rd;  // a seed source for the random number engine
			std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(1, 6);

			// Генерация случайных чисел
			std::string numbers = R"({"numbers": [)";
			for(int i = 0; i < count; ++i) {
				numbers += std::format("{},",distrib(gen)); // Генерируем случайное число от 1 до 6
			}
			if(!numbers.empty() && numbers.back() == ',') {
				numbers.pop_back(); // Удаляем последнюю запятую
			}
			numbers += "]}"; // Закрываем JSON-объект
			try 
			{
				auto numbers_json_ptr = std::make_unique<char[]>(numbers.size() + 1);
				std::memcpy(numbers_json_ptr.get(), numbers.c_str(), numbers.size() + 1); // include null terminator
				*numbers_json = numbers_json_ptr.release(); // Передаем владение указателем вызывающему коду
				return S_OK; 
			}
			catch(const std::bad_alloc& e) {
				trace(std::format("Exception: {}", e.what()).c_str());
				return E_OUTOFMEMORY; 
			}
			catch(...) {
				trace("Unknown exception occurred");
				return E_FAIL; // Возвращаем общую ошибку
			}
		}

		HRESULT __stdcall FreeResult(const char* result)  noexcept override
		{
			delete[] result;
			return S_OK;
		}
	public:
		// Деструктор
		virtual ~CA() { trace("Destructing"); }
		CA() = default;
		CA(const CA&) = delete; // Запрет копирования
		CA& operator=(const CA&) = delete; // Запрет присваивания
		CA(CA&&) = delete; // Запрет перемещения
		CA& operator=(CA&&) = delete; // Запрет перемещения

	private:
		unsigned long m_cRef{0U};
	};
	HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv) noexcept
	{
		if(ppv == nullptr) {
			trace("ppv is null");
			return E_POINTER;
		}
		if(iid == __uuidof(IUnknownReplica)) {
			trace("return IUnknown");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == __uuidof(IX)) {
			trace("return IX");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == __uuidof(IX2)) {
			trace("return IX");
			*ppv = static_cast<IX2*>(this);
		}
		else if(iid == __uuidof(IRandom)) {
			trace("return IRandom");
			*ppv = static_cast<IRandom*>(this);
		}
		else {
			trace("not supported");
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
		static_cast<IUnknown*>(*ppv)->AddRef();
		return S_OK;
	}
	unsigned long __stdcall CA::AddRef() noexcept
	{
		const auto res = InterlockedIncrement(&m_cRef);
		trace(std::format("AddRef: {}", res).c_str());
		return res;
	}
	unsigned long __stdcall CA::Release() noexcept
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
extern "C" IUnknownReplica* CreateInstance()
{
	IUnknownReplica* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}