import { Vector3 } from "three";

export const speed = {
  x: 0,
  y: 0,
  z: 0,
};

export const input = {
  north: false,
  south: false,
  east: false,
  west: false,
  above: false,
  below: false,
  mouse: {
    dx: 0,
    dy: 0,
  },
  lock: false,
  is_focused: document.hasFocus(),
};

const trans = new Vector3();

const updateSpeed = () => {
  trans.setX(Number(input.west) - Number(input.east));
  trans.setY(Number(input.south) - Number(input.north));
  trans.setZ(Number(input.above) - Number(input.below));
  trans.normalize();
  speed.x = trans.x;
  speed.y = trans.y;
  speed.z = trans.z;
};

window.addEventListener("mousemove", (ev) => {
  input.mouse.dx = ev.movementX;
  input.mouse.dy = ev.movementY;
  setTimeout(() => {
    input.mouse.dx = 0;
    input.mouse.dy = 0;
  }, 0);
});

window.addEventListener("keydown", (ev) => {
  switch (ev.code) {
    case "KeyW":
      input.north = true;
      break;
    case "KeyS":
      input.south = true;
      break;
    case "KeyA":
      input.west = true;
      break;
    case "KeyD":
      input.east = true;
      break;
    case "Space":
      input.above = true;
      break;
    case "ShiftLeft":
      input.below = true;
      break;
    case "Tab":
      document.exitPointerLock();
      break;
  }
  updateSpeed();
});

window.addEventListener("keyup", (ev) => {
  switch (ev.code) {
    case "KeyW":
      input.north = false;
      break;
    case "KeyS":
      input.south = false;
      break;
    case "KeyA":
      input.west = false;
      break;
    case "KeyD":
      input.east = false;
      break;
    case "Space":
      input.above = false;
      break;
    case "ShiftLeft":
      input.below = false;
      break;
  }
  updateSpeed();
});

document.addEventListener("pointerlockchange", () => {
  input.lock = !!document.pointerLockElement;
});

window.addEventListener("focus", () => {
  setTimeout(() => (input.is_focused = true), 50);
});

window.addEventListener("blur", () => {
  input.is_focused = false;
});
