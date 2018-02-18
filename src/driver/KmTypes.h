//////////////////////////////////////////////////////////////////////
// Project: pcap-ndis6
// Description: WinPCAP fork with NDIS6.x support 
// License: MIT License, read LICENSE file in project root for details
//
// Copyright (c) 2017 ChangeDynamix, LLC
// All Rights Reserved.
// 
// https://changedynamix.io/
// 
// Author: Andrey Fedorinin
//////////////////////////////////////////////////////////////////////

#ifndef KM_TYPES_H
#define KM_TYPES_H

#include <ndis.h>
#include "KmLock.h"
#include "KmList.h"
#include "NdisMemoryManager.h"
#include "..\shared\SharedTypes.h"

typedef struct _ADAPTER     ADAPTER, *PADAPTER;
typedef struct _DEVICE      DEVICE, *PDEVICE;
typedef struct _DRIVER_DATA DRIVER_DATA, *PDRIVER_DATA;

typedef struct _DEVICE
{
    PDRIVER_DATA    DriverData;

    PUNICODE_STRING Name;

    PUNICODE_STRING SymlinkName;

    PDEVICE_OBJECT  Device;

    PADAPTER        Adapter;

    KM_LOCK         OpenCloseLock;

    KM_LIST         ClientList;

    ULONG           Releasing;

    ULONG           IsAdaptersList;

} DEVICE, *PDEVICE;

typedef struct _KM_TIME
{
    long    Seconds;
    long    Microseconds;
} KM_TIME, *PKM_TIME;


#define NETWORK_EVENT_INFO_PROCESS_PATH_MAX_SIZE    1024

typedef struct _NETWORK_EVENT_INFO
{
    //  IP protocol (one of IPPROTO_XXX values)
    unsigned short  IpProtocol;

    //  Address family (AF_INET or AF_INET6)
    unsigned short  AddressFamily;

    struct Local
    {
        //  Local address
        IP_ADDRESS      Address;

        //  Local port
        unsigned short  Port;

    } Local;

    struct Remote
    {
        //  Remote address
        IP_ADDRESS      Address;

        //  Remote port
        unsigned short  Port;

    } Remote;

    struct Process
    {
        //  Process id
        unsigned long long  Id;

        //  Process executable path size (in bytes)
        unsigned long       NameSize;

        //  Process executable path buffer
        wchar_t             NameBuffer[NETWORK_EVENT_INFO_PROCESS_PATH_MAX_SIZE];

    } Process;

} NETWORK_EVENT_INFO, *PNETWORK_EVENT_INFO;

typedef struct _ADAPTER
{
    //  List link
    LIST_ENTRY      Link;

    //  Unicode string containing adapter id
    UNICODE_STRING      Name;

    //  Size of the data stored in DisplayName field
    ULONG               DisplayNameSize;

    //  Adapter display name
    char                DisplayName[256];

    //  Size of the data stored in MacAddress field
    ULONG               MacAddressSize;

    //  Physical adapter address
    UCHAR               MacAddress[NDIS_MAX_PHYS_ADDRESS_LENGTH];

    //  MTU size
    ULONG               MtuSize;

    //  NDIS adapter handle
    NDIS_HANDLE         AdapterHandle;

    //  Adapter lock
    PNDIS_SPIN_LOCK     Lock;

    //  Associated device instance
    PDEVICE             Device;

    //  Bind operation timestamp
    KM_TIME             BindTimestamp;

    // To complete Bind request if necessary
    NDIS_HANDLE         BindContext;

    // To complete Unbind request if necessary
    NDIS_HANDLE         UnbindContext;

    //  Readiness flag
    ULONG               Ready;

    //  Number of pending OID requests
    volatile ULONG      PendingOidRequests;

    //  Number of pending packet injectio requests
    volatile ULONG      PendingSendPackets;

    //  Pointer to driver data
    PDRIVER_DATA        DriverData;

    //  RESERVED. 
    //  Do not use outside of packet reading routine.
    //  
    //  Current network event info
    //  This field is being used during packet receive.
    NETWORK_EVENT_INFO  CurrentEventInfo;

} ADAPTER, *PADAPTER;


typedef struct _DRIVER_DATA
{
    LONG                DriverUnload;

    struct Ndis
    {
        KM_MEMORY_MANAGER   MemoryManager;
        NDIS_HANDLE         DriverHandle;
        NDIS_HANDLE         ProtocolHandle;
    } Ndis;

    struct Wfp
    {
        KM_MEMORY_MANAGER   MemoryManager;
        HANDLE  Instance;
    } Wfp;

    struct Other
    {
        PDRIVER_OBJECT  DriverObject;
        HANDLE          Connections;
    } Other;

    PDEVICE             ListAdaptersDevice;

    KM_LIST             AdaptersList;

} DRIVER_DATA, *PDRIVER_DATA;

#ifndef IPPROTO_TCP
#define IPPROTO_TCP     6
#endif

#ifndef IPPROTO_UDP
#define IPPROTO_UDP     17
#endif

#ifndef IPPROTO_ICMP
#define IPPROTO_ICMP    2
#endif

#ifndef IPPROTO_ICMPV6
#define IPPROTO_ICMPV6  58
#endif

#ifndef AF_INET
#define AF_INET 2
#endif

#ifndef AF_INET6
#define AF_INET6    23
#endif

#endif