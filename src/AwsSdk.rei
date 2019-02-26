open BsYarp;

module SecretsManager {
  type sm;

  [@bs.deriving abstract]
  type config = {
    endpoint: option(string),
    region: option(string)
  };

  let create: option(config) => sm;

  type secretData = {
    arn: string,
    createdDate: Js.Date.t,
    name: string,
    secretString: string,
    versionId: string
  };

  let getSecretValue: sm => string => Promise.t(secretData);
};
