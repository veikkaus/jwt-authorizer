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

let parseJsonResult: string => Belt.Result.t(Js.Json.t, exn) =
  data => try (Ok(Json.parseOrRaise(data))) {
    | e => Error(e)
  };

let getJwkListFromSecretsManager = secretsManagerConfig => secretsManagerKey =>
  AwsSdk.SecretsManager.create(secretsManagerConfig)
  -> AwsSdk.SecretsManager.getSecretValue(secretsManagerKey)
  -> Future.map(
    result =>
      result
      -> Belt.Result.flatMap(data => parseJsonResult(data.secretString))
      -> Belt.Result.flatMap(Decode.jwkList))
  -> FutureJs.resultToPromise;
