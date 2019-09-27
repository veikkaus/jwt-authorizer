type readFileOpt = {.
  "encoding": string,
  "flag": string
};
[@bs.val] [@bs.module "fs"] external readFileExternal: string => readFileOpt => string = "readFileSync";

let readFile: string => string =
  filename => readFileExternal(filename, {"encoding": "utf8", "flag": "r"});

[@bs.val] [@bs.module "../../../scripts/jwksLib.js"] external updateJwksExternal: Js.t({.}) => string => string => string = "updateJwks";

let getTestJwkList: unit => list(Jwk.publicJwk) =
  () =>
    readFile("./test/resources/publicKey.pem")
    -> updateJwksExternal(Js.Obj.empty(), _, "testID")
    -> Json.parseOrRaise
    -> Jwk.Decode.jwkList
    -> Belt.Result.getExn;

type tokenData = {.
  "clientId": string
};

type signingConfig = {.
  "algorithm": string,
  "expiresIn": int,
  "issuer": string,
  "keyid": string
};

type signingConfig2 = {.
  "algorithm": string,
  "expiresIn": int,
  "issuer": string
};

[@bs.val] [@bs.module "jsonwebtoken"] external signExternal: tokenData => string => signingConfig => string = "sign";
[@bs.val] [@bs.module "jsonwebtoken"] external signExternal2: tokenData => string => signingConfig2 => string = "sign";


let createValidJwtToken: unit => string =
  () => signExternal(
    {"clientId": "testing"},
    readFile("./test/resources/privateKey.pem"),
    {"algorithm": "RS512", "expiresIn": 600, "issuer": "self-issued", "keyid": "testID"});

let createJwtTokenWithInvalidKeyID: unit => string =
  () => signExternal(
    {"clientId": "testing"},
    readFile("./test/resources/privateKey.pem"),
    {"algorithm": "RS512", "expiresIn": 600, "issuer": "self-issued", "keyid": "wrongID"});

let createJwtTokenWithMissingKeyID: unit => string =
  () => signExternal2(
    {"clientId": "testing"},
    readFile("./test/resources/privateKey.pem"),
    {"algorithm": "RS512", "expiresIn": 600, "issuer": "self-issued"});
