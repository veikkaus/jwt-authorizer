type nodeCb('a, 'e) = (Js.Nullable.t('e), Js.Nullable.t('a)) => unit;

let convertToExn: 'e => exn =
  [%bs.raw {| function(raw_e) { return Caml_js_exceptions.internalToOCamlException(raw_e); } |} ];

module SecretsManager {

  type sm;

  [@bs.deriving abstract]
  type config = {
    endpoint: option(string),
    region: option(string)
  };

  [@bs.new] [@bs.module "aws-sdk"] external create: option(config) => sm = "SecretsManager";

  type gsvParams = {.
    "SecretId": string,
    "VersionStage": string
  };

  [@bs.deriving abstract]
  type secretDataJs = {
    [@bs.as "ARN"] arn: string,
    [@bs.as "CreatedDate"] createdDate: Js.Date.t,
    [@bs.as "Name"] name: string,
    [@bs.as "SecretString"] secretString: string,
    [@bs.as "VersionId"] versionId: string
  };

  [@bs.send] external getSecretValue: (sm, gsvParams, nodeCb(secretDataJs, Js.Exn.t)) => unit  = "getSecretValue";

  type secretData = {
    arn: string,
    createdDate: Js.Date.t,
    name: string,
    secretString: string,
    versionId: string
  };

  let secretDataFromJs: secretDataJs => secretData =
    data => ({
      arn: arnGet(data),
      createdDate: createdDateGet(data),
      name: nameGet(data),
      secretString: secretStringGet(data),
      versionId: versionIdGet(data)
    });

  let getSecretValue: sm => string => Future.t(Belt.Result.t(secretData, exn)) =
    (smObj, secretId) =>
      Future.make(resolve => {
        let nodeCb: nodeCb('a, 'e) =
          (error, value) => switch (Js.Nullable.toOption(error), Js.Nullable.toOption(value)) {
            | (_, Some(value)) => resolve(Belt.Result.Ok(value))
            | (errorOpt, _) => resolve(
              errorOpt
              -> Belt.Option.map(convertToExn)
              -> Belt.Option.getWithDefault(Invalid_argument("Callback error"))
              -> Belt.Result.Error)
          };
        getSecretValue(smObj, {"SecretId": secretId, "VersionStage": "AWSCURRENT"}, nodeCb);
      })
      -> Future.mapOk(secretDataFromJs);
};
