type maxAge = int;
type token = string;

let jwtOptions = maxAge => Jwt.verifyOptions(
  ~algorithms=[| "RS512" |],
  ~clockTolerance=10,
  ~maxAge=maxAge,
  ());

exception MalformedJWT;
exception NoMatchingJWK;

let verifyToken = jwkList => maxAge => token =>
  switch (Jwt.getKeyId(token)) {
    | Some(kid) => {
      switch (Belt.List.keep(jwkList, (x: Jwk.publicJwk) => x.kid == kid) -> Belt.List.head) {
        | Some(jwk) => Jwt.verify(token, Jwk.jwkToPem(jwk), jwtOptions(maxAge));
        | None => Belt.Result.Error(NoMatchingJWK);
      };
    };
    | None => Belt.Result.Error(MalformedJWT);
  };
