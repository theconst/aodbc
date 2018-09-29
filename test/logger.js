const { createLogger, format, transports } = require('winston');
const { combine, timestamp, prettyPrint } = format;

module.exports =  createLogger({
    level: 'trace',
    format: combine(
        timestamp(),
        format.splat(),
        format.simple(),
        prettyPrint()
    ),
    transports: [new transports.Console()],
})