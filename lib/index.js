const Path			= require('path')
const Binary		= require('node-pre-gyp')
const binding_path	= Binary.find(Path.resolve(Path.join(__dirname,'../package.json')))
const {Gist}		= require(binding_path)

module.exports = Gist