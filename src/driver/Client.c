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
// Author: Mikhail Burilov
// 
// Based on original WinPcap source code - https://www.winpcap.org/
// Copyright(c) 1999 - 2005 NetGroup, Politecnico di Torino(Italy)
// Copyright(c) 2005 - 2007 CACE Technologies, Davis(California)
// Filter driver based on Microsoft examples - https://github.com/Microsoft/Windows-driver-samples
// Copyrithg(C) 2015 Microsoft
// All rights reserved.
//////////////////////////////////////////////////////////////////////

#include "filter.h"
#include "Adapter.h"
#include "Client.h"
#include "Device.h"
#include "Events.h"
#include "Packet.h"
#include "KernelUtil.h"
#include <flt_dbg.h>

#include "..\shared\CommonDefs.h"

//////////////////////////////////////////////////////////////////////
// Client methods
//////////////////////////////////////////////////////////////////////

NTSTATUS CreateClient(
    __in    PDEVICE         Device,
    __in    PFILE_OBJECT    FileObject,
    __out   PCLIENT         *Client)
{
    NTSTATUS    Status = STATUS_SUCCESS;
    PCLIENT     NewClient = NULL;

    GOTO_CLEANUP_IF_FALSE_SET_STATUS(
        Assigned(Device),
        STATUS_INVALID_PARAMETER_1);
    GOTO_CLEANUP_IF_FALSE_SET_STATUS(
        Assigned(FileObject),
        STATUS_INVALID_PARAMETER_2);
    GOTO_CLEANUP_IF_FALSE_SET_STATUS(
        Assigned(Client),
        STATUS_INVALID_PARAMETER_3);

    NewClient = FILTER_ALLOC_MEM_TYPED(CLIENT, FilterDriverHandle);
    GOTO_CLEANUP_IF_FALSE_SET_STATUS(
        Assigned(NewClient),
        STATUS_INSUFFICIENT_RESOURCES);

    RtlZeroMemory(NewClient, sizeof(CLIENT));

    NewClient->Device = Device;
    NewClient->FileObject = FileObject;

    Status = InitializeEvent(&NewClient->Event);
    GOTO_CLEANUP_IF_FALSE(NT_SUCCESS(Status));

    Status = Km_List_Initialize(&NewClient->PacketList);
    GOTO_CLEANUP_IF_FALSE(NT_SUCCESS(Status));

    Status = Km_Lock_Initialize(&NewClient->ReadLock);
    GOTO_CLEANUP_IF_FALSE(NT_SUCCESS(Status));

    Status = Km_List_AddItem(
        Device->ClientList,
        &NewClient->Link);
    GOTO_CLEANUP_IF_FALSE(NT_SUCCESS(Status));
    
cleanup:

    if (!NT_SUCCESS(Status))
    {
        if (Assigned(NewClient))
        {
            FinalizeEvent(&NewClient->Event);

            FILTER_FREE_MEM(NewClient);
        }
    }
    else
    {
        *Client = NewClient;
    }

    return Status;
};

NTSTATUS FreeClient(
    __in    PCLIENT Client)
{
    NTSTATUS    Status = STATUS_SUCCESS;

    GOTO_CLEANUP_IF_FALSE_SET_STATUS(
        Assigned(Client),
        STATUS_INVALID_PARAMETER_1);

    Km_Lock_Acquire(&Client->ReadLock);
    __try
    {
        FreePacketList(Client->PacketList);
    }
    __finally
    {
        Km_Lock_Release(&Client->ReadLock);
    }

    FinalizeEvent(&Client->Event);

    FILTER_FREE_MEM(Client);

cleanup:
    return Status;
};

void FreeClientList(PLIST list)
{
	DEBUGP(DL_TRACE, "===>FreeClientList...\n");

	list->Releasing = TRUE;

	//Do not need to lock list because device is being closed anyways and there will be no new clients	
	PLIST_ITEM item = list->First;
	while(item)
	{
		PCLIENT client = (PCLIENT)item->Data;
		if(client)
		{
			FreeClient(client);
		}
		item->Data = NULL;

		item = item->Next;
	}

	FreeList(list);
	DEBUGP(DL_TRACE, "<===FreeClientList\n");
}
