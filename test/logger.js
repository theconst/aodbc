'use strict'

const {createLogger, format, transports} = require('winston');
const {timestamp, printf, combine} = format;

module.exports = createLogger({
    level: 'debug',
    format: combine(
        timestamp(),
        printf(m => `[${m.timestamp}] ${m.message}`)
    ),
    transports: [
      new transports.Console({}),
    ],
});