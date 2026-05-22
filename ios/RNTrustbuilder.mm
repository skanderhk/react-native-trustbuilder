#import "RNTrustbuilder.h"
#import "iw.h"
#import <React/RCTEventEmitter.h>

@interface RNTrustbuilder () <RCTBridgeModule>
@property (nonatomic, assign) IW *iw;
@property (nonatomic, assign) BOOL isInitialized;
@property (nonatomic, strong) NSCondition *webCallCondition;
@property (nonatomic, strong) NSString *webCallResult;
@end

@implementation RNTrustbuilder

RCT_EXPORT_MODULE()

- (instancetype)init {
  self = [super init];
  if (self) {
    _webCallCondition = [[NSCondition alloc] init];
    _isInitialized = NO;
  }
  return self;
}

- (void)saveStorage {
  if (IWStorageDataChanged(self.iw) > 0) {
    char *data = IWStorageDataGet(self.iw);
    if (data) {
      NSString *dataStr = [NSString stringWithCString:data encoding:NSISOLatin1StringEncoding];
      [self setStorageDataInternal:dataStr];
      IWStorageDataRelease(self.iw);
    }
  }
}

- (NSString *)getStorageDataInternal {
  NSString *data = [[NSUserDefaults standardUserDefaults] stringForKey:@"trustbuilder_storage_data"];
  return data ?: @"";
}

- (void)setStorageDataInternal:(NSString *)data {
  [[NSUserDefaults standardUserDefaults] setObject:data forKey:@"trustbuilder_storage_data"];
  [[NSUserDefaults standardUserDefaults] synchronize];
}

- (IW *)createIWInstance {
  return nil;
}

- (NSNumber *)initialize:(NSString *)config {
  NSError *error = nil;
  NSData *configData = [config dataUsingEncoding:NSUTF8StringEncoding];
  NSDictionary *configDict = [NSJSONSerialization JSONObjectWithData:configData options:0 error:&error];

  NSString *macId = configDict[@"macId"];
  NSString *server = configDict[@"server"] ?: @"https://www.myinwebo.com";
  NSString *hostVersion = configDict[@"hostVersion"] ?: @"react-native-mfa-trustbuilder-0.1.0";
  NSInteger timeout = [configDict[@"timeout"] integerValue] ?: 60000;
  NSString *lang = configDict[@"lang"] ?: @"1";

  NSString *sn = [[UIDevice currentDevice] identifierForVendor].UUIDString;
  NSString *appData = @"_";

  char *cSN = strdup([sn UTF8String]);
  char *cAppData = strdup([appData UTF8String]);

  __weak RNTrustbuilder *weakSelf = self;

  self.iw = IWInit(0, cSN, cAppData, ^(char *url, int timeoutMs, void *user) {
    RNTrustbuilder *strongSelf = weakSelf;
    if (!strongSelf) return;

    NSString *urlStr = [NSString stringWithCString:url encoding:NSISOLatin1StringEncoding];

    NSURL *nsUrl = [NSURL URLWithString:urlStr];
    NSURLRequest *request = [NSURLRequest requestWithURL:nsUrl
                                             timeoutInterval:timeoutMs / 1000.0];

    __block NSString *resultStr = nil;
    __block BOOL completed = NO;

    [[[NSURLSession sharedSession] dataTaskWithRequest:request
                                     completionHandler:^(NSData *data, NSURLResponse *response, NSError *err) {
      if (data && !err) {
        const char *bytes = (const char *)[data bytes];
        char *buffer = malloc([data length] + 1);
        memcpy(buffer, bytes, [data length]);
        buffer[[data length]] = '\0';

        IWSetWsBuffer(user, buffer);
        free(buffer);
      }

      [strongSelf.webCallCondition lock];
      completed = YES;
      [strongSelf.webCallCondition signal];
      [strongSelf.webCallCondition unlock];
    }] resume];

    [strongSelf.webCallCondition lock];
    while (!completed) {
      [strongSelf.webCallCondition wait];
    }
    [strongSelf.webCallCondition unlock];
  }, NULL);

  free(cSN);
  free(cAppData);

  IWHostVersionSet(self.iw, strdup([hostVersion UTF8String]));
  IWWsServerSet(self.iw, strdup([server UTF8String]));
  IWWsTimeoutSet(self.iw, (int)timeout);
  IWMaccessSet(self.iw, strdup([macId UTF8String]));
  IWLangSet(self.iw, strdup([lang UTF8String]));

  NSString *storedData = [self getStorageDataInternal] ?: @"";
  char *cData = strdup([storedData cStringUsingEncoding:NSISOLatin1StringEncoding]);
  IWStorageDataSet(self.iw, cData);
  free(cData);

  self.isInitialized = YES;

  NSDictionary *result = @{
    @"isActivated": @(IWIsActivated(self.iw) == 1),
    @"isBlocked": @(IWIsBlocked(self.iw) == 1)
  };

  NSData *jsonData = [NSJSONSerialization dataWithJSONObject:result options:0 error:nil];
  return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}

