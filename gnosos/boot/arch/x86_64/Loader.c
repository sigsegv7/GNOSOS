/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 *
 * Description:
 *      Implements the core loader logic used to bring the kernel
 *      to a start.
 * Author:
 *      Ian M. Moffett <ian@mirocom.org>
 */

#include <Uefi.h>
#include <Library/UefiLib.h>
#include "Common.h"
#include "File.h"
#include "Loader.h"

/* Length of kernel binary in bytes */
static UINTN KernelLength = 0;

/* Kernel binary base */
static VOID *KernelBinBase = NULL;

/*
 * Print the kernel size in a human readable
 * format
 */
static inline void
PrintKernelSize(void)
{
    PRINT_INFO("Kernel loaded : ");
    if (KernelLength >= UNIT_MIB) {
        Print(L"%d MiB\n", KernelLength / UNIT_MIB);
    } else if (KernelLength >= UNIT_KIB) {
        Print(L"%d KiB\n", KernelLength / UNIT_KIB);
    } else {
        Print(L"%d bytes\n", KernelLength);
    }
}

EFI_STATUS
LoaderPrepare(void)
{
    EFI_STATUS Status;
    EFI_FILE_HANDLE FileHandle;
    UINTN BytesRead;

    /* Only run once */
    if (KernelLength > 0) {
        return EFI_SUCCESS;
    }

    Status = FileServiceOpen(QUICKSTRAP_TARGET, &FileHandle);
    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to open kernel image\n");
        return Status;
    }

    KernelLength = FileServiceGetLength(FileHandle);
    Status = gBS->AllocatePool(
        EfiLoaderData,
        KernelLength,
        (VOID **)&KernelBinBase
    );

    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to allocate memory for kernel bin\n");
        FileServiceClose(FileHandle);
        return Status;
    }

    BytesRead = FileServiceRead(FileHandle, KernelLength, KernelBinBase);
    if (BytesRead == 0) {
        FileServiceClose(FileHandle);
        gBS->FreePool(KernelBinBase);
        KernelBinBase = NULL;
        return EFI_OUT_OF_RESOURCES;
    }

    PrintKernelSize();
    return EFI_SUCCESS;
}
