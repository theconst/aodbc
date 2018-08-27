'use strict';

const expect = require('chai').expect;


const CACHE_DSN = 'DSN=CacheWinHost';

describe('CacheAdapter DataMapper integration tests', function () {

    this.timeout(60000);

    let aodbc; //SUT

    beforeEach(function () {
        aodbc = require('aodbc');
    });

    it('should display version', function (done) {
        expect(function () {
           aodbc.version((err, version) => {
                expect(err).to.be.null;
                expect(version).to.not.be.empty;
                
                done();
           });
        }).to.not.throw();
    });
    
    it('should return connected for new instance', function (done) {
        const connection = new aodbc.ODBCConnection(CACHE_DSN);
            
        connection.isConnected((err, isConnected) => {
            expect(err).to.be.null;
            expect(isConnected).to.be.true;
                
            done();
        });
    }) 

    afterEach(function () {
     
    });
});


