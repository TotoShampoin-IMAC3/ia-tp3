import { loadObj } from "../HyperModules/HyperMap";

export const Socket = (
  url,
  hyperscene,
  chatzone,
  name,
  onSuccess = () => {},
  onFail = () => {}
) => {
  let is_connected = false;
  const socket = new WebSocket(url);
  const id = uuid();
  chatzone.addMessage("Server", "Connecting...");

  socket.onopen = (ev) => {
    is_connected = true;
    chatzone.addMessage("Server", "Connected to server");
    chatzone.onSend = (message) => {
      socket.send(
        JSON.stringify({
          type: "chat",
          data: { message },
        })
      );
    };
    socket.send(
      JSON.stringify({
        type: "establish",
        data: {
          id,
          name,
          position: hyperscene.camera.elements,
        },
      })
    );
    setInterval(() => {
      socket.send(
        JSON.stringify({
          type: "update",
          data: { position: hyperscene.camera.elements },
        })
      );
    }, 50);
    onSuccess();
  };

  socket.onmessage = async (ev) => {
    const data = JSON.parse(ev.data);
    switch (data.type) {
      case "player":
        await Socket_PlayerCase(data, hyperscene);
        break;
      case "player-bc":
        await Socket_PlayerBCCase(data, hyperscene);
        break;
      case "chat":
        Socket_chatCase(data, chatzone);
        break;
    }
  };

  socket.onclose = (ev) => {
    console.log("Socket closed");
    if (is_connected) {
      chatzone.addMessage("Server", "Disconnected from server");
      is_connected = false;
    } else {
      chatzone.addMessage("Server", "Connection failed");
      onFail();
    }
  };

  return socket;
};

const uuid = () => {
  return "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx".replace(/[xy]/g, (c) => {
    const r = (Math.random() * 16) | 0;
    const v = c === "x" ? r : (r & 0x3) | 0x8;
    return v.toString(16);
  });
};

const Socket_PlayerCase = async (socket, hyperscene) => {
  if (socket.type !== "player") return;
  const { event, data } = socket;
  switch (event) {
    case "add":
      const player = await hyperscene.addObject(
        await loadObj("/assets/obj/persona.glb"),
        data.id
      );
      player.position.set(...data.position);
      break;
    case "remove":
      const playerToRemove = hyperscene.findObject(data.id);
      if (playerToRemove) hyperscene.removeObject(playerToRemove);
      break;
  }
};

const Socket_PlayerBCCase = async (socket, hyperscene) => {
  if (socket.type !== "player-bc") return;
  const { event, data } = socket;
  switch (event) {
    case "add":
      for (const { id, position } of data) {
        const player = await hyperscene.addObject(
          await loadObj("/assets/obj/persona.glb"),
          id
        );
        player.position.set(...position).transpose();
      }
      break;
    case "update":
      for (const { id, position } of data) {
        const player = hyperscene.findObject(id);
        if (player) player.position.set(...position).transpose();
      }
      break;
  }
};

const Socket_chatCase = (socket, chatzone) => {
  if (socket.type !== "chat") return;
  const { data } = socket;
  chatzone.addMessage(data.name || data.id, data.message);
};
