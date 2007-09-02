#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Image.h"

/* The fractal data type */
#define FTYPE double
#define FTYPE_STR "%f "

/* The colormap */
int red[]   = {0, 0  , 0,   0  , 128, 255, 255, 255};
int green[] = {0, 0  , 128, 255, 128, 128, 255, 255};
int blue[]  = {0, 255, 255, 128, 0  , 0  , 128, 255};

double logtwo;

/* Generates given mandelbrot and returns pointer to data */
FTYPE *gen_mandel (double xmin, double xmax, 
		   double ymin, double ymax,
		   double res, int it);

/* Get value at point */
FTYPE get_val (double creal, double cimag, int it);

/* Get color for given value */
Rgb colormap (FTYPE val, int it);

/* Write out the colormap to see what it looks like*/
void write_colormap ();

int main ()
{
  write_colormap ();
  logtwo = log (2); /* Calculate this only once */
  
  /* Parameters */
  double xmin = -2.5;
  double xmax = 1.5;
  double ymin = -1.5;
  double ymax = 1.5;
  double res = 0.0005;
  int it = 256;
  
  /* Output options */
  int write_text = 0;
  int write_bmp = 1;
  
  /* Image dimensions */
  int w = ceil ((xmax - xmin) / res);
  int h = ceil ((ymax - ymin) / res);
  printf ("Image size: %dx%d\n", w, h);

  /* Generate fractal */
  printf ("Start.\n");
  FTYPE *img = gen_mandel (xmin, xmax, ymin, ymax, res, it);
  printf ("Done.\n");

  /* Write out data */
  
  if (write_text)
    {
      FILE *fout = fopen ("out.dat", "w");
      int i, j;
      for (j = 0; j < h; j++)
	{
	  for (i = 0; i < w; i++)
	    fprintf (fout, FTYPE_STR, img[i + j * w]);
	  fprintf (fout, "\n");
	}
      fclose (fout);
    }
  
  if (write_bmp)
    {
      Image bmp = createImage (w, h);
      int i, j;
      for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	  imageSetPixelRgb (bmp, i, j, 
			    colormap (img[i + j * w], it));
      saveImage (bmp, "out.bmp");
      destroyImage (bmp);
    }

  exit (EXIT_SUCCESS);
}

FTYPE *gen_mandel (double xmin, double xmax, double ymin, double ymax,
		   double res, int it)
{
  int w = ceil ((xmax - xmin) / res);
  int h = ceil ((ymax - ymin) / res);

  /* Fractal data */
  FTYPE *img = (FTYPE *) malloc (w * h * sizeof (FTYPE));

  int i, j;
  for (j = 0; j < h; j++)
    for (i = 0; i < w; i++)
      img[i + j * w] = get_val (xmin + i * res, ymin + j * res, it);

  return img;
}

FTYPE get_val (double creal, double cimag, int it)
{
  /* Init Z */
  double zreal = 0;
  double zimag = 0;

  int count = 0;
  double ztemp;
  while ((zreal * zreal + zimag * zimag < 2 * 2) && count < it)
    {
      /* Z = Z^2 + C */
      ztemp = zreal * zreal - zimag * zimag + creal;
      zimag = 2 * zreal * zimag + cimag;
      zreal = ztemp;

      count++;
    }
  if (zreal * zreal + zimag * zimag < 2 * 2)
    return 0;

  /* Smooth coloration */
  FTYPE out = (FTYPE) (count + 1 -
		       log (log (sqrt (zreal * zreal + zimag * zimag))) /
		       logtwo);
  return out;
}

Rgb colormap (FTYPE val, int it)
{
  Rgb color;
  
  /* Colormap size */
  int map_len = sizeof (red) / sizeof (int);
  int trans = map_len - 1;
  
  /* Base transition */
  int base = val/it * trans;
  if (base >= map_len)
    {
      base = map_len - 1;
      val = it;
    }

  /* Interpolate */
  color.red = red[base]
    + (val - (it/trans)*base) / (it/trans)
    * (red[base + 1] - red[base]);
  
  color.green = green[base]
    + (val - (it/trans)*base) / (it/trans)
    * (green[base + 1] - green[base]);
  
  color.blue = blue[base]
    + (val - (it/trans)*base) / (it/trans)
    * (blue[base + 1] - blue[base]);
  
  return color;
}

void write_colormap ()
{
  int w = 400;
  int h = 20;
  Image cmap = createImage (w, h);
  
  int i;
  for (i = 0; i < w; i++)
    imageFillRectRgb (cmap, i, 0, i+1, h, colormap (i, w));
  
  saveImage (cmap, "cmap.bmp");
  destroyImage (cmap);
}
