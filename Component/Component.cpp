#include <cstdio>
#include <details/IFace.h>
#include <format>
#include <source_location>
#include <random>
#include <string_view>
#include <atomic>
#include <system_error>
using namespace ComponentAPI;

namespace
{
	// Allocation-safe trace for OOM paths (no std::format usage)
	inline void trace_noalloc(const char* msg, const char* loc) noexcept {
		std::fputs("Component 1:\t", stdout);
		std::fputs(msg, stdout);
		std::fputs(" [", stdout);
		std::fputs(loc, stdout);
		std::fputs("]\n", stdout);
	}
	inline void trace(const char* msg, std::source_location loc = std::source_location::current()) noexcept
	{
		trace_noalloc(msg, loc.function_name());
	}
	//
	// Component
	//
	class CA: public IX2, public IRandom
	{
		// IUnknown implementation
		int32_t  QueryInterface(const uuids::uuid& iid, IUnknownReplica** ppv) noexcept override;
		unsigned long  AddRef() noexcept override;
		unsigned long  Release() noexcept override;
		// IX
		int32_t  Fx(void) noexcept override { trace("Fx"); return std::error_code{}.value(); }
		// IX2
		int32_t  GetVersion(const char** version) noexcept override
		{
			if(version == nullptr) {
				trace("version is null");
				return std::make_error_code(std::errc::invalid_argument).value();
			}
			try {
				std::string sversion("1.0.0");

				auto version_ptr = std::make_unique<char[]>(sversion.size() + 1);
				std::memcpy(version_ptr.get(), sversion.c_str(), sversion.size()); // include null terminator
				*version = version_ptr.release(); // Transfer ownership of the pointer to the caller

				return 0;
			}
			catch(const std::bad_alloc&) {
				trace_noalloc("bad_alloc in GetVersion", std::source_location::current().function_name());
				return std::make_error_code(std::errc::not_enough_memory).value();
			}
		}

		// IRandom
		int32_t  GenerateRandomNumbers(size_t count, const char** numbers_json)  noexcept override
		{
			try {
				if(numbers_json == nullptr) {
					trace("numbers_json is null");
					return std::make_error_code(std::errc::invalid_argument).value();
				}

				std::random_device rd;  // a seed source for the random number engine
				std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
				std::uniform_int_distribution distrib(1, 6);

				// Generate random numbers
				std::string numbers = R"({"numbers": [)";
				for(size_t i = 0; i < count; ++i) {
					numbers += std::format("{},", distrib(gen)); // Generate a random number from 1 to 6
				}
				if(!numbers.empty() && numbers.back() == ',') {
					numbers.pop_back(); // Remove the last comma
				}
				numbers += "]}"; // Close the JSON object
				auto numbers_json_ptr = std::make_unique<char[]>(numbers.size() + 1);
				std::memcpy(numbers_json_ptr.get(), numbers.c_str(), numbers.size() + 1); // include null terminator
				*numbers_json = numbers_json_ptr.release(); // Transfer ownership of the pointer to the caller
				return 0;
			}
			catch(const std::bad_alloc&) {
				trace_noalloc("bad_alloc in GenerateRandomNumbers", std::source_location::current().function_name());
				return std::make_error_code(std::errc::not_enough_memory).value();
			}
			catch(const std::format_error&) {
				trace("format_error in GenerateRandomNumbers");
				return std::make_error_code(std::errc::interrupted).value();
			}
		}

		int32_t  FreeResult(const char* result)  noexcept override
		{
            std::unique_ptr<const char[]> ptr(result);
			return 0;
		}
	public:
		// Destructor
		virtual ~CA() { trace("Destructing"); }
		CA() = default;
		CA(const CA&) = delete; // Disable copy
		CA& operator=(const CA&) = delete; // Disable assignment
		CA(CA&&) = delete; // Disable move
		CA& operator=(CA&&) = delete; // Disable move assignment
		static  ComponentAPI::IUnknownReplica*  create()
		{
			auto pA = std::make_unique<CA>();
			pA->AddRef();
            ComponentAPI::IX* px = pA.release();
			return px;
		}
	private:
		std::atomic<unsigned long> m_cRef{0U};
	};
	int32_t  CA::QueryInterface(const uuids::uuid& iid, IUnknownReplica** ppv) noexcept
	{
		if(ppv == nullptr) {
			trace("ppv is null");
			return std::make_error_code(std::errc::invalid_argument).value();
		}
		if(iid == IUnknownReplica::iid) {
			trace("return IUnknown");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == IX::iid) {
			trace("return IX");
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == IX2::iid) {
			trace("return IX");
			*ppv = static_cast<IX2*>(this);
		}
		else if(iid == IRandom::iid) {
			trace("return IRandom");
			*ppv = static_cast<IRandom*>(this);
		}
		else {
			trace("not supported");
			*ppv = nullptr;
			return std::make_error_code(std::errc::operation_not_supported).value();
		}
		(*ppv)->AddRef();
		return 0;
	}
	unsigned long  CA::AddRef() noexcept
	{
		const auto res = m_cRef.fetch_add(1, std::memory_order_relaxed) + 1;
		return res;
	}
	unsigned long  CA::Release() noexcept
	{
		const auto res = m_cRef.fetch_sub(1, std::memory_order_acq_rel) - 1;
		if(res == 0) {
            std::unique_ptr<CA> pA(this); // ensure deletion
			return 0;
		}
		return res;
	}
}
//
// Creation function
//
extern "C" ComponentAPI::IUnknownReplica* CreateInstance()
{
    return CA::create();
}