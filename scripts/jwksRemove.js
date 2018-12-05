'use strict'

const R = require('ramda')
const fs = require('fs')
const {removeJwksKey} = require('./jwksLib.js')

if (R.length(process.argv) < 4) {
  console.log(`Usage: node jwksRemove.js JWKS_FILE KEY_ID`)
  process.exit(1)
}
const jwksFilename = process.argv[2]
const jwks = fs.existsSync(jwksFilename) ? JSON.parse(fs.readFileSync(jwksFilename, 'utf8')) : {}
const kid = process.argv[3]

fs.writeFileSync(jwksFilename, removeJwksKey(jwks, kid), 'utf8')
console.log(`File ${jwksFilename} updated`)
