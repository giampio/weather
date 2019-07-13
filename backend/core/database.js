const Sequelize = require("sequelize");
const config = require("config");

// Connect to database. Use config/default.json parameters
const db = new Sequelize(config.database.dbname, config.database.username, config.database.password, config.database.options);

/* Alternative connection. Config inline
 const db = new Sequelize("test", "test", "12345678", {
  host: "localhost",
  dialect: "mysql",
  operatorsAliases: false,
  pool: {
    max: 5,
    min: 0,
    idle: 10000
  }
}); */

// Use the .authenticate() function like this to test the connection.
db.authenticate()
  .then(() => {
    console.log("Database connected");
  })
  .catch((err) => console.log(err));

// Synchronization database
db.sync({
  force: false,
  alter: true
})
  .then(() => {
    console.log("Database synchronization");
  })
  .catch((err) => console.log(err));

module.exports = db;
