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

[@bs.val] [@bs.module "jsonwebtoken"] external verifyExternal: string => string => verifyOptions => Js.t('a) = "verify";

[@bs.deriving jsConverter]
type token = {
  clientId: string
};

exception InvalidJWT(exn);

let verify = token => cert => options => switch (verifyExternal(token, cert, options)) {
  | value => Belt.Result.Ok(tokenFromJs(value));
  | exception e => Error(InvalidJWT(e));
  };

type decodeOptions = {.
  "complete": bool
};

[@bs.deriving abstract]
type decodeHeader = { kid: Js.Nullable.t(string) };

[@bs.deriving abstract]
type decodeResult = { header: Js.Nullable.t(decodeHeader) };

[@bs.val] [@bs.module "jsonwebtoken"] external decodeExternal: string => decodeOptions => Js.Nullable.t(decodeResult) = "decode";

let getKeyId: string => option(string) =
  token =>
    decodeExternal(token, {"complete": true})
    -> Js.Nullable.toOption
    -> Belt.Option.flatMap(result =>
      Js.Nullable.toOption(result->headerGet)
      -> Belt.Option.flatMap(header => Js.Nullable.toOption(header->kidGet)));
