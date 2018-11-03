# nc.js (nanodbc.js) - Wrapper for nanodbc library

## Nanodbc
Nanodbc is a proven wrapper for ODBC API. For more details see:
https://nanodbc.github.io/nanodbc/

This library aims to provide a simple access to ODBC data sources for node.js.

The library is primarily targeted at https://www.intersystems.com/products/cache/#what-sets-it-apart
but may be suitable for other databases as well.

# Current status

The library has alpha quality.
Integration tests are written for Cache database samples.

# Usage

See test specs for a usage example.

## Prerequisites

1. Git 2.x.x
1. MSVC Build tools (Windows), GCC 6.x.x + Make (UNIX)
1. Cmake >= 3.2
1. nodejs (tested versions are 8.x.x and 10.x.x)
1. UnixODBC (on Linux)
1. Configured ODBC datasource for the database

## Build steps

General build steps:

1. git clone `https://github.com/theconst/aodbc`
1. npm install

If the above steps do not work (or custom build options are required):

1. git clone `https://github.com/theconst/aodbc`
1. npm install
1. npm install -g cmake-js
1. cmake-js rebuild -a `<target arch>` -G `<generator to use>`

After installing and building the module, you can use it as a submodule.

# Docker build
To launch the application in Docker container, follow the following steps:

1. Change settings for your ODBC host in odbc.ini
1. `docker build -t nc:latest .`
1. 
    - For Linux run the following command
    `docker run -t --network host nc:latest &`
    - For Windows, follow the instructions on https://stackoverflow.com/questions/40746453/how-to-connect-to-docker-host-from-container-on-windows-10-docker-for-windows or if you happen to launch your database in docker, connect the two via bridge network.
1. `docker exec -it <docker_image_id> /bin/bash`
1. `npm t` will run the test for the library

# TODO List

- [x] Add support for setting fetch size
- [ ] Add tracing
- [x] Generalize delegation code
- [ ] Increase C++ test coverage
- [x] Automate build for linux
- [x] Add support for prepared statements
    - [x] Add support for dates in prepared statements 
- [x] Add support for transactions
- [x] Add support for binary type
- [x] Add support for Windows
- [ ] Rewrite tests to be database-agnostic
- [ ] Fix issued with tar module (low priority)