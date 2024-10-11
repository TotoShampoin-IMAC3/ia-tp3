class TurtleReader {
  constructor(path) {
    this.path = path;
    this.cursor = 0;
    this.sequence = [];
    while (this.cursor < this.path.length) {
      this.sequence.push(this.parseNext());
    }
  }

  next() {
    return this.path[this.cursor++];
  }

  parseNumber() {
    let num = "";
    while (
      this.cursor < this.path.length &&
      this.path[this.cursor] >= "0" &&
      this.path[this.cursor] <= "9"
    ) {
      num += this.next();
    }
    return parseInt(num) || 1;
  }

  parseNext() {
    const next = this.next();
    if (next === "F" || next === "f") {
      return {
        type: "forward",
        times: this.parseNumber(),
      };
    } else if (next === "L" || next === "l") {
      return {
        type: "left",
        times: this.parseNumber(),
      };
    } else if (next === "R" || next === "r") {
      return {
        type: "right",
        times: this.parseNumber(),
      };
    } else if (next === "U" || next === "u") {
      return {
        type: "up",
        times: this.parseNumber(),
      };
    } else if (next === "D" || next === "d") {
      return {
        type: "down",
        times: this.parseNumber(),
      };
    } else if (next === " " || next === "\n") {
      return this.parseNext();
    } else {
      throw new Error(`invalid character ${next}`);
    }
  }
}

export default TurtleReader;
