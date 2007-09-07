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

#ifndef BMP_FORMAT_H
#define BMP_FORMAT_H

typedef struct
{
  unsigned short int type;
  unsigned int size;
  unsigned short int reserved1, reserved2;
  unsigned int offset;
} __attribute__ ((__packed__)) HEADER;

typedef struct
{
  unsigned int size;
  int width, height;
  unsigned short int planes;
  unsigned short int bits;
  unsigned int compression;
  unsigned int imagesize;
  int xresolution, yresolution;
  unsigned int ncolours;
  unsigned int importantcolours;
} __attribute__ ((__packed__)) INFOHEADER;

#endif
