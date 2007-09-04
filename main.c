/* mandelbrot parallel fractal generator
 * Copyright (C) 2007 Christopher Wellons (mosquitopsu@gmail.com)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "mandel.h"
#include "common.h"
#include "colormap.h"

/* Program name */
char *progname;

int main (int argc, char **argv)
{
  (void) argc;
  progname = argv[0];

  write_colormap ("cmap.bmp");	/* Write out colormap for viewing */

  /* Parameters */
  int width = 800;
  int height = 600;
  double xmin = -2.5;
  double xmax = 1.5;
  double ymin = -1.5;
  double ymax = 1.5;
  int it = 256;
  int jobs = 1;

  /* Image description */
  printf ("Image : %dx%d, %d iterations, %d jobs\n", width, height, it, jobs);
  printf ("Data  : x => [%f, %f], y => [%f, %f]\n", xmin, xmax, ymin, ymax);

  FTYPE *img;			/* The fractal */

  /* Generate fractal */
  printf ("Generating fractal ...\n");
  if (jobs > 1)
    img = gen_mandel_p (width, height, xmin, xmax, ymin, ymax, it, jobs);
  else
    img = gen_mandel (width, height, xmin, xmax, ymin, ymax, it);

  /* Write out data */
  printf ("Writing data ...\n");
  write_data (img, width, height, it);
  free (img);

  exit (EXIT_SUCCESS);
}

void *xmalloc (size_t size)
{
  void *mem = malloc (size);
  if (mem == NULL)
    {
      fprintf (stderr, "%s: malloc failed - %s\n",
	       progname, strerror (errno));
      exit (EXIT_FAILURE);
    }

  return mem;
}
