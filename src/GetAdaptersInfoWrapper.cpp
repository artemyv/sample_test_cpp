#pragma once

// We are going to call GetAdaptersInfo function following c++ core guidelines

#include "GetAdaptersInfoWrapper.h"

#include <winsock2.h>
#include <iphlpapi.h>

//link with Iphlpapi.lib

#pragma comment(lib, "iphlpapi.lib")

#include <vector>
#include <bit>
#include <format>
#include <span>

namespace
{
    template<typename T>
    size_t countAddresses(T* addr)
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

        
        if(auto UnicastCount = countAddresses(pCurrAddresses->FirstUnicastAddress); UnicastCount > 0u) {
            trace("\tNumber of Unicast Addresses: {}\n", UnicastCount);
        }
        else
            trace("\tNo Unicast Addresses\n");

        
        if(auto AnycastCount = countAddresses(pCurrAddresses->FirstAnycastAddress); AnycastCount > 0u) {
            trace("\tNumber of Anycast Addresses: {}\n", AnycastCount);
        }
        else
            trace("\tNo Anycast Addresses\n");

        
        if(auto MulticastCount = countAddresses(pCurrAddresses->FirstMulticastAddress); MulticastCount > 0u) {
            trace("\tNumber of Multicast Addresses: {}\n", MulticastCount);
        }
        else
            trace("\tNo Multicast Addresses\n");

        
        if(auto DnServerCount = countAddresses(pCurrAddresses->FirstDnsServerAddress); DnServerCount > 0u) {
            trace("\tNumber of DNS Server Addresses: {}\n", DnServerCount);
        }
        else
            trace("\tNo DNS Server Addresses\n");

        trace(L"\tDNS Suffix: {}\n", pCurrAddresses->DnsSuffix);
        trace(L"\tDescription: {}\n", pCurrAddresses->Description);
        trace(L"\tFriendly name: {}\n", pCurrAddresses->FriendlyName);

        if(pCurrAddresses->PhysicalAddressLength != 0) {
            trace("\tPhysical address: ");
            bool first = true;
            for(const auto& val : std::span{pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength}) {
                if(!first) trace("-");
                trace("{:x}", val);
                first = false;
            }
        }
        trace("\n");
        trace("\tFlags: {}\n", pCurrAddresses->Flags);
        trace("\tMtu: {}\n", pCurrAddresses->Mtu);
        trace("\tIfType: {}\n", pCurrAddresses->IfType);
        trace("\tIpv6IfIndex (IPv6 interface): {}\n",
            pCurrAddresses->Ipv6IfIndex);
        trace("\tZoneIndices (hex): ");
        for(const auto& val : std::span{pCurrAddresses->ZoneIndices, 16}) {
            trace("{:x}", val);
        }
        trace("\n");

        trace("\tTransmit link speed: {}\n", pCurrAddresses->TransmitLinkSpeed);
        trace("\tReceive link speed: {}\n", pCurrAddresses->ReceiveLinkSpeed);

        auto PrefixCount = countAddresses(pCurrAddresses->FirstPrefix);
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