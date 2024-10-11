import * as THREE from "three";
import { setRenderer } from "./3d/index.ts";
import { input, speed } from "./input/index.ts";
import ChatZone from "./Modules/ChatZone/index.ts";
import { fetchMap, loadMap } from "./Modules/HyperModules/HyperMap.ts";
import {
  hyperTranslationMatrix,
  rotationMatrix,
} from "./Modules/HyperModules/HyperMaths.ts";
import HyperScene from "./Modules/HyperModules/HyperScene.ts";
import { Socket } from "./Modules/Socket/index.ts";
import { openConnectionScreen } from "./Modules/ChatZone/connect.ts";
import Railway from "./Modules/HCEntities/Train/Railway.ts";
import TurtleReader from "./Modules/TurtleReader/index.ts";

const world = await fetchMap("./assets/worlds/planes5.json");
const hyperscene = new HyperScene(world.nb_squares);
loadMap(world, hyperscene);

const railway = new Railway(
  new TurtleReader("L5R3U6"),
  "",
  world.nb_squares,
  Math.PI / 16
);
railway.createRails(hyperscene);

hyperscene.scene.rotateX(-Math.PI / 2).rotateZ(Math.PI);
hyperscene.camera.multiply(hyperTranslationMatrix(0, 0, 0.5));

const hud = document.getElementById("hud");
const cz_el = document.createElement("div");
const chatzone = new ChatZone(cz_el);
hud.appendChild(cz_el);

const conn = openConnectionScreen(hud, (name, server) => {
  Socket(
    `ws://${server}`,
    hyperscene,
    chatzone,
    name,
    conn.onSuccess,
    conn.onFail
  );
});

setRenderer(hyperscene, (tick) => {
  if (input.lock) {
    const movement = new THREE.Matrix4();
    movement.multiply(
      rotationMatrix(input.mouse.dy / 100, 0, -input.mouse.dx / 100)
    );
    movement.multiply(
      hyperTranslationMatrix(
        (speed.x * tick) / 1000,
        (speed.y * tick) / 1000,
        (speed.z * tick) / 1000
      )
    );
    hyperscene.camera.multiply(movement);
  }
  hyperscene.projection = "B";
  hyperscene.render();
});
