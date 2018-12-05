'use strict'

const R = require('ramda')
const fs = require('fs')
const {updateJwks} = require('./jwksLib.js')

if (R.length(process.argv) < 5) {
  console.log(`Usage: node jwksUpdate.js JWKS_FILE RSA_PUBLIC_KEY KEY_ID`)
  process.exit(1)
}

const jwksFilename = process.argv[2]
const jwks = fs.existsSync(jwksFilename) ? JSON.parse(fs.readFileSync(jwksFilename, 'utf8')) : {}
const pubkey = fs.readFileSync(process.argv[3], 'utf8')
const kid = process.argv[4]

fs.writeFileSync(jwksFilename, updateJwks(jwks, pubkey, kid), 'utf8')
console.log(`File ${jwksFilename} updated`)
