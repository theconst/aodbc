#!/usr/bin/env node
'use strict'

const nc = require('./promisified');

const { Promise } = require('bluebird');
const log = require('./logger');
const config = require('./config');

const table = 'Sample.StressTestTable';

const connetionsCount = config['connections'];

const connections = new Array(connetionsCount).fill(null)
    .map(() => nc.createConnection());

function getRandomConnection() {
    return connections[Math.floor(Math.random() * Math.floor(connections.length))];
}

async function ressurecter(no, i) {
    if (i > config['iterations']) {
        return 'finished';
    }
    return getRandomConnection().connectPromise(config['dsn'])
        .tap(() => log.info(`[${no}] connected`))
        .tapCatch(e => log.error(`[${no}] unable to connect`))
        .then(() => ressurecter(no, i + 1));
}

async function killer(no, i) {
    if (i > config['iterations']) {
        return 'finished';
    }
    return getRandomConnection().disconnectPromise()
        .tap(() => log.info(`[${no}] disconneccted`))
        .tapCatch(e => log.error(`[${no}] unable to disconnect ${e}`))
        .then(() => killer(no, i + 1));
}

(async () => {
    const array = new Array(config['connectorsCount']).fill(null);
    await Promise.all([
        ...array.map((_, i) => killer(i, 0)),
        ...array.map((_, i) => ressurecter(i, 0)),
    ]);
})();