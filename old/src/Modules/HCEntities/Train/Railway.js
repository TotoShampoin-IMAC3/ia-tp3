import { Group, Mesh, BoxGeometry, MeshStandardMaterial, Matrix4 } from "three";
import {
  square_eucl_size,
  rotationMatrix,
  hyperTranslationMatrix,
  square_distance,
  hyperTranslateTile,
} from "../../HyperModules/HyperMaths";

const get_rail_size = (squares_at_a_vertex) => {
  switch (squares_at_a_vertex) {
    case 5:
      return square_eucl_size(squares_at_a_vertex) / 1.789;
    case 6:
      return square_eucl_size(squares_at_a_vertex) / 2.04315;
  }
};

const rail_tmp = (squares_at_a_vertex) => {
  const size = get_rail_size(squares_at_a_vertex);
  const rail = new Group();
  rail.add(
    new Mesh(
      new BoxGeometry(size * 2, size / 2, size / 8),
      new MeshStandardMaterial({ color: 0x442200 })
    ).translateY(size / 2),
    new Mesh(
      new BoxGeometry(size * 2, size / 2, size / 8),
      new MeshStandardMaterial({ color: 0x442200 })
    ).translateY(-size / 2),
    new Mesh(
      new BoxGeometry(size / 4, size * 2, size / 4),
      new MeshStandardMaterial({ color: 0x666666 })
    ).translateX((size * 3) / 4),
    new Mesh(
      new BoxGeometry(size / 4, size * 2, size / 4),
      new MeshStandardMaterial({ color: 0x666666 })
    ).translateX((-size * 3) / 4)
  );
  return rail;
};

const TI2HM = (instruction, move_factor, angle) => {
  const size = move_factor / 4;
  const HM = [];
  HM.push(new Matrix4());
  for (let i = 0; i < instruction.times; i++) {
    const R = new Matrix4();
    switch (instruction.type) {
      case "left":
        R.multiply(rotationMatrix(0, 0, angle / 2));
        break;
      case "right":
        R.multiply(rotationMatrix(0, 0, -angle / 2));
        break;
      case "up":
        R.multiply(rotationMatrix(-angle / 2, 0, 0));
        break;
      case "down":
        R.multiply(rotationMatrix(angle / 2, 0, 0));
        break;
    }
    const H = R.clone()
      .multiply(hyperTranslationMatrix(0, -size, 0))
      .multiply(R);
    HM.push(H);
  }
  return HM;
};

class Railway {
  constructor(reader, origin_path, squares_at_a_vertex, angle) {
    this.reader = reader;
    this.origin_path = origin_path;
    this.squares_at_a_vertex = squares_at_a_vertex;
    this.angle = angle;
    this.rails = [];
    this.move_factor = square_distance(squares_at_a_vertex);
    this.origin_position = hyperTranslateTile(origin_path, this.move_factor);
  }

  createRails(hyperscene) {
    const mat = this.origin_position.clone();
    this.reader.sequence.forEach((instruction) => {
      const HM = TI2HM(instruction, this.move_factor, this.angle);
      HM.forEach((hypermatrix) => {
        const rail = hyperscene.addObject(rail_tmp(this.squares_at_a_vertex));
        mat.multiply(hypermatrix);
        rail.position.multiply(mat);
        this.rails.push(rail);
      });
    });
    return this;
  }
}

export default Railway;
