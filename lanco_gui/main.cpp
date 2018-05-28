





#include "inc/app_def.h"
#include "inc/KeyInput.h"
#include "inc/LocalSocketSend.h"

pthread_mutex_t mGloabalLock;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////









static pthread_t mSystemUIThreadId;
static pthread_t mKeyInputThreadId;
static pthread_t mLocalSocketSendThreadId;
static void* KeyInputThread(void * inParam);
static void* LocalSocketSendThread(void * inParam);
CSystemUIManager* pUI;




void sys_timer1_on_time(int interval)
{
	CMessage* msg = CMessage::Allocate(0);
	if (msg == NULL) {
		printf("TMR Out of Memory");
	}
	msg->SetMsgCode(EVENT_TIMER);
	
	if (!pUI->Post(msg)) {
		CMessage::Free(msg);
		printf("Timer Post msg failed.\n");
	}
}



void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = sys_timer1_on_time; //?????????
    act.sa_flags  = 0;

    sigemptyset(&act.sa_mask);
	//sigaction(ITIMER_REAL, &act, NULL);//?????SIGROF??
    sigaction(SIGALRM, &act, NULL);//?????SIGROF??
}

void init_sys_timer1(void)
{
    struct itimerval val;

    val.it_value.tv_sec = 0;
    val.it_value.tv_usec =100000;

    val.it_interval = val.it_value;
    setitimer(ITIMER_REAL, &val, NULL);	
}



//-----------------------------------------------------------------------------------------------------------




extern void * app_main(void * p);

int main() 
{
	CommandListener *cl = new CommandListener();
	
	ALOGI("Lansnia 4G OS version:1.0 starting");
	
	pthread_mutex_init(&mGloabalLock, NULL);

    pUI = CSystemUIManager::GetInstance();


    init_sigaction();	
	init_sys_timer1();
	
	
	//init lanco_gui system
	int ret=pthread_create(&mSystemUIThreadId, NULL, app_main, NULL);
	if(ret!=0){
		ALOGE("Create SystemUIThread failed!");
	}
	else {
		ALOGD("Create SystemUIThread ok!");
	}
	
	ret=pthread_create(&mKeyInputThreadId, NULL, KeyInputThread, NULL);
	if(ret!=0){
		ALOGE("Create KeyInputThread failed!");
	}
	else {
		ALOGD("Create KeyInputThread ok!");
	}

	ret=pthread_create(&mLocalSocketSendThreadId, NULL, LocalSocketSendThread, NULL);
	if(ret!=0){
		ALOGE("Create LocalSocketSendThread failed!");
	}
	else {
		ALOGD("Create LocalSocketSendThread ok!");
	}
	
	/*
     * Now that we're up, we can respond to commands
     */
    if (cl->startListener()) {
        ALOGE("Unable to start CommandListener (%s)", strerror(errno));
        exit(1);
    }

	ALOGI("startListener--LJ");

    // Eventually we'll become the monitoring thread
    while(1) {
        sleep(1000);
    }

    ALOGI("Lansnia exiting");
    exit(0);
}

typedef struct tKeyCustomMap {
	int key;
	unsigned char key_ui;
}TKeyCustomMap;

TKeyCustomMap mCustomKeyTable[] = {
	{524, TFKEY_STAR},
	{522, TFKEY_SHARP},
	{523, TFKEY_0},
	{517, TFKEY_SEND},
	{508, TFKEY_REDIAL},
	{502, TFKEY_HANDFREE},
};
void KeyEventHandle(struct input_event event)
{
	//printf("\n===syw=== keycode:%d keyvalue:%d", event.code, event.value);
	//ALOGI("===syw=== keycode:%d keyvalue:%d", event.code, event.value);

	//key-->UI custom
	if (event.code > 255) {
		for (int i = 0; i < sizeof(mCustomKeyTable)/sizeof(TKeyCustomMap); i++) {
			if (mCustomKeyTable[i].key == event.code) {
				event.code = mCustomKeyTable[i].key_ui;
				break;
			}
		}
	}

	//post key
	CMessage* msg = CMessage::Allocate(sizeof(input_event));
	if (msg == NULL) {
		printf("KEY Out of Memory");
	}
	msg->SetMsgCode(EVENT_KEY);
	msg->SetMsgLen(sizeof(input_event));
	memcpy(msg->GetMsgBuf(), (unsigned char*)&event, sizeof(input_event));
	
	if (!pUI->Post(msg)) {
		CMessage::Free(msg);
		printf("KeyEventHandle Post msg failed.\n");
	}
}

