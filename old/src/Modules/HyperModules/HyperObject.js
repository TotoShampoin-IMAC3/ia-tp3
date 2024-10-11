import * as THREE from "three";
import * as VertexMap from "../VertexMap";
import * as HyperMaths from "./HyperMaths";

export default class HyperObject {
  constructor(object, id) {
    this.object = object;
    this.id = id;
    this.position = new THREE.Matrix4();
    VertexMap.saveInitial(object);
  }

  hyperbolize(camera, squares_at_a_vertex, projection = "B") {
    VertexMap.softRestore(this.object);
    const eucl_size = HyperMaths.square_eucl_size(squares_at_a_vertex);
    const vert = new THREE.Vector4();
    VertexMap.vertexMap(this.object, (x, y, z) => {
      x *= eucl_size;
      y *= eucl_size;
      z *= eucl_size;
      const d = Math.sqrt(1 - x ** 2 - y ** 2 - z ** 2) || 0.0000001;
      vert.set(x, y, z, 1).divideScalar(d);
      // V = C P V
      vert.applyMatrix4(this.position);
      vert.applyMatrix4(camera);
      return HyperMaths.HyperProject(
        projection,
        vert.x,
        vert.y,
        vert.z,
        vert.w
      );
    });
  }

  getVertex() {
    return VertexMap.getVertex(this.object, 0);
  }
}
