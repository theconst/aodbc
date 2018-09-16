#!/usr/bin/env node
/**
 * Sample usage of bluebird-wrapped API 
 */

const nc = require("../js/index.js");

require("console.table"); // this dependency is used for demo only

const connectionString = "DSN=CacheWinHost";

(async () => {
    // open connection synchronously and then work async
    const connectionSync = nc.createConnection(connectionString);
    try {
        const result = await connectionSync
            .queryPromise("SELECT DISTINCT TOP 30 AircraftModel from Aviation.Aircraft");
        
        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionSync.disconnectPromise();
    }

    //connect asynchronously
    const connectionAsync = nc.createConnection();
    try {
        await connectionAsync.connectPromise(connectionString, 0);

        const select = nc.createQueryBuilder()
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
            connectionAsync.queryPromise(select),
            connectionAsync.queryPromise(selectCount)
        ]);
        console.log(`Number of rows ${JSON.stringify(result[1])}`);
        console.table(result[0]);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionAsync.disconnectPromise();
    }
})();