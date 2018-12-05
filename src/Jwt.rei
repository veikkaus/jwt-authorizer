[@bs.deriving abstract]
type verifyOptions = {
  [@bs.optional] algorithms: array(string),
  [@bs.optional] audience: Js.Re.t,
  [@bs.optional] issuer: array(string),
  [@bs.optional] ignoreExpiration: bool,
  [@bs.optional] subject: string,
  [@bs.optional] clockTolerance: int,
  [@bs.optional] maxAge: int,
  [@bs.optional] clockTimestamp: int,
  [@bs.optional] nonce: string
};

type token = {
  clientId: string
};

exception InvalidJWT(exn);

let verify: string => string => verifyOptions => Belt.Result.t(token, exn);

let getKeyId: string => option(string);
