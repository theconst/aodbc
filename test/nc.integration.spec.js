'use strict';

const expect = require('chai').expect;

describe('ODBC Connection integration tests', function () {
    
    const INTEGRATION_TEST_DSN = 'DSN=CacheWinHost';

    this.timeout(60000);

    let nc; //SUT

    beforeEach(function () {
        nc = require('nc');
    });
    
    
    it('should return connected for new instance', function (done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
            
        connection.isConnected((err, isConnected) => {
            expect(err).to.be.null;
            expect(isConnected).to.be.true;
                
            done();
        });
    });
    
    
    it('should display name of the dbms', function(done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
        connection.dbmsName((err, dbmsName) => {
            expect(err).to.be.null;
            expect(dbmsName).to.exist;
            
            console.debug(`DBMS Name: ${dbmsName}`);
            
            done();
        });
    });
    
    it('should display version of the database', function(done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
        connection.dbmsVersion((err, value) => {
            expect(err).to.be.null;
            
            console.debug(`DBMS version: ${value}`);
            
            expect(value).to.exist;
            
            done();
        });
    });
    
    it('should display driver name', function(done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
        connection.driverName((err, value) => {
            expect(err).to.be.null;
            
            console.debug(`Driver name: ${value}`);
            
            expect(value).to.exist;
            
            done();
        });
    });
    
    //optional feature not implemented by cache - target of the lib
    xit('should display catalog name', function (done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

        connection.catalogName((err, value) => {
            expect(err).to.be.null;

            console.debug(`Catalog name: ${value}`);

            expect(value).to.exist;
            
            done();
        });
    });
    
    it('should display dabase name', function (done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

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
       const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
       
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
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
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
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
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
        const connection = new nc.ODBCConnection();
        
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
        const connection = new nc.ODBCConnection();

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
    
    it('should execut query 6', function (done) {
        testQuery({
            'query': 'Select * FROM Aviation.Event',
            'batchSize': 1000,
        }, done);
    });
    
    function testQuery(args, done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);
        
        connection.query(args, (err, value) => {
            console.debug(`Error: ${err}`);

            // ucomment to see full output
            // console.debug(`Value: ${JSON.stringify(value)}`);

            expect(err).to.not.exist;
            expect(value).to.exist;

            done();
        });
    }

    afterEach(function () {
     
    });
});



