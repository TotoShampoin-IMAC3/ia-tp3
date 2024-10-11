import * as THREE from "three";
import * as base from "./base";
import HyperScene from "./HyperScene";

/**
 *
 * @param {HyperScene} hyperscene
 * @param {(tick: number) => void} onupdate
 */
export const setRenderer = async (hyperscene, onupdate = (tick) => {}) => {
  base.renderer.setClearColor(0x4488cc, 1);

  const light = new THREE.DirectionalLight(0xffffff);
  light.position.set(0, 5, 0);
  light.lookAt(0, 0, 0);

  base.scene.add(new THREE.AmbientLight(0xffffff, 2), hyperscene.scene);

  base.camera.position.set(0, 0, 0);
  base.camera.lookAt(0, 0, -0.01);
  base.camera.lookAt(0, 0, 0);

  let timer = 0;

  base.renderer.setAnimationLoop((time, frame) => {
    onupdate(time - timer);
    hyperscene.render();
    base.renderer.render(base.scene, base.camera);
    timer = time;
  });

  document.getElementById("monitor").addEventListener("click", (ev) => {
    base.requestPointerLock();
  });
};

window.scene = base.scene;
