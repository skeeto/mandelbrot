#ifndef MANDEL_H
#define MANDEL_H

#include "common.h"
#include "Image.h"

FTYPE *gen_mandel (int width, int height,
		   double xmin, double xmax,
		   double ymin, double ymax, int it);
FTYPE *gen_mandel_p (int width, int height,
		     double xmin, double xmax,
		     double ymin, double ymax, int it, int jobs);
FTYPE get_val (double creal, double cimag, int it);
int write_data (FTYPE * img, int w, int h, int it);

#endif
