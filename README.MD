# nc.js (nanodbc.js) - Wrapper for nanodbc library

## Nanodbc
Nanodbc is a proven wrapper for ODBC API. For more details see:
https://nanodbc.github.io/nanodbc/

This library aims to provide a simple access to ODBC data sources for node js for different drivers.


# Current status

**<span style="color:red"> WARNING </span>** - Work in progress.

See "samples" directory for examples.

See test specs for a comprehensive example

# Launch steps

For now Linux is the only supported platform
To launch the application in Docker container, follow the following steps:
1. `git --recurse-submodules clone https://github.com/theconst/aodbc`
2. cd aodbc
3. Change settings for your ODBC host in odbc.ini
4. `docker build -t nc:latest .`
5. 
    - For Linux run the following command
    `docker run -t --network host nc:latest &`
    - For Windows, follow the instructions on https://stackoverflow.com/questions/40746453/how-to-connect-to-docker-host-from-container-on-windows-10-docker-for-windows or if you happen to launch Caché in docker, connect the two via bridge network.
6. `docker exec -it <docker_image_id> /bin/bash`
7. `./samples/nc-crud.js` will run the sample of the library

# TODO List

- [x] Add support for setting fetch size
- [ ] Add tracing
- [x] Generalize delegation code
- [ ] Increase C++ test coverage
- [x] Automate build for linux
- [x] Add support for prepared statements
    - [ ] Add support for dates in prepared statements 
- [ ] Add support for transactions
- [x] Add support for binary type
- [ ] Add support for Windows