/*
 * Student Names: Marcel van de Lagemaat, Lennart Beekhuis
 * Student Numbers: 10886699, 11344873
 *
 */

function replace_smallest(v) {
  var m = Math.min.apply(null, v);
  var index = v.indexOf(m);

  if (index !== -1) {
      v[index] = 1;
  }

  return v;
}

function cross(A, B) {

    var x = A[1]*B[2] - A[2]*B[1];
    var y = A[0]*B[2] - A[0]*B[2];
    var z = A[0]*B[1] - A[1]*B[0];

    return [x,y,z];
}

// NOTE: We didn't get this to work, but R_T and the initial transformation
// we think are correct.

function myLookAt(eyeX, eyeY, eyeZ,
                  centerX, centerY, centerZ,
                  upX, upY, upZ) {

    // Initial transformation matrix
    var mat = [
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -eyeX, -eyeY, -eyeZ, 1.0
    ];

    // Compute and normalize cz
    var cz_norm = Math.hypot(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
    var cz = [
      (centerX - eyeX)/cz_norm, (centerY - eyeY)/cz_norm, (centerZ - eyeZ)/cz_norm
    ];

    // Compute and normalize cx
    var up = [
      upX, upY, upZ
    ];
    var cx_temp = cross(up, cz);
    cx_N = Math.hypot(cx_temp[0], cx_temp[1], cx_temp[2]);
    var cx = [cx_temp[0]/cx_N, cx_temp[1]/cx_N, cx_temp[2]/cx_N];

    // Compute cy
    cy = cross(cz, cx);

    var R_T = [
      cx[0],  cx[1],  cx[2],  0.0,
      cy[0],  cy[1],  cy[2],  0.0,
      -cz[0], -cz[1], -cz[2], 0.0,
      0.0,    0.0,    0.0,    1.0
    ];

    var result = m4.multiply(mat, R_T);
    return result;
}
