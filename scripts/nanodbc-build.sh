#!/usr/bin/env bash
#
# Builds nanodbc library from git submodule
#

NANODBC_SOURCE_FOLDER=${NANODBC_SOURCE_FOLDER:-"./nanodbc"}
NANODBC_BUILD_FOLDER="${NANODBC_SOURCE_FOLDER}/build"
rm -rf "$NANODBC_BUILD_FOLDER"
mkdir "${NANODBC_BUILD_FOLDER}"
cd "${NANODBC_BUILD_FOLDER}"
cmake -DBUILD_SHARED_LIBS=ON -DNANODBC_ENABLE_UNICODE=OFF ..
cmake .
make --always-make nanodbc
cd -