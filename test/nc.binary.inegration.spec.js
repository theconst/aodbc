'use strict';

const expect = require('chai').expect;

describe('ODBC Connection integration tests', function () {
    
    const INTEGRATION_TEST_DSN = 'DSN=CacheWinHost';

    this.timeout(60000);

    let nc; //SUT

    beforeEach(function () {
        nc = require('nc');
    });
    
    it('should exctract binary type to javascript buffer', function(done) {
        const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

        connection.query({
            'query': `SELECT ID AS id, PICTURE AS picture FROM Sample.Employee
                      WHERE ID IN (104, 105)`,
            'batchSize': 10,
        }, (err, resultSet) => {
            expect(err).to.not.exist;

            console.debug(`Query result: ${JSON.stringify(resultSet)}`);

            // resultSet.array.forEach(element => {
            //     console.debug(element.notes.toJSON());
            //     console.debug(element.picture.toJSON());
            // });

            done();
        });
    });
    
    afterEach(function () {
     
    });
});



