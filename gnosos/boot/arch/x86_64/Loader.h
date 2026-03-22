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

#ifndef _CORE_LOADER_H_
#define _CORE_LOADER_H_ 1

/*
 * Prepare the loader and see how large the kernel is so that memory
 * can be stolen for boot up.
 */
EFI_STATUS LoaderPrepare(void);

#endif  /* !_CORE_LOADER_H_ */
