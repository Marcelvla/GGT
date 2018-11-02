/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
 * Created by ...... Paul Melis
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"

float f(int x0, int y0, int x1, int y1, int x, int y) {
   return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
 }

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */

void
draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
  float alpha, beta, gamma;
  float f_a = f(x1, y1, x2, y2, x0, y0);
  float f_b = f(x2, y2, x0, y0, x1, y1);
  float f_c = f(x0, y0, x1, y1, x2, y2);

  // determine xMin, xMax, yMin, yMax
  int xMin = x0, xMax = x2, yMin = y0, yMax = y2;

  if (x1 < xMin) xMin = x1;
  if (x2 < xMin) xMin = x2;
  if (x1 > xMax) xMax = x1;
  if (x0 > xMax) xMax = x0;
  if (y1 < yMin) yMin = y1;
  if (y2 < yMin) yMin = y2;
  if (y1 > yMax) yMax = y1;
  if (y0 > yMax) yMax = y0;

  // algorithm almost literally copied from the book chapter
  for (int y=yMin; y<=yMax; y++) {
    for (int x=xMin; x<=xMax; x++) {

      alpha = f(x1, y1, x2, y2, x, y) / f_a;
      beta = f(x2, y2, x0, y0, x, y) / f_b;
      gamma = f(x0, y0, x1, y1, x, y) / f_c;

      if (alpha >= 0 && beta >= 0 && gamma >= 0) {
        if ((alpha > 0 || f_a*f(x1, y1, x2, y2, -1, -1) > 0) &&
            (beta > 0 || f_b*f(x2, y2, x0, y0, -1, -1) > 0) &&
            (gamma > 0 || f_c*f(x0, y0, x1, y1, -1, -1) > 0)) {
          PutPixel(x, y, r, g, b);
        }
      }
    }
  }
}


// NOTE: optimized version is working but there's a bug in it: some extra pixels
// get displayed while they shouldn't. We left it in because the biggest part of
// the optimalisation is done within the part that causes the bug (we think).

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
  float t_alpha, t_beta, t_gamma;

  float f_a = f(x1, y1, x2, y2, x0, y0);
  float f_b = f(x2, y2, x0, y0, x1, y1);
  float f_c = f(x0, y0, x1, y1, x2, y2);

  float alpha_check = f(x1, y1, x2, y2, -1, -1);
  float beta_check = f(x2, y2, x0, y0, -1, -1);
  float gamma_check = f(x0, y0, x1, y1, -1, -1);

  // determine xMin, xMax, yMin, yMax
  int xMin = x0, xMax = x2, yMin = y0, yMax = y2;

  if (x1 < xMin) xMin = x1;
  if (x2 < xMin) xMin = x2;
  if (x1 > xMax) xMax = x1;
  if (x0 > xMax) xMax = x0;
  if (y1 < yMin) yMin = y1;
  if (y2 < yMin) yMin = y2;
  if (y1 > yMax) yMax = y1;
  if (y0 > yMax) yMax = y0;

  // pre-determine alpha, beta, gamma and incremental values
  float alpha = f(x1, y1, x2, y2, xMin, yMin) / f_a;
  float beta = f(x2, y2, x0, y0, xMin, yMin) / f_b;
  float gamma = f(x0, y0, x1, y1, xMin, yMin) / f_c;

  float alpha_yInc = (x2 - x1) / f_a;
  float beta_yInc = (x0 - x2) / f_b;
  float gamma_yInc = (x1 - x0) / f_c;

  float alpha_xInc = (y1 - y2) / f_a;
  float beta_xInc = (y2 - y0) / f_b;
  float gamma_xInc = (y0 - y1) / f_c;

  for (int y=yMin; y<=yMax; y++) {

    // make temporary values so that y doesn't increment too many times
    t_alpha = alpha;
    t_beta = beta;
    t_gamma = gamma;

    for (int x=xMin; x<=xMax; x++) {
      // checks like this look ugly but it's better optimalisation-wise
      if (t_alpha >= 0) {
        if (t_beta >= 0) {
          if (t_gamma >= 0) {
            if (t_alpha > 0 || f_a*alpha_check > 0) {
              if (t_beta > 0 || f_b*beta_check > 0) {
                if (t_gamma > 0 || f_c*gamma_check > 0) {
                  PutPixel(x, y, r, g, b);
                }
              }
            }
          }
        }
      }
      t_alpha += alpha_xInc;
      t_beta += beta_xInc;
      t_gamma += gamma_xInc;
    }
    alpha += alpha_yInc;
    beta += beta_yInc;
    gamma += gamma_yInc;
  }
}
