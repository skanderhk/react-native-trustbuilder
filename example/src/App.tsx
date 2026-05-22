import { useState } from 'react';
import {
  SafeAreaView,
  ScrollView,
  StyleSheet,
  Text,
  TextInput,
  TouchableOpacity,
  Alert,
  View,
} from 'react-native';
import { trustbuilder, TrustbuilderError } from 'react-native-mfa-trustbuilder';

const DEFAULT_MAC_ID = 'example-mac-id-12345';
const DEFAULT_SERVER = 'https://www.myinwebo.com';

export default function App() {
  const [initialized, setInitialized] = useState(false);
  const [macId, setMacId] = useState(DEFAULT_MAC_ID);
  const [server, setServer] = useState(DEFAULT_SERVER);
  const [activationCode, setActivationCode] = useState('');
  const [pin, setPin] = useState('');
  const [deviceName, setDeviceName] = useState('');
  const [otp, setOtp] = useState('');
  const [status, setStatus] = useState('Enter MAC ID and tap Initialize');
  const [isActivated, setIsActivated] = useState(false);

  const handleError = (error: unknown) => {
    if (error instanceof TrustbuilderError) {
      Alert.alert(
        'Error',
        `${error.codeName} (${error.code}): ${error.message}`
      );
    } else if (error instanceof Error) {
      Alert.alert('Error', error.message);
    } else {
      Alert.alert('Error', 'An unknown error occurred');
    }
  };

  const handleInitialize = () => {
    try {
      const result = trustbuilder.initialize({
        macId: macId || DEFAULT_MAC_ID,
        server: server || DEFAULT_SERVER,
        hostVersion: 'example-1.0.0',
      });

      setInitialized(true);
      setIsActivated(result.isActivated);
      setStatus(
        result.isActivated
          ? 'Activated - Ready to generate OTP'
          : 'Not activated - Enter activation code'
      );
    } catch (error) {
      if (error instanceof Error) {
        setStatus(`Init error: ${error.message}`);
      } else {
        setStatus('Init error: Unknown error');
      }
    }
  };

  const handleActivate = async () => {
    if (!activationCode || !pin || !deviceName) {
      Alert.alert('Missing fields', 'Please fill in all fields');
      return;
    }

    try {
      await trustbuilder.activationStart(activationCode);
      await trustbuilder.activationFinalize(activationCode, pin, deviceName);
      trustbuilder.saveStorageIfNeeded();
      setIsActivated(true);
      setStatus('Activated - Ready to generate OTP');
      Alert.alert('Success', 'Device activated successfully');
    } catch (error) {
      handleError(error);
    }
  };

  const handleGenerateOtp = async () => {
    if (!pin) {
      Alert.alert('Missing PIN', 'Please enter your PIN');
      return;
    }

    try {
      const result = await trustbuilder.generateOnlineOtp(pin);
      setOtp(result.otp);
      setStatus(`OTP generated, expires in ${result.remainingTime}s`);
    } catch (error) {
      handleError(error);
    }
  };

  const handleGenerateOfflineOtp = async () => {
    if (!pin) {
      Alert.alert('Missing PIN', 'Please enter your PIN');
      return;
    }

    try {
      const result = await trustbuilder.generateOfflineOtp(pin);
      setOtp(result.otp);
      setStatus(`Offline OTP generated, expires in ${result.remainingTime}s`);
    } catch (error) {
      handleError(error);
    }
  };

  if (!initialized) {
    return (
      <SafeAreaView style={styles.container}>
        <ScrollView contentContainerStyle={styles.content}>
          <Text style={styles.title}>Trustbuilder MFA</Text>

          <View style={styles.section}>
            <Text style={styles.sectionTitle}>Initialization</Text>
            <TextInput
              style={styles.input}
              placeholder="MAC ID (mAccess ID)"
              value={macId}
              onChangeText={setMacId}
              autoCapitalize="none"
            />
            <TextInput
              style={styles.input}
              placeholder="Server URL"
              value={server}
              onChangeText={setServer}
              autoCapitalize="none"
            />
            <TouchableOpacity style={styles.button} onPress={handleInitialize}>
              <Text style={styles.buttonText}>Initialize</Text>
            </TouchableOpacity>
          </View>

          <Text style={styles.status}>{status}</Text>
        </ScrollView>
      </SafeAreaView>
    );
  }

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.content}>
        <Text style={styles.title}>Trustbuilder MFA</Text>
        <Text style={styles.status}>{status}</Text>

        {!isActivated && (
          <View style={styles.section}>
            <Text style={styles.sectionTitle}>Activation</Text>
            <TextInput
              style={styles.input}
              placeholder="Activation Code (from email)"
              value={activationCode}
              onChangeText={setActivationCode}
              autoCapitalize="none"
            />
            <TextInput
              style={styles.input}
              placeholder="PIN (4-6 digits)"
              value={pin}
              onChangeText={setPin}
              keyboardType="number-pad"
              maxLength={6}
              secureTextEntry
            />
            <TextInput
              style={styles.input}
              placeholder="Device Name"
              value={deviceName}
              onChangeText={setDeviceName}
            />
            <TouchableOpacity style={styles.button} onPress={handleActivate}>
              <Text style={styles.buttonText}>Activate</Text>
            </TouchableOpacity>
          </View>
        )}

        {isActivated && (
          <>
            <View style={styles.section}>
              <Text style={styles.sectionTitle}>Generate OTP</Text>
              <TextInput
                style={styles.input}
                placeholder="PIN"
                value={pin}
                onChangeText={setPin}
                keyboardType="number-pad"
                maxLength={6}
                secureTextEntry
              />
              <TouchableOpacity
                style={styles.button}
                onPress={handleGenerateOtp}
              >
                <Text style={styles.buttonText}>Generate Online OTP</Text>
              </TouchableOpacity>
              <TouchableOpacity
                style={[styles.button, styles.buttonSecondary]}
                onPress={handleGenerateOfflineOtp}
              >
                <Text style={styles.buttonText}>Generate Offline OTP</Text>
              </TouchableOpacity>
            </View>

            {otp ? (
              <View style={styles.section}>
                <Text style={styles.sectionTitle}>Generated OTP</Text>
                <Text style={styles.otp}>{otp}</Text>
              </View>
            ) : null}
          </>
        )}
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  content: {
    padding: 20,
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    marginBottom: 10,
    textAlign: 'center',
  },
  status: {
    fontSize: 16,
    color: '#666',
    textAlign: 'center',
    marginBottom: 20,
  },
  section: {
    backgroundColor: '#fff',
    borderRadius: 8,
    padding: 16,
    marginBottom: 16,
  },
  sectionTitle: {
    fontSize: 18,
    fontWeight: '600',
    marginBottom: 12,
  },
  input: {
    borderWidth: 1,
    borderColor: '#ddd',
    borderRadius: 6,
    padding: 12,
    marginBottom: 12,
    fontSize: 16,
  },
  button: {
    backgroundColor: '#007AFF',
    borderRadius: 6,
    padding: 14,
    alignItems: 'center',
    marginTop: 4,
  },
  buttonSecondary: {
    backgroundColor: '#34C759',
    marginTop: 8,
  },
  buttonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
  },
  otp: {
    fontSize: 32,
    fontWeight: 'bold',
    textAlign: 'center',
    letterSpacing: 4,
    color: '#333',
  },
});
