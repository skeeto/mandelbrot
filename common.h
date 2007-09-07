/* mandelbrot parallel fractal generator
 * Copyright (C) 2007 Christopher Wellons <ccw129@psu.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

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
