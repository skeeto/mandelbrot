#ifndef COLORMAP_H
#define COLORMAP_H

#include "common.h"
#include "Image.h"

/* Colormap functions */
Rgb colormap (FTYPE val, int it);
void write_colormap (char *filename);

extern int *red;
extern int *green;
extern int *blue;
extern int cmap_len;
extern int cmap_edit;

extern int cwidth;

#endif
