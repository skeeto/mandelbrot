#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/* The fractal data type */
#define FTYPE long double
#define FTYPE_STR "%Lf "

void *xmalloc (size_t size);	/* malloc wrapper */

extern char *progname;		/* Program name */
extern int verbose;		/* extra output */
extern int gzip_output;		/* gzip output */

/* Parameters */
extern int width;		/* Image width */
extern int height;		/* Image height */
extern FTYPE xmin;		/* Real mininum value */
extern FTYPE xmax;		/* Real maximum value */
extern FTYPE ymin;		/* Imaginary mininum value */
extern FTYPE ymax;		/* Imaginary maximum value */
extern int it;			/* Iterations per pixel */
extern int jobs;		/* Jobs per image */

/* Zoom */
extern int zoom_it;		/* Number of zoom frames */
extern FTYPE zoom_rate;	/* Rate of zoom */
extern int zoom_jobs;		/* Number of zoom jobs */
extern FTYPE zoomx;		/* Zoom real position */
extern FTYPE zoomy;		/* Zoom imaginary position */

#endif
