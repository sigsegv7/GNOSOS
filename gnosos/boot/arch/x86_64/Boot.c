/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 *
 * Abstract:
 *      UEFI loader entry point.
 * Author:
 *      Ian M. Moffett <ian@mirocom.org>
 */

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/LoadedImage.h>
#include "Loader.h"
#include "Common.h"
#include "File.h"

/* Globals */
static EFI_MEMORY_DESCRIPTOR *EfiMemoryMap;
static UINTN EfiMapKey;
static UINTN EfiMapSize;
static UINTN EfiMapDescSize;
static UINT32 EfiMapDescVersion;

#if QUICKSTRAP_DEBUG_MAP
#define MEMTYPE_STR(INDEX)          \
    (INDEX >= EfiMaxMemoryType)     \
        ? L"Bad"                    \
        : MemTypeTab[(INDEX)]

/* Memory typename table */
static CHAR16 *MemTypeTab[] = {
    [EfiReservedMemoryType]         = L"Bad",
    [EfiLoaderCode]                 = L"EfiLoaderCode",
    [EfiLoaderData]                 = L"EfiLoaderData",
    [EfiBootServicesCode]           = L"EfiBootServicesCode",
    [EfiBootServicesData]           = L"EfiBootServicesData",
    [EfiRuntimeServicesCode]        = L"EfiRuntimeServicesCode",
    [EfiRuntimeServicesData]        = L"EfiRuntimeServicesData",
    [EfiConventionalMemory]         = L"EfiConventionalMemory",
    [EfiUnusableMemory]             = L"EfiUnusableMemory",
    [EfiACPIReclaimMemory]          = L"EfiACPIReclaimMemory",
    [EfiACPIMemoryNVS]              = L"EfiACPIMemoryNVS",
    [EfiMemoryMappedIO]             = L"EfiMemoryMappedIO",
    [EfiMemoryMappedIOPortSpace]    = L"EfiMemoryMappedIOPortSpace",
    [EfiPalCode]                    = L"EfiPalCode",
    [EfiPersistentMemory]           = L"EfiPersistentMemory",
    [EfiUnacceptedMemoryType]       = L"EfiUnacceptedMemoryType"
};
#endif  /* QUICKSTRAP_DEBUG */

/*
 * Halt and catch fire
 */
static void
Hcf(void)
{
    for (;;) {
        asm volatile("cli; hlt");
    }
}

/*
 * Request the memory map from firmware
 */
static void
QueryMemoryMap(void)
{
    EFI_STATUS Status;
#if QUICKSTRAP_DEBUG_MAP
    EFI_MEMORY_DESCRIPTOR *MapIter;
#endif  /* QUICKSTRAP_DEBUG */

    EfiMapKey = 0;
    EfiMapSize = 0;
    EfiMapDescSize = 0;
    EfiMapDescVersion = 0;
    EfiMemoryMap = NULL;

    /* Obtain the size of the memory map */
    Status = gBS->GetMemoryMap(
        &EfiMapSize,
        EfiMemoryMap,
        &EfiMapKey,
        &EfiMapDescSize,
        &EfiMapDescVersion
    );

    if (Status != EFI_BUFFER_TOO_SMALL) {
        PRINT_FATAL("Failed to obtain memory map size\n");
        Hcf();
    }

    /* Allocate space for the memory map */
    PRINT_INFO("Firmware provides %d byte memory map\n", EfiMapSize);
    Status = gBS->AllocatePool(
        EfiLoaderData,
        EfiMapSize,
        (VOID **)&EfiMemoryMap
    );

    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to allocate memory map\n");
        Hcf();
    }

    /* Obtain the memory map */
    Status = gBS->GetMemoryMap(
        &EfiMapSize,
        EfiMemoryMap,
        &EfiMapKey,
        &EfiMapDescSize,
        &EfiMapDescVersion
    );

    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to obtain memory map\n");
        Hcf();
    }

#if QUICKSTRAP_DEBUG_MAP
    MapIter = EfiMemoryMap;
    while ((UINTN)MapIter < (UINTN)EfiMemoryMap + EfiMapSize - 1) {
        Print(L"* %s\n", MEMTYPE_STR(MapIter->Type));
        Print(L".... PhysicalStart=%p\n", MapIter->PhysicalStart);
        Print(L".... VirtualStart=%p\n", MapIter->VirtualStart);
        Print(L".... PageCount=%d\n", MapIter->NumberOfPages);
        MapIter = PTR_OFFSET(MapIter, EfiMapDescSize);
    }
#endif  /* QUICKSTRAP_DEBUG */
}

EFI_STATUS EFIAPI
UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;

    Status = FileServiceInitialize(ImageHandle);
    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to initialize file service\n");
        Hcf();
    }

    Status = LoaderPrepare();
    if (EFI_ERROR(Status)) {
        Hcf();
    }

    QueryMemoryMap();

    for (;;);
    return 0;
}
