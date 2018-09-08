# nc.js (nanodbc.js) - Wrapper for nanodbc library

## Nanodbc
Nanodbc is a proven wrapper for ODBC API. For more details see:
https://nanodbc.github.io/nanodbc/

This library aims to provide a simple access to ODBC data sources for node js for different drivers.


# Current status

**<span style="color:red"> WARNING </span>** - Work in progress.

See samples directory for example usage.

Example:

```javascript
const nanodbc = require("../js/index.js");

// this dependency is used for demo only
require("console.table");

const connectionString = "DSN=CacheWinHost";

(async () => {
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
        const result = await connection.queryPromise({
            "query": select, 
            "batchSize": 10,
            "timeout": 1000
        });
        result.forEach(e => e["STARTDATE"] = JSON.stringify(e["STARTDATE"]));
        console.log(JSON.stringify(result));
        console.table(result);
    } catch (error) {
        console.error(error.message);
    } finally {
        await connection.disconnectPromise();
    }
})();
```

# TODO List

- [x] Add support for setting fetch size
- [ ] Generalize delegation code
- [ ] Increase C++ test coverage
- [ ] Automate build for linux
- [ ] Add support for prepared statements
- [ ] Add support for transactions
- [ ] Add support for binary type
- [ ] Add support for windows