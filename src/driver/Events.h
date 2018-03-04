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

#pragma once

#include <ndis.h>
#include <minwindef.h>

#include "KmTypes.h"

#include "NdisMemoryManager.h"

//////////////////////////////////////////////////////////////////////
// Event functions
//////////////////////////////////////////////////////////////////////

NTSTATUS InitializeEvent(
    __in    PKM_MEMORY_MANAGER  MemoryManager,
    __inout PEVENT              Event);

NTSTATUS FinalizeEvent(
    __in    PEVENT  Event);

EVENT* CreateEvent();
BOOL FreeEvent(PEVENT event);