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
#define QUICKSTRAP_TARGET    L"dtkrnl.sys"
#define QUICKSTRAP_DEBUG     1
#define QUICKSTRAP_DEBUG_MAP 0

/* Pointer offset macros */
#define PTR_OFFSET(p, off) ((void *)(char *)(p) + (off))
#define PTR_NOFFSET(p, off) ((void *)(char *)(p) - (off))

/* Align up/down a value */
#define ALIGN_DOWN(value, align)      ((value) & ~((align)-1))
#define ALIGN_UP(value, align)        (((value) + (align)-1) & ~((align)-1))

/* Storage size units */
#define UNIT_GIB 0x40000000
#define UNIT_MIB 0x100000
#define UNIT_KIB 0x400

/* Constants */
#define PAGESIZE 4096

extern EFI_SYSTEM_TABLE *gST;
extern EFI_BOOT_SERVICES *gBS;

/* Print fatal error message */
#define PRINT_FATAL(Fmt, ...) \
    Print(L"Fatal: " Fmt, ##__VA_ARGS__)

/* Print an informational message */
#define PRINT_INFO(Fmt, ...) \
    Print(L"* " Fmt, ##__VA_ARGS__)

#if QUICKSTRAP_DEBUG
#define PRINT_DEBUG(Fmt, ...)   \
    Print(L"% " Fmt, ##__VA_ARGS__)
#else
#define PRINT_DEBUG(Fmt, ...)    \
    (VOID)0
#endif

#endif  /* !_CORE_COMMON_H_ */
