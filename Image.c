// Copyright (C) 2007  Andy Owen
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as 
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Andy Owen: andy-<YOUR NAME HERE>@ultra-premium.com
// http://ultra-premium.com/b


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "Image.h"
#include "bmpWriter.h"
#include "bmpReader.h"
#include "pnmReader.h"
#include "yuvReader.h"

static int isBmp (char *filename);
static int isPnm (char *filename);
static int isYuv (char *filename);
static int endsWith (char *string, char *endsWith);

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

Image loadImage (char *filename)
{
  Image temp;

  temp = (Image) malloc (sizeof temp[0]);
  temp->data = NULL;

  if (isBmp (filename))
    {
      readBmp (filename, &(temp->width), &(temp->height), &(temp->data));
    }
  else if (isPnm (filename))
    {
      readPnm (filename, &(temp->width), &(temp->height), &(temp->data));
    }
  else if (isYuv (filename))
    {
      // YUV format doesn't store size
      temp->width = 378;
      temp->height = 252;
      readYuv (filename, temp->width, temp->height, &(temp->data));
    }
  else
    {
      fprintf (stderr, "Unknown fileformat: %s\n", filename);
      free (temp);
      temp = NULL;
    }

  return temp;
}

Image loadImageFixedResolution (char *filename, int width, int height)
{
  Image temp;

  temp = (Image) malloc (sizeof temp[0]);
  temp->data = NULL;

  if (isYuv (filename))
    {
      // YUV format doesn't store size
      temp->width = width;
      temp->height = height;
      readYuv (filename, temp->width, temp->height, &(temp->data));
    }
  else
    {
      fprintf (stderr, "Unknown fixed resolution fileformat: %s\n", filename);
      free (temp);
      temp = NULL;
    }

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

void imageGetPixel (Image img, int x, int y, int *red, int *green, int *blue)
{
  (*blue) = img->data[(y * img->width + x) * 3];
  (*green) = img->data[(y * img->width + x) * 3 + 1];
  (*red) = img->data[(y * img->width + x) * 3 + 2];
}

Rgb imageGetPixelRgb (Image img, int x, int y)
{
  int red, green, blue;
  imageGetPixel (img, x, y, &red, &green, &blue);
  Rgb temp;
  temp.red = red;
  temp.green = green;
  temp.blue = blue;
  return temp;
}


void saveImage (Image img, char *filename)
{
  writeBmp (filename, img->width, img->height, img->data);
}

int imageGetWidth (Image img)
{
  assert (img != NULL);
  return img->width;
}

int imageGetHeight (Image img)
{
  assert (img != NULL);
  return img->height;
}

static int endsWith (char *string, char *endsWith)
{
  // Returns 1 if a string ends with another
  char *lookFrom = string;
  char *found;
  int length = strlen (endsWith);


  while (1)
    {
      found = strstr (lookFrom, endsWith);
      if (found == NULL)
	{
	  return 0;
	}
      if (found[length] == '\0')
	{
	  return 1;
	}
      lookFrom = found + 1;
    }
}

static int isBmp (char *filename)
{
  // Check if it ends with a .bmp
  return endsWith (filename, ".bmp");
}

static int isPnm (char *filename)
{
  // Check if it ends with a .pnm
  return endsWith (filename, ".pnm");
}

static int isYuv (char *filename)
{
  // Check if it ends with a .yuv
  return endsWith (filename, ".yuv");
}
