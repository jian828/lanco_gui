#ifndef __SYSTEM_UI_MANAGER__
#define __SYSTEM_UI_MANAGER__

#include "inc/Message.h"

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>


typedef struct tagSystemUICfg {
	unsigned char language;
	unsigned char font;
	unsigned char color;
}TSystemUICfg;

typedef struct tagDisplayInfo {
	unsigned char csq;
}TDisplayInfo;

typedef enum {
	rmtNotification,
	rmtSync,
}TRecvMsgType;

class CSystemUIManager : public CManipulateMessage<MESSAGE_QUEUE_SIZE_BIG>
{
public:
    virtual ~CSystemUIManager();
	
	void initSystemUI(void);

	CMessage * app_get_message(void);

	TBOOL Post(CMessage *mMsg);
	
    static CSystemUIManager* GetInstance(void);

protected:
    CSystemUIManager();

private:

	sem_t mUIMsgSem;
    CMsgQueue mUIMsgQueue;

	TSystemUICfg mSysUICfg;
	TDisplayInfo mDisplayInfo;
	
    static CSystemUIManager mSelf;

};

#endif