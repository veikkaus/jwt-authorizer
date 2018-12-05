type maxAge = int;
type token = string;

exception MalformedJWT;
exception NoMatchingJWK;

let verifyToken: list(Jwk.publicJwk) => maxAge => token => Belt.Result.t(Jwt.token, exn);
