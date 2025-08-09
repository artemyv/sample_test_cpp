#pragma once

#include "details/IUnknownRe.h"
#include <system_error>
#include <concepts>
#include <memory>

template<class T>
concept refCounting = requires(T& t) {
        {t.AddRef()} -> std::same_as<unsigned long>;
        {t.Release()} -> std::same_as<unsigned long>;
};

template<refCounting T>
struct ComReleaser
{
	void operator()(T* ptr) const noexcept
	{
		if(ptr) ptr->Release();
	}
};

template<typename Interface>
requires (std::derived_from<Interface, IUnknownReplica>)
class InterfaceWrapperBase
{
public:
	template<refCounting T>
	using unique_com_ptr = std::unique_ptr<T, ComReleaser<T>>;

	explicit InterfaceWrapperBase(Interface* pI):m_pI(pI)
	{
		if(!pI) 
			throw std::runtime_error("Interface is not supported");
	}
	explicit InterfaceWrapperBase(auto p) = delete; // Prevent implicit conversion from other types

	template<typename OtherInterface>
	InterfaceWrapperBase<OtherInterface> QueryInterface() const
	{
		OtherInterface* raw = nullptr;
		const auto ec =  std::make_error_code(static_cast<std::errc>(m_pI->QueryInterface(&raw)));
		if(!ec) {
			return InterfaceWrapperBase<OtherInterface>(raw);
		}
		throw std::system_error(ec,"Interface is not supported");
	}

	template<typename F >
	std::error_code CallInterfaceMethod(F&& call_method_result) const noexcept
	{
		return  std::invoke(std::forward<F>(call_method_result), m_pI.get());
	}

private:
	unique_com_ptr<Interface> m_pI{};
};