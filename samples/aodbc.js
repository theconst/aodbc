#!/usr/bin/node
/**
 * Demo sript of the native AODBC usage.
 * You typically may want to use a blubird wrapper in js/index.
 * 
 */

const aodbc = require("aodbc");

const connection = new aodbc.ODBCConnection("DSN=CacheWinHost");

connection.query("Select * from Aviation.Aircraft", (err, value) => {
    err && console.debug(`Error1: ${err}`);
    
    console.debug(`Nubmer of aircrafts 1: ${value.length}`);
});

//request on the same connection are serialized, we will wait for it

setTimeout(() => connection.query("Select Count(*) from Aviation.Aircraft", (err, value) => {
    err && console.debug(`Error2: ${err}`);
    console.debug(`Number of aircrafts 2: ${JSON.stringify(value)}`);
}), 0);


//request on the same connection are serialized, we will wait for it
connection.query("Select Sum(SeatsTotal) As Total from Aviation.Aircraft", (err, value) => {
    err && console.debug(`Error3: ${err}`);
    console.debug(`Number of seats: ${JSON.stringify(value)}`);
});


connection.query(`Select * from Aviation.Crew`, (err, value) => {
    err && console.debug(`Error3: ${err}`);
    console.debug(`Crew: ${value.length}`);
    
    // console.debug(`Crew: ${JSON.stringify(value)}`);
});
