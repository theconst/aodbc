#!/usr/bin/node
/**
 * Sample usage of bluebird-wrapped API 
 */

const aodbc = require("../js/index.js");

require("console.table"); // this dependency is used for demo only

const connectionString = "DSN=CacheWinHost";

(async () => {
    // open connection synchronously and then work async
    const connectionSync = aodbc.createConnection(connectionString);
    try {
        const result = await connectionSync
            .executePromise("SELECT DISTINCT TOP 30 AircraftModel from Aviation.Aircraft");
        
        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionSync.disconnectPromise();
    }

    //connect asynchronously
    const connectionAsync = aodbc.createConnection();
    try {
        await connectionAsync.connectPromise(connectionString, 0);

        const select = aodbc.createQueryBuilder()
            .select()
            .distinct()
            .from("Aviation.Event")
            .field("AirportName", "airport name")
            .field("LocationCountry", "location country")
            .where("AirportName IS NOT NULL")
            .order("AirportName")
            .toString();

        const selectCount = "SELECT COUNT(*) FROM Aviation.Event"

        console.log(`Executing: ${select}, ${selectCount}`);
    
        //illustrate contention on single connection
        const result = await Promise.all([
            connectionAsync.executePromise(select),
            connectionAsync.executePromise(selectCount)
        ]);
        console.log(`Number of rows ${JSON.stringify(result[1])}`);
        console.table(result[0]);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionAsync.disconnectPromise();
    }
})();