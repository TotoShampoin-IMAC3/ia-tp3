import * as THREE from "three";

const mx = new THREE.Matrix4();
const my = new THREE.Matrix4();
const mz = new THREE.Matrix4();

export const hyperTranslationMatrix = (x, y, z) => {
  const cx = Math.cosh(x),
    sx = Math.sinh(x);
  const cy = Math.cosh(y),
    sy = Math.sinh(y);
  const cz = Math.cosh(z),
    sz = Math.sinh(z);
  mx.set(cx, 0, 0, sx, 0, 1, 0, 0, 0, 0, 1, 0, sx, 0, 0, cx);
  my.set(1, 0, 0, 0, 0, cy, 0, sy, 0, 0, 1, 0, 0, sy, 0, cy);
  mz.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, cz, sz, 0, 0, sz, cz);
  return new THREE.Matrix4().multiplyMatrices(mx, my).multiply(mz);
};

export const rotationMatrix = (x, y, z) => {
  const rotationX = new THREE.Matrix4().makeRotationX(x);
  const rotationY = new THREE.Matrix4().makeRotationY(y);
  const rotationZ = new THREE.Matrix4().makeRotationZ(z);
  return new THREE.Matrix4()
    .multiplyMatrices(rotationX, rotationY)
    .multiply(rotationZ);
};

export const hyperTranslateTile = (path = "", move_factor) => {
  const m = new THREE.Matrix4().identity();
  for (const l of path) {
    switch (l) {
      case "U":
        m.multiply(hyperTranslationMatrix(0, -move_factor, 0));
        break;
      case "D":
        m.multiply(hyperTranslationMatrix(0, move_factor, 0));
        break;
      case "L":
        m.multiply(hyperTranslationMatrix(move_factor, 0, 0));
        break;
      case "R":
        m.multiply(hyperTranslationMatrix(-move_factor, 0, 0));
        break;
      case "T":
        m.multiply(hyperTranslationMatrix(0, 0, move_factor));
        break;
      case "B":
        m.multiply(hyperTranslationMatrix(0, 0, -move_factor));
        break;
      case "1":
        m.multiply(new THREE.Matrix4().makeRotationZ(Math.PI / 2));
        break;
      case "2":
        m.multiply(new THREE.Matrix4().makeRotationX(Math.PI / 2));
        break;
    }
  }
  return m;
};

export const HyperProject = (projection, x, y, z, w) => {
  switch (projection) {
    case "B":
    default:
      return [x / w, y / w, z / w];
    case "P":
      return [x / (w + 1), y / (w + 1), z / (w + 1)];
    case "G":
      return [x, y, z];
  }
};

export const square_size = (nb_squares) => {
  const angle = Math.PI / nb_squares;
  const leg = Math.cos(Math.PI / 4) / Math.sin(angle);
  const edge = 2 * Math.acosh(leg);
  return Math.acosh(Math.cosh(edge) * Math.cosh(edge / 2));
};

export const square_eucl_size = (nb_squares) => {
  const angle = Math.PI / nb_squares;
  const leg = Math.pow(Math.cos(angle), 2) - 0.5;
  return Math.sqrt(leg) / Math.cos(angle);
};

export const square_distance = (nb_squares) => {
  const angle = Math.PI / nb_squares;
  const leg = Math.cos(angle) / Math.sin(Math.PI / 4);
  return 2 * Math.acosh(leg);
};
