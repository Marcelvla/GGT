/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name .... Marcel van de Lagemaat, Lennart Beekhuis
 * Student email ... marcelvdla@gmail.com, lennart.beekhuis@student.uva.nl
 * Collegekaart .... 10886699, 11344873
 * Date ............ 7/12/208
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */

int triangles_created;

static vec3
interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2, unsigned char v1, unsigned char v2)
{
    /* Initially, simply return the midpoint between p1 and p2.
       So no real interpolation is done yet */

    return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

   Note: the output array "triangles" should have space for at least
         2 triangles.
*/

static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
    triangle t1, t2;

    int flag0 = c.value[v0] > isovalue ? 1 : 0;
    int flag1 = c.value[v1] > isovalue ? 1 : 0;
    int flag2 = c.value[v2] > isovalue ? 1 : 0;
    int flag3 = c.value[v3] > isovalue ? 1 : 0;

    int corner = flag0 + 2 * flag1 + 4 * flag2 + 8 * flag3;

    if (corner == 15 || corner == 0) {
        return 0;
    }

    if (corner == 1 || corner == 14) {
        t1.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        t1.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        t1.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
    }

    if (corner == 2 || corner == 13) {
        t1.p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
        t1.p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        t1.p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
    }

    if (corner == 4 || corner == 11) {
        t1.p[0] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
        t1.p[1] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
        t1.p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
    }

    if (corner == 8 || corner == 7) {
        t1.p[0] = interpolate_points(isovalue, c.p[v3], c.p[v0], c.value[v3], c.value[v0]);
        t1.p[1] = interpolate_points(isovalue, c.p[v3], c.p[v1], c.value[v3], c.value[v1]);
        t1.p[2] = interpolate_points(isovalue, c.p[v3], c.p[v2], c.value[v3], c.value[v2]);
    }

    if (corner == 3 || corner == 12) {
        t1.p[0] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        t1.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
        t1.p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);

        t2.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        t2.p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        t2.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
    }

    if (corner == 5 || corner == 10) {
        t1.p[0] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        t1.p[1] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
        t1.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);

        t2.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        t2.p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
        t2.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);
    }

    if (corner == 6 || corner == 9) {
        t1.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
        t1.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        t1.p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);

        t2.p[0] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
        t2.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
        t2.p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
    }

    if (t2 == NULL) {
        *triangle[sizeof(struct triangle) * triangles_created] = t1;
        return 1;
    } else {
        *triangle[sizeof(struct triangle) * triangles_created] = t1;
        *triangle[sizeof(struct triangle) * (triangles_created + 1)] = t2;
        return 2;
    }
}

/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
*/

int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
    triangles_created = 0;
    *triangles = malloc(sizeof(struct triangle)*12);

    // make different arrays which we combine into one triangle array

    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 1, 3, 7);
    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 1, 5, 7);
    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 2, 6, 7);
    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 2, 3, 7);
    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 4, 5, 7);
    triangles_created += generate_tetrahedron_triangles(*triangles, isovalue, c, 0, 4, 6, 7);

    return triangles_created;
}
