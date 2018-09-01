# AODBC - library that wraps nanodbc

Primarily targeted on intersystems Cache database

WIP - Work in progress

# Launch and build process

## Linux 

Linux x64 is the only supported platform (for now)

### Launch steps (for ubuntu and debian):

* install nodejs - https://nodejs.org/
* install unixodbc - http://www.unixodbc.org/
* copy configuration from odbc.ini with your host address to ~/.odbc.ini
* download ODBC driver for Intersystems Cache 
- http://www.intersystems.com/ru/services-support/product-support/cache-odbc-driver-faq/
* install boost library - sudo apt-get install libboost-all-dev
* run npm install in the project directory
* npm start will lauch the demo application

* binary of nanodbc library is included in repository - TODO - remove. If your architecture or driver differs, you may need to build nanodbc from source - https://github.com/nanodbc/nanodbc

# TODO List

- Add support for prepared statements
- Add support for transactions
- Add support for binary type
- Automate build for linux
- Try to build on Windows machines
- Fetch results in batches instead consuming one by one in result set