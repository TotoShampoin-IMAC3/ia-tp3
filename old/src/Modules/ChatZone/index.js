import createElement from "../html-with-js";
import "./style.scss";

class ChatZone {
  constructor(element) {
    this.element = element;
    this.onSend = (message) => {};
    element.appendChild(
      createElement(
        "form",
        [
          createElement("input", [], {
            type: "text",
            placeholder: "Type a message",
            className: "chatzone__input",
          }),
          createElement("button", "Send", {
            type: "submit",
            className: "chatzone__button",
          }),
        ],
        {
          className: "chatzone__form",
          onsubmit: (ev) => {
            ev.preventDefault();
            const form = ev.target;
            const input = form.querySelector(".chatzone__input");
            this.onSend(input.value);
            input.value = "";
          },
        }
      )
    );
    this.messages = createElement("div", [], {
      className: "chatzone__messages",
    });
    element.appendChild(this.messages);
    element.className = "chatzone";
  }

  reset() {
    this.messages.innerHTML = "";
    this.onSend = (message) => {};
  }

  entry(message) {
    this.messages.innerHTML += `<div class="chatzone__entry">${message}</div>`;
  }

  addMessage(id, message) {
    this.entry(`${id}: ${message}`);
  }
}

export default ChatZone;
