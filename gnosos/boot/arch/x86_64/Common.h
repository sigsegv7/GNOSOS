/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 *
 * Abstract:
 *      Provides common definitions.
 * Author:
 *      Ian M. Moffett <ian@mirocom.org>
 */

#ifndef _CORE_COMMON_H_
#define _CORE_COMMON_H_ 1

#include <Uefi.h>
#include <Library/UefiLib.h>

/* Knobs and settings */
#define QUICKSTRAP_TARGET L"dtkrnl.sys"

extern EFI_SYSTEM_TABLE *gST;
extern EFI_BOOT_SERVICES *gBS;

/* Print fatal error message */
#define PRINT_FATAL(Fmt, ...) \
    Print(L"Fatal: " Fmt, ##__VA_ARGS__)

#endif  /* !_CORE_COMMON_H_ */
