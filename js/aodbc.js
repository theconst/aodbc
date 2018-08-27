const aodbc = require('aodbc');

aodbc.version((err, version) => {
    console.log(`Version is ${version}`);
});


const connection = new aodbc.ODBCConnection("DSN=CacheWinHost");


connection.isConnected((err, connected) => {
    console.log(`Connected: ${connected}`);
})


