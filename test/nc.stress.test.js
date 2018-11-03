#!/usr/bin/env node
'use strict'

const nc = require('./nc.promisified');
const { Promise } = require('bluebird');
const log = require('./logger');
const uuid = require('uuid/v4');
const config = require('./config');

const table = 'Sample.StressTestTable';

async function loop(no, connectionWorker, i) {
    if (i > config['iterations']) {
        return "finished";
    }
    const result = (await Promise.all([
        connectionWorker.executePromise(`
            INSERT INTO ${table}(id, payload) VALUES ('${uuid()}', 'Worker#${no}')
        `).catch(e => log.error(`[${no}] Unable to insert ${e.message}`)),
        // disable query cache
        connectionWorker.queryPromise({
            query: `SELECT '${uuid()}', id, payload FROM ${table}`,

            // Setting batch size >1 produces random bugs
            // They depend solely on this parameter
            // not on the connection count
            // [Cache Error: <<MAXNUMBER>srvPQ+19^%SYS.DBSRV>]
            batchSize: 2,
        }).catch(e => log.error(`[${no}] Unable to query: ${e.message}`)),
    ]))[1].length;

    setTimeout(() => log.info(`[${no}] Selected=${result}`), 0);

    return loop(no, connectionWorker, i + 1);
}

// awaiting on non-promise works in node >7.10
(async () => {
    log.info('Starting stess-test');
    const connections = new Array(config['connectionsCount'])
        .fill(null)
        .map(() => nc.createConnection());

    await Promise.all(connections.map(c => c.connectPromise(config['dsn'])));

    const first = connections[0];
    try {
        await first.executePromise(`
            CREATE TABLE ${table}(
                id CHAR(40),
                payload VARCHAR(100),
                CONSTRAINT RECORDPK PRIMARY KEY(id)
            )
        `);
        log.info('Table created');
        log.info('Starting loop...');
        await Promise.all(connections.map((c, i) => loop(i, c, 0)));

        // cleanup manually
        // await first.executePromise(`DELETE TABLE ${table};`);
        // log.info('Cleaned up...');
    } catch (e) {
        log.error(e);
    } finally {
        await Promise.all(connections.map(c => c.disconnectPromise()));
    }
    log.info('Finished stress-test');
})();