'use strict';

const expect = require('chai').expect;

describe('CacheAdapter DataMapper integration tests', function () {

    this.timeout(60000);

    let acallin; //SUT

    beforeEach(function () {
        acallin = require('acallin');
    });

    it('should display version', function () {
        let version;
        expect(function () {
            version = acallin.versionSync();
        }).to.not.throw();
        expect(version).to.not.be.empty;
    });

    afterEach(function () {
     
    });
});


