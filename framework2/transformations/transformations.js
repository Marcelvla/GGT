/*
 * Student names: Lennart Beekhuis, Marcel van de Lagemaat
 * Student numbers: 11344873, 10886699
 *
 */

function myTranslate(x, y, z) {
    // Translate by x, y and z.
    var mat = [
               1.0, 0.0, 0.0, 0.0,
               0.0, 1.0, 0.0, 0.0,
               0.0, 0.0, 1.0, 0.0,
               x, y, z, 1.0
              ];

   return mat;
}

function myScale(x, y, z) {
    // Scale by x, y and z.
    var mat = [
               x, 0.0, 0.0, 0.0,
               0.0, y, 0.0, 0.0,
               0.0, 0.0, z, 0.0,
               0.0, 0.0, 0.0, 1.0
              ];

   return mat;
}

function orthonormalBase(x, y, z) {
    var aa = Math.hypot(x,y,z);
    var a = [x/aa, y/aa, z/aa];

    var tmp = a;
    tmp[tmp.indexOf(Math.min(a))] = 1;

    var tmpa = Math.cross(tmp, a);
    var tmpaa = Math.hypot(tmpa);
    var u = [tmpa[0] / tmpaa, tmpa[1] / tmpaa, tmpa[2] / tmpaa];

    var v = Math.cross(a, u);

    return [u, v, a];
}

function myRotate(angle, x, y, z) {
    // Rotate by angle around [x, y, z]^T.

    //
    // 1. Create the orthonormal basis
    //
    var base = orthonormalBase(x, y, z);

    //
    // 2. Set up the three matrices making up the rotation
    //
    var A = [
             1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0
            ];

    var B = [
             Math.cos(angle), Math.sin(angle), 0.0, 0.0,
             -Math.sin(angle), Math.cos(angle), 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0
            ];

    var C = [
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x, y, z, 1.0
            ];


    var mat = m4.multiply(A, m4.multiply(B, C));
    return mat;
}
