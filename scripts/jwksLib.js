'use strict'

const R = require('ramda')
const NodeRSA = require('node-rsa')
const jwkToPem = require('jwk-to-pem')

function createJwkEntry (pubkey, keyId) {
  const rsa = NodeRSA()
  rsa.importKey(pubkey, 'public')
  const components = rsa.exportKey('components-public')
  const entry = {
    alg: 'RS512',
    kty: 'RSA',
    n: encodeBase64urlUInt(components.n),                      // https://tools.ietf.org/html/rfc7518#section-6.3.1.1
    e: encodeBase64urlUInt(intToBuffer(components.e)),         // https://tools.ietf.org/html/rfc7518#section-6.3.1.2
    kid: keyId
  }
  if (R.trim(pubkey) !== R.trim(jwkToPem(entry))) {
    console.log('Failed to create a valid jwk entry. Check the RSA_PUBLIC_KEY')
    process.exit(1)
  }
  return entry
}

function intToBuffer (n) {
  const b = Buffer.alloc(4)
  b.writeUInt32BE(n, 0)
  return b
}

function encodeBase64urlUInt (buffer) {
  const b = R.dropWhile(x => x === 0, buffer)
  if (R.length(b) === 0) return 'AA'
  return R.dropLastWhile(x => x === '=', b.toString('base64').replace(/\+/g, '-').replace(/\//g, '_'))
}

function updateJwks(jwks, pubkey, keyId) {
  const keylist = R.filter(R.complement(R.propEq('kid', keyId)), R.propOr([], 'keys', jwks))
  const keyEntry = createJwkEntry(pubkey, keyId)
  return JSON.stringify({keys: R.append(keyEntry, keylist)})
}

function removeJwksKey(jwks, keyId) {
  const keylist = R.filter(R.complement(R.propEq('kid', keyId)), R.propOr([], 'keys', jwks))
  return JSON.stringify({keys: keylist})
}

module.exports = {
  updateJwks,
  removeJwksKey
}
