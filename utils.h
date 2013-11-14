#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <libgen.h> // basename

typedef enum bool_t {
	FALSE = 0,
	TRUE = 1
} bool_t;

// We need this because of some template shit when comparing Distance and uint64_t
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//#define DEBUG_PRINTS 1
#ifdef DEBUG_PRINTS
#define DEBUG(fmt, ...) \
            do { printf(fmt, __VA_ARGS__); } while (0)
#else
#define DEBUG(fmt, ...) \
            do {} while (0)
#endif

static inline char *strclone(char *s)
{
	char *result = (char *)malloc((strlen(s) + 1) * sizeof(char));
	strcpy(result, s);
	return result;
}

static inline char *my_basename(char *filename)
{
	char *clone = strclone(filename);
	char *result = basename(clone);
	free(clone);
	return strclone(result);
}

#endif
