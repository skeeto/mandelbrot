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

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "Image.h"
#include "bmpWriter.h"

struct img
{
  unsigned char *data;
  int width;
  int height;
};

Image createImage (int width, int height)
{
  Image temp;
  temp = (Image) malloc (sizeof temp[0]);
  temp->width = width;
  temp->height = height;
  temp->data = (unsigned char *) malloc (width * height * 3);

  return temp;
}

Image duplicateImage (Image img)
{
  Image temp;

  temp = (Image) malloc (sizeof temp[0]);
  temp->width = img->width;
  temp->height = img->height;
  temp->data = (unsigned char *) malloc (temp->width * temp->height * 3);
  int i;
  for (i = 0; i < temp->width * temp->height * 3; i++)
    {
      temp->data[i] = img->data[i];
    }

  return temp;
}

void destroyImage (Image img)
{
  free (img->data);
  free (img);
}

void imageFillRectRgb (Image img, int left, int top, int right, int bottom,
		       Rgb rgb)
{
  imageFillRect (img, left, top, right, bottom, rgb.red, rgb.green, rgb.blue);
}

void imageFillRect (Image img, int left, int top, int right, int bottom,
		    int red, int green, int blue)
{
  int x, y;
  for (y = top; y < bottom; y++)
    {
      for (x = left; x < right; x++)
	{
	  imageSetPixel (img, x, y, red, green, blue);
	}
    }
}

void imageSetPixel (Image img, int x, int y, int red, int green, int blue)
{
  img->data[(y * img->width + x) * 3] = blue;
  img->data[(y * img->width + x) * 3 + 1] = green;
  img->data[(y * img->width + x) * 3 + 2] = red;
}

void imageSetPixelRgb (Image img, int x, int y, Rgb rgb)
{
  imageSetPixel (img, x, y, rgb.red, rgb.green, rgb.blue);
}

void saveImage (Image img, char *filename)
{
  writeBmp (filename, img->width, img->height, img->data);
}
