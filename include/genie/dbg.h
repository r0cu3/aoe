/* Copyright 2018-2019 Folkert van Verseveld. */

/**
 * Common debug routines.
 *
 * Licensed under MIT-License.
 */

#ifndef GENIE_DBG_H
#define GENIE_DBG_H

#include <assert.h>
#include <stddef.h>

#ifdef DEBUG

#include <stdio.h>
#include <xt/string.h>

#define dbgf(f, ...) xtprintf(f, ## __VA_ARGS__)
#define dbgs(s) puts(s)

static inline void dump(const void *mem, size_t n)
{
	size_t i, j = 0;
#ifdef __cplusplus
	const unsigned char *data = (const unsigned char*)mem;
#else
	const unsigned char *data = mem;
#endif

	for (i = 0; i < n; ++i) {
		printf("%02X ", data[i]);
		if (++j == 16) {
			putchar('\n');
			j = 0;
		}
	}
	putchar('\n');
}

#else

#define dbgf(f, ...) ((void)0)
#define dbgs(s) ((void)0)

static inline void dump(const void *mem, size_t n)
{
	(void)mem;
	(void)n;
}

#endif

#endif
