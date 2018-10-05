'use strict'

const nc = require("nc");
const {promisifyAll} = require('bluebird');
const squel = require("squel");

squel.useFlavour('mysql');

const promisifySettings = {
    suffix: 'Promise',
};

class Statement extends nc.ODBCStatement {

    constructor(conneciton) {
        super(conneciton);
    }

    _doHandle(bindingsOrCb, cb, action) {
        if (typeof bindingsOrCb == 'function') {
            cb = bindingsOrCb;
            bindingsOrCb = [];
        }
        action(bindingsOrCb, cb);
    }

    query(bindingsOrCb, cb) {
        this._doHandle(bindingsOrCb, cb, (a, cb) => super.query(a, cb));
    }

    execute(bindingsOrCb, cb) {
        this._doHandle(bindingsOrCb, cb, (a, cb) => super.execute(a, cb));
    }
}
promisifyAll(Statement.prototype, promisifySettings);

class Connection extends nc.ODBCConnection {
    constructor() {
        super();
    }

    connect(dsn, timeoutOrCb, cb) {
        if (typeof timeoutOrCb == 'function') {
            cb = timeoutOrCb;
            timeoutOrCb = 0;
        }
        super.connect(dsn, timeoutOrCb, cb);
    }

    close(cb) {
        super.disconnect(cb);
    }

    prepareStatement(query, cb) {
        const statement = new Statement(this);
        statement.prepare(query, (err) => cb(err, statement));
    }
}
promisifyAll(Connection.prototype, promisifySettings);


class ConnectionFactory {
    static createConnection() {
        return new Connection();
    }
}

module.exports = ConnectionFactory;