# Cache ODBC - javascript access to Intersystems Cache Database
[![Build Status](https://travis-ci.org/theconst/aodbc.svg?branch=master)](https://travis-ci.org/theconst/aodbc)

## Description
This library aims to provide a simple access to ODBC data sources for node.js.

The library is primarily targeted at https://www.intersystems.com/products/cache/#what-sets-it-apart,
but may be suitable for other databases as well.

Library heavily relies on https://github.com/nanodbc/nanodbc - a proven wrapper for ODBC API.

# Current status

The library has alpha quality.
Integration tests are written for Cache database samples.

# Usage

See test specs for a usage example for a raw connector interface.
See https://github.com/theconst/odm-sample for an example web application.
You may also find a comprehensive instruction for different OS configuration.

## Prerequisites

1. Git 2.x.x
1. MSVC Build tools (Windows), GCC 6.x.x + Make (UNIX)
1. Cmake >= 3.2
1. nodejs (tested versions are 8.x.x and 10.x.x)
1. UnixODBC (on Linux)
1. Configured ODBC datasource for the database. You may find configuration samples in config directory.

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

# Docker
To launch the application in Docker container, follow the following steps:

1. Change settings for your ODBC host in odbc.ini
1. `docker build -t cacheodbc:latest .`
1. 
    - For Linux run the following command
    `docker run -t --network host cacheodbc:latest &`
    - For Windows, follow the instructions on https://stackoverflow.com/questions/40746453/how-to-connect-to-docker-host-from-container-on-windows-10-docker-for-windows or if you happen to launch your database in docker, connect the two via bridge network.
1. `docker exec -it <docker_image_id> /bin/bash`
1. `npm t` will run the test for the library

# Releasing

- To trigger a release, commit to a branch with release-* tag.
- Artifacts will be available in the corresponding git releases section

# Packaging

1. Download artifacts from release.
1. Add them to build directory.
1. Add new artifacts (if any to package.json)
1. run npm publish

# TODO List

- [x] Add support for setting fetch size
- [ ] Add tracing. You can enable tracing on ODBC labels
- [x] Generalize delegation code
- [ ] Increase C++ test coverage
- [ ] Rewrite tests to be database-agnostic
- [x] Automate build for linux
- [x] Add support for prepared statements
    - [x] Add support for dates in prepared statements 
- [x] Add support for transactions
- [x] Add support for binary type
- [x] Add support for Windows
- [ ] Rename library to cache-odbc
- [ ] Migrate to bintray to reduce binary package size