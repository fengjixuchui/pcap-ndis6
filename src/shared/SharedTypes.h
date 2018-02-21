//////////////////////////////////////////////////////////////////////
// Project: pcap-ndis6
// Description: WinPCAP fork with NDIS6.x support 
// License: MIT License, read LICENSE file in project root for details
//
// Copyright (c) 2018 ChangeDynamix, LLC
// All Rights Reserved.
// 
// https://changedynamix.io/
// 
// Author: Andrey Fedorinin
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef ETH_ADDRESS_LENGTH
#define ETH_ADDRESS_LENGTH  0x6
#endif

typedef struct _ETH_ADDRESS
{
    unsigned char   Addr[ETH_ADDRESS_LENGTH];
} ETH_ADDRESS, *PETH_ADDRESS, *LPETH_ADDRESS;

typedef struct _ETH_HEADER
{
    ETH_ADDRESS     DstAddr;
    ETH_ADDRESS     SrcAddr;
    unsigned short  EthType;
} ETH_HEADER, *PETH_HEADER;

#define ETH_TYPE_IP             0x0800  //  IPv4
#define ETH_TYPE_REVERSE_ARP    0x8035  //  Reverse ARP
#define ETH_TYPE_ARP            0x0806  //  ARP
#define ETH_TYPE_IP6            0x86dd  //  IPv6

#define ETH_TYPE_IP_BE          0x0008  //  IPv4, big endian
#define ETH_TYPE_REVERSE_ARP_BE 0x3580  //  Reverse ARP, big endian
#define ETH_TYPE_ARP_BE         0x0608  //  ARP, big endian
#define ETH_TYPE_IP6_BE         0xdd86  //  IPv6, big endian

typedef struct _IP_ADDRESS_V4
{
    union
    {
        unsigned char   b[4];
        unsigned short  s[2];
        unsigned long   l;
    };
} IP_ADDRESS_V4, *PIP_ADDRESS_V4;

typedef struct _IP_ADDRESS_V6
{
    union
    {
        unsigned char       b[16];
        unsigned short      s[8];
        unsigned long       l[4];
        unsigned long long  q[2];
    };
} IP_ADDRESS_V6, *PIP_ADDRESS_V6;

typedef struct _IP_ADDRESS
{
    union
    {
        IP_ADDRESS_V4   v4;
        IP_ADDRESS_V6   v6;
    };
} IP_ADDRESS, *PIP_ADDRESS;

typedef struct _IP6_HEADER
{
    unsigned long   VPF;

    //  Size of the data following the header
    unsigned short  PayloadLength;

    //  Transport protocol
    unsigned char   NextHeader;

    //  Number of hops
    unsigned char   HopLimit;

    //  Source address
    IP_ADDRESS_V6   SourceAddress;

    //  Destination address
    IP_ADDRESS_V6   DestinationAddress;

} IP6_HEADER, *PIP6_HEADER;

typedef struct _IP4_HEADER
{
    unsigned char   VerLen;

    unsigned char   Service;

    unsigned short  Length;

    unsigned short  Ident;

    unsigned short  FlagOff;

    unsigned char   TimeLive;

    unsigned char   Protocol;

    unsigned short  Checksum;

    IP_ADDRESS_V4   SourceAddress;

    IP_ADDRESS_V4   DestinationAddress;

} IP4_HEADER, *PIP4_HEADER;

typedef struct _TCP_HEADER
{
    unsigned short  SourcePort;

    unsigned short  DestinationPort;

    unsigned long   SequenceNumber;

    unsigned long   AckNumber;

    union
    {
        struct Data1
        {
            unsigned short  LenResvFlags;

            unsigned short  WindowSize;

            unsigned short  Checksum;

            unsigned short  UrgentPtr;

            unsigned char   Data[1];

        } Data1;

        struct Data2
        {
            unsigned char   LenRes;

            unsigned char   ResFlags;

        } Data2;

    };

} TCP_HEADER, *PTCP_HEADER;

typedef struct _UDP_HEADER
{
    unsigned short  SourcePort;
    unsigned short  DestinationPort;
    unsigned short  Length;
    unsigned short  Checksum;
} UDP_HEADER, *PUDP_HEADER;

typedef struct _ICMP_HEADER
{
    unsigned char   IcmpType;
    unsigned char   Code;
    unsigned short  Checksum;
    unsigned short  Ident;
    unsigned short  SeqNum;
} ICMP_HEADER, *PICMP_HEADER;

typedef struct _PACKET_DESC
{
    //  Owner process ID or zero
    unsigned long   ProcessId;

    ETH_ADDRESS     SourceEthAddress;

    ETH_ADDRESS     DestinationEthAddress;

    unsigned short  EthType;

    IP_ADDRESS      SourceIPAddress;

    IP_ADDRESS      DestinationIPAddress;

    unsigned char   IPProtocol;

    union SourcePortOrIcmpType
    {
        unsigned short  SourcePort;
        unsigned short  IcmpType;
    } SourcePortOrIcmpType;

    union DestinationPortOrIcmpCode
    {
        unsigned short DestinationPort;
        unsigned short IcmpCode;
    } DestinationPortOrIcmpCode;

} PACKET_DESC, *PPACKET_DESC, *LPPACKET_DESC;

#define PACKET_DESC_FLAG_IP_HEADER_VALID        0x1
#define PACKET_DESC_FLAG_TRANSPORT_HEADER_VALID 0x2
#define PACKET_DESC_FLAG_ETH_HEADER_VALID       0x4
#define PACKET_DESC_FLAG_PROCESS_ID_VALID       0x8