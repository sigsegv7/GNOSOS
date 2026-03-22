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
#include "File.h"
#include "Common.h"

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
    EFI_FILE_HANDLE Handle;

    Status = FileServiceInitialize(ImageHandle);
    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to initialize file service\n");
        Hcf();
    }

    Status = FileServiceOpen(QUICKSTRAP_TARGET, &Handle);
    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to open QUICKSTRAP_TARGET\n");
        Hcf();
    }

    Print(L"Hello, World! %d\n", FileServiceGetLength(Handle));
    for (;;);
    return 0;
}
