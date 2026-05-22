import { TurboModuleRegistry, type TurboModule } from 'react-native';

export type PinMode = 'none' | 'current' | 'new' | 'bio' | 'currentOrBio';

export type PushAction = 'authenticate' | 'activate';

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

export interface Spec extends TurboModule {
  addListener(eventName: string): void;
  removeListeners(count: number): void;

  initialize(config: string): string;
  setStorageData(data: string): boolean;
  getStorageData(): string;
  storageDataChanged(): boolean;

  isActivated(): boolean;
  isBlocked(): boolean;
  mustUpgrade(): boolean;

  activationStart(code: string): string;
  activationFinalize(code: string, pin: string, name: string): string;

  resetStart(code: string): string;
  resetFinalize(code: string, pin: string): string;

  synchronizeStart(): string;
  synchronizeFinalize(pin: string): string;

  pwdUpdateStart(): string;
  pwdUpdateFinalize(newPin: string, currentPin: string): string;

  setBiokeyStart(): string;
  setBiokeyFinalize(biokey: string, pin: string): string;

  unsetBiokeysStart(): string;
  unsetBiokeysFinalize(pin: string): string;

  onlineOtpStart(serviceIndex: number): string;
  onlineOtpFinalize(serviceIndex: number, pin: string, keyType: number): string;
  otpAnswerOtp(): string;

  otpShouldSynchronize(serviceId: number): boolean;
  otpModeQuery(serviceId: number): boolean;
  otpGenerate(pin: string): string;
  displayTime(): number;

  setDeviceOS(deviceOS: string): void;
  pushRegistrationStart(): string;
  pushRegistrationFinalize(pushId: string): string;
  checkPush(): string;
  pushAlias(): string;
  pushAction(): string;
  pushContext(): string;

  onlineSealStart(serviceIndex: number): string;
  onlineSealFinalize(
    serviceIndex: number,
    pin: string,
    keyType: number,
    sealData: string
  ): string;
  sealAnswerOtp(): string;
  sealShouldSynchronize(serviceId: number): boolean;
  sealModeQuery(serviceId: number): boolean;
  sealGenerate(pin: string, sealData: string): string;

  serviceNb(): number;
  serviceName(index: number): string;
  serviceLogo(index: number): string;
  serviceDisabled(index: number): boolean;

  getVersionInfo(): string;
}

export default TurboModuleRegistry.getEnforcing<Spec>('RNTrustbuilder');
