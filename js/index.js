const AODBC = require("aodbc");
const bluebird = require("bluebird");
const squel = require("squel");

class promisifiedAODBC {

    static createConnection(connectionString) {
        return bluebird.promisifyAll(new AODBC.ODBCConnection(connectionString), {
            suffix: "Promise",
        });
    }

    static createQueryBuilder() {
        //TODO: which flavor is cache closer to ?
        return squel;
    }

}

module.exports = promisifiedAODBC;