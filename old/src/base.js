import * as THREE from "three";

const $monitor = document.getElementById("monitor");
let width = window.innerWidth;
let height = window.innerHeight;
let ratio = width / height;

export const scene = new THREE.Scene();
export const camera = new THREE.PerspectiveCamera(70, ratio, 0.01);
export const renderer = new THREE.WebGLRenderer();
renderer.setSize(width, height);
$monitor.appendChild(renderer.domElement);

window.addEventListener("resize", function (ev) {
  width = window.innerWidth;
  height = window.innerHeight;
  ratio = width / height;
  camera.aspect = ratio;
  renderer.setSize(width, height);
  camera.updateProjectionMatrix();
});

export function requestPointerLock() {
  renderer.domElement.requestPointerLock();
}
