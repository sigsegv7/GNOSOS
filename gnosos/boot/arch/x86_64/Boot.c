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

    for (;;);
    return 0;
}
