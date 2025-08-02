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
#include <vector>
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
		HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) override;
		ULONG __stdcall AddRef() override;
		ULONG __stdcall Release() override;
		// Реализация интерфейса IX
		HRESULT STDMETHODCALLTYPE Fx(void)  override { trace("Fx"); return S_OK; }
		HRESULT STDMETHODCALLTYPE GetVersion(
			/* [retval][out] */ BSTR* version) override
		{
			if(version == nullptr) {
				trace("version is null");
				return E_POINTER; // Проверка на нулевой указатель
			}
			try {
				_bstr_t ver(L"1.0.0");
				*version = ver.Detach(); // Передаем владение BSTR вызывающему коду
				return S_OK; // Возвращаем BSTR
			}
			catch(const _com_error& e) {
				trace(std::format("Error: {:#10x}", e.Error()).c_str());
				return e.Error(); // Возвращаем HRESULT ошибки
			}
			catch(const std::bad_alloc& e) {
				trace(std::format("Exception: {}", e.what()).c_str());
				return E_OUTOFMEMORY; 
			}
		}

		// Реализация интерфейса IY
		HRESULT STDMETHODCALLTYPE GenerateRandomNumbers(
			/* [in] */ int count,
			/* [retval][out] */ BSTR* numbers_json) override
		{
			if(numbers_json == nullptr) {
				trace("numbers_json is null");
				return E_POINTER; // Проверка на нулевой указатель
			}

			std::random_device rd;  // a seed source for the random number engine
			std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(1, 6);

			// Генерация случайных чисел
			std::wstring numbers = LR"({"numbers": [)";
			for(int i = 0; i < count; ++i) {
				numbers += std::format(L"{},",distrib(gen)); // Генерируем случайное число от 1 до 6
			}
			if(!numbers.empty() && numbers.back() == ',') {
				numbers.pop_back(); // Удаляем последнюю запятую
			}
			numbers += L"]}"; // Закрываем JSON-объект
			try {
				_bstr_t json_result(numbers.c_str());
				*numbers_json = json_result.Detach(); // Передаем владение BSTR вызывающему коду
				return S_OK; // Возвращаем BSTR
			}
			catch(const _com_error& e) {
				trace(std::format("Error: {:#10x}", e.Error()).c_str());
				return e.Error(); // Возвращаем HRESULT ошибки
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