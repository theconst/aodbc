#!/usr/bin/node --max_old_space_size=100
/**
 * This is simple test that will create lots of connections.
 * It illustrates case of contention for a single table from multiple connections
 */

const nc = require("../js/index.js");
const connectionString = "DSN=CacheWinHost";

(async ()  => {
    const select1 = "SELECT * FROM Aviation.Event"

    unbufferedLog(`Executing: '${select1}' for unbounded number of times`);

    let interval = 1000;
    let counter;
    setInterval(() => {
        if (counter !== undefined) {
            process.stdout.write(`->${counter}`, true);
            counter += interval;
        }
    }, interval);

    for  (let i = 1; true; ++i) {  
        counter = 0;
        //connect asynchronously
        unbufferedLog(`Iteration # ${i}`);

        const connection1 = nc.createConnection(connectionString);
        const connection2 = nc.createConnection(connectionString);
        try {
            const delayInterval = 5000;
            const delay1 = Math.floor(Math.random() * delayInterval);
            const delay2 = Math.floor(Math.random() * delayInterval);

            unbufferedLog(`Delays: ${delay1}, ${delay2}`);
            await Promise.all([
                resolveAfter(delay1)
                    .then(() => unbufferedLog("Delay 1 ready"))
                    .then(() => connection1.queryPromise({
                      "query": select1,
                      "batchSize": 1200,
                    }))
                    .then(val => unbufferedLog(`Query on connection 1 ready, size: ${val.length}`)),
                resolveAfter(delay2)
                    .then(() => unbufferedLog("Delay 2 ready"))
                    .then(() => connection2.queryPromise(select1))
                    .then(val => unbufferedLog(`Query on connection 2 ready, size: ${val.length}`)),
            ]);
        } catch (error) {
            console.error(error.message);
        } finally {
            await Promise.all([connection1.disconnectPromise(), connection2.disconnectPromise()]);
        }
    } 
})();

function resolveAfter(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}

function unbufferedLog(msg, sameLine) {
    //we do not need to buffer sdout to be `real time`
    process.stdout.write(sameLine ?  msg : `\n${msg}\n`);
}
