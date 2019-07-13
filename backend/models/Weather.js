const Sequelize = require("sequelize");
const db = require("../core/database");

// Model for Weather
const Weather = db.define("Weather", {
  id: {
    type: Sequelize.INTEGER,
    primaryKey: true,
    autoIncrement: true
  },
  temperature: {
    type: Sequelize.FLOAT,
    allowNull: false
  },
  humidity: {
    type: Sequelize.FLOAT,
    allowNull: false
  }
},
{
  // Define the table's name
  // tableName: "Weathers"
}
);

module.exports = Weather;
