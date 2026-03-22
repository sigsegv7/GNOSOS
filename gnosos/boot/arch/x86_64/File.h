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

#ifndef _CORE_FILE_H_
#define _CORE_FILE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>

/*
 * Initialize bootloader file services
 *
 * @ImageHandle: Image handle in use
 */
EFI_STATUS FileServiceInitialize(EFI_HANDLE ImageHandle);

/*
 * Open a file and get a file handle
 *
 * @Path:   Path to file to open
 * @Result: Result is written here
 */
EFI_STATUS FileServiceOpen(CHAR16 *Path, EFI_FILE_HANDLE *Result);

/*
 * Close a file from its handle
 *
 * @Handle: Handle of file to close
 */
EFI_STATUS FileServiceClose(EFI_FILE_HANDLE Handle);

/*
 * Obtain the length of a file from its handle
 *
 * @Handle: File handle to check length of
 *
 * Returns the length greater than zero on success
 */
UINTN FileServiceGetLength(EFI_FILE_HANDLE Handle);

/*
 * Read the contents of a file into a buffer
 *
 * @Handle:  File handle of file to read
 * @Length:  Number of bytes to read
 * @Buffer:  Buffer to store file data
 *
 * Returns the number of bytes read on success and zero on failure
 */
UINTN FileServiceRead(EFI_FILE_HANDLE Handle, UINTN Length, VOID *Buffer);

#endif  /* !_CORE_FILE_H_ */
