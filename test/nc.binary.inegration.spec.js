'use strict';

const config = require('./config.json');

const expect = require('chai').expect;

//TODO(kko): with introduction of execute I can migrate to tests with full fixture

describe('ODBC Connection integration tests', function() {
  const INTEGRATION_TEST_DSN = config["dsn"];

  this.timeout(60000);

  let nc;  // SUT

  beforeEach(function() {
    nc = require('nc');
  });

  // BUG - batch blobs are not implemented
  xit('should exctract binary type to javascript buffer', function(done) {
    const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

    connection.query(
        {
          'query': `SELECT ID AS id, PICTURE AS picture FROM Sample.Employee
                      WHERE ID IN (104, 105)`,
          'batchSize': 10,
        },
        (err, resultSet) => {
          expect(err).to.not.exist;

          console.debug(`Query result: ${JSON.stringify(resultSet)}`);

          done();
        });
  });

  // OK - you can read one by one

  it('should extract binary type to javascript buffer', function(done) {
    const connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

    connection.query({
          'query': `SELECT ID AS id, PICTURE AS picture, NOTES as notes FROM Sample.Employee
                    WHERE ID IN (104, 105)`,
          'batchSize': 1,
        }, (err, resultSet) => {
            console.debug(`Query result: ${JSON.stringify(resultSet)}`);

            expect(err).to.not.exist;
            expect(resultSet).to.exist;
            
            expect(resultSet.length).to.equal(2);
            expectEmployee(resultSet[0], 104, 894);
            expectEmployee(resultSet[1], 105, 894);

            done();
    });

    function expectEmployee(employee, id, picSize) {
        expect(employee).to.exist;
        expect(employee.id).to.equal(id);
        expect(employee).to.have.property('picture');
        expect(employee.picture.length).to.equal(picSize);
        expect(employee).to.have.property('notes');
    }
  });

  afterEach(function() {

  });
});
