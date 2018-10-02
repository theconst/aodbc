'use strict'

const nc = require("nc");
const {promisifyAll} = require('bluebird');
const squel = require("squel");

squel.useFlavour('mysql');

class PromisifiedNC {

    static createConnection() {
        return promisifyAll(new nc.ODBCConnection(), {
            suffix: "Promise",
        });
    }

    static createQueryBuilder() {
        return squel;
    }

}

module.exports = PromisifiedNC;