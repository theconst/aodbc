const nc = require("nc");
const bluebird = require("bluebird");
const squel = require("squel");

class promisifiedNC {

    static createConnection(connectionString) {
        return bluebird.promisifyAll(new nc.ODBCConnection(connectionString), {
            suffix: "Promise",
        });
    }

    static createQueryBuilder() {
        //TODO: which flavor is cache closer to ?
        return squel;
    }

}

module.exports = promisifiedNC;