#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/* The fractal data type */
#define FTYPE float
#define FTYPE_STR "%f "

void *xmalloc (size_t size);	/* malloc wrapper */

extern char *progname;		/* Program name */
extern int verbose;

#endif