void* KeyInputThread(void * inParam)
{
	ALOGI("KeyInputThread starting...");
	
	get_key_input(KeyEventHandle);
	
	pthread_exit(0);
	
	return NULL;
}



void* LocalSocketSendThread(void * inParam)
{
	ALOGI("LocalSocketSendThread starting...");
	
	CLocalSocketSend* pLSSend = CLocalSocketSend::GetInstance();
	
	while(1) {
		pLSSend->ExeNetSend();
	}
	
	pthread_exit(0);
	
	return NULL;
}


  void send_android_command(char * str_cmd)
  {
		unsigned short  ptr_uni[1023];

        int data_len =0;
		
		memset((unsigned char *)ptr_uni,0,sizeof(ptr_uni));

        if( LANGUAGE_SPANISH== sysprop->byte_language_type  ||  LANGUAGE_PORTUGE== sysprop->byte_language_type)
        {
            a2w(ptr_uni, str_cmd);
			data_len = wstrlen(ptr_uni) *2;
		}
		else
        {
		    data_len=MultiByte_WideChar(ptr_uni, str_cmd) *2; 
        }

        if(data_len >0)   
        {
			CLocalSocketSend* pCMD = CLocalSocketSend::GetInstance(); 
		    CMessage* msg = CMessage::Allocate( data_len );
			if (msg == NULL) 
			{
				printf("CMD Out of Memory");
			}
			msg->SetMsgLen(data_len);
			memcpy(msg->GetMsgBuf(), (unsigned char * )ptr_uni, data_len);
			if(1 == appsys.flag_genie_trace)
			{
			     DebugPrintf("send_android_command gb2312: len=%d msg=%s\r\n", data_len, str_cmd);
				 print_hex_string("send_android_command unicode", (unsigned char * )ptr_uni,data_len);
			}
		    msg->SetMsgCode(EVENT_UI_TO_ANDROID);
			if (!pCMD->Post(msg)) 
			{
				CMessage::Free(msg);
				printf("CMD Post msg failed.\n");
			}
		}

  }





  
  void fake_android_message(char * str_cmd)
  {
		unsigned short  ptr_uni[1023];

        int data_len =0;
		
		memset((unsigned char *)ptr_uni,0,sizeof(ptr_uni));

        if( LANGUAGE_SPANISH== sysprop->byte_language_type  ||  LANGUAGE_PORTUGE== sysprop->byte_language_type)
        {
            a2w(ptr_uni, str_cmd);
			data_len = wstrlen(ptr_uni) *2;
		}
		else
        {
		    data_len=MultiByte_WideChar(ptr_uni, str_cmd) *2; 
        }

        if(data_len >0)   
        {
		    CMessage* msg = CMessage::Allocate( data_len );
			if (msg == NULL) 
			{
				printf("FAKE Out of Memory");
			}
			msg->SetMsgCode(EVENT_ANDROID_TO_UI);
			msg->SetMsgLen(data_len);
			memcpy(msg->GetMsgBuf(), (unsigned char * )ptr_uni,  data_len); // end with 00 00

		 	if(1 == appsys.flag_genie_trace)
			{
			     DebugPrintf("fake_android_message gb2312: len=%d msg=%s\r\n", data_len, str_cmd);
				 print_hex_string("fake_android_message unicode", (unsigned char * )ptr_uni,data_len);
			}
			if (!pUI->Post(msg)) {
				CMessage::Free(msg);
				printf("FAKE Post msg failed.\n");
			}
        }
  }


