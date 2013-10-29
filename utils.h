#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <assert.h>

typedef enum bool_t {
	FALSE = 0,
	TRUE = 1
} bool_t;

// We need this because of some template shit when comparing Distance and uint64_t
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//#define DEBUG_MODE 1
#ifdef DEBUG_MODE

	#define DEBUG(fmt, ...) \
				do { printf(fmt, __VA_ARGS__); } while (0)

	#define DEBUG_ASSERT(x) \
		do { assert(x); } while (0)

	#define DEBUG_DO(x) \
		do { x; } while (0)

#else

	#define DEBUG(fmt, ...) \
				do {} while (0)

	#define DEBUG_ASSERT(x) \
				do {} while (0)

	#define DEBUG_DO(x) \
		do {} while (0)

#endif

#endif
