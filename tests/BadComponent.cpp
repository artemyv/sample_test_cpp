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
	void trace(const char* msg) noexcept
	{
		std::puts(msg);
	}
	//
	// Component
	//
	class CA: public IX2, public IRandom, public IY
	{
		// IUnknown implementation
		int32_t  QueryInterface(const uuids::uuid& iid, IUnknownReplica** ppv) noexcept override;
		unsigned long  AddRef() noexcept override;
		unsigned long  Release() noexcept override;
		// IX
		int32_t  Fx(void) noexcept override 
		{
			trace("Fx");
			return std::error_code{}.value(); 
		}
		// IY
		int32_t  Fy(void) noexcept override {
			trace("Fy");
			return std::error_code{}.value();
		}
		// IX2
		int32_t  GetVersion(const char** /*version*/) noexcept override
		{
			trace("GetVersion");
			return std::make_error_code(std::errc::not_enough_memory).value();
		}

		// IRandom
		int32_t  GenerateRandomNumbers(size_t /*count*/, const char** /*numbers_json*/)  noexcept override
		{
			trace("GenerateRandomNumbers");
			return std::make_error_code(std::errc::not_enough_memory).value();
		}

		int32_t  FreeResult(const char* /*result*/)  noexcept override
		{
			return 0;
		}
	public:
		// Destructor
		virtual ~CA() = default;
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
			return std::make_error_code(std::errc::invalid_argument).value();
		}
		if(iid == IUnknownReplica::iid) {
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == IX::iid) {
			*ppv = static_cast<IX*>(this);
		}
		else if(iid == IY::iid) {
			*ppv = static_cast<IY*>(this);
		}
		else if(iid == IX2::iid) {
			*ppv = static_cast<IX2*>(this);
		}
		else if(iid == IRandom::iid) {
			*ppv = static_cast<IRandom*>(this);
		}
		else {
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