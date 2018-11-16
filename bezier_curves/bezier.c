/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
 *
 * Student name: Lennart Beekhuis, Marcel van de Lagemaat
 * Student email: lennart.beekhuis@student.uva.nl, marcelvdla@gmail.com
 * Collegekaart: 11344873, 10886699
 * Date 16/11/2018
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include "bezier.h"
#include <stdio.h>


/* Function factorial calculates the factorial of number.
 * Returns 1 if number is 0.
 */

int
factorial(int number) {
     if (number == 0) {
         return 1;
     }

     int fac = 1;
     for (int i=1; i<=number; i++) {
         fac *= i;
     }
     return fac;
}

/* Function binom returns binomial of n and k. Casts one value to a float because
 * n and k are both ints and we want the return value to be a float.
 */

float
binom(int n, int k) {
     return ((float) factorial(n))/(factorial(k) * factorial(n-k));
}

 /* Given a Bezier curve defined by the 'num_points' control points
  * in 'p' compute the position of the point on the curve for parameter
  * value 'u'.
  *
  * Return the x and y values of the point by setting *x and *y,
  * respectively.
  */

void
evaluate_bezier_curve(float *x, float *y, control_point p[], int num_points, float u)
{
    *x = 0.0;
    *y = 0.0;
    float B;

    // Loop over num_points and compute x and y value for each control_point
    for (int i = 0; i<num_points; i++) {
        B = binom(num_points-1, i) * (float) pow((1 - u), (num_points - 1 - i)) * pow(u, i);
        *x += B * p[i].x;
        *y += B * p[i].y;
    }
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 *
 * You will have to create a GLfloat array of the right size, filled with the
 * vertices in the appropriate format and bind this to the buffer.
 */

void
draw_bezier_curve(int num_segments, control_point p[], int num_points)
{
    GLuint buffer[1];

    float vertices[(num_segments + 1) * 2];


    // Loop over num_segments and compute x and y for each, then add them
    // into the array.
    for (int i=0; i<=num_segments; i++) {
        float x, y;
        evaluate_bezier_curve(&x, &y, p, num_points, i/((float)num_segments));
        vertices[i * 2] = x;
        vertices[i * 2 + 1] = y;
    }


    // This creates the VBO and binds an array to it.
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_segments + 1) * 2,
                 vertices, GL_STATIC_DRAW);

    // This tells OpenGL to draw what is in the buffer as a Line Strip.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINE_STRIP, 0, num_segments + 1);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, buffer);
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/

int
intersect_cubic_bezier_curve(float *y, control_point p[], float x)
{
    // Check if x is within the first and last control point.
    if (x > p[3].x || x < p[0].x) {
        return 0;
    }

    float X = 1;
    float t = 0.5;
    float step = 0.25;

    // Binary search until we are within the specified boundary.
    while (X-x > 0.001 || X-x < -0.001) {
        evaluate_bezier_curve(&X, y, p, 4, t);
        if (X < x) t += step;
        else t -= step;
        step = step/2;
    }
    return 1;
}
