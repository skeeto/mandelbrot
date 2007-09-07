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
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "mandel.h"
#include "common.h"
#include "colormap.h"
#include "parse.h"

/* Program name */
char *progname;
char *version = "0.1-alpha";

int verbose = 1;
int gzip_output = 0;

/* Parameters */
int width = 800;
int height = 600;
double xmin = -2.5;
double xmax = 1.5;
double ymin = -1.5;
double ymax = 1.5;
int it = 256;
int jobs = 1;

/* Zoom */
int zoom_it = 1;
double zoom_rate = 0.1;
int zoom_jobs = 2;
double zoomx = -1.268794803623;
double zoomy = 0.353676833206;

void print_version ()
{
  printf ("mandelgen, version %s.\n", version);
  printf ("Copyright (C) 2007 Chris Wellons\n");
  printf ("This is free software; see the source code ");
  printf ("for copying conditions.\n");
  printf ("There is ABSOLUTELY NO WARRANTY; not even for ");
  printf ("MERCHANTIBILITY or\nFITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

/* program usage information */
void print_usage (int ret)
{
  printf ("Usage: %s [options] CONFIG_FILE\n\nOptions:\n\n", progname);
  printf ("\t-c           Create a colormap image\n");
  printf ("\t-z           Zip output files\n");
  printf ("\t-q           Quiet. Only output errors\n");
  printf ("\t-v           Print version info\n");
  printf ("\t-h           Print this usage text\n");
  exit (ret);
}

int main (int argc, char **argv)
{
  progname = argv[0];

  int c;
  while ((c = getopt (argc, argv, "qzvhc")) != -1)
    switch (c)
      {
      case 'c':		/* colormap */
	write_colormap ("cmap.bmp");
	break;
      case 'z':		/* gzip output */
	gzip_output = 1;
	break;
      case 'q':		/* quiet */
	verbose = 0;
	break;
      case 'h':		/* print help */
	print_usage (EXIT_SUCCESS);
	break;
      case 'v':		/* version */
	print_version ();
	break;
      case '?':		/* bad argument */
	print_usage (EXIT_FAILURE);
      }

  if (argc - optind > 1)
    {
      fprintf (stderr, "%s: too many config files\n", progname);
      print_usage (EXIT_FAILURE);
    }

  FILE *infile;
  if (argc - optind == 1)
    {
      infile = fopen (argv[optind], "r");
      load_config (infile);
      fclose (infile);
    }
  else
    {
      load_config (stdin);
    }

  if (verbose)
    print_version ();

  if (zoom_jobs > zoom_it)
    zoom_jobs = zoom_it;

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
	      continue;
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
  if (verbose)
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
