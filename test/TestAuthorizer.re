let describe = BsMocha.Mocha.describe;
module Assert = BsMocha.Assert;
open BsMocha.Async;
open TestUtil;

describe("Authorizer", () => {

  it("should allow valid token", done_ => {
    let result = Authorizer.verifyToken(getTestJwkList(), 1, createValidJwtToken());
    Assert.ok(Belt.Result.isOk(result));
    done_();
  });

  it("should deny token with non-matching key id", done_ => {
    let result = Authorizer.verifyToken(getTestJwkList(), 1, createJwtTokenWithInvalidKeyID());
    Assert.ok(Belt.Result.isError(result));
    done_();
  });

  it("should deny token with no key id", done_ => {
    let result = Authorizer.verifyToken(getTestJwkList(), 1, createJwtTokenWithMissingKeyID());
    Assert.ok(Belt.Result.isError(result));
    done_();
  });

});