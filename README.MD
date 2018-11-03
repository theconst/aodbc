# nc.js (nanodbc.js) - Wrapper for nanodbc library

## Nanodbc
Nanodbc is a proven wrapper for ODBC API. For more details see:
https://nanodbc.github.io/nanodbc/

This library aims to provide a simple access to ODBC data sources for node js for different drivers.


# Current status

**<span style="color:red"> WARNING </span>** - Work in progress.

See test specs for a comprehensive example

# Build steps

## Prerequisites

1. Git 2.x.x
1. Cmake >= 3.2
1. nodejs (tested versions are 8.x.x and 10.x.x)
1. UnixODBC (on Linux)
1. Configured ODBC datasource for the database

## Linux




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
- [ ] Fix issued with tar module (low priority)