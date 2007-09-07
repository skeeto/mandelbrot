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

#include "pnmReader.h"

void readPnm (char *filename, int *width, int *height, unsigned char **data)
{
  FILE *in = fopen (filename, "r");

  // Read main header
  char headerP, header6;
  fscanf (in, "%c%c", &headerP, &header6);

  if (headerP != 'P' || header6 != '6')
    {
      (*data) = NULL;
      (*width) = -1;
      (*height) = -1;
      return;
    }

  // Read width and height        
  fscanf (in, "%d %d", width, height);

  // Read max intensity
  int maxIntensity;
  fscanf (in, "%d", &maxIntensity);

  if (maxIntensity != 255)
    {
      (*data) = NULL;
      (*width) = -1;
      (*height) = -1;
    }

  // Make room
  (*data) =
    (unsigned char *) malloc ((*width) * (*height) * 3 * sizeof ((*data)[0]));
  if (*data == NULL)
    {
      (*width) = -1;
      (*height) = -1;
      return;
    }

  fgetc (in);			// Skip newline

  // Read the data
  int x, y;
  for (y = 0; y < (*height); y++)
    {
      for (x = 0; x < (*width); x++)
	{
	  unsigned char red, green, blue;
	  fscanf (in, "%c%c%c", &red, &green, &blue);
	  (*data)[(y * (*width) + x) * 3] = blue;
	  (*data)[(y * (*width) + x) * 3 + 1] = green;
	  (*data)[(y * (*width) + x) * 3 + 2] = red;
	}
    }

  fclose (in);
}
