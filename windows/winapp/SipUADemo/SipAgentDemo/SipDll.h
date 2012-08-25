#pragma	 once

#define CONF_ITEM_LEN     64      //For user configure.

typedef enum eXosip_event_type
{
	/* REGISTER related events */
	EXOSIP_REGISTRATION_NEW,         /**< announce new registration.       */
	EXOSIP_REGISTRATION_SUCCESS,     /**< user is successfully registred.  */
	EXOSIP_REGISTRATION_FAILURE,     /**< user is not registred.           */
	EXOSIP_REGISTRATION_REFRESHED,   /**< registration has been refreshed. */
	EXOSIP_REGISTRATION_TERMINATED,  /**< UA is not registred any more.    */

	/* INVITE related events within calls */
	EXOSIP_CALL_INVITE,          /**< announce a new call                   */
	EXOSIP_CALL_REINVITE,        /**< announce a new INVITE within call     */

	EXOSIP_CALL_NOANSWER,        /**< announce no answer within the timeout */
	EXOSIP_CALL_PROCEEDING,      /**< announce processing by a remote app   */
	EXOSIP_CALL_RINGING,         /**< announce ringback                     */
	EXOSIP_CALL_ANSWERED,        /**< announce start of call                */
	EXOSIP_CALL_REDIRECTED,      /**< announce a redirection                */
	EXOSIP_CALL_REQUESTFAILURE,  /**< announce a request failure            */
	EXOSIP_CALL_SERVERFAILURE,   /**< announce a server failure             */
	EXOSIP_CALL_GLOBALFAILURE,   /**< announce a global failure             */
	EXOSIP_CALL_ACK,             /**< ACK received for 200ok to INVITE      */

	EXOSIP_CALL_CANCELLED,       /**< announce that call has been cancelled */
	EXOSIP_CALL_TIMEOUT,         /**< announce that call has failed         */

	/* request related events within calls (except INVITE) */
	EXOSIP_CALL_MESSAGE_NEW,            /**< announce new incoming MESSAGE. */
	EXOSIP_CALL_MESSAGE_PROCEEDING,     /**< announce a 1xx for MESSAGE. */
	EXOSIP_CALL_MESSAGE_ANSWERED,       /**< announce a 200ok  */
	EXOSIP_CALL_MESSAGE_REDIRECTED,     /**< announce a failure. */
	EXOSIP_CALL_MESSAGE_REQUESTFAILURE, /**< announce a failure. */
	EXOSIP_CALL_MESSAGE_SERVERFAILURE,  /**< announce a failure. */
	EXOSIP_CALL_MESSAGE_GLOBALFAILURE,  /**< announce a failure. */

	EXOSIP_CALL_CLOSED,          /**< a BYE was received for this call      */

	/* for both UAS & UAC events */
	EXOSIP_CALL_RELEASED,           /**< call context is cleared.            */

	/* response received for request outside calls */
	EXOSIP_MESSAGE_NEW,            /**< announce new incoming MESSAGE. */
	EXOSIP_MESSAGE_PROCEEDING,     /**< announce a 1xx for MESSAGE. */
	EXOSIP_MESSAGE_ANSWERED,       /**< announce a 200ok  */
	EXOSIP_MESSAGE_REDIRECTED,     /**< announce a failure. */
	EXOSIP_MESSAGE_REQUESTFAILURE, /**< announce a failure. */
	EXOSIP_MESSAGE_SERVERFAILURE,  /**< announce a failure. */
	EXOSIP_MESSAGE_GLOBALFAILURE,  /**< announce a failure. */

	/* Presence and Instant Messaging */
	EXOSIP_SUBSCRIPTION_UPDATE,       /**< announce incoming SUBSCRIBE.      */
	EXOSIP_SUBSCRIPTION_CLOSED,       /**< announce end of subscription.     */

	EXOSIP_SUBSCRIPTION_NOANSWER,        /**< announce no answer              */
	EXOSIP_SUBSCRIPTION_PROCEEDING,      /**< announce a 1xx                  */
	EXOSIP_SUBSCRIPTION_ANSWERED,        /**< announce a 200ok                */
	EXOSIP_SUBSCRIPTION_REDIRECTED,      /**< announce a redirection          */
	EXOSIP_SUBSCRIPTION_REQUESTFAILURE,  /**< announce a request failure      */
	EXOSIP_SUBSCRIPTION_SERVERFAILURE,   /**< announce a server failure       */
	EXOSIP_SUBSCRIPTION_GLOBALFAILURE,   /**< announce a global failure       */
	EXOSIP_SUBSCRIPTION_NOTIFY,          /**< announce new NOTIFY request     */

	EXOSIP_SUBSCRIPTION_RELEASED,        /**< call context is cleared.        */

	EXOSIP_IN_SUBSCRIPTION_NEW,          /**< announce new incoming SUBSCRIBE.*/
	EXOSIP_IN_SUBSCRIPTION_RELEASED,     /**< announce end of subscription.   */

	EXOSIP_EVENT_COUNT                /**< MAX number of events              */
} eXosip_event_type_t;
typedef struct _user_sip_configure
{
	char szPhoneNumber[CONF_ITEM_LEN];
	char szUserName[CONF_ITEM_LEN];
	char szPassword[CONF_ITEM_LEN];
	char szDomain[CONF_ITEM_LEN];
	char szProxy[CONF_ITEM_LEN];
	char szStun[CONF_ITEM_LEN];
	int iSipPort;
	int iRTPProt;
	bool bUseStun;
	bool bAutoRegister;
}SipUserConfig;

