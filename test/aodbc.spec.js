'use strict';

const expect = require('chai').expect;

describe('CacheAdapter DataMapper integration tests', function () {

    this.timeout(60000);

    let aodbc; //SUT

    beforeEach(function () {
        aodbc = require('aodbc');
    });

    it('should display version', function (done) {
        expect(function () {
           aodbc.version((err, version) => {
                if (err) {
                    throw err;
                }
                expect(version).to.not.be.empty;
                done();
           });
        }).to.not.throw();
    });

    afterEach(function () {
     
    });
});


