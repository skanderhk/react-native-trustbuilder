# react-native-trustbuilder

Trustbuilder MFA SDK for React Native. Provides multi-factor authentication capabilities including activation, OTP generation, biometric authentication, push notifications, and transaction sealing.

## Installation

```sh
npm install react-native-trustbuilder
```

### iOS

```sh
cd ios && pod install
```

### Android

No additional steps required.

## Usage

```ts
import { trustbuilder, TrustbuilderError } from 'react-native-trustbuilder';

// Initialize (call once at app startup)
const result = trustbuilder.initialize({
  macId: 'your_maccess_id',
  server: 'https://www.myinwebo.com',
  hostVersion: 'my-app-1.0.0',
});

// Restore saved state
const storedData = trustbuilder.getStorageData();
if (storedData) {
  trustbuilder.setStorageData(storedData);
}

// Check activation status
if (trustbuilder.isActivated()) {
  // Generate online OTP
  const { otp, remainingTime } = await trustbuilder.generateOnlineOtp('1234');
}
```

## API Reference

### Initialization

#### `initialize(config: InitConfig): InitResult`

Initialize the Trustbuilder SDK.

```ts
trustbuilder.initialize({
  macId: 'your_maccess_id',        // Required
  server: 'https://www.myinwebo.com', // Default
  hostVersion: 'my-app-1.0.0',     // Default: 'react-native-trustbuilder-0.1.0'
  timeout: 60000,                  // Default: 60000ms
  lang: 'en',                      // 'en' | 'fr', Default: 'en'
});
```

### Activation

#### `activationStart(code: string): Promise<void>`

Start the device activation process.

#### `activationFinalize(code: string, pin: string, name: string): Promise<void>`

Finalize device activation.

#### `activate(code: string, pin: string, name: string): Promise<void>`

Convenience method that calls both start and finalize.

### Unlock / Reset

#### `reset(code: string, pin: string): Promise<void>`

Unlock a blocked device with an unlock code.

### Synchronization

#### `synchronize(pin: string): Promise<void>`

Synchronize device state with Trustbuilder servers.

### PIN Management

#### `updatePin(newPin: string, currentPin: string): Promise<void>`

Update the user's PIN code.

### Biometrics

#### `registerBiokey(biokey: string, pin: string): Promise<void>`

Register a biometric key.

#### `resetBiokeys(pin: string): Promise<void>`

Reset all registered biometric keys.

### OTP Generation

#### `generateOnlineOtp(pin: string, serviceIndex?: number, keyType?: KeyType): Promise<OtpResult>`

Generate an OTP via server call.

#### `generateOfflineOtp(pin: string, serviceId?: number): Promise<OtpResult>`

Generate an OTP locally without network.

### Push Notifications

#### `registerForPush(pushId: string): Promise<void>`

Register device for push notifications.

#### `getPushInfo(): PushInfo | null`

Check for pending push notification.

#### `onPushReceived(listener: (pushInfo: PushInfo) => void): () => void`

Subscribe to push notification events.

### Sealing

#### `generateOnlineSeal(pin: string, sealData: string, serviceIndex?: number, keyType?: KeyType): Promise<SealResult>`

Generate an online seal.

#### `generateOfflineSeal(pin: string, sealData: string, serviceId?: number): Promise<SealResult>`

Generate an offline seal.

### Utility

#### `isActivated(): boolean`

Check if device is activated.

#### `isBlocked(): boolean`

Check if device is blocked.

#### `getServiceInfo(index?: number): ServiceInfo`

Get service information.

#### `getVersionInfo(): VersionInfo`

Get SDK version information.

#### `saveStorageIfNeeded(): void`

Save internal state if it has changed. Call after each operation.

## Error Handling

All methods throw `TrustbuilderError` on failure:

```ts
try {
  await trustbuilder.generateOnlineOtp('1234');
} catch (error) {
  if (error instanceof TrustbuilderError) {
    console.log(error.code);      // Numeric error code
    console.log(error.codeName);  // e.g., 'NETWORK', 'CODE', 'ACCESS'
    console.log(error.message);   // Human-readable message
  }
}
```

### Error Codes

| Code | Name | Description |
|------|------|-------------|
| 0 | OK | No error |
| 1 | NETWORK | Network or server unreachable |
| 2 | CODE | Activation code is incorrect |
| 3 | SN | Syntax error |
| 4 | ACCESS | Access refused |
| 5 | VERSION | Version error |
| 7 | BLOCKED | Account is blocked |
| 14 | FORBIDDEN | Forbidden operation |
| 15 | PINREFUSED | Bad PIN format |
| 16 | TIMEOUT | Timeout expired |
| 26 | BIOKEY | Device locked due to biokey errors |
| 27 | DESYNCHRONIZED | Device desynchronized |
| 999 | OTHER | Unknown error |

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
