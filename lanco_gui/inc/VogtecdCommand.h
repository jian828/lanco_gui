#ifndef _VOGTECD_COMMAND_H
#define _VOGTECD_COMMAND_H

#include <sysutils/FrameworkCommand.h>

class VogtecdCommand : public FrameworkCommand {
public:
    VogtecdCommand(const char *cmd);
    virtual ~VogtecdCommand() {}
};

#endif