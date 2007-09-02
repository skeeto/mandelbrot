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

#ifndef IMAGEH_D
#define IMAGEH_D

typedef struct img *Image;

typedef struct Rgb
{
  int red;
  int green;
  int blue;
} Rgb;


Image createImage (int width, int height);
Image loadImage (char *filename);
Image loadImageFixedResolution (char *filename, int width, int height);
Image duplicateImage (Image img);
void saveImage (Image img, char *filename);
void destroyImage (Image img);

void imageSetPixel (Image img, int x, int y, int red, int green, int blue);
void imageSetPixelRgb (Image img, int x, int y, Rgb rgb);
void imageGetPixel (Image img, int x, int y, int *red, int *green, int *blue);
Rgb imageGetPixelRgb (Image img, int x, int y);

void imageFillRect (Image img, int left, int top, int right, int bottom,
		    int red, int green, int blue);
void imageFillRectRgb (Image img, int left, int top, int right, int bottom,
		       Rgb rgb);
int imageGetWidth (Image img);
int imageGetHeight (Image img);

#endif
