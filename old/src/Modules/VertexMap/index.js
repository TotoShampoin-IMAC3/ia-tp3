import * as THREE from "three";

/**
 *
 * @param {THREE.Object3D} object
 */
export const saveInitial = (object) => {
  if (object instanceof THREE.Mesh) {
    const pos = object.position.toArray();
    const rot = object.rotation.toArray();
    const sca = object.scale.toArray();
    const pos_array = object.geometry.getAttribute("position").array;
    const initialPos_hard = [];
    const initialPos_soft = [];
    const newpos = new THREE.Vector3();
    const newrot = new THREE.Matrix4();
    for (let i = 0; i < pos_array.length; i += 3) {
      initialPos_hard.push({
        x: pos_array[i],
        y: pos_array[i + 1],
        z: pos_array[i + 2],
      });
      newpos
        .set(
          pos_array[i] * sca[0],
          pos_array[i + 1] * sca[1],
          pos_array[i + 2] * sca[2]
        )
        .applyMatrix4(newrot.makeRotationFromEuler(new THREE.Euler(...rot)))
        .add(object.position);
      pos_array[i] = newpos.x;
      pos_array[i + 1] = newpos.x;
      pos_array[i + 2] = newpos.x;
      initialPos_soft.push({
        x: newpos.x,
        y: newpos.y,
        z: newpos.z,
      });
    }
    object.position.set(0, 0, 0);
    object.rotation.set(0, 0, 0);
    object.scale.set(1, 1, 1);
    object.frustumCulled = false;
    object.geometry.userData.initialPos_hard = initialPos_hard;
    object.geometry.userData.initialPos_soft = initialPos_soft;
    object.userData.init = { pos, rot, sca };
  }
  if (object instanceof THREE.Group) {
    object.children.forEach((obj) => saveInitial(obj));
    object.position.set(0, 0, 0);
    object.rotation.set(0, 0, 0);
    object.scale.set(1, 1, 1);
    object.frustumCulled = false;
  }
};

/**
 *
 * @param {THREE.Object3D} object
 */
export const softRestore = (object) => {
  if (object instanceof THREE.Mesh) {
    let ini = object.geometry.userData.initialPos_soft;
    let array = object.geometry.getAttribute("position").array;
    for (let i in ini) {
      array[i * 3] = ini[i].x;
      array[i * 3 + 1] = ini[i].y;
      array[i * 3 + 2] = ini[i].z;
    }
  }
  if (object instanceof THREE.Group) {
    object.children.forEach((obj) => softRestore(obj));
  }
};

/**
 *
 * @param {THREE.Object3D} object
 */
export const hardRestore = (object) => {
  if (object instanceof THREE.Mesh) {
    const { pos, rot, sca } = object.userData.init;
    let ini = object.geometry.userData.initialPos_hard;
    let array = object.geometry.getAttribute("position").array;
    object.position.set(...pos);
    object.rotation.set(...rot);
    object.scale.set(...sca);
    for (let i in ini) {
      array[i * 3] = ini[i].x;
      array[i * 3 + 1] = ini[i].y;
      array[i * 3 + 2] = ini[i].z;
    }
  }
  if (object instanceof THREE.Group) {
    object.children.forEach((obj) => hardRestore(obj));
  }
};

/**
 *
 * @param {THREE.Object3D} object
 * @param {(x: number, y: number, z: number) => number[]} convertor
 */
export const vertexMap = (object, convertor = (x, y, z) => [x, y, z]) => {
  if (object instanceof THREE.Mesh) {
    const posArray = object.geometry.getAttribute("position").array;
    const newpos = new THREE.Vector3();
    for (let i = 0; i < posArray.length; i += 3) {
      newpos.set(...convertor(posArray[i], posArray[i + 1], posArray[i + 2]));
      posArray[i] = newpos.x;
      posArray[i + 1] = newpos.y;
      posArray[i + 2] = newpos.z;
    }
    object.geometry.getAttribute("position").needsUpdate = true;
  }
  if (object instanceof THREE.Group) {
    object.children.forEach((obj) => vertexMap(obj, convertor));
  }
};

/**
 *
 * @param {THREE.Object3D} object
 * @param {number} index
 * @returns {THREE.Vector3}
 */
export const getVertex = (object, index) => {
  if (object instanceof THREE.Mesh) {
    return new THREE.Vector3(
      object.geometry.getAttribute("position").array[index * 3],
      object.geometry.getAttribute("position").array[index * 3 + 1],
      object.geometry.getAttribute("position").array[index * 3 + 2]
    );
  }
  if (object instanceof THREE.Group) {
    return getVertex(object.children[0], index);
  }
};

//# sourceURL=webpack://hyperchat/./src/Modules/VertexMap/index.js?
