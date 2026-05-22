import { TrustbuilderError, TrustbuilderErrorCode } from '../errors';

describe('TrustbuilderError', () => {
  it('creates an error with correct code and message', () => {
    const error = new TrustbuilderError(TrustbuilderErrorCode.NETWORK);
    expect(error.code).toBe(TrustbuilderErrorCode.NETWORK);
    expect(error.codeName).toBe('NETWORK');
    expect(error.message).toBe('Network or server unreachable');
  });

  it('creates an error with custom message', () => {
    const error = new TrustbuilderError(
      TrustbuilderErrorCode.CODE,
      'Custom message'
    );
    expect(error.code).toBe(TrustbuilderErrorCode.CODE);
    expect(error.message).toBe('Custom message');
  });

  it('handles unknown error codes', () => {
    const error = new TrustbuilderError(9999);
    expect(error.code).toBe(9999);
    expect(error.codeName).toBe('UNKNOWN');
    expect(error.message).toBe('Unknown error code: 9999');
  });

  it('has correct name property', () => {
    const error = new TrustbuilderError(TrustbuilderErrorCode.ACCESS);
    expect(error.name).toBe('TrustbuilderError');
  });
});

describe('assertOk', () => {
  const { assertOk } = require('../errors');

  it('does not throw for OK code', () => {
    expect(() => assertOk(0)).not.toThrow();
  });

  it('throws for non-OK codes', () => {
    expect(() => assertOk(1)).toThrow(TrustbuilderError);
    expect(() => assertOk(4)).toThrow(TrustbuilderError);
    expect(() => assertOk(999)).toThrow(TrustbuilderError);
  });
});
