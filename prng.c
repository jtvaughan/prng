/*
 * Generate and Print Pseudo-Random Numbers
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

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "random/xorshift.h"

static void (*algorithm)();
static void (*print64)(uint64_t);
static uint64_t seed;
static int seeded;

static void print_decimal32(uint64_t value) {
    if (fprintf(stdout, "%u\n", (uint32_t)value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_lowerhex32(uint64_t value) {
    if (fprintf(stdout, "%08x\n", (uint32_t)value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_upperhex32(uint64_t value) {
    if (fprintf(stdout, "%08X\n", (uint32_t)value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_decimal64(uint64_t value) {
    if (fprintf(stdout, "%llu\n", value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_lowerhex64(uint64_t value) {
    if (fprintf(stdout, "%016llx\n", value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_upperhex64(uint64_t value) {
    if (fprintf(stdout, "%016llX\n", value) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void print_short_usage(FILE *stream) {
    (void) fputs(
"Usage: prng [-h] [-s SEED] [-u] [-x | -X] [-32] [algorithm]\n",
                 stream);
}

static void print_help(FILE *stream) {
    (void) fputs("\n\
Description:\n\
\n\
    Print an indefinite stream of pseudo-random numbers generated by an\n\
    Xorshift pseudo-random number generator (PRNG).\n\
\n\
Algorithms:\n\
\n\
    xorshift64*\n\
    xorshift1024*\n\
    xorshift4096*\n\
    xorshift128+\n\
\n\
    Default: xorshift1024*\n\
\n\
Options:\n\
\n\
    -h                     show this long help message and exit\n\
    -s SEED                set the generator's seed to the specified 64-bit\n\
                           integer (use the current time in seconds\n\
                           if omitted)\n\
    -u                     disable standard output buffering\n\
    -x                     print generated values in hexadecimal with\n\
                           lowercase letters instead of decimal\n\
    -X                     print generated values in hexadecimal with\n\
                           uppercase letters instead of decimal\n\
    -32                    print 32-bit values instead of 64-bit ones\n\
\n", stream);
}

#define xorshift_engine(type) \
    static void xorshift##type () { \
        xorshift##type##_t generator; \
        xorshift_init(&generator); \
        xorshift_seed(&generator, seed); \
        for (;;) { \
            (*print64)(xorshift_next(&generator)); \
        } \
    }

xorshift_engine(64star)
xorshift_engine(1024star)
xorshift_engine(4096star)
xorshift_engine(128plus)

int main(int argc, char **argv) {
    int n;
    int errors;
    int print_32bit_values;
    int print_hex;

    algorithm = xorshift1024star;
    print64 = print_decimal64;
    errors = print_32bit_values = print_hex = 0;
    for (n = 1; n < argc; ++n) {
        if (strcmp(argv[n], "-h") == 0) {
            print_short_usage(stdout);
            print_help(stdout);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[n], "-s") == 0) {
            if (++n == argc) {
                (void) fputs("error: no seed given to -s\n", stderr);
                errors = 1;
            } else {
                char *end;
                errno = 0;
                seed = strtoull(argv[n], &end, 0);
                if (errno || end == argv[n] || *end != '\0') {
                    (void) fprintf(stderr, "error: seed is not a number: %s\n",
                                   argv[n]);
                    errors = 1;
                }
                seeded = 1;
            }
        } else if (strcmp(argv[n], "-u") == 0) {
            if (setvbuf(stdout, NULL, _IONBF, 0) != 0) {
                (void) fprintf(stderr, "error: couldn't disable standard output buffering");
                errors = 1;
            }
        } else if (strcmp(argv[n], "-x") == 0) {
            print64 = (print_32bit_values ? print_lowerhex32 : print_lowerhex64);
            print_hex = 1;
        } else if (strcmp(argv[n], "-X") == 0) {
            print64 = (print_32bit_values ? print_upperhex32 : print_upperhex64);
            print_hex = 2;
        } else if (strcmp(argv[n], "-32") == 0) {
            print64 = (print_hex == 1 ? print_lowerhex32 : (print_hex == 2 ?
              print_upperhex32 : print_decimal32));
            print_32bit_values = 1;
        } else if (n == argc - 1) {
            if (strcmp(argv[n], "xorshift64*") == 0) {
                algorithm = xorshift64star;
            } else if (strcmp(argv[n], "xorshift1024*") == 0) {
                algorithm = xorshift1024star;
            } else if (strcmp(argv[n], "xorshift4096*") == 0) {
                algorithm = xorshift4096star;
            } else if (strcmp(argv[n], "xorshift128+") == 0) {
                algorithm = xorshift128plus;
            } else {
                (void) fprintf(stderr, "error: unrecognized algorithm: %s\n",
                               argv[n]);
                errors = 1;
            }
        } else {
            (void) fprintf(stderr, "error: unrecognized option: %s\n", argv[n]);
            errors = 1;
        }
    }
    if (errors) {
        exit(EXIT_FAILURE);
    }
    if (!seeded) {
        seed = time(NULL);
    }
    (*algorithm)();
    return 0;
}
