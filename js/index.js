const AODBC = require("NC");
const bluebird = require("bluebird");
const squel = require("squel");

class promisifiedNC {

    static createConnection(connectionString) {
        return bluebird.promisifyAll(new NC.ODBCConnection(connectionString), {
            suffix: "Promise",
        });
    }

    static createQueryBuilder() {
        //TODO: which flavor is cache closer to ?
        return squel;
    }

}

module.exports = promisifiedNC;