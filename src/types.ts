export type PinMode = 'none' | 'current' | 'new' | 'bio' | 'currentOrBio';

export type PushAction = 'authenticate' | 'activate';

export type KeyType = 'pin' | 'biokey';

export interface InitConfig {
  macId: string;
  server?: string;
  hostVersion?: string;
  timeout?: number;
  lang?: 'en' | 'fr';
}

export interface InitResult {
  isActivated: boolean;
  isBlocked: boolean;
  error?: string;
}

export interface ServiceInfo {
  name: string;
  logoUrl: string;
  isDisabled: boolean;
}

export interface PushInfo {
  alias: string;
  action: PushAction;
  context: string;
}

export interface OtpResult {
  otp: string;
  remainingTime: number;
}

export interface SealResult {
  seal: string;
}

export interface VersionInfo {
  libraryVersion: string;
  newVersionAvailable: string;
  newVersionUrl: string;
  majorVersionRequired: boolean;
  shouldAskForMinorUpdate: boolean;
}

export interface PinModeResult {
  pinMode: PinMode;
  requiresPin: boolean;
  requiresBio: boolean;
}
