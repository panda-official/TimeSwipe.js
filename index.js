var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
var addon = require(binding_path);

module.exports = new addon.TimeSwipeNAPI();
