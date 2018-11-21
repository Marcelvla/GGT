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

function cross(A, B) {

    var x = A[1]*B[2] - A[2] * B[1];
    var y = A[0]*B[2] - A[0] * B[2];
    var z = A[0]*B[1] - A[1] * B[0];

    return [x,y,z];
}

function replace_smallest(v) {
  var m = Math.min.apply(null, v);
  var index = v.indexOf(m);
  console.log(m);

  if (index !== -1) {
      v[index] = 1;
  }

  return v;
}

function orthonormalBase(x, y, z) {
    var aa = Math.hypot(x,y,z);
    var a = [x/aa, y/aa, z/aa];

    var tmp = a.slice();
    tmp = replace_smallest(tmp);

    var tmpa = cross(tmp, a);
    console.log(tmpa);

    var tmpaa = Math.hypot(tmpa[0], tmpa[1], tmpa[2]);
    var u = [tmpa[0] / tmpaa, tmpa[1] / tmpaa, tmpa[2] / tmpaa];

    var v = cross(a, u);

    return [u, v, a];
}

function myRotate(angle, x, y, z) {
    // Rotate by angle around [x, y, z]^T.

    //
    // 1. Create the orthonormal basis
    //
    var base = orthonormalBase(x, y, z);
    // console.log(base[0], base[1], base[2]);

    //
    // 2. Set up the three matrices making up the rotation
    //
    var A = [
             base[0][0], base[0][1], base[0][2], 0.0,
             base[1][0], base[1][1], base[1][2], 0.0,
             base[2][0], base[2][1], base[2][2], 0.0,
             0.0, 0.0, 0.0, 1.0
            ];

    var B = [
             Math.cos(angle), Math.sin(angle), 0.0, 0.0,
             -Math.sin(angle), Math.cos(angle), 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0
            ];

    var C = [
            base[0][0], base[1][0], base[2][0], 0.0,
            base[0][1], base[1][1], base[2][1], 0.0,
            base[0][2], base[1][2], base[2][2], 0.0,
            0.0, 0.0, 0.0, 1.0
            ];


    var mat = m4.multiply(A, m4.multiply(B, C));
    return mat;
}
