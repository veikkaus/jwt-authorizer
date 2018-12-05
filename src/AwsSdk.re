open BsYarp;

type nodeCb('a, 'e) = (Js.Nullable.t('e), Js.Nullable.t('a)) => unit;

module SecretsManager {

  type sm;

  [@bs.deriving abstract]
  type config = {
    endpoint: string
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

  [@bs.send] external getSecretValue: (sm, gsvParams, nodeCb(secretDataJs, Js.Exn.t)) => unit  = "";

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

  let getSecretValue: sm => string => Promise.t(secretData) =
    (smObj, secretId) =>
      Promise.makeWithCb(cb => getSecretValue(smObj, {"SecretId": secretId, "VersionStage": "AWSCURRENT"}, cb))
      -> Promise.mapOk(secretDataFromJs);
};
