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

        console.log(`Executing: ${select}`);
    
        const result = await connectionAsync.executePromise(select);

        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionAsync.disconnectPromise();
    }
})();