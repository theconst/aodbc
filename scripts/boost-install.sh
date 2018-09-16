#!/usr/bin/env bash
#
# Installs Boost library
#
BOOST_VERSION=${BOOST_VERSION:-'1.62.0'}
BOOST_VERSION_UNDERSCORE=$(echo ${BOOST_VERSION} | sed -e 's:\.:_:g')

cd /tmp 
wget "http://downloads.sourceforge.net/project/boost/boost/${BOOST_VERSION}/boost_${BOOST_VERSION_UNDERSCORE}.tar.gz"
tar xfz "boost_${BOOST_VERSION_UNDERSCORE}.tar.gz"
rm "boost_${BOOST_VERSION_UNDERSCORE}.tar.gz"
cd "boost_${BOOST_VERSION_UNDERSCORE}"
./bootstrap.sh --prefix=/usr/local --with-libraries=program_options
./b2 install
cd -
rm -rf "boost_${BOOST_VERSION_UNDERSCORE}"