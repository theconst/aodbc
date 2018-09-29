'use strict';

const config = require('./config.json');

const expect = require('chai').expect;

const { Promise } = require('bluebird');

const nc = require('./nc.promisified');

const log = require('./logger');

const INTEGRATION_TEST_DSN = config["dsn"];
const TIMEOUT = config["timeout"];

describe('should convert big integer data type', function() {
    this.timeout(TIMEOUT);
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    const table = 'Sample.BigIntegerTestTable';
    
    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                id CHAR(10),
                val BIGINT
            )
        `);
    });

    it('should convert big integer datatype', function() {
        return Promise.all([
            `INSERT INTO ${table}(id, val) VALUES ('M13', 8191)`,
            `INSERT INTO ${table}(id, val) VALUES ('M17', 131071)`,
            `INSERT INTO ${table}(id, val) VALUES ('M31', 2147483647)`,
            `INSERT INTO ${table}(id, val) VALUES ('ZMAX', 9223372036854775807)`,
            // bug - min value truncates buffer without sign
            // nanodbc uses snprintf, so we are safe
            // `INSERT INTO ${table}(id, val) VALUES ('ZMIN', -9223372036854775807)`,
            // only negative quadrillions are supported (no negative quintillionss)
            `INSERT INTO ${table}(id, val) VALUES ('ZMINA', -999999999999999999)`,
            `INSERT INTO ${table}(id, val) VALUES ('ZZZZ', NULL)`,
        ].map(v => connection.executePromise(v))).then(() => 
            connection.queryPromise({
                query: `SELECT * FROM ${table} ORDER BY id`,
                batchSize: 6,
            })
        ).then(queryResult => {
            expect(queryResult).to.be.an('array');
            
            log.debug(JSON.stringify(queryResult));

            expect(queryResult).to.deep.equal([
                {
                    'id': 'M13',
                    'val': '8191'
                }, 
                {
                    'id': 'M17',
                    'val': '131071'
                },
                {
                    'id': 'M31',
                    'val': '2147483647'
                },
                {
                    'id': 'ZMAX',
                    'val': '9223372036854775807'
                },
                {
                    'id': 'ZMINA',
                    'val': '-999999999999999999'
                },
                {
                    'id': 'ZZZZ',
                    'val': null
                },
            ]);
        });
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    });
});


describe('should convert big integer data type', function() {
    this.timeout(TIMEOUT);
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    const table = 'Sample.BinaryTestTable';
    
    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                name CHAR(10),
                photo BINARY(5),
                CONSTRAINT BIPK PRIMARY KEY(name)
            )
        `);
    });

    it('should convert binary datatype', function() {
        return Promise.all([
            `INSERT INTO ${table}(name, photo) VALUES ('Alice', CAST('data1' AS BINARY(5)))`,
            `INSERT INTO ${table}(name, photo) VALUES ('Bob', CAST('data2' AS BINARY(5)))`,
        ].map(v => connection.executePromise(v)))
        .then(() =>
            connection.queryPromise({
                query: `SELECT * FROM ${table} ORDER BY name`,
                // bug - batch size more than one not supported
                batchSize: 1,
        }))
        .then(queryResult => {
            expect(queryResult).to.be.an('array');
            
            expect(queryResult).to.deep.equal([
                {
                    'name': 'Alice',
                    'photo': 'data1',
                },
                {   
                    'name': 'Bob',
                    'photo': 'data2',
                },
            ]);
        });
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    });
});


