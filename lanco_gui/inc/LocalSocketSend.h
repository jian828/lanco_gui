

#ifndef _LOCAL_SOCKET_SEND_H_
#define _LOCAL_SOCKET_SEND_H_

#include "Message.h"

#include <sysutils/FrameworkListener.h>

class CLocalSocketSend : public CManipulateMessage<MESSAGE_QUEUE_SIZE_BIG>
{
public:
    virtual ~CLocalSocketSend();

    void ExeNetSend(void);
    TBOOL Post(CMessage *mMsg);
	TBOOL IsConnectToServer();
	void SetConnect(SocketClient* pSC);

	static CLocalSocketSend* GetInstance(void);

protected:
    CLocalSocketSend();

private:
    SocketClient *mpSocketClient;
    static CLocalSocketSend mSelf;
};
#endif

