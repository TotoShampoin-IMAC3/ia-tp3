import * as THREE from "three";
import HyperObject from "./HyperObject";
import HyperTile from "./HyperTile";

const disposeObject = (obj) => {
  if (obj instanceof THREE.Mesh) {
    obj.geometry.dispose();
    obj.material.dispose();
  }
  if (obj instanceof THREE.Group) {
    obj.children.forEach((child) => disposeObject(child));
  }
};

export default class HyperScene {
  constructor(squares_at_a_vertex) {
    this.squares_at_a_vertex = squares_at_a_vertex;
    this.scene = new THREE.Group();
    this.tiles = [];
    this.objects = [];
    this.camera = new THREE.Matrix4();
    this.projection = "B";
  }

  addTile(path, object) {
    this.tiles.push(new HyperTile(path, object));
    this.scene.add(object);
  }

  addObject(object, id = "") {
    const obj = new HyperObject(object, id);
    this.objects.push(obj);
    this.scene.add(object);
    return obj;
  }

  resetMap() {
    disposeObject(this.scene);
    this.scene.children = [];
    this.tiles = [];
    this.objects = [];
    this.camera = new THREE.Matrix4();
  }

  removeObject(object) {
    this.scene.remove(object.object);
    this.objects.splice(this.objects.indexOf(object), 1);
  }

  findObject(id) {
    return this.objects.find((obj) => obj.id === id);
  }

  render() {
    const camera = this.camera.clone().invert();
    this.tiles.forEach((tile) =>
      tile.hyperbolize(camera, this.squares_at_a_vertex, this.projection)
    );
    this.objects.forEach((obj) =>
      obj.hyperbolize(camera, this.squares_at_a_vertex, this.projection)
    );
  }
}

//# sourceURL=webpack://hyperchat/./src/Modules/HyperModules/HyperScene.ts?
