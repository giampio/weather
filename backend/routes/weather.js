const express = require("express");
const router = express.Router();
const Weather = require("../models/Weather");

// /api/v1/weather/ Get all weathers
router.get("/", (req, res) => {
  // Select all Weather
  Weather.findAll()
    .then(result => res.json(result))
    .catch(err => console.log(err));
});

// /api/v1/weather/ Insert weather
router.post("/", (req, res) => {
  // Insert weather
  Weather.create(req.body)
    .then(result => res.status(200).json(result))
    .catch(err => console.log(err));
});

module.exports = router;