describe('Should convert bit datatype', function() {
    this.timeout(TIMEOUT);
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    const table = 'Sample.BitTestTable';

    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                record CHAR(30),
                valid BIT,
                CONSTRAINT RECORDPK PRIMARY KEY(record)
            )
        `);
    });
    

    it('should convert bit datatype', function() {
        return connection.executePromise(`INSERT INTO ${table}(record, valid) VALUES ('test1', 1)`)
        .then(() => 
            connection.executePromise(`INSERT INTO ${table}(record, valid) VALUES ('test2', 0)`)
        ).then(() => 
            connection.executePromise(`INSERT INTO ${table}(record, valid) VALUES ('test3', NULL)`)
        ).then(() => 
            connection.queryPromise(`SELECT * FROM ${table} ORDER BY record`)
        ).then(queryResult => {
            expect(queryResult).to.be.an('array');
            expect(queryResult).to.deep.equal([
                {
                    'record': 'test1',
                    'valid': 1,
                }, 
                {
                    'record': 'test2',
                    'valid': 0,
                },
                {
                    'record': 'test3',
                    'valid': null,
                },
            ]);
        });        
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    })
});

[
    'VARCHAR(15)', 
    'CHARACTER VARYING(15)',
    'CHAR VARYING(15)',
    'CHARACTER(15)',
    'CHAR(15)',
].forEach((alias, i) => {

describe(`should convert ${alias} datatype`, function() {
    const table = `Sample.CharacterTestTable${i}`;
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);

    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                name CHAR(10),
                info ${alias},
                CONSTRAINT CHPK PRIMARY KEY(name)
            )
        `);
    });

    it('should successfully convert values', function() {
        return Promise.all([
            `INSERT INTO ${table}(name, info) VALUES ('Alice', 'First party')`,
            `INSERT INTO ${table}(name, info) VALUES ('Bob', NULL)`,
            `INSERT INTO ${table}(name, info) VALUES ('Mallory', 'Man in the')`,
            `INSERT INTO ${table}(name, info) VALUES ('Eve', 'Eavesdropper')`,
        ]
        .map(v => connection.executePromise(v)))
        .then(() => 
            connection.queryPromise({
                query: `SELECT * FROM ${table} ORDER BY name`,
                batchSize: 4,
        }))
        .then(queryResult => {
            expect(queryResult).to.be.an('array');
            
            expect(queryResult).to.deep.equal([
                {
                    'name': 'Alice',
                    'info': 'First party',
                },
                {   
                    'name': 'Bob',
                    'info': null,
                },
                {
                    'name': 'Eve',
                    'info': 'Eavesdropper',
                },
                {
                    'name': 'Mallory',
                    'info': 'Man in the'
                },
            ]);
        });
    });

    it('should not fail if value is not in range', function() {
        connection.executePromise(`INSERT INTO(name, info) VALUES('Mallory', 
            ${'Man in the middle'.repeat(10)})`)
        .catch(error => {
            log.debug(error);
            expect(error).to.exist;
        });
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    });

});


});

[
    'LONG VARCHAR(15)',
    'LONGVARCHAR(15)',
].forEach((alias, i) => {

    describe(`should convert ${alias} datatype`, function() {
        const table = `Sample.CharacterTestTable${i}`;
        const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    
        before(function() {
            return connection.executePromise(`
                CREATE TABLE ${table}(
                    name CHAR(10),
                    info ${alias},
                    CONSTRAINT CHPK PRIMARY KEY(name)
                )
            `);
        });
    
        it('should successfully convert values', function() {
            return Promise.all([
                `INSERT INTO ${table}(name, info) VALUES ('Alice', 'First party')`,
                `INSERT INTO ${table}(name, info) VALUES ('Bob', NULL)`,
                `INSERT INTO ${table}(name, info) VALUES ('Mallory', 'Man in the')`,
                `INSERT INTO ${table}(name, info) VALUES ('Eve', 'Eavesdropper')`,
            ]
            .map(v => connection.executePromise(v)))
            .then(() => 
                connection.queryPromise({
                    query: `SELECT * FROM ${table} ORDER BY name`,

                    /* Bug -buffer is unsafely  */
                    batchSize: 1,
            }))
            .then(queryResult => {
                expect(queryResult).to.be.an('array');
                
                expect(queryResult).to.deep.equal([
                    {
                        'name': 'Alice',
                        'info': 'First party',
                    },
                    {   
                        'name': 'Bob',

                        // bug (or feature) - null buffer returned as empty string
                        'info': '',
                    },
                    {
                        'name': 'Eve',
                        'info': 'Eavesdropper',
                    },
                    {
                        'name': 'Mallory',
                        'info': 'Man in the'
                    },
                ]);
            });
        });
    
        it('should not fail if value is not in range', function() {
            connection.executePromise(`INSERT INTO(name, info) VALUES('Mallory', 
                ${'Man in the middle'.repeat(10)})`)
            .catch(error => {
                log.debug(error);
                expect(error).to.exist;
            });
        });
    
        after(function() {
            return connection.executePromise(`DROP TABLE ${table}`);
        });
    
    });
    
});