- (BOOL)setStorageData:(NSString *)data {
  [self setStorageDataInternal:data];
  char *cData = strdup([data cStringUsingEncoding:NSISOLatin1StringEncoding]);
  int result = IWStorageDataSet(self.iw, cData);
  free(cData);
  return result == IW_ERR_OK;
}

- (NSString *)getStorageData {
  return [self getStorageDataInternal];
}

- (BOOL)storageDataChanged {
  return IWStorageDataChanged(self.iw) > 0;
}

- (BOOL)isActivated {
  return IWIsActivated(self.iw) == 1;
}

- (BOOL)isBlocked {
  return IWIsBlocked(self.iw) == 1;
}

- (BOOL)mustUpgrade {
  return IWMustUpgrade(self.iw) == 1;
}

- (NSString *)activationStart:(NSString *)code {
  char *cCode = strdup([code UTF8String]);
  int result = IWActivationStart(self.iw, cCode);
  free(cCode);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)activationFinalize:(NSString *)code pin:(NSString *)pin name:(NSString *)name {
  char *cCode = strdup([code UTF8String]);
  char *cPin = strdup([pin UTF8String]);
  char *cName = strdup([name UTF8String]);
  int result = IWActivationFinalize(self.iw, cCode, cPin, cName);
  free(cCode);
  free(cPin);
  free(cName);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)resetStart:(NSString *)code {
  char *cCode = strdup([code UTF8String]);
  int result = IWResetStart(self.iw, cCode);
  free(cCode);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)resetFinalize:(NSString *)code pin:(NSString *)pin {
  char *cCode = strdup([code UTF8String]);
  char *cPin = strdup([pin UTF8String]);
  int result = IWResetFinalize(self.iw, cCode, cPin);
  free(cCode);
  free(cPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)synchronizeStart {
  int result = IWSynchronizeStart(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)synchronizeFinalize:(NSString *)pin {
  char *cPin = strdup([pin UTF8String]);
  int result = IWSynchronizeFinalize(self.iw, cPin);
  free(cPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)pwdUpdateStart {
  int result = IWPwdUpdateStart(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)pwdUpdateFinalize:(NSString *)newPin currentPin:(NSString *)currentPin {
  char *cNewPin = strdup([newPin UTF8String]);
  char *cCurrentPin = strdup([currentPin UTF8String]);
  int result = IWPwdUpdateFinalize(self.iw, cNewPin, cCurrentPin);
  free(cNewPin);
  free(cCurrentPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)setBiokeyStart {
  int result = IWSetBiokeyStart(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)setBiokeyFinalize:(NSString *)biokey pin:(NSString *)pin {
  char *cBiokey = strdup([biokey UTF8String]);
  char *cPin = strdup([pin UTF8String]);
  int result = IWSetBiokeyFinalize(self.iw, cBiokey, cPin);
  free(cBiokey);
  free(cPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)unsetBiokeysStart {
  int result = IWUnsetBiokeysStart(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)unsetBiokeysFinalize:(NSString *)pin {
  char *cPin = strdup([pin UTF8String]);
  int result = IWUnsetBiokeysFinalize(self.iw, cPin);
  free(cPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)onlineOtpStart:(double)serviceIndex {
  int result = IWOnlineOtpStart(self.iw, (int)serviceIndex);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)onlineOtpFinalize:(double)serviceIndex pin:(NSString *)pin keyType:(double)keyType {
  char *cPin = strdup([pin UTF8String]);
  int result = IWOnlineOtpFinalizeExt(self.iw, (int)serviceIndex, cPin, (int)keyType);
  free(cPin);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)otpAnswerOtp {
  char *otp = IWOtpAnswerOtp(self.iw);
  if (!otp) return @"";
  NSString *result = [NSString stringWithCString:otp encoding:NSISOLatin1StringEncoding];
  return result;
}

- (BOOL)otpShouldSynchronize:(double)serviceId {
  return IWOtpShouldSynchronize(self.iw, (int)serviceId) == 1;
}

- (BOOL)otpModeQuery:(double)serviceId {
  return IWOtpModeQuery(self.iw, (int)serviceId) == 1;
}

- (NSString *)otpGenerate:(NSString *)pin {
  char *cPin = strdup([pin UTF8String]);
  char *otp = IWOtpGenerateMa(self.iw, cPin);
  free(cPin);
  [self saveStorage];
  if (!otp) return @"";
  NSString *result = [NSString stringWithCString:otp encoding:NSISOLatin1StringEncoding];
  return result;
}

- (double)displayTime {
  return (double)IWDisplayTime(self.iw);
}

- (void)setDeviceOS:(NSString *)deviceOS {
  char *cOS = strdup([deviceOS UTF8String]);
  IWSetDeviceOS(self.iw, cOS);
  free(cOS);
}

- (NSString *)pushRegistrationStart {
  int result = IWPushRegistrationStart(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)pushRegistrationFinalize:(NSString *)pushId {
  char *cPushId = strdup([pushId UTF8String]);
  int result = IWPushRegistrationFinalize(self.iw, cPushId);
  free(cPushId);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)checkPush {
  int result = IWCheckPush(self.iw);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)pushAlias {
  char *alias = IWPushAlias(self.iw);
  if (!alias) return @"";
  NSString *result = [NSString stringWithCString:alias encoding:NSISOLatin1StringEncoding];
  return result;
}

- (NSString *)pushAction {
  char *action = IWPushAction(self.iw);
  if (!action) return @"";
  NSString *result = [NSString stringWithCString:action encoding:NSISOLatin1StringEncoding];
  return result;
}

- (NSString *)pushContext {
  char *context = IWPushContext(self.iw);
  if (!context) return @"";
  NSString *result = [NSString stringWithCString:context encoding:NSISOLatin1StringEncoding];
  return result;
}

- (NSString *)onlineSealStart:(double)serviceIndex {
  int result = IWOnlineSealStart(self.iw, (int)serviceIndex);
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)onlineSealFinalize:(double)serviceIndex pin:(NSString *)pin keyType:(double)keyType sealData:(NSString *)sealData {
  char *cPin = strdup([pin UTF8String]);
  char *cSealData = strdup([sealData UTF8String]);
  int result = IWOnlineSealFinalizeExt(self.iw, (int)serviceIndex, cPin, (int)keyType, cSealData);
  free(cPin);
  free(cSealData);
  [self saveStorage];
  return [NSString stringWithFormat:@"%d", result];
}

- (NSString *)sealAnswerOtp {
  char *otp = IWSealAnswerOtp(self.iw);
  if (!otp) return @"";
  NSString *result = [NSString stringWithCString:otp encoding:NSISOLatin1StringEncoding];
  return result;
}

- (BOOL)sealShouldSynchronize:(double)serviceId {
  return IWSealShouldSynchronize(self.iw, (int)serviceId) == 1;
}

- (BOOL)sealModeQuery:(double)serviceId {
  return IWSealModeQuery(self.iw, (int)serviceId) == 1;
}

- (NSString *)sealGenerate:(NSString *)pin sealData:(NSString *)sealData {
  char *cPin = strdup([pin UTF8String]);
  char *cSealData = strdup([sealData UTF8String]);
  char *seal = IWSealGenerate(self.iw, cPin, cSealData);
  free(cPin);
  free(cSealData);
  [self saveStorage];
  if (!seal) return @"";
  NSString *result = [NSString stringWithCString:seal encoding:NSISOLatin1StringEncoding];
  return result;
}

- (double)serviceNb {
  return (double)IWServiceNb(self.iw);
}

- (NSString *)serviceName:(double)index {
  char *name = IWServiceName(self.iw, (int)index);
  if (!name) return @"";
  NSString *result = [NSString stringWithCString:name encoding:NSISOLatin1StringEncoding];
  return result;
}

- (NSString *)serviceLogo:(double)index {
  char *logo = IWServiceLogo(self.iw, (int)index);
  if (!logo) return @"";
  NSString *result = [NSString stringWithCString:logo encoding:NSISOLatin1StringEncoding];
  return result;
}

- (BOOL)serviceDisabled:(double)index {
  return IWServiceDisabled(self.iw, (int)index) == 1;
}

- (NSString *)getVersionInfo {
  NSDictionary *result = @{
    @"libraryVersion": @(IWVersionGet(self.iw) ? [NSString stringWithCString:IWVersionGet(self.iw) encoding:NSISOLatin1StringEncoding] : @""),
    @"newVersionAvailable": @(IWNewVersionAvailable(self.iw) ? [NSString stringWithCString:IWNewVersionAvailable(self.iw) encoding:NSISOLatin1StringEncoding] : @""),
    @"newVersionUrl": @(IWNewVersionURL(self.iw) ? [NSString stringWithCString:IWNewVersionURL(self.iw) encoding:NSISOLatin1StringEncoding] : @""),
    @"majorVersionRequired": @(IWMajorVersionRequired(self.iw) == 1),
    @"shouldAskForMinorUpdate": @(IWShouldAskForMinorUpdate(self.iw) == 1)
  };

  NSData *jsonData = [NSJSONSerialization dataWithJSONObject:result options:0 error:nil];
  return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}

- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return std::make_shared<facebook::react::NativeRNTrustbuilderSpecJSI>(params);
}

+ (NSString *)moduleName {
  return @"RNTrustbuilder";
}

+ (BOOL)requiresMainQueueSetup {
  return NO;
}

- (NSArray<NSString *> *)supportedEvents {
  return @[@"TrustbuilderPushEvent"];
}

@end
