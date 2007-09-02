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


#include <stdio.h>
#include "bmpFormat.h"

static int padOut (int n)
{
  return n + ((4 - (n % 4)) % 4);
}

static void writeHeader (HEADER h, FILE * f)
{
  fwrite (&h, sizeof (h), 1, f);
}

static void writeInfoHeader (INFOHEADER h, FILE * f)
{
  fwrite (&h, sizeof (h), 1, f);
}

void writeBmp (char *filename, int width, int height, unsigned char *data)
{
  FILE *out = fopen (filename, "w");

  // Write main header
  HEADER head;
  head.type = 0x4d42;
  head.size =
    sizeof (HEADER) + sizeof (INFOHEADER) + (padOut (width * 3) * height);
  head.reserved1 = 0;
  head.reserved2 = 0;
  head.offset = sizeof (HEADER) + sizeof (INFOHEADER);

  writeHeader (head, out);

  // Write "info header"
  INFOHEADER infoHead;
  infoHead.size = sizeof (INFOHEADER);
  infoHead.width = width;
  infoHead.height = height;
  infoHead.planes = 1;
  infoHead.bits = 24;
  infoHead.compression = 0;
  infoHead.imagesize = (padOut (width * 3) * height);
  infoHead.xresolution = 0;
  infoHead.yresolution = 0;
  infoHead.ncolours = 0;
  infoHead.importantcolours = 0;

  writeInfoHeader (infoHead, out);

  // Write the data
  int padSize = padOut (width * 3) - (width * 3);
  int y, x;
  for (y = height - 1; y >= 0; y--)
    {
      for (x = 0; x < width; x++)
	{
	  fwrite (&(data[(y * width + x) * 3]), 1, 1, out);
	  fwrite (&(data[(y * width + x) * 3 + 1]), 1, 1, out);
	  fwrite (&(data[(y * width + x) * 3 + 2]), 1, 1, out);
	}

      // Write padding
      //      char pad = '\0';
      int i;
      for (i = 0; i < padSize; i++)
	{
	  fputc (0, out);
//                      fwrite(&pad, 1, 1, out);
	}
    }

  fclose (out);
}
