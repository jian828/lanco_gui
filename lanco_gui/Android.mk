LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=                                      \
          CommandListener.cpp \
          KeyInput.c \
				  VogtecdCommand.cpp \
          main.cpp \
				  SystemUIManager.cpp \
				  Message.cpp \
				  app_flash.cpp\
				  app_global.cpp\
				  app_lcd.cpp\
				  app_main.cpp\
				  app_module.cpp\
				  app_phonebook.cpp\
				  app_xmodem.cpp\
				  base64.cpp\
				  constvar.cpp\
				  FindBX.cpp\
				  FindLX.cpp\
				  FindPY.cpp\
				  menudef.cpp\
				  multistrings.cpp\
				  time_alarm.cpp\
				  wnd_call.cpp\
				  wnd_edit.cpp\
				  wnd_menu.cpp\
				  wnd_record.cpp\
				  DataFile.c \
				  LocalSocketSend.cpp
				  
			  
				  


LOCAL_MODULE:= lanco_gui

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
                    external/mdnsresponder/mDNSShared \
                    external/openssl/include \
                    external/stlport/stlport \
                    bionic \
                    bionic/libc/private \
                    $(call include-path-for, libhardware_legacy)/hardware_legacy

#LOCAL_CFLAGS := -Werror=format
LOCAL_CFLAGS := -Wformat -fpermissive

LOCAL_SHARED_LIBRARIES := libstlport libsysutils liblog libcutils libnetutils \
                          libcrypto libhardware_legacy libmdnssd libdl \
                          liblogwrap

include $(BUILD_EXECUTABLE)
