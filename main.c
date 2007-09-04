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
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

#include "mandel.h"
#include "common.h"
#include "colormap.h"

/* Program name */
char *progname;

int verbose = 1;

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
  int jobs = 4;

  /* Zoom */
  int zoom_it = 1;		/* zoom iterations */
  double zoom_rate = 0.25;	/* image reduction per step */
  int zoom_jobs = 1;
  double zoomx = -0.745;
  double zoomy = 0.1;

  int zi = 0;
  int zj = 0;
  while (zi < zoom_it)
    {
      /* Spin off jobs */
      while (zj < zoom_jobs)
	{
	  if (verbose)
	    printf ("Generating image: %d\n", zi);

	  /* Adjust image location */
	  if (zi > 0)
	    {
	      double xrange = xmax - xmin;
	      double yrange = ymax - ymin;

	      if (fabs (xmin + xrange / 2 - zoomx) <
		  (zoom_rate * xrange * 0.5))
		{
		  xmin +=
		    (zoomx - (xmin + xrange / 2 - xrange * zoom_rate * 0.5));
		  xmax -=
		    ((xmax - xrange / 2 + xrange * zoom_rate * 0.5) - zoomx);
		}
	      else if (zoomx > xmin + xrange / 2)
		xmin += xrange * zoom_rate;
	      else
		xmax -= xrange * zoom_rate;

	      if (fabs (ymin + yrange / 2 - zoomy) <
		  (zoom_rate * yrange * 0.5))
		{
		  ymin +=
		    (zoomy - (ymin + yrange / 2 - yrange * zoom_rate * 0.5));
		  ymax -=
		    ((ymax - yrange / 2 + yrange * zoom_rate * 0.5) - zoomy);
		}
	      else if (zoomy > ymin + yrange / 2)
		ymin += yrange * zoom_rate;
	      else
		ymax -= yrange * zoom_rate;
	    }

	  /* Now actually spin off the process */
	  pid_t child_pid = fork ();

	  if (child_pid < 0)
	    fprintf (stderr, "%s: failed to fork zoom child\n", progname);

	  if (child_pid > 0)
	    {
	      zi++;
	      zj++;
	      break;
	    }

	  if (zi > 0)
	    it *= (1 + zoom_rate * .25 * zi);

	  /* Image description */
	  if (verbose)
	    {
	      printf ("Image : %dx%d, %d iterations, %d jobs\n",
		      width, height, it, jobs);
	      printf ("Data  : x => [%f, %f], y => [%f, %f]\n",
		      xmin, xmax, ymin, ymax);
	    }

	  FTYPE *img;		/* The fractal */

	  /* Generate fractal */
	  if (verbose)
	    printf ("Generating fractal ...\n");
	  if (jobs > 1)
	    img = gen_mandel_p (width, height,
				xmin, xmax, ymin, ymax, it, jobs);
	  else
	    img = gen_mandel (width, height, xmin, xmax, ymin, ymax, it);

	  /* Write out data */
	  if (verbose)
	    printf ("Writing data ...\n");
	  char filename[20];
	  snprintf (filename, 20, "out-%010d", zi);
	  write_data (filename, img, width, height, it);

	  free (img);
	  exit (EXIT_SUCCESS);
	}

      wait (NULL);
      zj--;
    }
  printf ("Completed %d frames.\n", zoom_it);

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
