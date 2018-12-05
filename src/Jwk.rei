
type publicJwk = {
  alg: string,
  kty: string,
  n: string,
  e: string,
  kid: string
}

let jwkToPem: publicJwk => string;

module Decode {
  let jwkList: Js.Json.t => Belt.Result.t(list(publicJwk), exn);
}

let getJwkListFromSecretsManager: option(AwsSdk.SecretsManager.config) => string => Js.Promise.t(list(publicJwk));
