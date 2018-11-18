/**
 * Entry point of the cacheodbc.js library
 */
'use strict'

// Mappings from CMAKE CMAKE_SYSTEM to node.js platform
var platformMapping = {
    'linux': 'Linux',
    'win32': 'Windows',
    'darwin': 'Darwin',
    'freebsd': 'FreeBSD',
    'sunos': 'SunOS'
};

// Fallback to process platform to allow building from source
var platform = platformMapping[process.platform] || process.platform;
var arch = process.arch;
var version = process.version;
var majorVersion = version.split('.')[0]; // v{majorVersion}

var library = ['cacheodbc', platform, arch, majorVersion].join('-');

module.exports = require("bindings")(library);