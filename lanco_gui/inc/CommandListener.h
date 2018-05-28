#ifndef _COMMANDLISTENER_H__
#define _COMMANDLISTENER_H__

#include <sysutils/FrameworkListener.h>


#include "VogtecdCommand.h"



class CommandListener: public FrameworkListener {
	
public:
	CommandListener();
	virtual ~CommandListener() {}

protected:
	bool onDataAvailable(SocketClient *c);
	
private:

	void dispatchCommandLanco(SocketClient *c, char *data);
	
	class OtaCmd : public VogtecdCommand {
	public:
		OtaCmd();
		virtual ~OtaCmd() {}
		int runCommand(SocketClient *c, int argc, char ** argv);
	};
	
	class TcpdumpCmd: public VogtecdCommand {
	public:
		TcpdumpCmd();
		virtual ~TcpdumpCmd(){}
		int runCommand(SocketClient *c, int argc, char ** argv);
	};
};

#endif