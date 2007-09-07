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

#include "bmpFormat.h"


static int padOut (int n)
{
  return n + ((4 - (n % 4)) % 4);
}

void readBmp (char *filename, int *width, int *height, unsigned char **data)
{
  FILE *in = fopen (filename, "r");

  // Read main header
  HEADER head;
  fread (&head, sizeof (head), 1, in);
  if (head.type != 0x4d42)
    {
      (*data) = NULL;
      (*width) = -1;
      (*height) = -1;
      return;
    }

  // Read "info header"
  INFOHEADER infoHead;
  fread (&infoHead, sizeof (infoHead), 1, in);

  // Make sure it is a format we understand
  if (infoHead.planes != 1 ||
      infoHead.bits != 24 ||
      infoHead.compression != 0 ||
      infoHead.ncolours != 0 || infoHead.importantcolours != 0)
    {
      (*data) = NULL;
      (*width) = -1;
      (*height) = -1;
      return;
    }

  (*width) = infoHead.width;
  (*height) = infoHead.height;

  long offset = head.offset;

  fseek (in, offset, SEEK_SET);

  // Make room
  (*data) =
    (unsigned char *) malloc ((*width) * (*height) * 3 * sizeof ((*data)[0]));
  if (*data == NULL)
    {
      (*width) = -1;
      (*height) = -1;
      return;
    }

  // Read the data
  int padSize = padOut ((*width) * 3) - ((*width) * 3);
  int y, x;
  for (y = (*height) - 1; y >= 0; y--)
    {
      for (x = 0; x < (*width); x++)
	{
	  fread (&((*data)[(y * (*width) + x) * 3]), 1, 1, in);
	  fread (&((*data)[(y * (*width) + x) * 3 + 1]), 1, 1, in);
	  fread (&((*data)[(y * (*width) + x) * 3 + 2]), 1, 1, in);
	}

      // Skip padding;
      fseek (in, padSize, SEEK_CUR);
    }

  fclose (in);
}
