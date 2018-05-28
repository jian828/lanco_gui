#include "inc/LocalSocketSend.h"

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define LOG_TAG "LocalSocketSend"
#include "cutils/log.h"


static sem_t mSendSem = {0};

void CLocalSocketSend::ExeNetSend(void) {
    CMessage* pMsg = NULL;

//    while(1)
    sem_wait(&mSendSem);
    {
    	if (mMsgQueue.DeQueue(pMsg)) {
			//ALOGD("%s.", (char*)pMsg->GetMsgBuf());
			if (mpSocketClient != NULL) {
				mpSocketClient->sendData(pMsg->GetMsgBuf(), pMsg->GetMsgLen());
			} else {
				ALOGD("mpSocketClient not exit.");
			}
    		CMessage::Free(pMsg);
    	}
    }
}

CLocalSocketSend::~CLocalSocketSend() {

}

CLocalSocketSend::CLocalSocketSend()
{
    mpSocketClient = NULL;
    sem_init(&mSendSem, 0, 0);
}


TBOOL CLocalSocketSend::Post(CMessage *mMsg)
{
	TBOOL ret = mMsgQueue.EnQueue(mMsg);
	if (!ret) {
		CMessage::Free(mMsg);
		return (bFALSE);
	}

	sem_post(&mSendSem);
	return (bTRUE);
}

TBOOL CLocalSocketSend::IsConnectToServer()
{
	if (mpSocketClient != NULL)
	{
		return (bTRUE);
	}
	else
	{
		return (bFALSE);
	}
}


void CLocalSocketSend::SetConnect(SocketClient* pSC)
{
	if (pSC != NULL) {
		mpSocketClient = pSC;
	}
}

CLocalSocketSend CLocalSocketSend::mSelf;


CLocalSocketSend* CLocalSocketSend::GetInstance(void)
{
    return (&mSelf);
}
