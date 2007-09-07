#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/* The fractal data type */
#define FTYPE float
#define FTYPE_STR "%f "

void *xmalloc (size_t size);	/* malloc wrapper */

extern char *progname;		/* Program name */
extern int verbose;		/* extra output */
extern int gzip_output;		/* gzip output */

/* Parameters */
extern int width;		/* Image width */
extern int height;		/* Image height */
extern double xmin;		/* Real mininum value */
extern double xmax;		/* Real maximum value */
extern double ymin;		/* Imaginary mininum value */
extern double ymax;		/* Imaginary maximum value */
extern int it;			/* Iterations per pixel */
extern int jobs;		/* Jobs per image */

/* Zoom */
extern int zoom_it;		/* Number of zoom frames */
extern double zoom_rate;	/* Rate of zoom */
extern int zoom_jobs;		/* Number of zoom jobs */
extern double zoomx;		/* Zoom real position */
extern double zoomy;		/* Zoom imaginary position */

#endif
