#ifndef UTILS_H_
#define UTILS_H_

typedef enum bool_t {
	FALSE = 0,
	TRUE = 1
} bool_t;

// We need this because of some template shit when comparing Distance and uint64_t
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define DEBUG_PRINTS 1
#define DEBUG(fmt, ...) \
            do { if (DEBUG_PRINTS) printf(fmt, __VA_ARGS__); } while (0)

#endif
