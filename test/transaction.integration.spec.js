'use strict';

const config = require('./config');

const expect = require('chai').expect;

const nc = require('../index');

describe('ODBC Transaction integration tests', function () {
    
    const INTEGRATION_TEST_DSN = config["dsn"];

    const tableName = 'Sample.TransactionTest';

    this.timeout(60000);

    let connection;

    beforeEach(function (done) {
        connection = new nc.ODBCConnection();

        connection.connect(INTEGRATION_TEST_DSN, () => {
            connection.setAutoCommit(false, err => {
                expect(err).to.not.exist;
                connection.execute(`CREATE TABLE ${tableName}(
                    id INTEGER,
                    info VARCHAR(100),
                    CONSTRAINT IDPK PRIMARY KEY(id)
                )`, (err) => {
                    expect(err).to.not.exist;
                    done();
                });
            });
        });
    });

    it('should create transaction', function(done) {
        const tx = new nc.ODBCTransaction(connection);

        expect(tx).to.exist;
        done();
    });

    it('should commit transation', function(done) {
        const tx = new nc.ODBCTransaction(connection);

        tx.begin((err) => {
            expect(err).to.not.exist;

            const stmt = new nc.ODBCStatement(connection);

            stmt.prepare(`INSERT INTO ${tableName}(id, info) VALUES(1, 'test')`, err => {
                expect(err).to.not.exist;

                stmt.execute([], err => {
                    expect(err).to.not.exist;
                    tx.commit(err => {
                        expect(err).to.not.exist;

                        const other = new nc.ODBCConnection();
                        other.connect(INTEGRATION_TEST_DSN, () => {
                            other.query(`SELECT * FROM ${tableName} WHERE id = 1`, (err, res) => {
                                expect(err).to.not.exist;
                                expect(res).to.deep.equal([{id: 1, info: 'test'}]);
    
                                done();
                            });
                        });
                    });
                });
            });
        });
    });


    it('should rollback transation', function(done) {
        const tx = new nc.ODBCTransaction(connection);

        tx.begin((err) => {
            expect(err).to.not.exist;

            const stmt = new nc.ODBCStatement(connection);

            stmt.prepare(`INSERT INTO ${tableName}(id, info) VALUES(2, 'test')`, err => {
                expect(err).to.not.exist;

                stmt.execute([], err => {
                    expect(err).to.not.exist;
                    tx.rollback(err => {
                        expect(err).to.not.exist;

                        const other = new nc.ODBCConnection();
                        other.connect(INTEGRATION_TEST_DSN, () => {
                            other.query(`SELECT * FROM ${tableName} WHERE id = 2`, (err, res) => {
                                expect(err).to.not.exist;
                                expect(res).to.be.empty;
    
                                done();
                            });
                        });
                    });
                });
            });
        });
    })

    afterEach(function (done) {
        connection.execute(`DROP TABLE ${tableName}`, () => done());
    });

});



