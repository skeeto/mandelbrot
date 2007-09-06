#include <math.h>

#include "colormap.h"
#include "Image.h"

/* The colormap */
/* *INDENT-OFF* */
int red[]   = { 0, 0,   0,   0,   128, 255, 255, 255, 0   };
int green[] = { 0, 0,   128, 255, 128, 128, 255, 255, 0   };
int blue[]  = { 0, 255, 255, 128, 0,   0,   128, 255, 255 };
/* *INDENT-ON* */

Rgb colormap (FTYPE val, int it)
{
  Rgb color;

  /* Colormap size */
  int map_len = sizeof (red) / sizeof (int);

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
  int cwidth = 50;
  if (val < cwidth)
    {
      color.red = red[1] - (red[1] - red[0]) * (cwidth - val) / cwidth;
      color.green =
	green[1] - (green[1] - green[0]) * (cwidth - val) / cwidth;
      color.blue = blue[1] - (blue[1] - blue[0]) * (cwidth - val) / cwidth;
      return color;
    }

  val -= cwidth;
  int base = ((int) val / cwidth % (map_len - 2)) + 1;
  double perc = (val - (cwidth * (int) (val / cwidth))) / cwidth;

  color.red = red[base] + (red[base + 1] - red[base]) * perc;
  color.green = green[base] + (green[base + 1] - green[base]) * perc;
  color.blue = blue[base] + (blue[base + 1] - blue[base]) * perc;

#endif
  return color;
}

void write_colormap (char *filename)
{
  int w = 400;
  int h = 20;
  Image cmap = createImage (w, h);

  int i;
  for (i = 0; i < w; i++)
    imageFillRectRgb (cmap, i, 0, i + 1, h, colormap (i, w));

  saveImage (cmap, filename);
  destroyImage (cmap);
}
