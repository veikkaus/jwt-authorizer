# Usage

```reason
let secretsManagerConfig = None;
let maxAge = 3600;

let jwkList = JwtAuthorizer.Jwk.getJwkListFromSecretsManager(secretsManagerConfig, "my-jwk-secret");
let result = JwtAuthorizer.Authorizer.verifyToken(jwkList, maxAge, "token");

switch (result) {
  | Belt.Result.Ok(token) => Js.log2("Verified JWT", token);
  | Belt.Result.Error(error) => Js.log2("Could not verify JWT", error);
}
```

## Running with local secretsManager

```reason
let secretsManagerConfig = JwtAuthorizer.AwsSdk.SecretsManager.config(~endpoint="http://localhost:4584");
[...]
```

# Run tests

```sh
npm run test
```

# Howto create compatible keypairs:

Private key (for jwt issuer):
```
openssl genpkey -algorithm RSA -out privateKey.pem -pkeyopt rsa_keygen_bits:4096
```
Public key (for verifying):
```
openssl rsa -pubout -in privateKey.pem -out publicKey.pem
```

# Properties for creating tokens:

- Header:
  - "alg": "RS512"
  - "typ": "JWT"
  - "kid": "keyId-e.g. MyService"
- Payload:
  - "iat": 1545046543     //timestamp
  - "exp": 1545047143     //timestamp
  - "iss": "self-issued"
  - "clientId": "service name: e.g. my-service"

Tokens signed with private key, public key is
stored in jwks format in aws secrets manager.

# Create jwks or update:

1. If aws secrets manager contains key `jwks-...` copy it into JWKS_FILE or omit this step.
2. Add a public key to jwks:
    ```
    node ./scripts/jwksUpdate.js JWKS_FILE PUBLIC_KEY KEY_ID
    ```
3. Update aws `jwks-...` key with the new jwks file content.


# Secrets Manager commands
```
aws --output json secretsmanager create-secret --name "<name>" --secret-string "<value>"
```
or update
```
aws --output json secretsmanager update-secret --secret-id "<name>" --secret-string "<value>"
```

List :
```
aws --output json secretsmanager list-secrets
```
