'use strict';

const config = require('./config');

const expect = require('chai').expect;

const nc = require("bindings")("nc.node");

describe('ODBC Connection integration tests', function () {
    
    const INTEGRATION_TEST_DSN = config["dsn"];

    this.timeout(60000);

    let connection;

    beforeEach(function (done) {
        connection = new nc.ODBCConnection();
        connection.connect(INTEGRATION_TEST_DSN, () => done());
    });

    it('should connect with dsn, username, password', function(done) {
        new nc.ODBCConnection().connect(INTEGRATION_TEST_DSN, '_SYSTEM', 'SYS', err => {
            expect(err).to.be.null;
            connection.isConnected((err, isConnected) => {
                console.log(err);
                expect(err).to.be.null;
                expect(isConnected).to.be.true;

                done();
            });
        });
    });
    
    
    it('should return connected for new instance', function (done) {
        connection.isConnected((err, isConnected) => {
            expect(err).to.be.null;
            expect(isConnected).to.be.true;
                
            done();
        });
    });
    
    
    it('should display name of the dbms', function(done) {        
        connection.dbmsName((err, dbmsName) => {
            expect(err).to.be.null;
            expect(dbmsName).to.exist;
            
            console.debug(`DBMS Name: ${dbmsName}`);
            
            done();
        });
    });
    
    it('should display version of the database', function(done) {
        connection.dbmsVersion((err, value) => {
            expect(err).to.be.null;
            
            console.debug(`DBMS version: ${value}`);
            
            expect(value).to.exist;
            
            done();
        });
    });
    
    it('should display driver name', function(done) {        
        connection.driverName((err, value) => {
            expect(err).to.be.null;
            
            console.debug(`Driver name: ${value}`);
            
            expect(value).to.exist;
            
            done();
        });
    });
    
    //optional feature not implemented by cache - target of the lib
    xit('should display catalog name', function (done) {
        connection.catalogName((err, value) => {
            expect(err).to.be.null;

            console.debug(`Catalog name: ${value}`);

            expect(value).to.exist;
            
            done();
        });
    });
    
    it('should display dabase name', function (done) {
        connection.databaseName((err, value) => {
            expect(err).to.not.exist;

            console.log(`Database name: ${value}`);

            expect(value).to.exist;
            
            done();
        });
    });
    
    // prints error, but not crashes. disconnect should be performed once in the 
    // javascript wrapper
    xit('should return disconnected after disconnect', function(done) {       
       connection.isConnected((err, connected) => {
          expect(err).to.be.null;
          expect(connected).to.be.true;
          
          connection.disconnect((err, value) => {
              expect(err).to.be.null;
              expect(value).to.be.null;
              
              connection.isConnected((err, connected) => {
                  expect(connected).to.be.false;
                  
                  connection.dbmsName((err, name) => {
                      expect(err).to.exist;
                      
                      console.debug(`Error: ${err}`);
                      
                      done();
                  });
              });
          });
       });
    });
    
    // test is wrong - connection is not usable after disconnect
    xit('should reconnect after disconnect', function(done) {        
        connection.isConnected((err, connected) => {
            expect(err).to.be.null;
            expect(connected).to.be.true;
            
            connection.disconnect((err) => {
                expect(err).to.be.null;
                
                connection.connect(INTEGRATION_TEST_DSN, 10000, (err, value) => {
                    expect(err).to.be.null;
                    expect(value).to.be.undefined;
                    
                    connection.isConnected((err, connected) => {
                        expect(connected).to.be.true;
                        
                        done();
                    });
                });
            });
        });
    });
    
    it('should do do nothing on connection close', function(done) {    
        connection.isConnected((err, connected) => {
            expect(connected).to.be.true;
            
            connection.disconnect((err) => {
                expect(err).to.be.null;
                        
                connection.isConnected((err, connected) => {
                    expect(connected).to.be.false;
                    
                    connection.disconnect(err => {
                        expect(err).to.be.null;
                        connection.isConnected((err, connected) => {
                            expect(err).to.be.null;
                            expect(connected).to.be.false;
                            
                            done();
                        });
                    });
                });    
            });
        });
    });
    
    it('should create with default constructor and no timeout args', function(done) {
        connection = new nc.ODBCConnection();
        connection.isConnected((err, connected) => {
            expect(connected).to.be.false;
            
            connection.connect(INTEGRATION_TEST_DSN, (err, val) => {
                expect(err).to.be.null;
                expect(val).to.not.exist;
                
                connection.isConnected((err, connected) => {
                    expect(err).to.be.null;
                    expect(connected).to.be.true;
                    
                    done();
                });
            });
        });
    });

    it('should create with default constructor and timeout args', function(done) {
        connection.connect(INTEGRATION_TEST_DSN, 10000, (err, val) => {
            expect(err).to.be.null;
            expect(val).to.not.exist;
            
            connection.isConnected((err, connected) => {
                expect(err).to.be.null;
                expect(connected).to.be.true;

                done();
            });
        });
    })
    
    it('should execute query 1', function(done) {
       testQuery('select 1 as Q', done);
    });
    
    it('should execute query 2', function(done) {
        testQuery('SELECT AircraftModel from Aviation.Aircraft', done);
    });
    
    it('should execute query 3', function(done) {
        testQuery('SELECT AircraftSerialNo, AircraftModel from Aviation.Aircraft', done);
    });
    
    it('should execute query 4', function(done) {
        testQuery('Select * from Aviation.Aircraft', done);
    });
    
    it('should execute query 5', function(done) {
        testQuery({
            'query': 'Select Count(*) FROM Aviation.Aircraft',
            'batchSize' : 1,
        }, done);
    });
    
    // TODO: get uncomment
    xit('should execute query 6', function (done) {
        testQuery({
            'query': 'Select * FROM Aviation.Event',
            'batchSize': 1000,
        }, done);
    });
    
    function testQuery(args, done) {        
        connection.query(args, (err, value) => {
            console.debug(`Error: ${err}`);

            // ucomment to see full output
            // console.debug(`Value: ${JSON.stringify(value)}`);

            expect(err).to.not.exist;
            expect(value).to.exist;

            done();
        });
    }

    it('should call stored procedure', function(done) {
        connection.query("CALL Sample.PersonSets('D','NY')", (err, res) => {
            expect(err).to.not.exist;
  
            console.debug(res);
  
            expect(res).to.exist;
  
            done();
        });
    });

    afterEach(function () {
     
    });
});



