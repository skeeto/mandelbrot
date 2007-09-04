#include "colormap.h"
#include "Image.h"

/* The colormap */
/* *INDENT-OFF* */
int red[]   = { 0, 0,   0,   0,   128, 255, 255, 255 };
int green[] = { 0, 0,   128, 255, 128, 128, 255, 255 };
int blue[]  = { 0, 255, 255, 128, 0,   0,   128, 255 };
/* *INDENT-ON* */

Rgb colormap (FTYPE val, int it)
{
  Rgb color;

  /* Colormap size */
  int map_len = sizeof (red) / sizeof (int);
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
