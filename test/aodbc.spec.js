'use strict';

const expect = require('chai').expect;


const CACHE_DSN = 'DSN=CacheWinHost';

describe('CacheAdapter DataMapper integration tests', function () {

    this.timeout(60000);

    let aodbc; //SUT

    beforeEach(function () {
        aodbc = require('aodbc');
    });
    
    it('should return connected for new instance', function (done) {
        const connection = new aodbc.ODBCConnection(CACHE_DSN);
            
        connection.isConnected((err, isConnected) => {
            expect(err).to.be.null;
            expect(isConnected).to.be.true;
                
            done();
        });
    });
    
    
    it('should display name of the database', function(done) {
        const connection = new aodbc.ODBCConnection(CACHE_DSN);
        
        connection.dbmsName((err, dbmsName) => {
            expect(err).to.be.null;
            expect(dbmsName).to.not.be.null;
            
            console.debug(`DBMS Name: ${dbmsName}`);
            
            done();
        });
    });
    
    it('should return disconnected after disconnect', function(done) {
       const connection = new aodbc.ODBCConnection(CACHE_DSN);
       
       connection.isConnected((err, connected) => {
          expect(err).to.be.null;
          expect(connected).to.be.true;
          
          connection.disconnect((err, value) => {
              expect(err).to.be.null;
              expect(value).to.be.undefined;
              
              connection.isConnected((err, connected) => {
                  expect(connected).to.be.false;
                  
                  connection.dbmsName((err, name) => {
                      expect(err).to.not.be.null;
                      
                      console.debug(`Error: ${err}`);
                      
                      done();
                  });
              });
          });
       });
    });

    afterEach(function () {
     
    });
});


