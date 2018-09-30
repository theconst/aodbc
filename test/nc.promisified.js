'use strict'

const nc = require("nc");
const {promisifyAll} = require('bluebird');
const squel = require("squel");

squel.useFlavour('mysql');

class PromisifiedNC {

    static createConnection(connectionString) {
        return promisifyAll(new nc.ODBCConnection(connectionString), {
            suffix: "Promise",
        });
    }

    static createQueryBuilder() {
        return squel;
    }

}

module.exports = PromisifiedNC;