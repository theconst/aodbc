#!/usr/bin/env node
'use strict'

const nc = require('nc-promisified');

const {Promise} = require('bluebird');

const log = require('./logger');

const uuid = require('uuid');

const config = require('config');

async function loop(connectionWorker, i) {
    if (i > config['iterations']) {
        return "finished";
    }
    uuid.
}

// awaiting on non-promise works in node >7.10
(async () => {
    const connections = await Promise.all(
        new Array(25).fill(null)
            .map(() => nc.createConnection())
            .map(connection => connection.connectPromise(config['dsn'])));

    const first = connections[0];
    
    try {
        await first.executePromise(`
            CREATE TABLE Sample.StressTestTable(
                id CHAR(30),
                payload TEXT,
                CONSTRAINT RECORDPK PRIMARY KEY(id)
            )
        `);

        first.m

    } finally {
        try {
            await first.executePromise(`
                DELETE TABLE Sample.StressTestTable;
            `);
        } catch (e) {
            console.error(e);
        }
        await Promise.all(connection.map(x => x.disconnectPromise()));
    }

})();