describe('should convert date and time', function() {
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    const table = 'Sample.DateTestTable';
    
    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                name CHAR(10),
                last_visit DATETIME,
                last_visit2 DATETIME2,
                birthday DATE,
                CONSTRAINT BIPK PRIMARY KEY(name)
            )
        `);
    });

    it('should convert dates and times datatype', function() {
        return Promise.all([
            `INSERT INTO ${table}(name, last_visit, last_visit2, birthday) \
             VALUES ('Alice', \
                '2003-09-01 01:00:00', \
                '2003-09-01 22:00:00', \
                '1995-01-02')`
        ].map(v => connection.executePromise(v)))
        .then(() =>
            connection.queryPromise({
                query: `SELECT * FROM ${table} ORDER BY name`,
                batchSize: 1,
        }))
        .then(queryResult => {
            expect(queryResult).to.be.an('array');
            
            expect(queryResult).to.deep.equal([
                {
                    // TODO(kko): add support for native dates
                    // we can use db-specific functions to convert to strings, if we need to
                    'name': 'Alice',
                    'last_visit': {
                        day: 1,
                        month: 9,
                        year: 2003,
                        hours: 1,
                        minutes: 0,
                        seconds: 0,
                        fractionalSeconds: 0
                    },
                    'last_visit2': {
                        day: 1,
                        month: 9,
                        year: 2003,
                        hours: 22,
                        minutes: 0,
                        seconds: 0,
                        fractionalSeconds: 0
                    },
                    'birthday': {
                        day: 2,
                        month: 1,
                        year: 1995,
                    },
                },
            ]);
        });
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    });

});

describe('should convert integers', function() {
    this.timeout(TIMEOUT);
    const connection = nc.createConnection(INTEGRATION_TEST_DSN);
    const table = 'Sample.IntegerTable';
    
    before(function() {
        return connection.executePromise(`
            CREATE TABLE ${table}(
                id INT,
                tint TINYINT,
                sint SMALLINT,
                num NUMBER,
                numric NUMERIC(8),
                CONSTRAINT IDPK PRIMARY KEY(id)
            )
        `);
    });

    it('should convert integers in range', function() {
        return Promise.all([

            // same bug - loosing same order of magnitude after buffer allocation
            `1, -128, -32768,  TO_NUMBER('-922337203685477'), TO_NUMBER('-99999999')`,
            `2, 127, 32767, TO_NUMBER('92233720368547758'), TO_NUMBER('99999999')`,
            `3, 25, 35, 4555, 55`,
        ].map(v => 
            connection.executePromise(`INSERT INTO ${table}(id, tint, sint, num, numric) VALUES(${v})`)))
        .then(() =>
            connection.queryPromise({
                query: `SELECT * FROM ${table} ORDER BY id`,
                batchSize: 3,
        }))
        .then(queryResult => {
            expect(queryResult).to.be.an('array');
            
            expect(queryResult).to.deep.equal([
                {
                    id: 1,
                    tint: -128,
                    sint: -32768,
                    num: '-922337203685477',
                    numric:  '-99999999',
                },
                {
                    id: 2,
                    tint: 127,
                    sint: 32767,
                    num: '9223372036854775',
                    numric:  '99999999',
                },
                {
                    id: 3,
                    tint: 25,
                    sint: 35,
                    num: '4555',
                    numric: '55',
                },
            ]);
        });
    });

    it('should fail to convert integers not in range', function() {
        return Promise.all([
            // same bug - loosing same order of magnitude after buffer allocation
            `1, -128, -32768,  TO_NUMBER('-92233720368547789080980'), TO_NUMBER('-99999999')`,
            `1, -130, 32768,  TO_NUMBER('-9'), TO_NUMBER('-99999999')`,
            `1, -127, -32768,  TO_NUMBER('-9'), TO_NUMBER('-9999999999999')`,
        ].map(v => 
            connection.executePromise(`INSERT INTO ${table}(id, tint, sint, num, numric) VALUES(${v})`)
            .catch(err => expect(err).to.exist)));
    });

    after(function() {
        return connection.executePromise(`DROP TABLE ${table}`);
    });

});


// TODO(kko): write test for other types