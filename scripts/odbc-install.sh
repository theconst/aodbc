#!/usr/bin/env bash
#
# Script that installs all needed dependencies on Debian Linux distro
# needed for successfull running of the application
#

DRIVER_NAME=${DRIVER_NAME:-"ODBC-2016.2.3.903.6-lnxrhx64.tar.gz"}
DRIVER_FOLDER=${DRIVER_FOLDER:-'2016'}
DRIVER_PATH="ftp://ftp.intersys.com/pub/cache/odbc/${DRIVER_FOLDER}/${DRIVER_NAME}"
ODBCINSTDIR="/usr/lib/cacheodbc"

mkdir -p "${ODBCINSTDIR}"
cd "${ODBCINSTDIR}"
wget --show-progress "${DRIVER_PATH}"
tar -xvzf "${DRIVER_NAME}"

chmod +x ODBCinstall
./ODBCinstall
cd -
