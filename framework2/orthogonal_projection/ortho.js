/*
 * Student Names: Marcel van de Lagemaat, Lennart Beekhuis
 * Student Numbers: 10886699, 11344873
 *
 */

function myOrtho(l, r, b, t, n, f) {

    var mat = [
      2/(r-l), 0.0, 0.0, 0.0,
      0.0, 2/(t-b), 0.0, 0.0,
      0.0, 0.0, -2/(f-n), 0.0,
      -(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1
    ]
    return mat;
}
