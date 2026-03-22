/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 *
 * Description:
 *      Implements a file interface to allow loading of files before
 *      boot services is exited.
 * Author:
 *      Ian M. Moffett <ian@mirocom.org>
 */

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include "File.h"
#include "Common.h"

/* Globals */
static EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
static EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *BootFs;
static EFI_FILE_HANDLE BootVolume;

EFI_STATUS
FileServiceInitialize(EFI_HANDLE ImageHandle)
{
    EFI_STATUS Status;
    EFI_GUID LoadedImageGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

    Status = gBS->HandleProtocol(
        ImageHandle,
        &LoadedImageGuid,
        (VOID **)&LoadedImage
    );

    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID **)&BootFs
    );

    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = BootFs->OpenVolume(BootFs, &BootVolume);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
FileServiceOpen(CHAR16 *Path, EFI_FILE_HANDLE *Result)
{
    if (Path == NULL || Result == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    return BootVolume->Open(
            BootVolume,
            Result,
            Path,
            EFI_FILE_MODE_READ,
            EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM
    );
}

EFI_STATUS
FileServiceClose(EFI_FILE_HANDLE Handle)
{
    return Handle->Close(
            Handle
    );
}

UINTN
FileServiceGetLength(EFI_FILE_HANDLE Handle)
{
    EFI_STATUS Status;
    UINTN OldPosition, FileSize;

    /* Obtain the old position */
    Status = Handle->GetPosition(
        Handle,
        &OldPosition
    );

    if (EFI_ERROR(Status)) {
        return 0;
    }

    /* Seek all the way to the end */
    Status = Handle->SetPosition(
            Handle,
            0xFFFFFFFFFFFFFFFF
    );

    if (EFI_ERROR(Status)){
        return 0;
    }

    /* Obtain the file size */
    Status = Handle->GetPosition(
        Handle,
        &FileSize
    );

    if (EFI_ERROR(Status)){
        return 0;
    }

    /* Restore the old position */
    Status = Handle->SetPosition(
            Handle,
            OldPosition
    );

    if (EFI_ERROR(Status)){
        return 0;
    }

    return FileSize;
}
