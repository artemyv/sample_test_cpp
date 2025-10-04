#pragma once

// We are going to call GetAdaptersInfo function following c++ core guidelines

#include <winsock2.h>
#include <iphlpapi.h>

//link with Iphlpapi.lib

#pragma comment(lib, "iphlpapi.lib")

#include <vector>
#include <bit>
#include <format>

namespace
{
    template<typename T>
    size_t countAdresses(T* addr)
    {
        size_t count = 0u;
        while(addr) {
            count++;
            addr = addr->Next;
        }
        return count;
    }


    template< class... Args >
    void trace(std::format_string<Args...> fmt, Args&&... args)
    {
        std::fputs(std::format(fmt, std::forward<Args>(args)...).c_str(), stdout);
    }

    template< class... Args >
    void trace(std::wformat_string<Args...> fmt, Args&&... args)
    {
        std::fputws(std::format(fmt, std::forward<Args>(args)...).c_str(), stdout);
    }

    inline void printDetails(IP_ADAPTER_ADDRESSES* pCurrAddresses)
    {
        trace("\tLength of the IP_ADAPTER_ADDRESS struct: {}\n",
            pCurrAddresses->Length);
        trace("\tIfIndex (IPv4 interface): {}\n", pCurrAddresses->IfIndex);
        trace("\tAdapter name: {}\n", pCurrAddresses->AdapterName);

        auto UnicastCount = countAdresses(pCurrAddresses->FirstUnicastAddress);
        if(UnicastCount > 0) {
            trace("\tNumber of Unicast Addresses: {}\n", UnicastCount);
        }
        else
            trace("\tNo Unicast Addresses\n");

        auto AnycastCount = countAdresses(pCurrAddresses->FirstAnycastAddress);
        if(AnycastCount > 0) {
            trace("\tNumber of Anycast Addresses: {}\n", AnycastCount);
        }
        else
            trace("\tNo Anycast Addresses\n");

        auto MulticastCount = countAdresses(pCurrAddresses->FirstMulticastAddress);
        if(MulticastCount > 0) {
            trace("\tNumber of Multicast Addresses: {}\n", MulticastCount);
        }
        else
            trace("\tNo Multicast Addresses\n");

        auto DnServerCount = countAdresses(pCurrAddresses->FirstDnsServerAddress);
        if(DnServerCount > 0) {
            trace("\tNumber of DNS Server Addresses: {}\n", DnServerCount);
        }
        else
            trace("\tNo DNS Server Addresses\n");

        trace(L"\tDNS Suffix: {}\n", pCurrAddresses->DnsSuffix);
        trace(L"\tDescription: {}\n", pCurrAddresses->Description);
        trace(L"\tFriendly name: {}\n", pCurrAddresses->FriendlyName);

        if(pCurrAddresses->PhysicalAddressLength != 0) {
            trace("\tPhysical address: ");
            for(unsigned long i = 0u; i < pCurrAddresses->PhysicalAddressLength;
                i++) {
                trace("{:x}", pCurrAddresses->PhysicalAddress[i]);
                if(i + 1u < pCurrAddresses->PhysicalAddressLength)
                    trace("-");
            }
        }
        trace("\n");
        trace("\tFlags: {}\n", pCurrAddresses->Flags);
        trace("\tMtu: {}\n", pCurrAddresses->Mtu);
        trace("\tIfType: {}\n", pCurrAddresses->IfType);
        //trace("\tOperStatus: {}", pCurrAddresses->OperStatus);
        trace("\tIpv6IfIndex (IPv6 interface): {}\n",
            pCurrAddresses->Ipv6IfIndex);
        trace("\tZoneIndices (hex): ");
        for(int i = 0; i < 16; i++)
            trace("{:x} ", pCurrAddresses->ZoneIndices[i]);
        trace("\n");

        trace("\tTransmit link speed: {}\n", pCurrAddresses->TransmitLinkSpeed);
        trace("\tReceive link speed: {}\n", pCurrAddresses->ReceiveLinkSpeed);

        auto PrefixCount = countAdresses(pCurrAddresses->FirstPrefix);
        trace("\tNumber of IP Adapter Prefix entries: {}\n", PrefixCount);
        trace("------------------\n");
    }
}
int test()
{
    unsigned long outBufLen = 0u;
    auto res = GetAdaptersAddresses(AF_UNSPEC, 0u, nullptr, nullptr, &outBufLen);
    if(res != ERROR_BUFFER_OVERFLOW) {
        trace("Unexpected result from GetAdaptersAddresses {}", res);
        return 1;
    }

    std::vector<unsigned char> buffer(outBufLen);
    auto pAddresses = std::bit_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());
    res = GetAdaptersAddresses(AF_UNSPEC, 0u, nullptr, pAddresses, &outBufLen);
    if(res != ERROR_SUCCESS) {
        trace("Failed GetAdaptersAddresses {}", res);
        return 2;
    }
    while(pAddresses) {
        printDetails(pAddresses);

        pAddresses = pAddresses->Next;
    }
    return 0;
}