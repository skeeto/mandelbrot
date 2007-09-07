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

#include <math.h>

#include "colormap.h"
#include "Image.h"

/* The colormap */
/* *INDENT-OFF* */
int prered[]   = { 0, 0,   0,   0,   128, 255, 255, 255 };
int pregreen[] = { 0, 0,   128, 255, 128, 128, 255, 255 };
int preblue[]  = { 0, 255, 255, 128, 0,   0,   128, 255 };
/* *INDENT-ON* */
int cmap_len = 8;
int cmap_edit = 0;

int *red = prered;
int *green = pregreen;
int *blue = preblue;

int cwidth = 50;

Rgb colormap (FTYPE val, int it)
{
  Rgb color;

  /* Colormap size */
  int map_len = cmap_len;

#if FLAT_CMAP
  map_len--;
  int trans = map_len - 1;

  /* Base transition */
  int base = val / it * trans;
  if (base >= map_len)
    {
      base = map_len - 1;
      val = it;
    }

  /* Interpolate */
  color.red = red[base]
    + (val - (it / trans) * base) / (it / trans)
    * (red[base + 1] - red[base]);

  color.green = green[base]
    + (val - (it / trans) * base) / (it / trans)
    * (green[base + 1] - green[base]);

  color.blue = blue[base]
    + (val - (it / trans) * base) / (it / trans)
    * (blue[base + 1] - blue[base]);
#else
  (void) it;
  if (val < cwidth)
    {
      color.red = red[1] - (red[1] - red[0]) * (cwidth - val) / cwidth;
      color.green =
	green[1] - (green[1] - green[0]) * (cwidth - val) / cwidth;
      color.blue = blue[1] - (blue[1] - blue[0]) * (cwidth - val) / cwidth;
      return color;
    }

  val -= cwidth;
  int base = ((int) val / cwidth % (map_len - 1)) + 1;
  double perc = (val - (cwidth * (int) (val / cwidth))) / cwidth;
  int top = base + 1;
  if (top >= map_len)
    top = 1;

  color.red = red[base] + (red[top] - red[base]) * perc;
  color.green = green[base] + (green[top] - green[base]) * perc;
  color.blue = blue[base] + (blue[top] - blue[base]) * perc;

#endif
  return color;
}

void write_colormap (char *filename)
{
  int w = cwidth * cmap_len * 1.5;
  int h = 20;
  Image cmap = createImage (w, h);

  int i;
  for (i = 0; i < w; i++)
    imageFillRectRgb (cmap, i, 0, i + 1, h, colormap (i, w));

  saveImage (cmap, filename);
  destroyImage (cmap);
}
