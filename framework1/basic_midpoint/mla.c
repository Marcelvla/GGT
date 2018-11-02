/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
 *
 * Student name: Marcel van de Lagemaat, Lennart Beekhuis
 * Student email: marcelvdla@gmail.com, lennart.beekhuis@student.uva.nl
 * Collegekaart: 10886699, 11344873
 * Date: 2/11/2018
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include "SDL.h"
#include "init.h"

// Function to calculate first instance of d
int f(int x0, int y0, int x1, int y1, int x, int y) {
    return (y0 - y1) * x + (x1 - x0) * y + x0*y1 - x1 * y0;
}

// Places the pixel on the screen, handles different cases when looping over
// x and y
void placePixel(SDL_Texture *t, Uint32 colour, int x, int y, int oct) {
    if (oct == 1) {
        PutPixel(t, x, y, colour);
    } else {
        PutPixel(t, y, x, colour);
    }
}

// Calculates next value of d
float nextD(float d, int d1, int d2) {
    if (d < 0) {
        return d + d1 + d2;
    } else {
        return d + d2;
    }
}

// Loops over x or y in positive direction, then increments y or x depending
// on the value of d, places pixel and calculates next value for d
void doublepos(SDL_Texture *t, Uint32 colour, int q0, int r0, int q1, int r1, float d, int oct) {
     int r = r0;
     for (int q = q0; q<=q1; q++) {
         placePixel(t, colour, q, r, oct);
         if (d < 0) {
             r++;
         }

         d = nextD(d, q1 - q0, r0 - r1);
     }
}

// Loops over x and y in negative direction, then decrements y or x depending
// on the value of d, places pixel and calculates next value for d
void doubleneg(SDL_Texture *t, Uint32 colour, int q0, int r0, int q1, int r1, float d, int oct) {
    int r = r0;
    for (int q = q0; q>=q1; q--) {
        placePixel(t, colour, q, r, oct);
        if (d < 0) {
            r--;
        }

        d = nextD(d, q0 - q1, r1 - r0);
    }
}

// Loops over x or y in positive direction, decrements the value of y or x
// depending on the value of d, places pixel and calculates next value for d
void posneg(SDL_Texture *t, Uint32 colour, int q0, int r0, int q1, int r1, float d, int oct) {
   int r = r0;
   for (int q = q0; q<=q1; q++) {
       placePixel(t, colour, q, r, oct);
       if (d < 0) {
           r--;
       }

       d = nextD(d, q1 - q0, r1 - r0);
   }
}

// Loops over x or y in negative direction, increments the value of y or x 
// depending on the value of d, places pixel and calculates next value for d
void negpos(SDL_Texture *t, Uint32 colour, int q0, int r0, int q1, int r1, float d, int oct) {
    int r = r0;
    for (int q = q0; q>=q1; q--) {
        placePixel(t, colour, q, r, oct);
        if (d < 0) {
            r++;
        }

        d = nextD(d, q0 - q1, r0 - r1);
    }
}

//  Main function for the midpoint line algorithm.
void mla(SDL_Texture *t, int x0, int y0, int x1, int y1, Uint32 colour) {
    // Make a double of x0 so that m can be calculated as a double,
    // else with an int value e.g. 0.5 becomes 0
	double X = x0;
    double m = (y1 - y0) / (x1 - X);
    float d;

    // check in which octant the line is based on the value of m and if the
    // starting values are higher or lower than the end values of x and y
    // then calculate d for the first time and draw the line.
    if (m < 1 && m >= 0) {
        if (x0 < x1) {
            // OCTANT 1
            d = f(x0, y0, x1, y1, x0 + 1, y0 + 0.5);
            doublepos(t, colour, x0, y0, x1, y1, d, 1);
        } else {
            // OCTANT 5
            d = f(x0, y0, x1, y1, x0 - 1, y0 - 0.5);
            doubleneg(t, colour, x0, y0, x1, y1, d, 1);
        }
    } else if (m >= 1 && m < INFINITY) {
        if (y0 < y1) {
            // OCTANT 2
            d = f(x0, y0, x1, y1, x0 + 0.5, y0 + 1);
            doublepos(t, colour, y0, x0, y1, x1, d, 0);
        } else {
            // OCTANT 6
            d = f(x0, y0, x1, y1, x0 - 0.5, y0 - 1);
            doubleneg(t, colour, y0, x0, y1, x1, d, 0);
        }
    } else if ((m < -1 && m >= -INFINITY) || m == INFINITY) {
        if (y0 < y1) {
            // OCTANT 3
            d = f(x0, y0, x1, y1, x0 - 0.5, y0 + 1);
            posneg(t, colour, y0, x0, y1, x1, d, 0);
        } else {
            // OCTANT 7
            d = f(x0, y0, x1, y1, x0 + 0.5, y0 - 1);
            negpos(t, colour, y0, x0, y1, x1, d, 0);
        }
    } else if (m >= -1 && m <= 0) {
        if (x0 > x1) {
            // OCTANT 4
            d = f(x0, y0, x1, y1, x0 - 1, y0 + 0.5);
            negpos(t, colour, x0, y0, x1, y1, d, 1);
        } else {
            // OCTANT 8
            d = f(x0, y0, x1, y1, x0 + 1, y0 - 0.5);
            posneg(t, colour, x0, y0, x1, y1, d, 1);
        }
    }
}
