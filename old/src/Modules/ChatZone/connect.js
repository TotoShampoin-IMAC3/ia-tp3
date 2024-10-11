import createElement from "../html-with-js";

export const openConnectionScreen = (element, onClick) => {
  let onFail;
  const connection_screen = createElement(
    "form",
    [
      createElement("input", [], {
        type: "text",
        placeholder: "Name",
        name: "connscr-name",
        id: "connscr-name",
        className: "connscreen__input",
      }),
      createElement("input", [], {
        type: "text",
        placeholder: "Server",
        name: "connscr-server",
        id: "connscr-server",
        className: "connscreen__input",
      }),
      createElement("button", "Connect", {
        onclick: (ev) => {
          ev.preventDefault();
          const name = connection_screen.querySelector("#connscr-name").value;
          const server =
            connection_screen.querySelector("#connscr-server").value;
          const button = connection_screen.querySelector("#connscr-button");
          onClick(name, server);
          button.disabled = true;
          button.innerText = "Connecting...";
          onFail = () => {
            button.disabled = false;
            button.innerText = "Connect";
          };
        },
        id: "connscr-button",
        className: "connscreen__button",
      }),
    ],
    { className: "connscreen" }
  );
  element.appendChild(connection_screen);
  return {
    connection_screen,
    onFail,
    onSuccess: () => {
      connection_screen.remove();
    },
  };
};
