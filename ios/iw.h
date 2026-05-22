
#define MACCESS_ONLY
#define HIDE_SECRET

#include<stdio.h>

//#define DBG_MEM

#ifdef DBG_MEM
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif

#include<string.h>

//comment of uncomment following line to activate library traces
//#define TRON

#ifdef __linux__ // or something like that...
#define __stdcall
#endif

//DO NOT TOUCH BELOW
#ifdef _USRDLL
#define DLLIMPIW __declspec (dllexport)
#define DLLCALL __stdcall
#define CALLBACK __stdcall
#else
#define DLLIMPIW
#define DLLCALL
#define CALLBACK
#endif

#ifdef WINPRT
extern "C" {
#endif

#define MSGSYNC_DELAY (3600*24*7)
#define LEN_OLDPWD 10
#define OTPSYNC_DELAY (2*60)
#define SHA512_LENGTH 65

#define CA_LIBVERSION "0.2.1"
#define CA_LIBVERSION_0_0_10 "CA.0.0.10"
#define CA_LIBVERSION_0_0_10b "0.0.10"
#define CA_LIBVERSION_0_0_11 "0.0.11"
#define CA_LIBVERSION_0_1_0 "0.1.0"
#define CA_WEBSERVICE "/iwws/CA"

#define MA_LIBVERSION_0_0_10 "0.0.10"
#define MA_LIBVERSION_0_0_11 "0.0.11"
#define MA_LIBVERSION_0_0_12 "0.0.12"
#define MA_LIBVERSION_0_0_13 "0.0.13"
#define MA_LIBVERSION_0_2_1 "0.2.1"
#define MA_LIBVERSION_0_2_2 "0.2.2"
#define MA_LIBVERSION_0_2_3 "0.2.3"
#define MA_LIBVERSION_0_2_4 "0.2.4"
#define MA_LIBVERSION_0_2_5 "0.2.5"
#define MA_LIBVERSION_0_2_6 "0.2.6"
#define MA_LIBVERSION_0_2_7 "0.2.7"
#define MA_LIBVERSION_0_2_8 "0.2.8"
#define MA_LIBVERSION_0_2_9 "0.2.9"
#define MA_LIBVERSION_0_2_10 "0.2.10"
#define MA_LIBVERSION_0_2_11 "0.2.11"
#define MA_LIBVERSION_0_2_12 "0.2.12"
#define MA_LIBVERSION_0_2_13 "0.2.13"
#define MA_LIBVERSION_0_2_14 "0.2.14"
	
#define MA_LIBVERSION "0.2.16" // warning: update iwdata.c when change
#define MA_WEBSERVICE "/iwws/MA"

#define MAC_LIBVERSION_0_2_1 "0.2.1"
#define MAC_LIBVERSION_0_2_2 "0.2.2"
#define MAC_LIBVERSION_0_2_3 "0.2.3"
#define MAC_LIBVERSION_0_2_4 "0.2.4"
#define MAC_LIBVERSION_0_2_5 "0.2.5"
#define MAC_LIBVERSION_0_2_6 "0.2.6"
#define MAC_LIBVERSION_0_2_7 "0.2.7"
#define MAC_LIBVERSION_0_2_8 "0.2.8"
#define MAC_LIBVERSION_0_2_11 "0.2.11"
#define MAC_LIBVERSION_0_2_15 "0.2.15"
#define MAC_LIBVERSION_0_2_16 "0.2.16"
#define MAC_LIBVERSION_0_2_17 "0.2.17"
#define MAC_LIBVERSION_0_2_18 "0.2.18"
#define MAC_LIBVERSION_0_2_19 "0.2.19"
#define MAC_LIBVERSION "0.2.19"

#define MAC_WEBSERVICE "/iwcxf/MAC"

#define separe "&&"

#define RESULT_USED 0
#define RESULT_CANCEL 1
#define STACKRAND_LEN 10
#define STACK_LEN 4

#define IWNEW(type,n) (type*)malloc((n)*sizeof(type))
#define IWDELETE(f) if (f) free(f)

extern int MYTRON;
#define PRINTF if(MYTRON)printf

//#define PKCS1

typedef int (CALLBACK *WEBSERVICECALL)(char* url,int timeout,void* user);
typedef int (*IWCALLBACK)(void* user, int ret);
typedef int (CALLBACK *WEBSERVICECALLASYNC)(char* url,int timeout, IWCALLBACK callback, void* user);

typedef struct xmlattr {
	char* attr;
	char* val;
	struct xmlattr *next;
}xmlattr;

typedef struct xmltag {
	char* tag;
	xmlattr *lattr;
	struct xmltag *sons;
	struct xmltag *next;
	char* data;
}xmltag;

xmltag* xmlTagCreate(char* tag);
void xmlTagLink(xmltag* father,xmltag* son);
void xmlTagData(xmltag* x,char* data);
void xmlTagAttr(xmltag* x,char* attr,char* val);
void xmlTagAttrI(xmltag* x,char* attr,int val);
void xmlTagAttrF(xmltag* x,char* attr,float val);
void xmlTagDelete(xmltag* x);

void xmlToString(xmltag* x);

xmltag* xmlSubtagNth(xmltag* x,char* tag,int n);
int xmlSubtagNb(xmltag* x,char* tag);
xmltag* xmlSubtag(xmltag* x,char* tag);
xmltag* xmlSubtag0(xmltag* x);
char* xmlAttr(xmltag* x,char* attr);
xmltag* xmlSubtagAttr(xmltag* x,char* tag,char* attr,char* val);
void xmlInvert(xmltag* x);
xmltag* xmlFromString(char *src);

typedef struct
{
	int id;
	char* name;
	char* logo;
	char* secure;
	char* ksc;
	int connected;
	int onlineotp;
// CA / MA
	char* url;
	char *accountname;
	char* pattern;
//
}IWSRV;

typedef struct
{
	int id;
	char* val;
}IWSEC;
typedef struct
{
	char* key;
	char* val;
}IWLOCAL;

typedef struct
{
	int id;
	char* title;
	char* content;
	int ack;
}IWMSG;

typedef struct IWDATA
{
	int mustupgrade;
	char* initialState;
	char* localkey;
	int dataupdate;

	char* id;
	char* alea;
	int blocked;
	int hasnopin;
	int Tsync;
	char* KpubFactory;
// CA and MAC
	int connected;
// MAC
	char* server;
//
// MA
	char* J;
	char* K;
	char* K0;
	char* K1;
	int cancelpin;
	int nboka;
	int lastt1;
	int lastt2;
	int lastt3;
	int lastbp;
	char* stackrand;
	char* stack;
	char* H;
	int Tref;
//
	int srvn;
	IWSRV* srv;

	int secn;
	IWSEC* sec;

	int localn;
	IWLOCAL* local;

	int msgtime;
	int msgn;
	IWMSG* msg;

	int majorversion;
	char* newversion;
	char* newversionurl;

}IWDATA;

typedef struct IWPROMPT
{
	char* id;
	char* label;
	int hidden;
	int save;
	char* value;
}IWPROMPT;

typedef struct IWFIELD
{
	char* id;
	char* value;
}IWFIELD;

typedef struct IWWARNING
{
	char* code;
	int argc;
	char** argv;
}IWWARNING;

typedef struct IWINTERN
{
	int serviceid;
	char* Kiw;
	int needsync;
	int pinmode;
	char* code;
	char* action;
	char* challenge;
	int defi;
	char* format;
	char* pushAction;
	char *pushAlias;
	char *pushContext;
	int synchroJustDone;
	int tmpMinorPrompted;
	int warningNb;
	IWWARNING* warning;
	char* wsbuffer;
	char* databuffer;
	char* Kps;
// CA
	char* antiphishing;
	int nopopup;

	int mask;
	char* form;
	char* OTP;
	char* OTPField;

	int promptNb;
	IWPROMPT* prompt;
	int fieldNb;
	IWFIELD* field;
//
// MA
	int option;
	char* lastJ;
	char* lastK;
	char* lastH;
	int lastTref;
	int retry;
	char lastcode[32];

	int otpRetryT0;
	int otpRetryT1;
	int otpRetryService;
	int nounsync;
	int deltarhodt;
	char *Kma;
	char *nKma;
	char *serial;
	char *newpin;
//
}IWINTERN;

typedef struct IW
{
	int ca;
	char* addr;
	int timeout;
	char* hostversion;
	char* SN;
	char* DataAppli;
	char* macid;
	char* lang;
	char* DeviceOS;
	IWDATA data;
	IWINTERN intern;
	WEBSERVICECALL webservicecall;
	void* user;
	IWCALLBACK callback;
	void *callback_user;
	IWCALLBACK wscallback;

	int id;
	char code[128];
}IW;

typedef struct IWCBS
{
	int sid;
	IW *iw;
	IWCALLBACK callback;
	void *callback_user;
	char *pin;
	char *sealdata;
}IWCBS;

#define MSK_FORM 1
#define MSK_OTP 2
#define MSK_OTPFIELD 4
#define MSK_SEAL 8

#define IW_MODE_URL 1
#define IW_MODE_ERROR 2
#define IW_MODE_SYNC 4
#define IW_MODE_PIN 256
#define IW_MODE_MAOTP 512
#define IW_MODE_LOGIN 1024
#define IW_MODE_FAIL (1+2)

#define ACTION_ACTIVATE "activate"
#define ACTION_PASSWORD "password"
#define ACTION_SYNCHRO "synchro"
#define ACTION_ACTIVATETB "activateTB"
#define ACTION_EXPORT "export"
#define ACTION_ADDSERVICE "addService"
#define ACTION_RESET "reset"
#define ACTION_UPGRADE "upgrade"
#define ACTION_MESSAGES "messages"
#define ACTION_MS "ms"
#define ACTION_DISCONNECT "disconnect"
#define ACTION_STATUS "status"
#define ACTION_OTP "otp"
#define ACTION_SEAL "seal"
#define ACTION_NEWSN "newsn"
#define ACTION_GETSN "getsn"
#define ACTION_PUSHREGISTRATION "pushRegistration"
#define ACTION_PUSHACTIVATECA "pushActivateCa"
#define ACTION_PUSHOTP "pushOtp"
#define ACTION_CHECKPUSH "checkPush"
#define ACTION_SETBIOKEY "setBiokey"
#define ACTION_UNSETBIOKEYS "unsetBiokeys"

#define IW_ACTION_SYNC "synchronize"
#define IW_ACTION_SRVADD "serviceadd"
#define IW_ACTION_PWD "password"
#define NOK_VERSION "NOK:VERSION"
#define NOK_CODE "NOK:CODE"
#define NOK_SN "NOK:SN"
#define NOK_BLOCKED "NOK_BLOCKED"
#define OK_BLOCKED "OK:BLOCKED"
#define NOK_ACCESS "NOK:ACCESS"
#define NOK_SAT "NOK:SAT"
#define NOK_STATE "NOK:STATE"
#define NOK_NODEVICE "NOK:NODEVICE"
#define NOK_URL "NOK:URL"
#define NOK_NOCA "NOK:NOCA"
#define NOK_NOSRV "NOK:NOSRV"
#define NOK_FORBIDDEN "NOK:FORBIDDEN"
#define NOK_PINREUSED "NOK:PINREUSED"
#define NOK_PINREFUSED "NOK:PINREFUSED"
#define NOK_TIMEOUT "NOK:TIMEOUT"
#define NOK_SYNCHROFAILED "NOK:SYNCHROFAILED"
#define NOK_NOPUSH "NOK:NOPUSH"
#define NOK_MAXNBTOOLS "NOK:MAXNBTOOLS"
#define NOK_BIOKEY "NOK:BIOKEY"
#define NOK_ERR1 "NOK:ERR1"

#define WARN_SRVLIMITED "WRN:SRVLIMITED"

#define IW_ERR_OK 0
#define IW_ERR_NETWORK 1
#define IW_ERR_CODE 2
#define IW_ERR_SN 3
#define IW_ERR_ACCESS 4
#define IW_ERR_VERSION 5
#define IW_ERR_SAT 6
#define IW_ERR_BLOCKED 7
#define IW_ERR_STATE 8
#define IW_ERR_NODEVICE 9
#define IW_ERR_NOCA 10
#define IW_ERR_NOSRV 11
#define IW_ERR_PINREUSED 12
#define IW_ERR_SYNCHROFAILED 13
#define IW_ERR_FORBIDDEN 14
#define IW_ERR_PINREFUSED 15
#define IW_ERR_TIMEOUT 16
#define IW_ERR_URL 17
#define IW_ERR_MAXNBTOOLS 25
#define IW_ERR_BIOKEY 26
#define IW_ERR_DESYNCHRONIZED 27

#define IW_ERR_OTHER 999

#define IW_PINMODE_NONE 0
#define IW_PINMODE_CURRENT 1
#define IW_PINMODE_NEW 2
#define IW_PINMODE_BIO 8

// CA + MA + MAC
DLLIMPIW IW* IWInit(int ca,char* SN,char* DataAppli,WEBSERVICECALL webservicecall,void* user);
DLLIMPIW void IWRelease(IW* iw);
DLLIMPIW void IWSetWsBuffer(IW* iw,char* src);
DLLIMPIW int IWIsCa(IW* iw);
DLLIMPIW int IWIsMa(IW* iw);
DLLIMPIW int IWIsMac(IW* iw);
DLLIMPIW char* IWWEBSERVICE(IW* iw);
DLLIMPIW char* IWLIBVERSION(IW* iw);
DLLIMPIW void IWSkipTime(IW* iw,long dt);
DLLIMPIW void IWNoUnsync(IW* iw,int b);
DLLIMPIW void IWDeltaRhodt(IW* iw,int i);
DLLIMPIW char* IWgetId(IW* iw);
DLLIMPIW void IWDump(IW* iw,char* pin);
DLLIMPIW char* IWSN(IW* iw);

// CA + MA + MAC
DLLIMPIW void IWWsTimeoutSet(IW* iw,int t);
DLLIMPIW void IWWsServerSet(IW* iw,char* server);
DLLIMPIW void IWMaccessSet(IW* iw,char* macid);
DLLIMPIW void IWSetLang(IW* iw,char* lang);
DLLIMPIW void IWHostVersionSet(IW* iw,char* version);
DLLIMPIW char* IWVersionGet(IW* iw);
DLLIMPIW int IWWarningNb(IW* iw);
DLLIMPIW char* IWWarningCode(IW* iw,int i);
DLLIMPIW int IWWarningArgc(IW* iw,int i);
DLLIMPIW char* IWWarningArgv(IW* iw,int i,int j);
DLLIMPIW void IWWarningAck(IW* iw);

// CA + MA + MAC
DLLIMPIW int IWStorageDataChanged(IW* iw);
DLLIMPIW char* IWStorageDataGet(IW* iw);
DLLIMPIW void IWStorageDataRelease(IW* iw);
DLLIMPIW int IWStorageDataSet(IW* iw,char *src);
DLLIMPIW int IWIsActivated(IW* iw);
DLLIMPIW int IWIsBlocked(IW* iw);
DLLIMPIW int IWHasNoPin(IW* iw);
DLLIMPIW int IWServiceNb(IW* iw);
DLLIMPIW int IWServiceId(IW* iw,int i);
DLLIMPIW int IWServiceIndex(IW *iw);
DLLIMPIW char* IWServiceName(IW* iw,int i);
DLLIMPIW char* IWServiceLogo(IW* iw,int i);
DLLIMPIW char* IWServiceAccountName(IW* iw, int i);

DLLIMPIW int IWServiceDisabled(IW* iw,int i);

// MA
DLLIMPIW int IWServiceOnlineOtp(IW* iw,int i);

// CA + MAC
DLLIMPIW int IWServiceConnected(IW* iw,int i);

// CA
DLLIMPIW char* IWServiceUrl(IW* iw,int i);
DLLIMPIW int IWServiceCheckUrl(IW* iw,int i,char* url);

// CA + MA + MAC
DLLIMPIW int IWMessageNb(IW* iw);
DLLIMPIW char* IWMessageTitle(IW* iw,int i);
DLLIMPIW char* IWMessageContent(IW* iw,int i);
DLLIMPIW int IWMessageNew(IW* iw,int i);
DLLIMPIW void IWMessageAck(IW* iw,int i);
DLLIMPIW int IWMajorVersionRequired(IW* iw);
DLLIMPIW char* IWNewVersionAvailable(IW* iw);
DLLIMPIW char* IWNewVersionURL(IW* iw);
DLLIMPIW int IWShouldAskForMinorUpdate(IW* iw);
DLLIMPIW int IWSynchroJustDone(IW* iw);
DLLIMPIW int IWMustUpgrade(IW* iw);

// CA + MA + MAC
DLLIMPIW int IWCallback(IW *iw, int ret);

// CA + MA + MAC
DLLIMPIW int IWActivationStart(IW* iw,char* code);
DLLIMPIW int IWActivationFinalize(IW* iw,char* code,char* pin,char* name);
DLLIMPIW int IWActivationStartAsync(IW* iw,char* code, IWCALLBACK callback, void *user);
DLLIMPIW int IWActivationFinalizeAsync(IW* iw,char* code,char* pin,char* name, IWCALLBACK callback, void *user);
DLLIMPIW void IWDeactivate(IW* iw);

// CA + MA + MAC
DLLIMPIW int IWResetStart(IW* iw,char* code);
DLLIMPIW int IWResetFinalize(IW* iw,char* code,char* pin);
DLLIMPIW int IWResetStartAsync(IW* iw,char* code, IWCALLBACK callback, void *user);
DLLIMPIW int IWResetFinalizeAsync(IW* iw,char* code,char* pin, IWCALLBACK callback, void *user);

// CA + MA + MAC
DLLIMPIW int IWUpgradeStart(IW* iw);
DLLIMPIW int IWUpgradeFinalize(IW* iw,char* pin,char* oldserial);
DLLIMPIW int IWUpgradeStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWUpgradeFinalizeAsync(IW* iw,char* pin,char* oldserial, IWCALLBACK callback, void *user);

// CA + MA + MAC
DLLIMPIW int IWActivationcodeRequestStart(IW* iw);
DLLIMPIW int IWActivationcodeRequestFinalize(IW* iw,char* pin);
DLLIMPIW int IWActivationcodeRequestStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWActivationcodeRequestFinalizeAsync(IW* iw,char* pin, IWCALLBACK callback, void *user);

// CA + MA
DLLIMPIW int IWExportcodeRequestStart(IW* iw);
DLLIMPIW int IWExportcodeRequestFinalize(IW* iw,char* pin);
DLLIMPIW int IWExportcodeRequestStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWExportcodeRequestFinalizeAsync(IW* iw,char* pin, IWCALLBACK callback, void *user);

// CA + MA + MAC
DLLIMPIW int IWServiceAddStart(IW* iw, char *code);
DLLIMPIW int IWServiceAddFinalize(IW* iw,char* code,char* pin);
DLLIMPIW int IWServiceAddStartAsync(IW* iw, char *code, IWCALLBACK callback, void *user);
DLLIMPIW int IWServiceAddFinalizeAsync(IW* iw,char* code,char* pin, IWCALLBACK callback, void *user);

// CA + MAC
DLLIMPIW int IWPwdUpdateStart(IW* iw);
DLLIMPIW int IWPwdUpdateFinalize(IW* iw,char* newpin,char* pin);
DLLIMPIW int IWPwdUpdateStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWPwdUpdateFinalizeAsync(IW* iw,char* newpin,char* pin, IWCALLBACK callback, void *user);

// MA
DLLIMPIW int IWNewSNStart(IW* iw);
DLLIMPIW int IWNewSNFinalize(IW* iw,char* pin);
DLLIMPIW int IWNewSNStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWNewSNFinalizeAsync(IW* iw,char* pin, IWCALLBACK callback, void *user);

// MA
DLLIMPIW int IWGetSNStart(IW* iw);
DLLIMPIW int IWGetSNFinalize(IW* iw);
DLLIMPIW int IWGetSNStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWGetSNFinalizeAsync(IW* iw, IWCALLBACK callback, void *user);

// CA + MAC
DLLIMPIW int IWConnected(IW* iw);
DLLIMPIW int IWPinMode(IW* iw);
DLLIMPIW char* IWCode(IW* iw);

// CA + MAC
DLLIMPIW int IWDisconnectStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWDisconnectFinalizeAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWDisconnectAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWDisconnect(IW* iw);

// CA + MAC
DLLIMPIW int IWCheckStatusStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWCheckStatusFinalizeAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWCheckStatusAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWCheckStatus(IW* iw);

// CA + MA + MAC Synchro
DLLIMPIW int IWMessageShouldSynchronize(IW* iw);
DLLIMPIW int IWMessagePostponeSynchro(IW* iw);
DLLIMPIW int IWSynchronizeStart(IW* iw);
DLLIMPIW int IWSynchronizeFinalize(IW* iw,char* pin);
DLLIMPIW int IWSynchronizeStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWSynchronizeFinalizeAsync(IW* iw,char* pin, IWCALLBACK callback, void *user);
DLLIMPIW int IWMessageSynchronizeStart(IW* iw);
DLLIMPIW int IWMessageSynchronizeFinalize(IW* iw);
DLLIMPIW int IWMessageSynchronizeStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWMessageSynchronizeFinalizeAsync(IW* iw, IWCALLBACK callback, void *user);

// MAC Online otp
DLLIMPIW int IWOnlineOtpStart(IW* iw,int i);
DLLIMPIW int IWOnlineOtpFinalize(IW* iw,int i,char* pin);
DLLIMPIW int IWOnlineOtpFinalizeExt(IW* iw, int i, char* pin, int keytype);
DLLIMPIW int IWOnlineOtpStartAsync(IW* iw,int i, IWCALLBACK callback, void *user);
DLLIMPIW int IWOnlineOtpFinalizeAsync(IW* iw,int i,char* pin, IWCALLBACK callback, void *user);
DLLIMPIW int IWOnlineOtpFinalizeExtAsync(IW* iw, int i, char* pin, int keytype, IWCALLBACK callback, void *user);

// MA + MAC Offline otp
DLLIMPIW int IWOtpShouldSynchronize(IW* iw, int serviceid);
DLLIMPIW int IWOtpModeQuery(IW* iw, int serviceid);
DLLIMPIW char* IWOtpGenerateMa(IW* iw, char* pin);
DLLIMPIW int IWDisplayTime(IW* iw);
DLLIMPIW int IWOtpResult(IW* iw, int result);

// MAC seal
DLLIMPIW int IWOnlineSealStart(IW* iw, int i);
DLLIMPIW int IWOnlineSealFinalize(IW* iw, int i, char* pin, char *sealdata);
DLLIMPIW int IWOnlineSealFinalizeExt(IW* iw, int i, char* pin, int keytype, char *sealdata);
DLLIMPIW int IWOnlineSealStartAsync(IW* iw, int i, IWCALLBACK callback, void *user);
DLLIMPIW int IWOnlineSealFinalizeAsync(IW* iw, int i, char* pin, char *sealdata, IWCALLBACK callback, void *user);
DLLIMPIW int IWOnlineSealFinalizeExtAsync(IW* iw, int i, char* pin, int keytype, char *sealdata, IWCALLBACK callback, void *user);

//CA
DLLIMPIW int IWOtpStart(IW* iw,char* url);
DLLIMPIW int IWOtpFinalize(IW* iw,char* url,char* pin);
DLLIMPIW int IWOtpStartAsync(IW* iw,char* url, IWCALLBACK callback, void *user);
DLLIMPIW int IWOtpFinalizeAsync(IW* iw,char* url,char* pin, IWCALLBACK callback, void *user);

// CA
DLLIMPIW char* IWAntiphishing(IW* iw);
DLLIMPIW int IWExtraPromptNb(IW* iw);
DLLIMPIW char* IWExtraPromptId(IW* iw,int i);
DLLIMPIW char* IWExtraPromptLabel(IW* iw,int i);
DLLIMPIW int IWExtraPromptHidden(IW* iw,int i);
DLLIMPIW char* IWExtraPromptGetVal(IW* iw,int i);
DLLIMPIW int IWExtraPromptSetVal(IW* iw,int i,char* val);
// CA
DLLIMPIW int IWNoPopup(IW* iw);
DLLIMPIW int IWExtraFieldNb(IW* iw);
DLLIMPIW char* IWExtraFieldId(IW* iw, int i);
DLLIMPIW char* IWExtraFieldValue(IW* iw, int i);

//DLLIMPIW int IWOtpGenerate(IW* iw,char* url,char* ip,char* info);
// CA + MAC
DLLIMPIW int IWOtpAnswersGet(IW* iw);
DLLIMPIW char* IWOtpAnswerOtp(IW* iw);
// CA
DLLIMPIW char* IWOtpAnswerForm(IW* iw);
DLLIMPIW char* IWOtpAnswerOtpField(IW* iw);


// MA + MAC Push
DLLIMPIW int IWPushRegistrationStart(IW* iw);
DLLIMPIW int IWPushRegistrationFinalize(IW* iw, char *push_id);
DLLIMPIW int IWPushRegistrationStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushRegistrationFinalizeAsync(IW* iw, char *push_id, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushActivateCaStart(IW* iw, char *alias);
DLLIMPIW int IWPushActivateCaFinalize(IW* iw,char *alias, char *pin, int ok);
DLLIMPIW int IWPushActivateCaStartAsync(IW* iw, char *alias, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushActivateCaFinalizeAsync(IW* iw,char *alias, char *pin, int ok, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushOtpStart(IW* iw, char *alias);
DLLIMPIW int IWPushOtpFinalize(IW* iw,char *alias, char *pin, int ok);
DLLIMPIW int IWPushOtpFinalizeExt(IW* iw, char *alias, char *pin, int ok, int keytype);
DLLIMPIW int IWPushOtpStartAsync(IW* iw, char *alias, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushOtpFinalizeAsync(IW* iw,char *alias, char *pin, int ok, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushOtpFinalizeExtAsync(IW* iw, char *alias, char *pin, int ok, int keytype, IWCALLBACK callback, void *user);
DLLIMPIW int IWPushShouldSynchronize(IW* iw);
DLLIMPIW char* IWPushAction(IW* iw);
DLLIMPIW char* IWPushAlias(IW* iw);
DLLIMPIW char* IWPushContext(IW* iw);
DLLIMPIW int IWCheckPushAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWCheckPush(IW* iw);
DLLIMPIW void IWSetDeviceOS(IW* iw, char *deviceos);

// MAC Seal
DLLIMPIW int IWSealShouldSynchronize(IW* iw,int serviceid);
DLLIMPIW int IWSealModeQuery(IW* iw,int serviceid);
DLLIMPIW char* IWSealGenerate(IW* iw,char* pin, char *sealdata);
DLLIMPIW int IWSealAnswersGet(IW* iw);
DLLIMPIW char* IWSealAnswerOtp(IW* iw);

// Mac
DLLIMPIW int IWSetBiokeyStart(IW* iw);
DLLIMPIW int IWSetBiokeyFinalize(IW* iw, char* biokey, char* pin);
DLLIMPIW int IWSetBiokeyStartAsync(IW* iw, IWCALLBACK callback, void *user);
DLLIMPIW int IWSetBiokeyFinalizeAsync(IW* iw, char* biokey, char* pin, IWCALLBACK callback, void *user);
DLLIMPIW int IWUnsetBiokeysStart(IW* iw);
DLLIMPIW int IWUnsetBiokeysFinalize(IW* iw, char* pin);
DLLIMPIW int IWUnsetBiokeysStartAsync(IW* iw, IWCALLBACK callback, void* user);
DLLIMPIW int IWUnsetBiokeysFinalizeAsync(IW* iw, char* pin, IWCALLBACK callback, void* user);

//
char* stringCopy(char* src);
char* stringCopyCut(char* src,int len);
void stringRelease(char* src);
char* stringSwap(char* srcold,char* srcnew);
char* stringCat(char* src,int releasesrc,char* append,int releaseappend);
char* stringCatMem(char* src,int releasesrc,char* append,int len);
char* stringInt(int v);
char* stringReplace(char* src,int releasesrc,char* patt,char* rep,int releaserep);
char* stringToHex(char* src,int len);
char* stringFromHex(char* src,int *len);
int sha256int(char* src);
long long sha256int38(char* src);
long long sha256int64(char* src);
char* sha256(char* src);
char* sha256_128(char* src);
char* rand128();
char* generateKma(IW* iw,char* pin);
char* encodeAesFromAsc(IW* iw,char* val,char* key);
char* decodeAesToAsc(IW* iw,char* val,char* key);
char* encodeAesFromHex(char* val,char* key);
char* decodeAesFromHex128(char* val,char* key);

long long IWOtpGenerateWithFormat(IW* iw,char* pin, char *format, char *kma);

char* KpubDecode(char* val,char* KpubFactory);
char* KpubEncode(char* val,char* k);
char* KpubEncodePin(char* val,char* k);

void tobase36(char* dst,long long x,int n);
void tobase10(char* dst,long long x,int n);
char* nexttoken(char* sep,char** p);
void stackretrieve(int* tb,char* src,int n,int def);
void stackclear(int* tb,int n,int def);
void stackpush(int* tb,int n,int val);
char* stacktostr(int* tb,int n);
int stackcontains(int* tb,int n,int val);
void stackclearfirst(int* tb);
int stacklast(int* tb,int n);
char* filterPin(IW* iw,char* pin);
int checkPin(IW* iw,char* pin);

xmltag* wscall(IW* iw,char* url,char* action,char* args);
int wscallAsync(IW* iw,char* url,char* action,char* args, IWCALLBACK callback);
xmltag* wsbuffer2xml(IW *iw);

char* wsfilter(char* x);
char* wsfilterrelease(char* x);
char* tagvalue(xmltag* x);
char* mainvalue(xmltag* root);
xmltag* firstsubtag(xmltag* x,char* name);
char* firstsubtagvalue(xmltag* x,char* name);
xmltag* nthsubtag(xmltag* x,char* name,int n);
char* nthsubtagvalue(xmltag* x,char* name,int n);
int countsubtag(xmltag* x,char* name);
int errorfromserver(IW* iw,xmltag* root,char* err);
int iw_time70();

void dataReleaseSrv(IW* iw);
void dataReleaseSec(IW* iw);
void dataReleaseMsg(IW* iw);
void dataRelease(IW* iw);
void dataInit(IW* iw);
char* filterLoad(char* src);
int load0xx(IW* iw,int vn,char* src);
int load1xx(IW* iw,int vn,char* src);
void dataTouch(IW* iw);
void dataAddSecret(IW* iw,int id,char* key);
char* dataGetSecret(IW* iw,int id);
int dataGetServiceIndex(IW* iw, int id);
void dataAddLocal(IW* iw,int serviceid,char* id,char* val);
char* dataGetLocal(IW* iw,int serviceid,char* id);
void dataConvertLocal(IW* iw,char* Kpsprev,char* Kps);
int dataGetUrlIndex(IW* iw, char* url);

void dataDump(IW* iw,char* pin);

void checknewversion(IW* iw,xmltag* root);

void internRelease(IW* iw);
void internInit(IW* iw);
void internReleaseWarning(IW* iw);
void internReleaseField(IW* iw);
void internReleasePrompt(IW* iw);

int checkForbidden(IW* iw,int activated,int blocked,int upgrade);
int simpleRequestAsync(IW* iw, char* action, IWCALLBACK callback, void *user);

int ActionSetupStart(IW* iw,char* action,char* args);
int ActionSetupFinalize(IW* iw,char* pin,char* args);
int ActionSetupFinalizeEx(IW* iw,char* pin,char* args,char* serial,char* newpin,char* nKma);

int ActionSetupStartAsync(IW* iw,char* action,char* args, IWCALLBACK callback, void *user);
int ActionSetupFinalizeAsync(IW* iw,char* pin,char* args, IWCALLBACK callback, void *user);
int ActionSetupFinalizeExAsync(IW* iw,char* pin,char* args,char* serial,char* newpin,char* nKma, IWCALLBACK callback, void *user);

char* getSerial(IW* iw);
void MakeSNFromSerial(IW* iw, char *serial);
#ifdef WINPRT 
}
#endif

