open BsYarp;

type publicJwk = {
  alg: string,
  kty: string,
  n: string,
  e: string,
  kid: string
}

type publicJwkAbstract = {.
  "alg": string,
  "kty": string,
  "n": string,
  "e": string,
  "kid": string
};

[@bs.module] external jwkToPemExternal: publicJwkAbstract => string = "jwk-to-pem";

let jwkToPem: publicJwk => string =
  jwk =>
    jwkToPemExternal({"alg": jwk.alg, "kty": jwk.kty, "n": jwk.n, "e": jwk.e, "kid": jwk.kid})

module Decode {
  let jwk = json => Json.Decode.{
    alg: json |> field("alg", string),
    kty: json |> field("kty", string),
    n:   json |> field("n", string),
    e:   json |> field("e", string),
    kid: json |> field("kid", string)
  };

  let jwkList = json => switch (Json.Decode.field("keys", Json.Decode.list(jwk), json)) {
    | value => Belt.Result.Ok(value);
    | exception exn => Belt.Result.Error(exn);
  };
}

let getJwkListFromSecretsManager = secretsManagerConfig => secretsManagerKey =>
  AwsSdk.SecretsManager.create(secretsManagerConfig)
  -> AwsSdk.SecretsManager.getSecretValue(secretsManagerKey)
  -> Promise.mapOk(data => Json.parseOrRaise(data.secretString))
  -> Promise.mapOkResult(Decode.jwkList)
  -> Promise.toJs
