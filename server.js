/*
 * @author Mircea Ionut Dobre (293117@via.dk)
 * @author Florina Toldea(299116@via.dk)
 * @date 2022-11-14
 */
var http = require("http");
var file_system = require("fs");
var path = require("path");
const executing_sync = require("child_process").execSync;
const executeFile = require("child_process").execFile;

//Set heater's starting value to closed
executeFile("./greenhouse", ["setHeaterStatus", "off"], (error) => {
  if (error) {
    throw error;
  }
});

//Set window's starting value to closed
executeFile("./greenhouse", ["setWindowStatus", "close"], (error) => {
  if (error) {
    throw error;
  }
});

//Set light's starting value to 0.
executeFile("./greenhouse", ["setLedLight", "0"], (error) => {
  if (error) {
    throw error;
  }
});

//Creating a server, then start to listen to it.
var server = http
  .createServer(function (req, res) {
    var file = "." + (req.url == "/" ? "/index.html" : req.url);
    path.extname(file);
    var contentType = "text/html";

    file_system.exists(file, function (exists) {
      if (exists) {
        file_system.readFile(file, function (error, content) {
          if (!error) {
            // Write content when the page is found.
            res.writeHead(200, { "content-type": contentType });
            res.end(content);
          }
        });
      } else {
        //If page is not found.
        res.writeHead(404);
        res.end("Page not found");
      }
    });
  })
  .listen(8888);

var socket_io = require("socket.io")(server);

// for recieving client requests.
socket_io.on("connection", function (socket) {
  // Passing both data and socket that was sent to the function.
  socket.on("latest_data", (data) => handleRefreshData(socket, data));
  socket.on("changeStateLight", (data) => handleChangeStateLight(socket, data));
  socket.on("changeWindowState", (data) =>
    handleChangeWindowState(socket, data)
  );
  socket.on("changeHeaterState", (data) =>
    handleChangeHeaterState(socket, data)
  );
});

// Read current values form the peripherals and afterwards returning them to the client connected to the socket.
async function handleRefreshData(socket, data) {
  var temperature = "NaN";
  var humidity = "NaN";
  var light = "NaN";
  var window = "NaN";
  var heater = "NaN";
  var ledLight = "NaN";

  // Reading the humidity and the temperature.
  var result = executing_sync("./greenhouse readTempAndHumidity");
  strings = result.toString("utf8").split("\n");
  temperature = strings[0].substring(13, 17);
  humidity = strings[1].substring(13, 17);

  // Perform synchronus calls to the peripheral, get result then parse it

  //Reading the heater.
  var result = executing_sync("./greenhouse readHeater");
  heater = result.toString("utf8").substring(13, 16);

  //Reading the light.
  var result = executing_sync("./greenhouse readLightLevel");
  light = result.toString("utf8").substring(13, 18);

  //Reading the led light.
  var result = executing_sync("./greenhouse readLedLight");
  ledLight = result.toString("utf8").substring(13, 19);

  //Reading the window.
  var result = executing_sync("./greenhouse readWindow");
  window = result.toString("utf8").substring(13, 19);

  // Using a specific json dictionary, which is sent to the user and getting every result compiled.
  socket.emit("responseRefresh", {
    temperature: temperature,
    humidity: humidity,
    light: light,
    window: window,
    heater: heater,
    ledLight: ledLight,
  });
}

// Handle user changing led light state.
async function handleChangeStateLight(socket, data) {
  // Parsing json data into a dictionary.
  var jsonDictionary = JSON.parse(data);

  // By passing what functionality to perform, and which parameter to set,
  // it asynchronously changes the state of the led light.
  executeFile(
    "./greenhouse",
    ["setLedLight", jsonDictionary.state],
    (error) => {
      if (error) {
        throw error;
      }

      // After each modify peripheral call make a call to read the current state, then return it to the client.

      // Reading led light.
      var result = executing_sync("./greenhouse readLedLight");
      var ledLight = result.toString("utf8").substring(13, 19);

      socket.emit("responseSetLight", {
        ledLight: ledLight,
      });
    }
  );
}

// Same as handleChangeStateLight() function, with different peripheral calls.
async function handleChangeWindowState(socket, data) {
  var jsonDictionary = JSON.parse(data);
  var command;
  if (jsonDictionary.state == 1) {
    command = "open";
  } else {
    command = "close";
  }
  executeFile("./greenhouse", ["setWindowStatus", command], (error) => {
    if (error) {
      throw error;
    }

    //Read window.
    var result = executing_sync("./greenhouse readWindow");
    var window = result.toString("utf8").substring(13, 19);

    socket.emit("responseSetWindow", {
      window: window,
    });
  });
}

// Same as handleChangeStateLight(), with different peripheral calls.
async function handleChangeHeaterState(socket, data) {
  var jsonDictionary = JSON.parse(data);
  if (jsonDictionary.state == 1) {
    command = "on";
  } else {
    command = "off";
  }
  executeFile("./greenhouse", ["setHeaterStatus", command], (error) => {
    if (error) {
      throw error;
    }

    //Read heater
    var result = executing_sync("./greenhouse readHeater");
    var heater = result.toString("utf8").substring(13, 16);
    socket.emit("responseSetHeater", {
      heater: heater,
    });
  });
}

console.log("Server Running ...");
