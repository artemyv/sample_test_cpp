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
class InterfaceWrapper
{
public:
	template<refCounting T>
	using unique_com_ptr = std::unique_ptr<T, ComReleaser<T>>;

	explicit InterfaceWrapper(Interface* pI):m_pI(pI)
	{
		if(!pI) 
			throw std::runtime_error("Interface is not supported");
	}
	explicit InterfaceWrapper(auto p) = delete; // Prevent implicit conversion from other types

	template<typename OtherInterface>
	InterfaceWrapper<OtherInterface> QueryInterface() const
	{
		OtherInterface* raw = nullptr;
		const auto ec =  std::make_error_code(static_cast<std::errc>(m_pI->QueryInterface(&raw)));
		if(!ec) {
			return InterfaceWrapper<OtherInterface>(raw);
		}
		throw std::system_error(ec,"Interface is not supported");
	}

	Interface* operator->() { return m_pI.get(); }
	const Interface* operator->() const { return m_pI.get(); }

private:
	unique_com_ptr<Interface> m_pI{};
};