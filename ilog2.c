/* ----------------------------------------------------------------------- *
 *
 *   Copyright 1996-2010 The NASM Authors - All Rights Reserved
 *   See the file AUTHORS included with the NASM distribution for
 *   the specific copyright holders.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------- */

#include "compiler.h"
#include "nasmlib.h"

#if defined(__GNUC__) && defined(__x86_64__)

int ilog2_32(uint32_t v)
{
    int n;

    __asm__("bsrl %1,%0 ; cmovel %2,%0"
	    : "=&r" (n)
	    : "rm" (v), "rm" (0));
    return n;
}

#elif defined(__GNUC__) && defined(__i386__)

int ilog2_32(uint32_t v)
{
    int n;

    __asm__("bsrl %1,%0 ; jnz 1f ; xorl %0,%0\n"
	    "1:"
	    : "=&r" (n)
	    : "rm" (v));
    return n;
}

#elif defined(HAVE_GNUC_4)

int ilog2_32(uint32_t v)
{
    if (!v)
	return 0;
    else
	return __builtin_clz(v) ^ 31;
}

#else

int ilog2_32(uint32_t v)
{
	int p = 0;

	if (v & 0xffff0000) {
		p += 16;
		v >>= 16;
	}
	if (v & 0xff00) {
		p += 8;
		v >>= 8;
	}
	if (v & 0xf0) {
		p += 4;
		v >>= 4;
	}
	if (v & 0xc) {
		p += 2;
		v >>= 2;
	}
	if (v & 0x2) {
		p += 1;
		v >>= 1;
	}

	return p;
}

#endif

#if defined(__GNUC__) && defined(__x86_64__)

int ilog2_64(uint64_t v)
{
    uint64_t n;

    __asm__("bsrq %1,%0 ; cmoveq %2,%0"
	    : "=&r" (n)
	    : "rm" (v), "rm" (UINT64_C(0)));
    return n;
}

#elif defined(HAVE_GNUC_4)

int ilog2_64(uint64_t v)
{
    if (!v)
	return 0;
    else
	return __builtin_clzll(v) ^ 63;
}

#else

int ilog2_64(uint64_t vv)
{
	int p = 0;
	uint32_t v;

	if ((v = vv >> 32) != 0) {
	    p += 32;
	} else {
	    v = vv;
	}
	if (v & 0xffff0000) {
		p += 16;
		v >>= 16;
	}
	if (v & 0xff00) {
		p += 8;
		v >>= 8;
	}
	if (v & 0xf0) {
		p += 4;
		v >>= 4;
	}
	if (v & 0xc) {
		p += 2;
		v >>= 2;
	}
	if (v & 0x2) {
		p += 1;
		v >>= 1;
	}

	return p;
}

#endif

/*
 * v == 0 ? 0 : is_power2(x) ? ilog2_X(v) : -1
 */
int alignlog2_32(uint32_t v)
{
    if (unlikely(v & (v-1)))
	return -1;		/* invalid alignment */

    return ilog2_32(v);
}

int alignlog2_64(uint64_t v)
{
    if (unlikely(v & (v-1)))
	return -1;		/* invalid alignment */

    return ilog2_64(v);
}
