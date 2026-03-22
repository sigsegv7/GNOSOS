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

EFI_STATUS
LoaderPrepare(void)
{
    EFI_STATUS Status;
    EFI_FILE_HANDLE FileHandle;

    Status = FileServiceOpen(QUICKSTRAP_TARGET, &FileHandle);
    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to open kernel image\n");
        return Status;
    }

    return EFI_SUCCESS;
}
