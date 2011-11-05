#ifndef MANDEL_H
#define MANDEL_H

#include "common.h"
#include "Image.h"

FTYPE *gen_mandel (int width, int height,
		   FTYPE xmin, FTYPE xmax,
		   FTYPE ymin, FTYPE ymax, int it);
FTYPE *gen_mandel_p (int width, int height,
		     FTYPE xmin, FTYPE xmax,
		     FTYPE ymin, FTYPE ymax, int it, int jobs);
FTYPE get_val (FTYPE creal, FTYPE cimag, int it);
int write_data (char *basename, FTYPE * img, int w, int h, int it);
void gzip_file (char *filename);

#endif
