/* Computer Graphics and Game Technology, Assignment Ray-tracing
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip)
{
        return v3_create(1, 0, 0);
}

vec3
shade_matte(intersection_point ip)
{
    float intensity = scene_ambient_light;

    for (int i = 0; i < scene_num_lights; i++) {
        // n x l_i
        vec3 vec_light = v3_subtract(scene_lights[i].position, ip.p);
        vec_light = v3_normalize(vec_light);
        float light_cont = v3_dotprod(ip.n, vec_light) * scene_lights[i].intensity;

        int shadow = shadow_check(v3_add(ip.p, v3_create(0.001, 0.001, 0.001)), vec_light);
        shadow += shadow_check(v3_add(ip.p, v3_create(-0.001, -0.001, -0.001)), vec_light);

        // n x l_i must be bigger than 0, otherwise the light doesn't shine on the surface
        // one of the two shadow checks must fail, otherwise the point is in the shadows
        if (light_cont > 0 && shadow < 2) {
            intensity += light_cont;
        }
    }
    return v3_create(intensity, intensity, intensity);
});

vec3
shade_blinn_phong(intersection_point ip)
{
    float k_d = 0.8;
    float k_s = 0.5;
    float alpha = 50;
    vec3 c_d = v3_create(1,0,0);
    vec3 c_s = v3_create(1,1,1);
    vec3 n = ip.n;

    float cd_component = 0;
    float cs_component = 0;

    for (int i = 0; i < scene_num_lights; i++) {
        // n x l_i
        vec3 vec_light = v3_subtract(scene_lights[i].position, ip.p);
        vec_light = v3_normalize(vec_light);
        float light_cont = v3_dotprod(ip.n, vec_light) * scene_lights[i].intensity;

        int shadow = shadow_check(v3_add(ip.p, v3_create(0.001, 0.001, 0.001)), vec_light);
        shadow += shadow_check(v3_add(ip.p, v3_create(-0.001, -0.001, -0.001)), vec_light);

        // compute h
        vec3 h = v3_normalize(v3_subtract(scene_lights[i].position, ip.i));

        if (shadow < 2) {
            if (light_cont > 0) {
                cd_component += light_cont;
            }
            // add (n*h)^alpha
            cs_component += scene_lights[i].intensity * pow((v3_dotprod(n, h)), alpha);
        }
    }

    vec3 cd_vec = v3_multiply(c_d, scene_ambient_light + k_d * cd_component);
    vec3 cs_vec = v3_multiply(c_s, k_s * cs_component);
    vec3 c_f = v3_add(cd_vec, cs_vec);
    
    return c_f;
}

vec3
shade_reflection(intersection_point ip)
{
    vec3 matte_vector = shade_matte(ip);

    // 2n(i x n) - i
    vec3 r = v3_multiply(v3_multiply(ip.n, 2), v3_dotprod(ip.i, ip.n));
    r = v3_subtract(r, ip.i);

    // shoot two rays with offset to prevent black dots
    vec3 ray1 = v3_add(ip.p, v3_create(0.001, 0.001, 0.001));
    vec3 ray2 = v3_add(ip.p, v3_create(-0.001, -0.001, -0.001));
    vec3 reflection1 = ray_color(ip.ray_level + 1, ray1, r);
    vec3 reflection2 = ray_color(ip.ray_level + 1, ray2, r);
    vec3 reflective_vector = v3_add(v3_multiply(reflection1, 0.5),
                             v3_multiply(reflection2, 0.5));

    return v3_add(v3_multiply(matte_vector, 0.75), v3_multiply(reflective_vector, 0.25));
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
