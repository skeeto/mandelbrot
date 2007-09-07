/* mandelbrot parallel fractal generator
 * Copyright (C) 2007 Christopher Wellons <ccw129@psu.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parse.h"
#include "common.h"
#include "colormap.h"

int *load_array (char *list);

/* Load configuration */
void load_config (FILE * infile)
{
  if (infile == NULL)
    {
      fprintf (stderr, "%s: failed to read config file - %s\n",
	       progname, strerror (errno));
      exit (EXIT_FAILURE);
    }

  int comment = 0;
  int lineno = 1;

  /* Prepare buffer */
  char *buffer, *ptr;
  size_t buffer_len = 128;
  buffer = ptr = xmalloc (buffer_len);
  *buffer = 0;

  while (!feof (infile))
    {
      char c = getc (infile);
      /* End of line */
      if (c == 10 || c == 13)
	{
	  *ptr = 0;
	  int result = process_line (buffer);
	  if (result != 0)
	    {
	      fprintf (stderr, "%s:%d: config parse error\n",
		       progname, lineno);
	      exit (EXIT_FAILURE);
	    }
	  *buffer = 0;
	  ptr = buffer;
	  lineno++;
	  comment = 0;
	  continue;
	}

      if (comment)
	continue;

      if (c == '#')
	{
	  comment = 1;
	  continue;
	}

      *ptr = c;
      ptr++;
      if (ptr - buffer > (int) buffer_len - 4)
	{
	  size_t off = ptr - buffer;
	  buffer_len *= 2;
	  buffer = realloc (buffer, buffer_len);
	  if (buffer == NULL)
	    {
	      fprintf (stderr, "%s: realloc failed\n", progname);
	      exit (EXIT_FAILURE);
	    }
	  ptr = buffer + off;
	}
    }
}

/* Parse a single line */
int process_line (char *line)
{
  char *base = line;

  /* trim whitespace */
  while (*base == ' ')
    base++;

  if (*base == 0)
    return 0;

  char *ptr = base;
  while (*ptr != '=' && *ptr != ' ' && *ptr != 0)
    ptr++;

  if (*ptr == 0)
    return 1;
  *ptr = 0;

  /* Now find value */
  ptr++;
  while (*ptr == '=' || *ptr == ' ')
    ptr++;

  if (*ptr == 0)
    return 1;
  char *val = ptr;

  /* Match keyword */
  if (strcmp (base, "xmin") == 0)
    xmin = strtod (val, NULL);
  else if (strcmp (base, "xmax") == 0)
    xmax = strtod (val, NULL);
  else if (strcmp (base, "ymin") == 0)
    ymin = strtod (val, NULL);
  else if (strcmp (base, "ymax") == 0)
    ymax = strtod (val, NULL);
  else if (strcmp (base, "zoomx") == 0)
    zoomx = strtod (val, NULL);
  else if (strcmp (base, "zoomy") == 0)
    zoomy = strtod (val, NULL);
  else if (strcmp (base, "iterations") == 0)
    it = atoi (val);
  else if (strcmp (base, "image_width") == 0)
    width = atoi (val);
  else if (strcmp (base, "image_height") == 0)
    height = atoi (val);
  else if (strcmp (base, "image_jobs") == 0)
    jobs = atoi (val);
  else if (strcmp (base, "zoom_jobs") == 0)
    zoom_jobs = atoi (val);
  else if (strcmp (base, "zoom_frames") == 0)
    zoom_it = atoi (val);
  else if (strcmp (base, "zoom_rate") == 0)
    zoom_rate = strtod (val, NULL);
  else if (strcmp (base, "color_width") == 0)
    cwidth = atoi (val);
  else if (strcmp (base, "red") == 0)
    {
      red = load_array (val);
      if (red == NULL)
	return 1;
    }
  else if (strcmp (base, "green") == 0)
    {
      green = load_array (val);
      if (green == NULL)
	return 1;
    }
  else if (strcmp (base, "blue") == 0)
    {
      blue = load_array (val);
      if (blue == NULL)
	return 1;
    }
  else
    return 1;

  return 0;
}

/* Load an array for a colormap */
int *load_array (char *list)
{
  int *c = xmalloc (cmap_len * sizeof (int));
  int ci = 0;

  char *ptr = list;
  while (*ptr != '{' && *ptr != 0)
    ptr++;
  if (*ptr == 0)
    return NULL;

  ptr++;
  while (*ptr != 0)
    {
      if (*ptr == '}')
	break;

      c[ci] = atoi (ptr);
      ci++;
      if (ci > cmap_len)
	{
	  /* Badly formed colormap */
	  if (cmap_edit)
	    return NULL;

	  cmap_len *= 2;
	  c = realloc (c, cmap_len);
	  if (c == NULL)
	    {
	      fprintf (stderr, "%s: cmap realloc failed - %s\n",
		       progname, strerror (errno));
	      exit (EXIT_FAILURE);
	    }
	}

      while (*ptr != ',' && *ptr != 0)
	ptr++;
      ptr++;
    }

  /* Make sure we read in enough values. */
  if (cmap_edit && cmap_len != ci)
    return NULL;

  cmap_len = ci;
  cmap_edit = 1;
  return c;
}
