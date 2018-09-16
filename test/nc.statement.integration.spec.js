'use strict';

const expect = require('chai').expect;

describe('ODBC Connection integration tests', function () {
    
    const INTEGRATION_TEST_DSN = 'DSN=CacheWinHost';

    this.timeout(60000);

    let nc;
    let connection;
    let statement; //SUT

    beforeEach(function () {
        nc = require('nc');
        connection = new nc.ODBCConnection(INTEGRATION_TEST_DSN);

        statement = new nc.ODBCStatement(connection);
    });
    
    
    it('should create prepared statement', function(done) {
        expect(statement).to.exist;

        done();
    });

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

            statement.query(["F", "L"], (err, result) => {
                expect(err).to.not.exist;
    
                expect(result).to.exist;

                console.debug(result);

                setTimeout(() => statement.query(["H", "K"], (err, result) => {
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

            statement.query(["F", "L"], (err, result) => {
                expect(err).to.not.exist;
    
                expect(result).to.exist;

                console.debug(result);

                done();
            });
        });
    });
    

    afterEach(function () {
     
    });
});



