#include "inc/SystemUIManager.h"

#define LOG_TAG "LANCO_GUI_SYSUI"
#include "cutils/log.h"

#include "inc/app_def.h"
#include "inc/multistrings.h"
#include "inc/Message.h"

CSystemUIManager CSystemUIManager::mSelf;

CSystemUIManager::CSystemUIManager()
{
	sem_init(&mUIMsgSem, 0, 0);
	
	memset(&mSysUICfg, 0, sizeof(TSystemUICfg));
	memset(&mDisplayInfo, 0, sizeof(TDisplayInfo));
	
	mSysUICfg.language = 1;
}


CSystemUIManager::~CSystemUIManager()
{

}

CMessage * CSystemUIManager::app_get_message(void)
{
	CMessage* pMsg = NULL;
	
	sem_wait(&mUIMsgSem);
    {
    	if (mUIMsgQueue.DeQueue(pMsg)) 
		{
            return pMsg;
    	}
    }

	return 0;
}

TBOOL CSystemUIManager::Post(CMessage *mMsg)
{
	TBOOL ret = mUIMsgQueue.EnQueue(mMsg);
	if (!ret) {
		CMessage::Free(mMsg);
		return (bFALSE);
	}

	sem_post(&mUIMsgSem);
	return (bTRUE);
}


CSystemUIManager* CSystemUIManager::GetInstance(void)
{
    return (&mSelf);
}


