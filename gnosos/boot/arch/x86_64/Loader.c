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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include "Elf.h"
#include "Common.h"
#include "File.h"
#include "Loader.h"

/* Length of kernel binary in bytes */
static UINTN KernelLength = 0;

/* Kernel binary base */
static VOID *KernelBinBase = NULL;

/* Used to allocate after boot services exit */
static VOID *LoaderBumpPool = NULL;

/* Kernel program size */
static UINTN KernelProgSize = 0;

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

/*
 * Check the validity of an ELF64 header
 *
 * @Ehdr: Header to check
 */
static EFI_STATUS
LoaderVerifyElf64(Elf64_Ehdr *Ehdr)
{
    if (Ehdr == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    if (CompareMem(&Ehdr->e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0) {
        PRINT_FATAL("Bad kernel executable magic\n");
        return EFI_INVALID_PARAMETER;
    }

    if (Ehdr->e_type != ET_EXEC) {
        PRINT_FATAL("Kernel is not executable\n");
        return EFI_INVALID_PARAMETER;
    }

    if (Ehdr->e_machine != EM_X86_64) {
        PRINT_FATAL("Kernel binary is not for x86_64\n");
        return EFI_INVALID_PARAMETER;
    }

    if (Ehdr->e_version != EV_CURRENT) {
        PRINT_FATAL("Bad version in kernel binary\n");
        return EFI_INVALID_PARAMETER;
    }

    return EFI_SUCCESS;
}

/*
 * Prepare ELf operation
 */
static EFI_STATUS
LoaderElfPrepare(void)
{
    Elf64_Ehdr *Ehdr;
    Elf64_Phdr *Phdr, *PhdrTableStart;
    EFI_STATUS Status;

    /* Is this a valid ELF64 binary? */
    Ehdr = (Elf64_Ehdr *)KernelBinBase;
    Status = LoaderVerifyElf64(Ehdr);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    KernelProgSize = 0;
    PhdrTableStart = PTR_OFFSET(Ehdr, Ehdr->e_phoff);
#define PHDR_I(INDEX) PTR_OFFSET(PhdrTableStart, Ehdr->e_phentsize * (Index))
    /*
     * Go through every program header and add up the sizes so we know the
     * total program size.
     */
    for (UINTN Index = 0; Index < Ehdr->e_phnum; ++Index) {
        Phdr = PHDR_I(Index);

        /* We only want to count loadable segments */
        if (Phdr->p_type != PT_LOAD) {
            continue;
        }

        PRINT_DEBUG("Program header @ [%p]\n", Phdr->p_vaddr);
        Print(L".... MemorySize=%d\n", Phdr->p_memsz);
        Print(L".... FileSize=%d\n", Phdr->p_filesz);
        Print(L".... Offset=%d\n", Phdr->p_offset);

        /* Add to the program size counter */
        KernelProgSize += ALIGN_UP(Phdr->p_memsz, PAGESIZE);
    }
#undef PHDR_I

    /* Allocate the bump pool */
    Status = gBS->AllocatePool(
        EfiLoaderData,
        KernelProgSize,
        (VOID **)&LoaderBumpPool
    );

    if (EFI_ERROR(Status)) {
        PRINT_FATAL("Failed to allocate loader bump pool\n");
        return EFI_OUT_OF_RESOURCES;
    }

    return EFI_SUCCESS;
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
    Status = LoaderElfPrepare();

    if (EFI_ERROR(Status)) {
        FileServiceClose(FileHandle);
        gBS->FreePool(KernelBinBase);
        KernelBinBase = NULL;
        return Status;
    }

    return EFI_SUCCESS;
}
