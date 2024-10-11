import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";
import HyperScene from "./HyperScene";

const loader = new GLTFLoader();

export const loadObj = async (url) => {
  return new Promise((res, rej) => {
    loader.load(
      url,
      (gltf) => {
        res(gltf.scene);
      },
      undefined,
      rej
    );
  });
};

export const fetchMap = async (url) => {
  const response = await fetch(url);
  const map = await response.json();
  return map;
};

/**
 *
 * @param {*} world
 * @param {HyperScene} hyperscene
 */
export const loadMap = async (world, hyperscene) => {
  for (const { path, tile } of world.model) {
    const tileObj = await loadObj(`./assets/tiles/${tile}.glb`);
    hyperscene.addTile(path, tileObj);
  }
};

/* world.entities.forEach(({type, origin, sequence, angle}) => {
    switch(type) {
        case "train":
            const railway = new Railway(new TurtleReader(sequence), origin, world.nb_squares, angle);
            railway.createRails(hyperscene);
        break;
        default:
            console.error("Unknown entity type:", type);
        break;
    }
}); */
