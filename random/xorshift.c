/*
 * 力 C Library
 * Written in 2015 by Jordan Vaughan
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/* Much of this code was written by Sebastiano Vigna (vigna@acm.org)
   and was released to the public domain using the same dedication as
   above.  The original notice follows:

   Written in 2014 by Sebastiano Vigna (vigna@acm.org)

   To the extent possible under law, the author has dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xorshift.h"
#include <assert.h>

void xorshift64star_init(xorshift64star_t *x) {}

void xorshift64star_seed(xorshift64star_t *x, uint64_t seed) {
    assert(x);
    x->state = seed;
}

uint64_t xorshift64star_next(xorshift64star_t *x) {
    assert(x);
    x->state ^= x->state >> 12; // a
    x->state ^= x->state << 25; // b
    x->state ^= x->state >> 27; // c
    return x->state * 2685821657736338717ULL;
}

void xorshift1024star_init(xorshift1024star_t *x) {
    assert(x);
    x->current = 0;
}

void xorshift1024star_seed(xorshift1024star_t *x, uint64_t seed) {
    assert(x);
    xorshift64star_t generator;
    xorshift_seed(&generator, seed);
    for (intptr_t n = 15; n >= 0; --n) {
        x->state[n] = xorshift_next(&generator);
    }
}

uint64_t xorshift1024star_next(xorshift1024star_t *x) {
    assert(x);
    uint64_t s0 = x->state[x->current];
    x->current = (x->current + 1) & 15;
    uint64_t s1 = x->state[x->current];
    s1 ^= s1 << 31;    // a
    s1 ^= s1 >> 11;    // b
    s0 ^= s0 >> 30;    // c
    const uint64_t temp = s0 ^ s1;
    x->state[x->current] = temp;
    return temp * 1181783497276652981ULL;
}

void xorshift4096star_init(xorshift4096star_t *x) {
    assert(x);
    x->current = 0;
}

void xorshift4096star_seed(xorshift4096star_t *x, uint64_t seed) {
    assert(x);
    xorshift64star_t generator;
    xorshift_seed(&generator, seed);
    for (intptr_t n = 63; n >= 0; --n) {
        x->state[n] = xorshift_next(&generator);
    }
}

uint64_t xorshift4096star_next(xorshift4096star_t *x) {
    assert(x);
    uint64_t s0 = x->state[x->current];
    x->current = (x->current + 1) & 63;
    uint64_t s1 = x->state[x->current];
    s1 ^= s1 << 25;    // a
    s1 ^= s1 >> 3;     // b
    s0 ^= s0 >> 49;    // c
    const uint64_t temp = s0 ^ s1;
    x->state[x->current] = temp;
    return temp * 8372773778140471301ULL;
}

void xorshift128plus_init(xorshift128plus_t *x) {}

void xorshift128plus_seed(xorshift128plus_t *x, uint64_t seed) {
    assert(x);
    xorshift64star_t generator;
    xorshift_seed(&generator, seed);
    x->state[0] = xorshift_next(&generator);
    x->state[1] = xorshift_next(&generator);
}

uint64_t xorshift128plus_next(xorshift128plus_t *x) {
    assert(x);
    uint64_t s1 = x->state[0];
    const uint64_t s0 = x->state[1];
    x->state[0] = s0;
    s1 ^= s1 << 23;    // a
    const uint64_t temp = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26));
    x->state[1] = temp;
    return temp + s0;  // b, c
}
