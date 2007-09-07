/* Copyright (C) 2007  Andy Owen
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Andy Owen: andy-<YOUR NAME HERE>@ultra-premium.com
 * http://ultra-premium.com/b
 */

#include <stdio.h>
#include <stdlib.h>

// Comment this out to hear about when we clip values in the transforming from YUV->RGB
#define QUIET

static int yuvToRgb (int y, int u, int v, int *r, int *g, int *b);

void readYuv (char *filename, int width, int height, unsigned char **data)
{
  FILE *in = fopen (filename, "r");
  int clipped = 0;

  // Make room
  (*data) =
    (unsigned char *) malloc (width * height * 3 * sizeof ((*data)[0]));
  if (*data == NULL)
    {
      return;
    }

  //int yDataSize = width * height;
  //int uvDataSize = width * height / 2;

  // Read the data
  unsigned char yData[height][width];
  unsigned char uData[height / 2][width / 2];
  unsigned char vData[height / 2][width / 2];

  fread (&(yData[0][0]), 1, width * height, in);
  fread (&(vData[0][0]), 1, width * height / 4, in);
  fread (&(uData[0][0]), 1, width * height / 4, in);

  // Convert to RGB space
  int y, x;
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
	{
	  int yy = yData[y][x];
	  int uu;
	  int vv;

/*
			// Deinterlacing (with bonus accessing of illegal memory)
			if (y & 1) {
				// We have u info
				uu = uData[y / 2][x / 2];
				vv = (vData[(y - 1) / 2][x / 2] + vData[(y - 1) / 2][x / 2]) / 2;
			}
			else {
				// We have v info
				uu = (uData[(y + 1) / 2][x / 2] + uData[(y + 1) / 2][x / 2]) / 2;
				vv = vData[y / 2][x / 2];
			}
*/

	  vv = vData[y / 2][x / 2];
	  uu = uData[y / 2][x / 2];

	  int r, g, b;
	  clipped += yuvToRgb (yy, uu, vv, &r, &g, &b);

	  (*data)[((y * width) + x) * 3 + 0] = r;	//yData[y][x];
	  (*data)[((y * width) + x) * 3 + 1] = g;	//yData[y][x];
	  (*data)[((y * width) + x) * 3 + 2] = b;	//yData[y][x];
	}
    }

  fclose (in);

#ifndef QUIET
  if (clipped > 0)
    {
      fprintf (stderr,
	       "%d pixels can't properly be represented in RGB space for image '%s'\n",
	       clipped, filename);
    }
#endif
}

static int yuvToRgb (int y, int u, int v, int *r, int *g, int *b)
{
  y -= 16;
  u -= 128;
  v -= 128;
  float intensity = 1.164 * y;
  (*r) = (int) (intensity + 1.596 * v);
  (*g) = (int) (intensity + -0.392 * u + -0.813 * v);
  (*b) = (int) (intensity + 2.017 * u);

  int clipped = 0;
  if (*r < 0)
    {
      clipped = 1;
      *r = 0;
    }
  if (*g < 0)
    {
      clipped = 1;
      *g = 0;
    }
  if (*b < 0)
    {
      clipped = 1;
      *b = 0;
    }

  if (*r > 255)
    {
      clipped = 1;
      *r = 255;
    }
  if (*g > 255)
    {
      clipped = 1;
      *g = 255;
    }
  if (*b > 255)
    {
      clipped = 1;
      *b = 255;
    }

  return clipped;
}
