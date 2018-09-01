#!/usr/bin/node --max_old_space_size=100
/**
 * This is simple test that will create lots of connection and not close them
 * we will rely on GC to free any resrces
 */

const aodbc = require("../js/index.js");
const connectionString = "DSN=CacheWinHost";

(async ()  => {
    const select1 = aodbc.createQueryBuilder()
        .select()
        .distinct()
        .from("Aviation.Event")
        .field("AirportName", "airport name")
        .field("LocationCountry", "location country")
        .where("AirportName IS NOT NULL")
        .order("AirportName")
        .toString();

    console.log(`Executing: ${select1} for unbounded number of times`);

    for  (let i = 1; true; ++i) {  
        //connect asynchronously
        console.log(`Iteration # ${i}`);
        const connection1 = aodbc.createConnection(connectionString);
        const connection2 = aodbc.createConnection(connectionString);
      
        try {
            const delay1 = Math.floor(Math.random() * 20001);
            const delay2 = Math.floor(Math.random() * 20001);

            console.log(`Delays: ${delay1}, ${delay2}`);
            await Promise.all([
                resolveAfter(delay1)
                    .then(() => console.log("Delay 1 ready"))
                    .then(() => connection1.executePromise("SELECT 1"))
                    .then(val => console.log(`Query on connection 1 ready, size: ${val.length}`)),
                resolveAfter(delay2)
                    .then(() => console.log("Delay 2 ready"))
                    .then(() => connection2.executePromise(select1))
                    .then(val => console.log(`Query on connection 2 ready, size: ${val.length}`)),
            ]);
        } catch (error) {
            console.error(error.message);
        } finally {
            await Promise.all([connection1.disconnectPromise(), connection2.disconnectPromise()]);
        }
    } 
})();

function resolveAfter(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}
