#!/usr/bin/node
/**
 * Sample usage of crud.
 * Note the difference between query and execute.
 * https://github.com/nanodbc/nanodbc/issues/113
 */

const aodbc = require("../js/index.js");

require("console.table"); // this dependency is used for demo only

const connectionString = "DSN=CacheWinHost";

(async () => {
    // open connection synchronously and then work async
    const connectionSync = aodbc.createConnection(connectionString);
    try {
        console.log("Cleaning up");
        try {
            await connectionSync.executePromise(`DROP TABLE SQLUser.EmployeeTest2`);
        } catch (error) {
            console.error("Error executing query");
            console.error("Table does not exist and procedures are not supported (yet)");
            console.error(error);
        }

        console.log("Creating table...");
        await connectionSync.executePromise(`
            CREATE TABLE EmployeeTest2 (
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
        const insert = aodbc.createQueryBuilder()
            .insert()
            .into("EmployeeTest2")
            .set("EMPNUM", 1)
            .set("STARTDATE", "GETDATE()", { "dontQuote" : true })
            .set("NAMEFIRST", "John")
            .set("NAMELAST", "Smith")
            .set("SALARY", 2)
            .toString();
        console.log(insert);
        await connectionSync.executePromise(insert);
                
        console.log("Selecting result");
        const select = aodbc.createQueryBuilder()
            .select()
            .from("EmployeeTest2")
            .toString();
        const result = await connectionSync.queryPromise(select);
        result.forEach(e => e["STARTDATE"] = JSON.stringify(e["STARTDATE"]));
        console.log(JSON.stringify(result));
        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connectionSync.disconnectPromise();
    }
})();