typedef struct _codec_info
{
	int iPayLoadType;
	int iSampleRate;
	char szName[32];
}CodecInfo;

typedef struct _support_codec
{
	int iSupportCodecNumbers;
	CodecInfo CodecInfos[16];//
}SipSupportCodec;

typedef struct _user_configure
{
	SipSupportCodec SupportCodec;
	SipUserConfig SipConfig;
}UserConfig;


#define DTMF_RECEIVE    1100

#define DLL_IMPORT extern "C" __declspec(dllimport)

DLL_IMPORT void SipRegister(UserConfig* conf,
	void (CALLBACK* SipEvent)(int iLineNumber, int  iEventType, WPARAM wParam, LPARAM lParam, void* pInstance), void* pAppInstance);
DLL_IMPORT  int SipDialCall(char* szDialNumber, int iAuidoIn, int iAudioOut);
DLL_IMPORT  BOOL SipHangup(int iChoseLine);
DLL_IMPORT  BOOL SipAnswer(int iChoseLine,int iAudioIn,int  iAudioOut);
DLL_IMPORT  BOOL SipTransfer(int iChoseLine,char* szTransferNumber);
DLL_IMPORT  BOOL SipOnHold(int iLineNum, BOOL bOnHold);
DLL_IMPORT  int SipSendInstantMessage(char* szTo, char* szMessage, int iLength);
DLL_IMPORT  int FormateXosipEventMessage(int iEventType, char* szMessage, int iMessageLength);
DLL_IMPORT  void SipSetVoiceMailMode(int iLineNumber, BOOL bVM);
DLL_IMPORT  void SipExit();
DLL_IMPORT  void SipSetVolume(int iLineNumber, int iVolumeMic, int iVolumeSpeaker);
DLL_IMPORT int SipSendDTMF(int iLineNumber, char chDTMF, int iDTMFMode);
DLL_IMPORT void SetDTMFAudioFileName(char chDTMF, char* szFileName);
DLL_IMPORT void SipUsingShit(int );
DLL_IMPORT int AudioGetPlaybackDeviceNb();
DLL_IMPORT const char* AudioGetPlaybackDeviceNameByID(int iID);
DLL_IMPORT int AudioGetCaptureDeviceNb();
DLL_IMPORT const char* AudioGetCaptureDeviceNameByID(int iID);
