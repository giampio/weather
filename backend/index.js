const express = require("express");
const bodyParser = require("body-parser");
const config = require("config");
const port = config.server.port;

const app = express();

// CORS
app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Methods", "GET,PUT,POST,DELETE,PATCH,OPTIONS");
  res.header("Access-Control-Allow-Headers", "Content-Type, Authorization, Content-Length, X-Requested-With");
  next();
});

// Body Parser
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Endpoint for /
app.get("/", (req, res) => {
  res.send("Index");
});

// Route for /api/v1/weather/
app.use("/api/v1/weather/", require("./routes/weather"));

// Start express server
app.listen(port, () => {
  console.log(`Example app listening on port ${port}!`);
});
