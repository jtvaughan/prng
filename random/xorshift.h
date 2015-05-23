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

#ifndef __CHIKARA_XORSHIFT_H
#define __CHIKARA_XORSHIFT_H

/* This file provides the xorshift* and xorshift+ pseudo-random number
   generators (PRNGs) described by Sebastiano Vigna (vigna@acm.org) at
   <http://xorshift.di.unimi.it/>.  These PRNGs are fast, have excellent
   statistical properties, and can be used in most
   non-cryptographic applications.

   NOTE: As was just mentioned, these generators are NOT suitable for
         cryptographic applications. */

#include <stddef.h>
#include <stdint.h>


/* This structure stores state for an xorshift64* generator.
   The original C source code says this: "This is a fast, good generator
   if you're short on memory, but otherwise we rather suggest to use a
   xorshift128+ or xorshift1024* (for a very long period) generator." */
typedef struct xorshift64star {
    uint64_t state;
} xorshift64star_t;

/* Initialize the specified xorshift64* generator.  This does nothing; it
   is provided so that the xorshift_init macro will work with
   xorshift64* generators. */
extern void xorshift64star_init(xorshift64star_t *x);

/* Seed the specified xorshift64* generator with a 64-bit unsigned integer. */
extern void xorshift64star_seed(xorshift64star_t *x, uint64_t seed);

/* Generate a psuedo-random 64-bit unsigned integer using the specified
   xorshift64* generator. */
extern uint64_t xorshift64star_next(xorshift64star_t *x);


/* This structure stores state for an xorshift1024* generator.
   The original C source code says this: "This is a fast, top-quality
   generator.  If 1024 bits of state are too much, try a
   xorshift128+ generator." */
typedef struct xorshift1024star {
    uint64_t state[16];
    intptr_t current;
} xorshift1024star_t;

/* Initialize the specified xorshift1024* generator. */
extern void xorshift1024star_init(xorshift1024star_t *x);

/* Seed the specified xorshift1024* generator using an xorshift64* generator
   seeded with the specified 64-bit unsigned integer. */
extern void xorshift1024star_seed(xorshift1024star_t *x, uint64_t seed);

/* Generate a psuedo-random 64-bit unsigned integer using the specified
   xorshift1024* generator. */
extern uint64_t xorshift1024star_next(xorshift1024star_t *x);


/* This structure stores state for an xorshift4096* generator.
   The original C source code says this: "While you can use this generator,
   we rather suggest to use a xorshift1024* generator." */
typedef struct xorshift4096star {
    uint64_t state[64];
    intptr_t current;
} xorshift4096star_t;

/* Initialize the specified xorshift4096* generator. */
extern void xorshift4096star_init(xorshift4096star_t *x);

/* Seed the specified xorshift4096* generator using an xorshift64* generator
   seeded with the specified 64-bit unsigned integer. */
extern void xorshift4096star_seed(xorshift4096star_t *x, uint64_t seed);

/* Generate a psuedo-random 64-bit unsigned integer using the specified
   xorshift4096* generator. */
extern uint64_t xorshift4096star_next(xorshift4096star_t *x);


/* This structure stores state for an xorshift128+ generator.
   The original C source code says this: "This is the fastest generator
   passing BigCrush without systematic failures, but due to the relatively
   short period it is acceptable only for applications with a mild amount
   of parallelism; otherwise, use a xorshift1024* generator." */
typedef struct xorshift128plus {
    uint64_t state[2];
} xorshift128plus_t;

/* Initialize the specified xorshift128+ generator. */
extern void xorshift128plus_init(xorshift128plus_t *x);

/* Seed the specified xorshift128+ generator using an xorshift64* generator
   seeded with the specified 64-bit unsigned integer. */
extern void xorshift128plus_seed(xorshift128plus_t *x, uint64_t seed);

/* Generate a psuedo-random 64-bit unsigned integer using the specified
   xorshift128+ generator. */
extern uint64_t xorshift128plus_next(xorshift128plus_t *x);


/* This generic macro initializes the specified xorshift generator.
   The parameter must be a pointer to one of the xorshift structures. */
#define xorshift_init(xorshift) _Generic((xorshift),\
  xorshift64star_t *:   xorshift64star_init,\
  xorshift1024star_t *: xorshift1024star_init,\
  xorshift4096star_t *: xorshift4096star_init,\
  xorshift128plus_t *:   xorshift128plus_init)(xorshift)

/* This generic macro seeds the specified xorshift generator with a 64-bit
   unsigned integer.  The xorshift parameter must be a pointer to one of the
   xorshift structures. */
#define xorshift_seed(xorshift, seed) _Generic((xorshift),\
  xorshift64star_t *:   xorshift64star_seed,\
  xorshift1024star_t *: xorshift1024star_seed,\
  xorshift4096star_t *: xorshift4096star_seed,\
  xorshift128plus_t *:  xorshift128plus_seed)(xorshift, seed)

/* This generic macro generates a pseudo-random 64-bit unsigned integer
   using the specified xorshift generator.  The parameter must be a pointer
   to one of the xorshift structures. */
#define xorshift_next(xorshift) _Generic((xorshift),\
  xorshift64star_t *:   xorshift64star_next,\
  xorshift1024star_t *: xorshift1024star_next,\
  xorshift4096star_t *: xorshift4096star_next,\
  xorshift128plus_t *:  xorshift128plus_next)(xorshift)

#endif  /* __CHIKARA_XORSHIFT_H */
