#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define LOG_TAG "LANCO_GUI"

#include <cutils/log.h>
#include <netutils/ifc.h>
#include <sysutils/SocketClient.h>

#include "inc/CommandListener.h"
#include "inc/ResponseCode.h"
#include "inc/LocalSocketSend.h"
#include "inc/SystemUIManager.h"
#include "inc/app_def.h"

CommandListener::CommandListener() : FrameworkListener("lanco_gui", false)
{
	registerCmd(new OtaCmd());
	registerCmd(new TcpdumpCmd());
}

static const int CMD_BUF_SIZE = 1024;
bool CommandListener::onDataAvailable(SocketClient *c) {
    char buffer[CMD_BUF_SIZE];
    int len;

    len = TEMP_FAILURE_RETRY(read(c->getSocket(), buffer, sizeof(buffer)));
    if (len < 0) {
        SLOGE("read() failed (%s)", strerror(errno));
        return false;
    } else if (!len)
        return false;
   if(buffer[len-1] != '\0')
        SLOGW("String is not zero-terminated");

    int offset = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (buffer[i] == '\0') {
            /* IMPORTANT: dispatchCommand() expects a zero-terminated string */
            dispatchCommandLanco(c, buffer + offset);
            offset = i + 1;
        }
    }

    return true;
}

#define LANCO_GUI_CMD_HEAD	"ls938h"
void CommandListener::dispatchCommandLanco(SocketClient *cli, char *data) {
    ALOGD("===syw=== CMD:%s", data);
	CLocalSocketSend* pLSSend = CLocalSocketSend::GetInstance();
	//if (!pLSSend->IsConnectToServer()) {
		pLSSend->SetConnect(cli);
	//}

	if (memcmp(data, LANCO_GUI_CMD_HEAD, strlen(LANCO_GUI_CMD_HEAD)) == 0) {

		//Post SystemUI
		int len = strlen(data) - (strlen(LANCO_GUI_CMD_HEAD) + 1);
		CMessage* msg = CMessage::Allocate(len);
		if (msg == NULL) {
			printf("KEY Out of Memory");
		}
		msg->SetMsgCode(EVENT_ANDROID_TO_UI);
		msg->SetMsgLen(len);
		memcpy(msg->GetMsgBuf(), (unsigned char*)(data + (strlen(LANCO_GUI_CMD_HEAD) + 1)), len);

		CSystemUIManager* pUI = CSystemUIManager::GetInstance();
		if (!pUI->Post(msg)) {
			CMessage::Free(msg);
			ALOGE("EVENT_ANDROID_TO_UI Post msg failed.\n");
		}
	} else {
		ALOGE("unknow cmd: %s", data);
	}
}

CommandListener::OtaCmd::OtaCmd() : VogtecdCommand("ota")
{

}
					
CommandListener::TcpdumpCmd::TcpdumpCmd() : VogtecdCommand("ls938h")
{

}

int CommandListener::OtaCmd::runCommand(SocketClient *cli, int argc, char ** argv)
{
	if (argc < 2) {
        cli->sendMsg(ResponseCode::CommandSyntaxError, "Missing argument", false);
        return 0;
    }
	ALOGE("===syw=== ota runcommand");
	for (int i = 0; i < argc; i++) {
		ALOGE("%s", argv[i]);
	}

	if (!strcmp(argv[0], "ota")) {
		if (!strcmp(argv[1], "update")) {
			system("echo \"--update_package=/sdcard/update.zip\" > /cache/recovery/command");
			system("reboot recovery");
		}
	}else {
		ALOGE("unknow cmd: %s", argv[1]);
	}

	return 0;
}

int CommandListener::TcpdumpCmd::runCommand(SocketClient *cli, int argc, char ** argv)
{
	if (argc < 2) {
        cli->sendMsg(ResponseCode::CommandSyntaxError, "Missing argument", false);
        return 0;
    }
	
	CLocalSocketSend* pLSSend = CLocalSocketSend::GetInstance();
	//if (!pLSSend->IsConnectToServer()) {
		pLSSend->SetConnect(cli);
	//}
	
	ALOGE("===syw=== tcpdump runcommand");
	for (int i = 0; i < argc; i++) {
		ALOGE("%s", argv[i]);
	}
	
	if (!strcmp(argv[0], "ls938h")) {
		
		//Post SystemUI
		int len = strlen(argv[1]);
		CMessage* msg = CMessage::Allocate(len);
		if (msg == NULL) {
			printf("KEY Out of Memory");
		}
		msg->SetMsgCode(EVENT_ANDROID_TO_UI);
		msg->SetMsgLen(len);
		memcpy(msg->GetMsgBuf(), (unsigned char*)argv[1], len);
		
		CSystemUIManager* pUI = CSystemUIManager::GetInstance();
		if (!pUI->Post(msg)) {
			CMessage::Free(msg);
			printf("EVENT_ANDROID_TO_UI Post msg failed.\n");
		}
	} else {
		ALOGE("unknow cmd: %s", argv[0]);
	}
	return 0;
}
