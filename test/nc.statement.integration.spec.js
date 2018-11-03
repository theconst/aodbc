'use strict';

const config = require('./config');

const expect = require('chai').expect;

const nc = require("bindings")("nc.node");

describe('ODBC Statement integration tests', function () {
    
    const INTEGRATION_TEST_DSN = config["dsn"];

    this.timeout(60000);

    let connection;
    let statement; //SUT

    beforeEach(function (done) {
        connection = new nc.ODBCConnection();
        connection.connect(INTEGRATION_TEST_DSN, () => {
            statement = new nc.ODBCStatement(connection);
            done(); 
        });
    });
    
    
    it('should create prepared statement', function(done) {
        expect(statement).to.exist;

        statement.isOpen((err, res) => {
            expect(err).to.not.exist;
            expect(res).to.be.false;

            statement.prepare('select 1 as P', err => {
                statement.isOpen((err, res) => {
                    expect(err).to.not.exist;
                    expect(res).to.be.true;

                    done();
                });
            });
        });
    });

    it('should execute prepared statement with no args 1', function(done) {
       statement.prepare('Select 1 as Q', (err) => {
            expect(err).to.not.exist;

            statement.query([], (err, res) => {
                expect(err).to.not.exist;
                expect(res).to.deep.equal([{'Q': 1}]);

                done();
            })
       });
    });

    it('should execute prepared statement with no args 2', function(done) {
        statement.prepare('Select 1 as Q', (err) => {
            expect(err).to.not.exist;

            statement.query({
                batchSize: 2,
            }, (err, res) => {
                expect(err).to.not.exist;
                expect(res).to.deep.equal([{'Q': 1}]);

                done();
            })
        });
     });

    //TODO(kko): looks like a  bug
    it('should execute prepared statement twice 1', function(done) {
        statement.prepare('select ? as Q', (err, result) => {
            expect(result).to.not.exist;

            //TODO(kko): make overload less ugly to use
            statement.query([1], (err, result) => {
                expect(err).to.not.exist;
    
                expect(result).to.deep.equal([{'Q': '1'}]);

                console.debug(result);

                statement.query([2], (err, result) => {
                    expect(err).to.not.exist;
        
                    expect(result).to.deep.equal([{'Q': '2'}]);
    
                    console.debug(result);
    
                    done();
                });
            });
        });
    });

    it('should execute prepared statement twice 2', function(done) {
        statement.prepare(`
            Select ID, Name, Company->Name from Sample.Employee 
            Where Name < ? and Company->Name > ? 
            Order By Company->Name
        `, (err, result) => {
            expect(err).to.not.exist;
            expect(result).to.not.exist;

            statement.query(['F', 'L'], (err, result) => {
                expect(err).to.not.exist;
    
                expect(result).to.exist;

                console.debug(result);

                setTimeout(() => statement.query(['H', 'K'], (err, result) => {
                    expect(err).to.not.exist;
        
                    expect(result).to.exist;
    
                    console.debug(result);
    
                    done();
                }), 0);
            });
        });
    });

    it('should execute no args prepared statement', function(done) {
        statement.prepare(`
            Select ID, Name, Company->Name from Sample.Employee 
            Where Name < ? and Company->Name > ? 
            Order By Company->Name
        `, (err, result) => {
            expect(err).to.not.exist;
            expect(result).to.not.exist;

            statement.query(['F', 'L'], (err, result) => {
                expect(err).to.not.exist;
    
                expect(result).to.exist;

                console.debug(result);

                done();
            });
        });
    });
    
    it('should close statment and not query thereafter', function(done) {
        statement.prepare(`
            Select ID, Name, Company->Name from Sample.Employee 
            Where Name < ? and Company->Name > ? 
            Order By Company->Name
        `, (err) => {
            expect(err).to.not.exist;
            statement.query(['H', 'L'], (err, res) => {
                expect(err).to.not.exist;

                expect(res).to.exist;

                statement.close((err) => {
                    expect(err).to.not.exist;

                    statement.query(['L', 'Z'], (err) => {
                        console.debug(err);

                        expect(err).to.exist;
                        done();
                    })
                })
            });
        });
    });

    afterEach(function (done) {
        statement.close((err) => {
            expect(err).to.not.exist;

            done();
        })
    });
});



