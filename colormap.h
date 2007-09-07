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

#ifndef COLORMAP_H
#define COLORMAP_H

#include "common.h"
#include "Image.h"

/* Colormap functions */
Rgb colormap (FTYPE val, int it);
void write_colormap (char *filename);

extern int *red;
extern int *green;
extern int *blue;
extern int cmap_len;
extern int cmap_edit;

extern int cwidth;

#endif
