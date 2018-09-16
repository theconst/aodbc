#!/usr/bin/env node
/**
 * Sample usage of crud.
 * Note the difference between query and execute.
 * https://github.com/nanodbc/nanodbc/issues/113
 */

const nanodbc = require("../js/index.js");

require("console.table"); // this dependency is used for demo only

const connectionString = "DSN=CacheWinHost";

(async () => {
    // open connection synchronously and then work async
    const connection = nanodbc.createConnection(connectionString);
    try {
        console.log("Cleaning up");
        try {
            await connection.executePromise(`DROP TABLE SQLUser.Employees`);
        } catch (error) {
            console.error("Error executing query");
            console.error("Table does not exist and procedures are not supported (yet)");
            console.error(error);
        }

        console.log("Creating table...");
        await connection.executePromise(`
            CREATE TABLE Employees (
                EMPNUM     INT NOT NULL,
                NAMELAST   CHAR(30) NOT NULL,
                NAMEFIRST  CHAR(30) NOT NULL,
                STARTDATE  TIMESTAMP,
                SALARY     MONEY,
                ACCRUEDVACATION   INT,
                ACCRUEDSICKLEAVE  INT,
                CONSTRAINT EMPLOYEEPK PRIMARY KEY (EMPNUM))
        `.replace(/\n\r\t/g, " "));

        console.log("Executing inert");
        const insert = nanodbc.createQueryBuilder()
            .insert()
            .into("Employees")
            .set("EMPNUM", 1)
            .set("STARTDATE", "GETDATE()", { "dontQuote" : true })
            .set("NAMEFIRST", "John")
            .set("NAMELAST", "Smith")
            .set("SALARY", 2)
            .toString();
        console.log(insert);
        await connection.executePromise(insert);
                
        console.log("Selecting result");
        const select = nanodbc.createQueryBuilder()
            .select()
            .from("Employees")
            .toString();
        const result = await connection.queryPromise(select);
        result.forEach(e => e["STARTDATE"] = JSON.stringify(e["STARTDATE"]));
        console.log(JSON.stringify(result));
        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connection.disconnectPromise();
    }
})();