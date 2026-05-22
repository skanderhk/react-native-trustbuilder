export enum TrustbuilderErrorCode {
  OK = 0,
  NETWORK = 1,
  CODE = 2,
  SN = 3,
  ACCESS = 4,
  VERSION = 5,
  BLOCKED = 7,
  STATE = 8,
  NODEVICE = 9,
  NOCA = 10,
  NOSRV = 11,
  PINREUSED = 12,
  SYNCHROFAILED = 13,
  FORBIDDEN = 14,
  PINREFUSED = 15,
  TIMEOUT = 16,
  BIOKEY = 26,
  DESYNCHRONIZED = 27,
  OTHER = 999,
}

const ERROR_MESSAGES: Record<number, string> = {
  [TrustbuilderErrorCode.OK]: 'No error',
  [TrustbuilderErrorCode.NETWORK]: 'Network or server unreachable',
  [TrustbuilderErrorCode.CODE]: 'The activation code is incorrect',
  [TrustbuilderErrorCode.SN]: 'One argument does not have the right syntax',
  [TrustbuilderErrorCode.ACCESS]: 'Access refused',
  [TrustbuilderErrorCode.VERSION]:
    'Version error, user must upgrade the device',
  [TrustbuilderErrorCode.BLOCKED]: 'Account is blocked',
  [TrustbuilderErrorCode.STATE]: 'Internal state not correct',
  [TrustbuilderErrorCode.NODEVICE]: 'Device is disabled or unknown',
  [TrustbuilderErrorCode.NOCA]:
    'User must activate helium or Desktop Token before achieving this operation',
  [TrustbuilderErrorCode.NOSRV]: 'No service is available',
  [TrustbuilderErrorCode.PINREUSED]: 'The new password equals the previous one',
  [TrustbuilderErrorCode.SYNCHROFAILED]:
    'The operation succeeded but synchronization failed',
  [TrustbuilderErrorCode.FORBIDDEN]:
    'Forbidden operation (due to activated/blocked/upgradable state)',
  [TrustbuilderErrorCode.PINREFUSED]: 'The password is refused (bad format)',
  [TrustbuilderErrorCode.TIMEOUT]: 'Timeout expired',
  [TrustbuilderErrorCode.BIOKEY]: 'Device is locked due to biokey errors',
  [TrustbuilderErrorCode.DESYNCHRONIZED]: 'Device desynchronized',
  [TrustbuilderErrorCode.OTHER]: 'Unknown error',
};

export class TrustbuilderError extends Error {
  public readonly code: TrustbuilderErrorCode;
  public readonly codeName: string;

  constructor(code: number, message?: string) {
    const errorCode = code as TrustbuilderErrorCode;
    const defaultMessage =
      ERROR_MESSAGES[errorCode] || `Unknown error code: ${code}`;
    super(message || defaultMessage);
    this.name = 'TrustbuilderError';
    this.code = errorCode;
    this.codeName = TrustbuilderErrorCode[errorCode] || 'UNKNOWN';
  }
}

export function assertOk(resultCode: number): void {
  if (resultCode !== TrustbuilderErrorCode.OK) {
    throw new TrustbuilderError(resultCode);
  }
}

export function parseResult(resultCode: number): void {
  assertOk(resultCode);
}
