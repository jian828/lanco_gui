#if !defined(APP_DEF_H)
#define APP_DEF_H


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h> 
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include "cutils/log.h"



#include "base64.h"
#include "multistrings.h"
#include "menudef.h"
#include "time.h"
#include "FindCode.h"
#include "PyCodeTbl.h"
#include "CommandListener.h"
#include "SystemUIManager.h"
#include "DataFile.h"




#ifndef NULL
    #define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#define LOG_TAG "LANCO_GUI"

#define MAX_INT32_FFFFFFFF   (0xFFFFFFFF)

#define MODEM_MAX_SIZE       (2400)
#define PKET_DATA_SIZE       (1024)


#define MAX_FAST_DIAL_NUMS     (3)

#define DEBUG_MAX_SIZE       (256)
#define STBUF_MAX_SIZE       (1200)


#define XMODEM_BLOCKSIZE        (1024)


#define HUGEBUF_SIZE         (1024 *10)


#define ANSI_GB_BUF_SIZE   (255)
#define CHS_POS_LEFT   0
#define CHS_POS_TOP    SCREEN_HEIGHT-12
#define CHS_POS_RIGHT  SCREEN_WIDTH-1

#define BX_POS_LEFT    2
#define BX_POS_TOP    SCREEN_HEIGHT-12-12
#define BX_POS_RIGHT  SCREEN_WIDTH-7

#define MAX_SIM_SMS_CNT  50

#define YEAR_MIN_VALUE   (1950)
#define YEAR_MAX_VALUE   (2050)


#define REG_STATE_REG_OK          (0)
#define REG_STATE_NOT_REG         (1)
#define REG_STATE_REG_EMERGENCY   (2)
#define REG_STATE_ATENA_CLOSED    (3)

#define NET_STATE_UNKNOWN   (0)
#define NET_STATE_GSM       (1)
#define NET_STATE_3G        (2)
#define NET_STATE_4G        (3)


#define ROAM_STATE_HOME_NETWORK  (0)
#define ROAM_STATE_ROAMING       (1)


#define VOCTYPE_INCOMING_CALL    (1)
#define VOCTYPE_SMS_INCOMING     (2)
#define VOCTYPE_TALKING          (3)

#define MAX_INCOME_RING_CNT       (10)
#define MAX_ALARM_RING_CNT        (5)
#define MAX_SMS_RING_CNT          (5)
#define MAX_VOLUM_LEVEL           (5)

#define SCREEN_WIDTH    (128)
#define SCREEN_HEIGHT   (64)


#define AUTO_ACTIVATE   0
#define MANUAL_ACTIVATE 1

#define TALK_STOP_BY_CALLER 0
#define TALK_STOP_BY_CALLEE 1
#define TALK_STOP_BY_LOWBAT 3

#define RING_VOL_LOWEST     3




#define BATTERY_FAST_GATE_FOR_5V (1300)

#define BATTERY_FULL_VOLTAGE     (4150)
#define BAT_SERVICE_DELAY        (6)

#define BAT_CHARGE_MAX_UI_TIME   (9999)


#define PASSWORD_TYPE_USER_PIN           (1)
#define PASSWORD_TYPE_MASTER_PIN         (3)

#define UNLOCK_TYPE_SIMPIN      0
#define UNLOCK_TYPE_CIMI        1


#define FAST_NUMBER_LIFE_SERVICE_POS  (3)
#define FAST_NUMBER_EMERGENCY_POS     (4)


#define TCP_COMM_BY_XMODEM   (1)
#define TCP_COMM_BY_ANY      (0)



#define	TFNET_ID_MAX		 (125+2 +10) //otid 0 is not ever used.
#define	TFNET_FILE_SIZE  	 (TFNET_ID_MAX*1024)



#define SCREEN_PAGES    ((SCREEN_HEIGHT+7)/8)



#define GPIO_DIR_OUTPUT  1
#define GPIO_DIR_INPUT   0



#define APP_GPIO_PIN59_DAT           59
#define APP_GPIO_PIN60_CLK           60

//背光(输出)
#define APP_GPIO_PIN66_LCD_BL        66
#define APP_GPIO_PIN73_LCD_RST       73






#define EVENT_KEY     4000 
#define EVENT_TIMER   4001
#define EVENT_HANDSET 4002
#define EVENT_MODULE  4003
#define EVENT_USART1  4004
#define EVENT_USART2  4005
#define EVENT_ANDROID_TO_UI	4006
#define EVENT_UI_TO_ANDROID	4007



#define DIALING_NO_INPUT_TIMEOUT_S  (5)
#define DIALING_RET_NO_SENSE        (0)
#define DIALING_RET_DIALOUT         (1)
#define DIALING_RET_SUPERSETTING    (2)
#define DIALING_RET_GET_IMEI        (3)
#define DIALING_RET_GET_MAINMENU    (4)
#define DIALING_RET_EN_DEBUG        (5)
#define DIALING_RET_SPECIAL_TEST    (6)
#define DIALING_RET_GET_VERISON     (7)
#define DIALING_RET_PREFIX_RW       (8)
#define DIALING_RET_PROJECT_TEST    (9)
#define DIALING_RET_LOCK_OPER       (10)
#define DIALING_RET_SIM_ENCYPT      (12)
#define DIALING_RET_LOCK_PHONE      (18)


#define AUTO_REDIAL_CNT         (3)
#define AUTO_REDIAL_INTERVAL    (30) // In seconds


#define WAIT_CSSI_7_TIME_OUT        (60)
#define MONEY_MINDER_TIME           (10)
#define WINDOW_TIME_OUT             (2)
#define VOICE_PATH_HANDFREE         (0)
#define VOICE_PATH_HANDSET          (1)
#define REDRAW_PATITIAL             (1) 


#define CLOCK_FORMAT_12         (0)
#define CLOCK_FORMAT_24         (1)

#define KEY_STATE_PRESSED       (0)
#define KEY_STATE_RELEASED      (1)

#define HOOK_STATE_ONHOOK       (0)
#define HOOK_STATE_OFFHOOK      (1)


#define BOOK_ENTRY_NAME_FIELD_SIZE  (16)
#define BOOK_ENTRY_NUM_FIELD_SIZE   (20)
#define NUMBER_PREFIX_LENGTH        (13)
#define SMS_CONTEXT_LENGTH          (160)

#define BLUE_WIFI_NAME_LEN          (40)
#define MAX_BLUE_WIFI_CNT           (32)
#define MAX_DIVT_CNT                (4)




#define ACTIVE_CALL     (0)
#define HOLDING_CALL    (1)
#define DIALING_CALL    (2)
#define RINGING_CALL    (3)
#define WAITING_CALL    (5)

#define EMERGENCY_NUM_LEN  13

#define UPDATE_CALL_NUM             (0x01)
#define UPDATE_CALL_TIME            (0x02)
#define UPDATE_CALL_CHARGE_INFO     (0x04)
#define UPDATE_CALL_KEY_PROMPT      (0x08)


#define NETWORK_TYPE_DMTD            0
#define NETWORK_TYPE_2G              1
#define NETWORK_TYPE_TD              2



#define MAX_MONEY_VALUE (2000000000)
#define MONEY_DIGI_NUM  (11)


#define DEFAULT_LCD_CONTRAST    (44 +2)
#define LCD_CONTRAST_STEP       (4)
#define MAX_LCD_CONTRAST        (DEFAULT_LCD_CONTRAST + 3*LCD_CONTRAST_STEP)
#define MIN_LCD_CONTRAST        (DEFAULT_LCD_CONTRAST - 3*LCD_CONTRAST_STEP)






#define DIVERT_ALWAYS       1 
#define DIVERT_BUSY         2 
#define DIVERT_NO_REPLY     3 
#define DIVERT_UNREACHABLE  4 

#define BARRING_ALL_OUT_CALL            (0x01)
#define BARRING_ALL_INTERNATIONAL_CALL  (0x02)
#define BARRING_ALL_IN_CALL             (0x04)
#define BARRING_ALL_IN_CALL_WITH_PASS   (0x08)

#define MAX_OPER_CNT            (10)

#define TCP_IP_ADDRESS_LENGTH   (15)
#define TCP_IP_PORT_LENGTH      (5)
#define TCP_APN_STR_LENGTH      (50)



///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////0000000000000000000000000000000000000000000000000
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////



#define DATABASEINFO_CNT     (10)
#define DBINFO_TABLE_START   (3)




#define TABLEINFO_SMS_INBOX    (0)
#define TABLEINFO_SMS_OUTBOX   (1)
#define TABLEINFO_PHONEBOOK    (2)
#define TABLEINFO_CALL_IN      (3)
#define TABLEINFO_MISSED_CALL  (4)
#define TABLEINFO_CALL_OUT     (5)
#define TABLEINFO_SMS_DRAFT    (6)


#define DBINFO_LOCAL_FILES        (0)
#define DBINFO_TEMP_FILES         (1)
#define DBINFO_INDEXS_TABLE       (2) 


#define DB_LOCAL_PROP_CELL_POS       (0)// 1 CELL  
#define DB_LOCAL_SADV_CELL_POS       (1)// 1 CELL  
#define DB_LOCAL_ALL_CIMI_POS        (3)// 1 CELL  



#define DB_TEMP_PROP_CELL_POS     (0)
#define DB_TEMP_PRI_CELL_POS      (1)
#define DB_TEMP_RATE_CELL_POS     (3)

#define REC_PREFIX_APPEND         (0)
#define REC_POSTFIX_APPEND        (1)





//11.1服务器下发的确认收到信息的动作标识（通讯服务器->终端）
//11.2服务器下发的命令的动作标识（通讯服务器->终端）
//11.9服务器下发给终端主机的同步动作标识（通讯服务器->终端）

#define MAX_TMS_POS      10


///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#define MAX_BMP_COUNT  (71) 
#define BMP_REGISTERING_BIN (0) 
#define BMP_ALARM_CLOCK_BIN (1) 
#define BMP_ARROW_LEFT_BIN (2) 
#define BMP_ARROW_RIGHT_BIN (3) 
#define BMP_ASK_BIN (4) 
#define BMP_BATTARY_BIN (5) 
#define BMP_CALLFAILED_BIN (6) 
#define BMP_CALLIN_BIN (7) 
#define BMP_CALLOUT_BIN (8) 
#define BMP_CHECKED_BIN (9) 
#define BMP_FORBIDDEN_BIN (10) 
#define BMP_MAIN_ALARM_BIN (11) 
#define BMP_MAIN_ANTENNA_UNKN_BIN (12) 
#define BMP_MAIN_ANTENNA_2G_BIN (13) 
#define BMP_MAIN_ANTENNA_3G_BIN (14) 
#define BMP_MAIN_ANTENNA_4G_BIN (15) 
#define BMP_MAIN_CALLIN_BIN (16) 
#define BMP_MAIN_FORWARD_BIN (17) 
#define BMP_MAIN_KEYLOCK_BIN (18) 
#define BMP_MAIN_MAILBOX_BIN (19) 
#define BMP_MAIN_MUTE_BIN (20) 
#define BMP_MAIN_DEFEND_BIN (21) 
#define BMP_MISSEDCALL_BIN (22) 
#define BMP_NOTICE_BIN (23) 
#define BMP_PASSWORD_BIN (24) 
#define BMP_PHONE_BIN (25) 
#define BMP_INTERNET_BIN (26) 
#define BMP_POWERDOWN_BIN (27) 
#define BMP_RETURN_BIN (28) 
#define BMP_SEG0_BIN (29) 
#define BMP_SEG1_BIN (30) 
#define BMP_SEG2_BIN (31) 
#define BMP_SEG3_BIN (32) 
#define BMP_SEG4_BIN (33) 
#define BMP_SEG5_BIN (34) 
#define BMP_SEG6_BIN (35) 
#define BMP_SEG7_BIN (36) 
#define BMP_SEG8_BIN (37) 
#define BMP_SEG9_BIN (38) 
#define BMP_SEGCOLON_BIN (39) 
#define BMP_SELECTED_BIN (40) 
#define BMP_SIMCARD_BIN (41) 
#define BMP_SMSREADED_BIN (42) 
#define BMP_SMSUNREADED_BIN (43) 
#define BMP_SUCCEED_BIN (44) 
#define BMP_TIME_A_BIN (45) 
#define BMP_TIME_P_BIN (46) 
#define BMP_TOOL1_BIN (47) 
#define BMP_TOOL2_BIN (48) 
#define BMP_TOOL3_BIN (49) 
#define BMP_TOOL4_BIN (50) 
#define BMP_TOOL5_BIN (51) 
#define BMP_TOOL6_BIN (52) 
#define BMP_TOOL7_BIN (53) 
#define BMP_TOOL8_BIN (54) 
#define BMP_TOOL9_BIN (55) 
#define BMP_UNCHECKED_BIN (56) 
#define BMP_UNSELECTED_BIN (57) 
#define BMP_WARNING_BIN (58) 
#define BMP_MAIN_STBOX_ON_BIN (59) 
#define BMP_MAIN_STBOX_OFF_BIN (60) 
#define BMP_START_UNICOM_BIN (61) 
#define BMP_BATTARY_BIG_BIN (62) 
#define BMP_MAIN_SLEEP_BIN (63) 
#define BMP_MAIN_MAILFUL_BIN (64) 
#define BMP_MAIN_SPEAKER_BIN (65) 
#define BMP_MAIN_WIFI_BIN (66) 
#define BMP_MAIN_BLUETOOTH_BIN (67) 
#define BMP_TOOLB_BIN (68) 
#define BMP_TOOLC_BIN (69) 
#define BMP_TALK_MUTE_BIN (70) 
























#define FL_AUDIO_1  (1)
#define FL_AUDIO_2  (2)
#define FL_AUDIO_3  (3)
#define FL_AUDIO_4  (4)   
#define FL_AUDIO_5  (5)   
#define FL_AUDIO_6  (6)
#define FL_AUDIO_7  (7)
#define FL_AUDIO_8  (8)
#define FL_AUDIO_9  (9)
#define FL_AUDIO_10 (10)


#define FL_ALARM_1  (11)
#define FL_ALARM_2  (12)
#define FL_ALARM_3  (13)
#define FL_ALARM_4  (14)   
#define FL_ALARM_5  (15)  




#define FL_SMSRING_1  'A'
#define FL_SMSRING_2  'B'
#define FL_SMSRING_3  'C'
#define FL_SMSRING_4  'D'   
#define FL_SMSRING_5  'E'  



   #define UART_SEND_APPLI_DATA gu_uart1_send_data 
   #define UART_SEND_MODEM_DATA gu_uart2_send_data






//FONT
#define FONT_T_X_10 1
#define FONT_B_X_10 2
#define FONT_B_X_16 3
#define FONT_DEFAULT FONT_T_X_10
#define LANGUAGE_ENGLISH     0
#define LANGUAGE_SPANISH     1
#define LANGUAGE_PORTUGE     2
#define LANGUAGE_CHINESE     3
//SCREEEN






//------------------------------------------------------------------------------------------
//对应于2.4G 字母机注册状态

//子机正在注册
#define CHLD_STATE_REGISTING       (1)

//子机注册成功
#define CHLD_STATE_REGIST_OK       (2)

//子机注册失败
#define CHLD_STATE_REGIST_FAIL     (3)

//子机正在注销
#define CHLD_STATE_UNREGISTING     (4)

//子机注销成功
#define CHLD_STATE_UNREGIST_OK     (5)

//子机注销失败
#define CHLD_STATE_UNREGIST_FAIL   (6)

//子机注册命令
#define CHLD_OPER_REGIST       (1)
//子机注销命令
#define CHLD_OPER_UNREGIST     (0)



///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////2222222222222222222222222222222222222222222222222
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


// keys

      #define TFKEY_SOFT_LEFT   139
      #define TFKEY_SOFT_RIGHT  158
	  #define TFKEY_ALARM       251
	  #define TFKEY_MUSIC       248
      #define TFKEY_UP          103
      #define TFKEY_LEFT        105
      #define TFKEY_RIGHT       106
      #define TFKEY_DOWN        108
      #define TFKEY_1           2
      #define TFKEY_2           3
      #define TFKEY_3           4
      #define TFKEY_4           5
      #define TFKEY_5           6
      #define TFKEY_6           7
      #define TFKEY_7           8
      #define TFKEY_8           9
      #define TFKEY_9           10
      #define TFKEY_STAR        122
      #define TFKEY_0           11
      #define TFKEY_SHARP       123
	  #define TFKEY_SEND        132
	  #define TFKEY_POWER       116
	  #define TFKEY_MUTE         249
	  #define TFKEY_REDIAL       67
      #define TFKEY_HANDFREE     63

	  


      #define TFKEY_EXIT        TFKEY_POWER
      #define TFKEY_VOL_UP      TFKEY_UP
      #define TFKEY_VOL_DOWN    TFKEY_DOWN
      #define TFKEY_MODE        TFKEY_SHARP


	  //---------NO USED DEFINE
      #define TFKEY_CONFIRM     901
      #define TFKEY_SMS         903
	  //--------
	  



	  #define TFKEY_ON_HOOK     107
	  #define TFKEY_OFF_HOOK    231





  






	  






	  








#define VOICE_ACTION_HANDSET 0x00
#define VOICE_ACTION_KEY     0x01

#define EXIT_BY_TIMEOUT      0x01
#define EXIT_BY_SOFT_AWAKE   0x04




///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////3333333333333333333333333333333333333333333333333
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#define NOTICE_TYPE_NULL      0
#define NOTICE_TYPE_ALARM     1
#define NOTICE_TYPE_INFO      2
#define NOTICE_TYPE_QUESTION  3
#define NOTICE_TYPE_FORBIDDEN 4
#define NOTICE_TYPE_SUCCEED   5
#define NOTICE_TYPE_POWEROFF  6


//edit box style
#define EDIT_STYLE_NOEXSTYLE      0x0000
#define EDIT_STYLE_READONLY       0x0001
#define EDIT_STYLE_DIGITALONLY    0x0002
#define EDIT_STYLE_HIDE_CURSOR    0x0004
#define EDIT_STYLE_PASSWD_CHAR    0x0008
#define EDIT_STYLE_SHOW_FRAME     0x0010
#define EDIT_STYLE_VERTICAL_BAR   0x0020
#define EDIT_STYLE_ALIGE_RIGHT    0x0040
#define EDIT_STYLE_ALIGE_MIDDLE   0x0080
#define EDIT_STYLE_ALIGE_CENTER   0x0100
#define EDIT_STYLE_SPLIT_WORDS    0x0200
#define EDIT_STYLE_DECIMALINPUT   0x0400
#define EDIT_STYLE_IPADDRESSINPUT 0x0800
#define EDIT_STYLE_MONEYINPUT     0x1000
#define EDIT_STYLE_SMS_FEATURE    0X2000
#define EDIT_STYLE_NUM_PARTICIAL  0X4000



#define EDIT_REDRAW_CAPTION   0x80
#define EDIT_REDRAW_BOTTOM    0x40
#define EDIT_REDRAW_INPUTMODE 0x20
#define EDIT_REDRAW_TOKEN     0x10
#define EDIT_REDRAW_SIZE      0x08
#define EDIT_REDRAW_DATA      0x01
#define EDIT_REDRAW_DATA_PTL  0x02


//edit input type
#define IN_FIXED   0
#define IN_ABC     1   
#define IN_abc     2   
#define IN_123     3  
#define IN_SIGN    4
#define IN_PHRA    5
#define IN_PY5D    6
#define IN_BX5D    7

//normal soft key type
#define IGNORE_SOFT_KEY_INPUT   0
#define NEED_SOFT_KEY_INPUT     1


#define SOFT_KEY_TYPE_NULL_NULL_NULL    0
#define SOFT_KEY_TYPE_DEL_CANCLE_OK     1
#define SOFT_KEY_TYPE_NO_NULL_YES       2
#define SOFT_KEY_TYPE_NULL_NULL_OK      3

#define SOFT_KEY_TYPE_BACK_SELECT_OK    4
#define SOFT_KEY_TYPE_BACK_NULL_OK      5
#define SOFT_KEY_TYPE_BACK_NULL_NULL    6

//menu box style
#define MENU_STYLE_SHOW_FRAME          0x01
#define MENU_STYLE_SHOW_SEQ            0x02
#define MENU_STYLE_SINGLE_OPTION       0x04
#define MENU_STYLE_MULTI_OPTION        0x08
#define MENU_STYLE_VERTICAL_BAR        0x10
#define MENU_STYLE_MUSIC_VOICE         0X20
#define MENU_STYLE_ALARM_VOICE         0X40
#define MENU_STYLE_SMSRING_VOICE       0X80



#define MENU_MASK_SELECTED             0x01


#define MENU_OPTION_READONLY           0x01
#define MENU_OPTION_MULTISELECT        0x02


// record box style
#define RECORD_STYLE_SHOW_FRAME          0x01
#define RECORD_STYLE_SHOW_SEQ            0x02
#define RECORD_STYLE_VERTICAL_BAR        0x04
#define RECORD_STYLE_RET_SELECT          0x08


/*
#define DIAL_TONE_ASIA             (17)
#define BUSY_TONE_ASIA             (19)
#define HRRY_TONE_ASIA             (21)
#define DIAL_TONE_USA              (18)
#define BUSY_TONE_USA              (20)
#define HRRY_TONE_USA              (22)
*/

#define DIAL_TONE_ASIA             (21)
#define BUSY_TONE_ASIA             (22)
#define HRRY_TONE_ASIA             (23)
#define DIAL_TONE_USA              (24)
#define BUSY_TONE_USA              (25)
#define HRRY_TONE_USA              (26)

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////4444444444444444444444444444444444444444444444444
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////



typedef struct BATTERYSTATUSTag
{
   int adc;  
}BATTERYSTATUS;


typedef struct KEY_EVTTag          
{
   unsigned char   key_val;   
   unsigned char   isPressed;   
}KEY_EVT;



typedef union EventDataTag
{
    KEY_EVT          key_evt;
    unsigned char    handset_evt; 
}EventData;


typedef struct FlSignalBufferTag
{
    unsigned long    eventTyp; 
	EventData sig_p;
}
FlSignalBuffer;




typedef struct FLASH_CELLTag
{
    short       cell_start;			// 单元的开始地址
    short       cell_len;			// 一个单元的长度，比如1K字节
	short       cell_cnt;			// 单元的数量，比如8个数量(如果单元长度是1k，则数据大小为8K)
	short       cell_max_rec_cnt;	// 单元长度/记录长度
}
FLASH_CELL;



typedef struct FLASH_TABLETag
{
	short       rec_max_cnt;		// 最多的记录数量
	short       rec_len;			// 单条记录的长度
}
FLASH_TABLE;




typedef struct DATABASEINFOTag
{
    char   **    p_flh;
    FLASH_CELL  cell_info;
    FLASH_TABLE tb_info;
}
DATABASEINFO;

cell_info.cell_max_rec_cnt

typedef struct
{
    unsigned short year;
    unsigned char  mon;
    unsigned char  day;
    unsigned char  week;
}DATE;
typedef struct
{
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
}TIME;



typedef struct T_ALARM_CLOCKTag
{
    TIME alarm_time;
	unsigned char alarm_ring;
    unsigned char alarm_on;
} T_ALARM_CLOCK;



typedef struct T_CALL_RECORDTag
{
    char  num[BOOK_ENTRY_NUM_FIELD_SIZE+1];
    unsigned short year;
    unsigned char  month;
    unsigned char  day;
    unsigned char  hour;
    unsigned char  minute;
    unsigned char  second;
    unsigned char  len_hour;
    unsigned char  len_minute;
    unsigned char  len_second;
    unsigned long  money_cost;	
    unsigned char  not_answer;
} T_CALL_RECORD;

typedef struct T_BOOK_ENTRYTag
{
    char name[BOOK_ENTRY_NAME_FIELD_SIZE+1];
    char num[BOOK_ENTRY_NUM_FIELD_SIZE+1];
    char location[2];     
} T_BOOK_ENTRY;

typedef struct T_PRE_RECORDTag
{
    char rec_string[32];  
	unsigned short pos;
}T_PRE_RECORD;



typedef union SMSFLAGTag
{
   unsigned char flag_sent;
   unsigned char flag_read;
}SMSFLAG;

typedef struct T_SMS_RECORDTag
{
    char   str_num[BOOK_ENTRY_NUM_FIELD_SIZE+1];
    DATE   date;
	TIME   time;
	char   context[SMS_CONTEXT_LENGTH+1];
	SMSFLAG  sms_flag;
} T_SMS_RECORD;






typedef struct t_auto_redial_status
{
    unsigned char redial_cnt;
    unsigned char redial_interval;
    char          num[BOOK_ENTRY_NUM_FIELD_SIZE+1];
	unsigned long redial_tick;
} T_AUTO_REDIAL_STATUS;


typedef struct T_CALL_STATETag
{
    unsigned char id;
    unsigned char dir;
    unsigned char stat;
    unsigned char mode;
    unsigned char mpty;
    char num[BOOK_ENTRY_NUM_FIELD_SIZE+1];
    char name[BOOK_ENTRY_NUM_FIELD_SIZE+1];
}T_CALL_STATE;

typedef struct T_CALL_DIVERT_STATUSTag
{
    unsigned char status;   // 0.not active/1.active
    char num[BOOK_ENTRY_NUM_FIELD_SIZE];
}T_CALL_DIVERT_STATUS;

typedef struct T_NETWORKSTag
{
    unsigned char stat;
    char oper[10];
    char numeric[6];
}T_NETWORKS;



typedef struct t_smsid_list
{
   int    sms_id;
   void * next;
} T_SMSID_LIST;









typedef struct T_BACKGROUND_RATINGTag
{
   T_CALL_RECORD * pcall_record;
   unsigned char flag_end_dial;
   unsigned char flag_start_rating;
   unsigned char flag_money_minder;
   unsigned char flag_last_duration;
   unsigned long dword_last_minter_tick;
   unsigned long real_money;
   unsigned long real_duration;   
   
} T_BACKGROUND_RATING;





typedef struct T_POWER_SERVICETag
{
	BATTERYSTATUS  battary_status;	
	unsigned char  flag_extern_power; 
	unsigned char  flag_having_battery;
	unsigned char  flag_battery_full;
	
	
	

	unsigned char  gui_flag_extern_power; 
	unsigned char  gui_flag_having_battery;
	unsigned char  gui_byte_battery_lvl;	

	unsigned long  dword_charge_state;
	unsigned long  dword_charge_tot_time;
	unsigned long  dword_no_ext_power_st;
} T_POWER_SERVICE;


typedef struct T_TABLE_INFOTag
{
   unsigned char   flag_loaded;
   unsigned short  rec_cnt;
   unsigned char * precords;
} T_TABLE_INFO;






typedef struct EDITWNDTag          
{

	unsigned char   frame_left;
	unsigned char   frame_top;
	unsigned char   frame_right;
	unsigned char   frame_bottom;

	unsigned char   body_left;
	unsigned char   body_top;
	unsigned char   body_right;
	unsigned char   body_bottom;

	unsigned char   notice_style;
	unsigned char   flag_show_cursor;

	unsigned char   cursor_x_last;
	unsigned char   cursor_y_last;
	unsigned char   cursor_x;
	unsigned char   cursor_y;

    char          * pcaption; 

	unsigned char   cur_line;
	unsigned char   body_lines;
	
	unsigned char   line_pos[40];
	unsigned char   line_cnt;
	unsigned char   line_start;

    unsigned short  data_buf[255];  
	unsigned char   data_pos;

	unsigned char data_len;
	unsigned char max_len;
	unsigned char max_bkp;

	unsigned short style;
	unsigned char input_mode;
	char  lx_buf[3];
	
	unsigned char data_font;
	unsigned char soft_key_type;
}EDITWND;



typedef struct MENUWNDTag          
{
	unsigned char   frame_left;
	unsigned char   frame_top;
	unsigned char   frame_right;
	unsigned char   frame_bottom;

	unsigned char   body_left;
	unsigned char   body_top;
	unsigned char   body_right;
	unsigned char   body_bottom;

    unsigned char * menu_mask; // look at mask define

    char          * pcaption; 
	unsigned char   cur_line;
	unsigned char   body_lines;
	
	unsigned char   line_cnt;
	unsigned char   line_start;

    char      ***    menu_buf;  

	unsigned char selected_pos;

	unsigned char selected_left;
	unsigned char selected_top;	
	unsigned char selected_right;	
	unsigned char selected_bottom;	
	unsigned char roll_pos;

	unsigned char style;
	unsigned char menu_font;
	unsigned char soft_key_type;
	unsigned char context_width;
	
}MENUWND;





typedef struct RECORDWNDTag          
{
	unsigned char   frame_left;
	unsigned char   frame_top;
	unsigned char   frame_right;
	unsigned char   frame_bottom;

	unsigned char   body_left;
	unsigned char   body_top;
	unsigned char   body_right;
	unsigned char   body_bottom;

    unsigned char   table_idx;
	unsigned char  * pindex;
    char           * pcaption; 
	char            fast_buf[48];
	T_PRE_RECORD    pre_rec[4];


    unsigned short  line_cnt;
	unsigned short  line_start;	
    unsigned short  selected_pos;
	unsigned short  body_lines;
	unsigned short  cur_line;	

	unsigned char   operate_mode;//add,delete,edit

	unsigned char selected_left;
	unsigned char selected_top;	
	unsigned char selected_right;	
	unsigned char selected_bottom;	
	unsigned char roll_pos;
	unsigned char style;


	unsigned char record_font;
    unsigned char context_width;

	
}RECORDWND;













typedef struct t_fast_dialtag
{
    RECORDWND * p_record_wnd;
	unsigned char rec_pos;
}
T_FAST_DIAL;
    


	

typedef struct APPSYSTag
{

	DATE struct_date;
	TIME struct_time;
	T_BACKGROUND_RATING back_rating;
    T_POWER_SERVICE power_service;
	
	T_TABLE_INFO    book_info;
	



    unsigned char flag_half_second;
	unsigned char flag_keypad_locked;
    unsigned char flag_backlight_on;
	unsigned char flag_dialing;
	unsigned char flag_dialout_succeed;
	unsigned char flag_fast_dial;
	unsigned char flag_doing_auto_redial;
	unsigned char flag_pre_dial;
	unsigned char flag_talk_rating;
	unsigned char flag_time_changed;
	unsigned char flag_color_reverse;
	unsigned char flag_redial;
	unsigned char flag_showing_lowbat;
	unsigned char flag_sms_full;
	unsigned char flag_selecting_ring;
	

	
	unsigned char flag_key_state;
	unsigned char flag_hook_state;
	unsigned char flag_doing_fast;
	unsigned char flag_hand_free;
	unsigned char flag_no_carrier;
	unsigned char flag_call_comming;
	unsigned char flag_having_unread_sms;
	unsigned char flag_having_missed_call;
	unsigned char flag_rate_incomming;
	unsigned char flag_enable_handfree;
	unsigned char flag_playing_tone;
	unsigned char flag_muted;
	unsigned char flag_key_tone;
	unsigned char flag_flash_checked;
    unsigned char flag_nouim_emergency_call;
	unsigned char flag_voice_path;
	unsigned char flag_signal_led_on;
	unsigned char flag_hdfree_led_on;
	unsigned char flag_speaker_on;
	unsigned char flag_is_sleeping;
	unsigned char flag_any_key_stop_music;
	unsigned char flag_genie_trace;
	unsigned char flag_got_andr_message;
	unsigned char flag_sending_sms;
	unsigned char flag_talk_stop_type;
	unsigned char flag_alarm_ringing;
	unsigned char flag_super_mode;
	unsigned char flag_record_showing;
	unsigned char flag_editing_sms;
	unsigned char flag_need_reset_flash;
	unsigned char flag_prerec_changed;	
    unsigned char flag_keypower_down;
	unsigned char flag_waiting_power_down;
	unsigned char flag_low_layer_dial;
	unsigned char flag_hardware_test;
	unsigned char flag_sim_encypted;


	unsigned char flag_need_clock_alarm;
	unsigned char flag_editing_draft;	
	
	unsigned char flag_voice_doing;


 	unsigned char flag_wish_input_star;
	unsigned char flag_holding_call;
	unsigned char flag_sending_numbers;
	unsigned char flag_handfree_pressed;


	unsigned char flag_got_421;
	unsigned char flag_got_422;
	unsigned char flag_got_423;
 	unsigned char flag_got_424;
 	unsigned char flag_got_425;	
 	unsigned char flag_got_426;		

	unsigned char flag_got_452;	
	unsigned char flag_got_453;	
	unsigned char flag_got_454;	
	unsigned char flag_got_455;	
	unsigned char flag_got_456;	
	unsigned char flag_got_457;	
	unsigned char flag_got_458;	
	unsigned char flag_got_459;	
	unsigned char flag_got_45A;	


    unsigned char flag_ring_mute;

	

	unsigned char byte_simcard_state;

 	unsigned char byte_input_key_when_delay;	
    unsigned char byte_cur_draft_rec_no;

    unsigned char byte_blue_cnt;
	unsigned char byte_wifi_cnt;

    unsigned char byte_alarm_clock_hour;
	unsigned char byte_alarm_clock_min;
	
		
    unsigned char byte_sysrun_state;
    unsigned char byte_need_redraw;
	unsigned char byte_font_type;
	
	unsigned char byte_csq_level;
	unsigned char byte_err_rate;
	unsigned char byte_reg_state;
    unsigned char byte_net_state;
    unsigned char byte_roam_state;
    unsigned char byte_tlk_state;
	unsigned char byte_ossc_state;
    unsigned char byte_pinret_state;
	unsigned char byte_smsret_state;
    unsigned char byte_divtret_state;
	unsigned char byte_callwaitret_state;
	unsigned char byte_blueret_state;
	unsigned char byte_wifiret_state;

		
	unsigned char byte_last_music_id;
	unsigned char byte_music_max_time;
 	unsigned char byte_power_service_delay;
	unsigned char byte_last_minute;
    unsigned char byte_need_exit;
	unsigned char byte_fast_key_times;
	unsigned char byte_redial_cnt;
	unsigned char byte_syn_failed_cnt;
	unsigned char byte_simcard_sms_cnt;
	unsigned char byte_simcard_book_cnt;
	unsigned char byte_simsms_idpos;
	unsigned char byte_simbook_idpos;
	unsigned char byte_delay_cnt_handfree;

	unsigned short word_change_times;


	unsigned long dword_last_input_tick;
	unsigned long dword_last_key_tick;
	unsigned long dword_half_second_tick;
	unsigned long dword_one_second_tick;
	unsigned long dword_dialtone_st;
	unsigned long dword_prepare_enspk_tick;

	
    unsigned long dword_key_power_tick;
 	unsigned long dword_callwait_clcc_tick;

	unsigned long dword_last_clock_alarm_tick;





    char *  combined_buf;
	char str_sys_time[20];
	char str_operator_name[20];
	char str_dialed_numbers[BOOK_ENTRY_NUM_FIELD_SIZE+1];

	char          str_incom_number[BOOK_ENTRY_NUM_FIELD_SIZE+1];
	char          str_debug_msg[255];

 	char str_imei_no[32];
    char str_simcimi_number[32];
    char str_iccid[32];
	char str_andr_ver[200];


	T_FAST_DIAL fst_dial;

    T_SMS_RECORD  tp_sms;
	T_BOOK_ENTRY  tp_book;
	T_CALL_DIVERT_STATUS DIVT_LIST[MAX_DIVT_CNT];

	char  BLUE_LIST[MAX_BLUE_WIFI_CNT][BLUE_WIFI_NAME_LEN];
	char  WIFI_LIST[MAX_BLUE_WIFI_CNT][BLUE_WIFI_NAME_LEN];
    
}
APPSYS;


// 系统参数
typedef struct SYSPROPTag
{
    unsigned long prop_size;   // high word == ~low word
    char dversion[12];
	char rversion[12];

#if 0//总共定义20个字节保留，以预防中后期增加字段
    unsigned char prop_reserved[20]; 
#else 
    unsigned char prop_reserved[14]; 
    //===============================	



    unsigned char byte_call_delay_s;	
#endif
	
    unsigned char flag_auto_lock_keypad;
	unsigned char flag_lock_phone;				// 锁住电话
	unsigned char flag_auto_redial;
	unsigned char flag_key_tone;

	
 	unsigned char flag_incomecall_allow;
	unsigned char flag_outcall_allow;
	unsigned char flag_internationalcall_allow;
	
	unsigned char flag_clock_format;
	unsigned char flag_call_waiting;
	unsigned char flag_lock_operator;
	unsigned char flag_lock_base;
	unsigned char flag_encyped_only;	
	unsigned char flag_blue_enabled;
	unsigned char flag_wifi_enabled;
	unsigned char flag_simpin_enabled;



	unsigned char byte_income_ring;
	unsigned char byte_sms_ring;
	unsigned char byte_contrast_volume;
	
	unsigned char byte_ring_volume;
 	unsigned char byte_sms_volume;
 	unsigned char byte_talk_volume;

	
	unsigned char byte_decimal_pos;
    unsigned char byte_backlightoff_timeout;
	unsigned char byte_sleep_timeout;
	unsigned char byte_auto_lock_keypad_timeout;
	unsigned char byte_language_type;
    unsigned long dword_call_in_tm_total;
	unsigned long dword_call_out_tm_total;


	T_ALARM_CLOCK struct_alarm;
	T_AUTO_REDIAL_STATUS auto_redial;


	char str_master_pin[10];
	char str_keypad_pin[10];
	char str_simpin_1[10];
	char str_additional_prefix[BOOK_ENTRY_NUM_FIELD_SIZE];
	char str_cur_bluetooth[BLUE_WIFI_NAME_LEN];
 	char str_cur_wifi[BLUE_WIFI_NAME_LEN];

}
SYSPROP;




typedef struct VERTICALBARTag          
{
    unsigned char   left;
	unsigned char   top;
	unsigned char   right;
	unsigned char   bottom;
	unsigned char   body_lines;
	unsigned short  line_cnt;
	unsigned short  cur_line;
}VERTICALBAR;

















#define	DCS_MODE_DEFA     0x00
#define	DCS_MODE_UCS2     0x08
#define	DCS_MODE_UCS1     0x0b
#define	DCS_MODE_BIT8	  0x04  

#define SMS_CONTINUE_FLAG 0x40
#define PHONE_NUMBER_SIZE BOOK_ENTRY_NUM_FIELD_SIZE
#define DATE_TIME_SIZE    14
#define SMS_DBIDX_SIZE    10
#define SMS_DATA_LENGTH   180

#define SMS_PKT_UNIC_MLEN  (67)
#define SMS_PKT_7BIT_MLEN  (153)

#define SMS_PKT_UNIC_SLEN  (70)
#define SMS_PKT_7BIT_SLEN  (160)

	


typedef	struct	 
{
    unsigned char  sca_len;
	unsigned char  sca_type;
	char sca_addr[PHONE_NUMBER_SIZE+1];

	unsigned char  pdu_type;

	unsigned char  cid_len;
	char cid_number[PHONE_NUMBER_SIZE+1];

	unsigned char  pro_id;
	unsigned char  dcs;
	char time_stamp[DATE_TIME_SIZE +1];
	
	unsigned char  data_len;

	unsigned char  sms_uqid;

	unsigned char  tot_packets;
	unsigned char  cur_packet;
	char str_db_index[SMS_DBIDX_SIZE+1];

	union TAG
	{
		unsigned short sms_wdata[ SMS_DATA_LENGTH +1];
		unsigned char  sms_cdata[(SMS_DATA_LENGTH +1)*2];
	}uni_or_chr;
    
}T_SMS_INFO;






extern void  f_memset( void *s,int c,unsigned short n );
extern void  f_memcpy( void *s1,const void *s2, unsigned short n );
extern unsigned short f_strlen( char *s1 );
extern void f_strcpy( char *s1,const char *s2 );
extern unsigned char is_hex(char c);
extern unsigned char is_hex_str(char * pstr);
extern unsigned char is_ansi(char c);
extern unsigned char is_ansi_str(char * pstr);

extern unsigned char get_hex(char c);
extern unsigned char get_send_dcs(char * pdt);






///////////

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
/////////////////////GLOBALS///////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

extern void app_main();
extern void app_init_global_vars();
extern void app_check_valid_paras();
extern unsigned char app_check_ring_mute();
extern void app_initial_system();
extern void app_query_hw_info(unsigned short wait_ms);
extern unsigned char app_wait_simcard_ready(int wait_ms);
extern void app_system_reset();
extern void app_feed_dog();
extern void delay_ms(unsigned short timeout_ms);
extern void app_shut_down(unsigned char flag_reboot);
extern void app_test_heap();
extern unsigned char get_crc_value(unsigned char * pdata, unsigned short length);
extern unsigned char app_get_message(FlSignalBuffer * p_evt );
extern unsigned long app_get_tick_count();
extern unsigned long get_duration_tick(unsigned long st_tick);
extern unsigned long get_duration_tick_ex(unsigned long st_tick, unsigned long et_tick);
extern void app_run_sleeping(void);
extern void app_run_wakeup(void);
extern unsigned short findsubstr( char *src,const char *sub,unsigned short st_pos, unsigned short et_pos );
extern unsigned short findsubdata(unsigned char *src, unsigned short src_len,unsigned char * sub,  unsigned short sub_len ,unsigned short st_pos, unsigned short et_pos);
extern unsigned short dumpstr(  char *src,unsigned short src_len,char *dest,unsigned short deslen,const char *begin,const char *end );
extern unsigned char get_module_line(unsigned short timeout_ms);
extern unsigned char app_pre_process_msg(FlSignalBuffer  * p_evt );
extern unsigned long asc_to_digit( char * str_asc,unsigned char d_cnt );
extern unsigned char is_digit(char c);
extern unsigned char is_dtmf(char c);
extern unsigned char is_digit_str(char * p, unsigned short len);
extern unsigned char is_dtmf_str(char * p, unsigned short len);
extern unsigned char is_ip_str(char * p, unsigned short len);
extern char * trim_left ( char * p );
extern char * trim_right ( char * p );
extern char * trim ( char * p );
extern char asc_2_lower( int ch );
extern char asc_2_upper( int ch );
extern void str_replace( char * pchar, char old_c, char new_c );

extern char * str_pad_left(char * pstr,unsigned char max_len, char c);
extern char * str_pad_right(char * pstr,unsigned char max_len, char c);
extern unsigned char convert_csq_level_grade( unsigned char csq_level );
extern void app_get_system_datetime();
extern void app_show_predial_window();
extern void show_soft_key_prompt(  char *left, char *right );
extern void show_network_prompt( void );
extern void app_refresh_default_screen( void );
extern void show_seg_clock();
extern void show_time_colon(unsigned char flag_show);
extern void show_clock_time( void );
extern char *  get_multi_string(char ** p_str);
extern char *  get_multi_string_ex(char *** p_str, unsigned char line);
extern void run_main_menu( void );
extern void app_test_function();
extern void refresh_interface_registering();
extern void process_soft_key_on_def_screen(unsigned char soft_key);
extern void app_show_battery(unsigned char flag_cycle);
extern void process_auto_redraw();
extern void process_alarm_ring();
extern void initial_controlbox();

extern void set_modem_buf_size(unsigned char type);
extern unsigned char app_check_password(unsigned char pin_type);
extern unsigned char edit_sms_message( T_SMS_RECORD * psms_record);
extern void show_sms_info( T_SMS_RECORD * psms_record ,unsigned char table_idx );
extern void app_volumn_setting(unsigned char lvl_nums, unsigned char cur_lvl);
extern void app_set_lcd_contrast(void);
extern void run_call_barring_set( void );
extern char * app_get_datetime_str();




extern void run_set_call_wait( void );
extern void run_set_call_divert( void );
extern void app_set_ring_volume(void);
extern void app_set_sms_volume();
extern void app_set_speech_volume();
extern void app_view_version( void );
extern unsigned char get_dial_delay();
extern unsigned char  app_user_change_simpin( void );
extern void app_pin_setting( void );
extern void app_send_data_2_pc( char * data );

extern unsigned char app_blue_enabled_query();
extern unsigned char app_bluetooth_device_query(unsigned char * def_pos);
extern void app_bluetooth_setting( void );
extern unsigned char app_wifi_enabled_query();
extern unsigned char app_wifi_device_query(unsigned char * def_pos);
extern void app_wifi_setting( void );


extern void show_device_serial_number();
extern void show_device_csq_level();
extern void show_device_power_info();
extern void app_set_apn_string( char * pbuf,  char * pcap);
extern unsigned char hex_to_byte(char hex);
extern unsigned long hex_str_to_long(char * hex_str);
extern unsigned long hex_str_to_long_ex(char * hex_str, unsigned char hex_len);
extern unsigned char check_if_having_new_sms();
extern unsigned char check_if_having_missed_call();
extern void process_recv_call();
extern unsigned char app_music_show(unsigned char * def_pos);
extern unsigned char app_alarm_show(unsigned char * def_pos);
extern unsigned char app_smsring_show(unsigned char * def_pos);

extern void app_play_dial_tone(unsigned char tone_id,  unsigned char max_time);
extern void app_play_income_ring(unsigned char ring_id, unsigned char max_time);
extern void app_play_alarm_ring(unsigned char alarm_id, unsigned char max_time);
extern void app_play_sms_ring(unsigned char smsvoc_id);
extern void app_set_voice_volumn(unsigned char music_id,  unsigned char volumn);
extern void app_stop_music();
extern void app_free_table_info(T_TABLE_INFO * ptable_info);
extern unsigned short app_load_table_info(unsigned char table_idx,  T_TABLE_INFO * ptable_info);
extern unsigned char check_if_rec_in_table_index(unsigned short * pshort_buf, unsigned short length, unsigned short rec_no);

extern void app_show_no_simcard();
extern unsigned char app_battery_query(void);
extern void app_enable_speaker();
extern void app_disable_speaker();
extern void app_signal_led_on();
extern void app_signal_led_off();



extern char * left_trim_for_base64( char * p );
extern char * right_trim_for_base64( char * p );
extern char * trim_for_base64( char * p );
extern unsigned char calcuate_verify_value(unsigned char *data, unsigned int data_size);

extern void app_view_call_meter_total( void );
extern void app_view_sms_total( void );

extern unsigned char get_battery_lvl();
extern void app_power_service();
extern void app_get_power_info();

extern void app_handset_test(void);
extern void app_handfree_test(void);
extern void app_battery_test(void);
extern void app_lcd_test(void);
extern void setup_auto_redial(void);
extern void app_simpin_test();
extern void app_protocols_test();
extern void app_signal_test_resp();


///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////PHONEBOOK functions//////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
extern unsigned char entry_name_compare( char * name1,char * name2 );
extern char * get_cmp_numbers(char * pnumber);
extern unsigned char entry_numbers_compare(char * pnumber1,char * pnumber2 );
extern unsigned char find_book_entry_by_numbers( char * numbers,T_BOOK_ENTRY * pdest_entry);
extern unsigned char find_book_entry_by_speed_location( char * str_location,T_BOOK_ENTRY * pdest_entry );
extern unsigned char edit_book_entry( T_BOOK_ENTRY * pbook_entry );
extern void search_book_option(RECORDWND * p_record_wnd);
extern unsigned short find_book_entry_insert_location( T_BOOK_ENTRY * pbook_entry );
unsigned short save_book_entry( T_BOOK_ENTRY *pbook_entry );
extern unsigned short modify_book_entry( unsigned short rec_no );
extern unsigned char delete_book_entry( unsigned short rec_no );
extern unsigned short new_book_entry( char * numbers );




///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////LCD functions///////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

extern void lcd_backlight_on();
extern void lcd_backlight_off();
extern void lcd_init_my();
extern void lcd_set_contrast_my(unsigned char value);
extern void lcd_clear_screen(void);
extern void lcd_update_screen_area(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1);
extern void lcd_goto_xy(unsigned char x, unsigned char y);
extern void lcd_goto_x_line(unsigned char x, unsigned line);
extern unsigned char lcd_get_x(void);
extern unsigned char lcd_get_y(void);
extern void lcd_add_x( unsigned char step );


extern unsigned char get_bit_from_bytes(const unsigned char * data,unsigned char pos);
extern void set_bit_to_bytes(unsigned char * data,unsigned char pos,unsigned char bitval);
extern unsigned char get_bits_num_from_bytes(const unsigned char * data);
extern void lcd_display_data(unsigned char width,unsigned char height ,const unsigned char * pdata);
extern unsigned char lcd_put_char(unsigned char data);// return width
extern void lcd_put_string( char * data);
extern void lcd_put_rect(unsigned char left,unsigned char top ,unsigned char right,unsigned char bottom);
extern void lcd_put_rect_ex(unsigned char left,unsigned char top ,unsigned char right,unsigned char bottom, unsigned char bshow);
extern unsigned char * get_char_font_data(unsigned short data);

extern unsigned char lcd_show_to_colum(char * pstring, unsigned char left, unsigned char top, unsigned char dst_col);
extern unsigned char lcd_put_wchar(unsigned short data);




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++FLASH CMDS+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern void app_check_database();
extern void reset_table_index( unsigned char tb_idx);
extern void reset_table_index_ex( unsigned char tb_idx, unsigned short rec_used);
extern unsigned char flash_cell_read(  unsigned short cell_id, unsigned short data_len, unsigned char * pdata );
extern unsigned char flash_cell_write( unsigned short cell_id, unsigned short data_len, unsigned char * pdata );
extern unsigned char app_flash_read( unsigned char dbinfo_idx, unsigned short rec_id, unsigned char * buf);
extern unsigned char app_flash_write(unsigned char dbinfo_idx, unsigned short rec_id, unsigned char * buf);

extern void app_flash_might_save(char * str_reason);
extern void app_test_syn_data(signed char tb_idx);

extern unsigned char * app_alloc_memory(unsigned short mem_size);
extern void   app_free_memory(void * mem_buf);
extern unsigned short app_get_record_used(unsigned char tb_idx);
extern unsigned short app_get_record_index(unsigned char tb_idx, unsigned short rec_no);
extern unsigned long app_get_record_used_and_index(unsigned char tb_idx, unsigned short rec_no);
extern unsigned char app_record_read(unsigned char tb_idx, unsigned short  rec_no, unsigned char * pbuf );
extern unsigned char app_record_edit(unsigned char tb_idx, unsigned short  rec_no, unsigned char * pbuf );
extern unsigned char app_record_insert(unsigned char tb_idx, unsigned short  rec_no, unsigned char * pbuf );
extern unsigned char app_record_delete(unsigned char tb_idx, unsigned short  rec_no);
extern unsigned char app_record_append(unsigned char tb_idx,  unsigned char * pdata, unsigned char flag_fix_type );
extern void save_file_to_flash(unsigned char file_idx,unsigned short cell_pos, unsigned char * pdata);
extern unsigned char load_file_from_flash(unsigned char file_idx,unsigned short cell_pos, unsigned char * pdata);
extern unsigned char app_check_prop();
extern void fill_prop_with_pri();
extern void app_save_prop();

extern void load_from_factory(void); 
extern void send_android_command(char * str_cmd);
extern void fake_android_message(char * str_cmd);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++USER INTERFACE+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern unsigned char get_wchar_width(unsigned short c);

extern unsigned char get_str_dots_width( char * pdata);
extern unsigned char get_str_dots_width_ex( char * pdata, unsigned char length);

extern unsigned char get_wstr_dots_width( unsigned short * pdata);
extern unsigned char get_wstr_dots_width_ex( unsigned short * pdata, unsigned char length);


extern unsigned char wnd_wstr_insert_char(EDITWND * p_edit_wnd, unsigned short c);
extern unsigned char wnd_wstr_delete(EDITWND * p_edit_wnd);
extern unsigned char wnd_batch_wstr_delete(EDITWND * p_edit_wnd);
extern unsigned char wnd_str_insert_str(EDITWND * p_edit_wnd, char * str_add);

extern unsigned char get_cursor_height();
extern char get_key_value(unsigned char key_value);

extern void show_caption(char * pcap);


extern void wnd_put_cursor(EDITWND * p_edit_wnd);
extern void wnd_cursor_move(EDITWND * p_edit_wnd,unsigned char key_code);
extern unsigned char wnd_edit_init( EDITWND * p_edit_wnd);
extern void wnd_edit_redraw( EDITWND * p_edit_wnd);
extern void wnd_data_redraw(EDITWND * p_edit_wnd);
extern void wnd_get_lines_info(EDITWND * p_edit_wnd);
extern unsigned char wnd_edit_txt (EDITWND * p_edit_wnd);
extern void wstr_insert_char(unsigned short * pdata, unsigned char max_len, unsigned char pos , unsigned short c);
extern void wstr_delete(unsigned short * pdata, unsigned char max_len, unsigned char pos );
extern void lcd_clear_line(unsigned char dest_col);
extern void lcd_clear_line_ex(unsigned char height,unsigned char dest_col);
extern void lcd_fill_area( unsigned char def_data,unsigned char x,unsigned char y,unsigned char width,unsigned char height );


extern void show_vertical_bar(VERTICALBAR* pbar);
extern void lcd_draw_vertical(unsigned char def_val,unsigned char height);
extern void lcd_draw_horizontal(unsigned char width);
extern void lcd_mem_set_pixel(unsigned char x, unsigned char y, unsigned char color);
extern unsigned char lcd_mem_get_pixel(unsigned char x, unsigned char y);
extern void lcd_mem_bresenham_Line(int x1,int y1,int x2,int y2,int color);
extern unsigned short lcd_mem_put_ellipse(unsigned char Ox, unsigned char Oy, unsigned char radii, unsigned char el_rate, unsigned char mask);
extern void lcd_mem_put_circle(unsigned char Ox,unsigned char Oy, unsigned char radii);
extern unsigned short lcd_put_percent_pillar(unsigned char percent);

//////////////////////////////control box lcd method


extern unsigned char get_input_mode( EDITWND *  p_edit_wnd);
extern unsigned short wnd_get_sign_option(EDITWND * p_edit_wnd);
extern char wnd_edit_IN_123(EDITWND * p_edit_wnd,unsigned char key_code)  ;
extern char wnd_edit_IN_AaBbCc(EDITWND * p_edit_wnd,unsigned char key_code) ;
extern unsigned short wnd_edit_IN_PY5D(EDITWND * p_edit_wnd,unsigned char key_code);
extern unsigned short wnd_edit_IN_BX5D(EDITWND * p_edit_wnd,unsigned char key_code);
extern unsigned char wnd_input_password(char * buf,  char * pcap,unsigned char maxlen);
 
extern unsigned char wnd_input_decimal(char * buf,  char * pcap,unsigned char maxlen, unsigned short ex_style);
extern unsigned char wnd_input_text(char * buf,  char * pcap,unsigned char maxlen,unsigned char input_mode, unsigned short ex_style);
extern unsigned char check_valid_decimal(EDITWND * p_edit_wnd, char key_val);
extern unsigned char check_valid_ipaddress(EDITWND * p_edit_wnd, char key_val);
extern unsigned char check_valid_money(EDITWND * p_edit_wnd, char key_val);
extern unsigned char check_valid_money_ex(char * data_buf, unsigned char data_len, char key_val);
extern unsigned char wnd_show_notice( char * pcap, char * buf,unsigned char notice_style,unsigned char need_soft_key );
extern void wnd_show_password_style();
extern void wnd_show_decimal_style();


extern unsigned char ConvPYInpCode(unsigned char bInpMode, char *pInpCode, T_PYINPUTCODE *ptPYInpCode);
extern unsigned char ReadPYHzCode(char bInpMode, char *pInputCode, unsigned char bSelectNo, T_PYINPUTCODE *ptPYInpCode, unsigned char *psResult);
extern unsigned char ReadBXHzCode(char *pInpCode,unsigned char *pResult);
extern unsigned char InitLXIndexTbl();
extern unsigned char InitPYIndexTbl();
extern unsigned char InitBHIndexTbl();

extern unsigned char ReadPYNextHz(unsigned char *pResult);
extern unsigned char ReadLXNextHz(unsigned char *pResult);
extern unsigned char ReadLXCode(unsigned char *pHzCode, unsigned char *pResult);


extern char * get_combined_string(char * str1, char * str2 , char * str3);
extern void msg(char * disp_txt);
extern void msg_area_ex(char * disp_txt, unsigned char line);
extern void msg_area(char * disp_txt);


extern void msg_warn(char * disp_txt);
extern unsigned char choose_yes_no(char * strcap, char * disp_txt);
extern void msgok(char * disp_txt);
extern unsigned char wnd_show_sms_txt( EDITWND * p_edit_wnd);
extern void app_show_sms_txt(RECORDWND * p_record_wnd);
extern void app_show_normal_text(char * buf,  char * pcap);
extern void app_show_sms_record(T_SMS_RECORD * psms_record, unsigned char id_pos);

extern void remove_simsms_id(unsigned char id_pos);
extern void app_show_sms_from_simcard(void);
extern void app_show_book_from_simcard();
extern void app_show_sim_sms_array();
extern void app_show_sim_book_array();

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++MENU SHOW+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern unsigned char app_menu_option( char *** menus, unsigned char * menu_mask, unsigned char max_len,unsigned short def_pos,unsigned char option_mask, unsigned char ex_style);
extern unsigned char app_menu_select( char *** menus, unsigned char max_len,unsigned short * p_pos_info);
extern unsigned char wnd_menu_show( MENUWND * p_menu_wnd);
extern void wnd_menu_redraw(MENUWND * p_menu_wnd);
extern void wnd_menu_move(MENUWND * p_menu_wnd,unsigned char key_code);
extern void wnd_menu_init(MENUWND * p_menu_wnd);
extern unsigned short show_bitmap(unsigned char bmp_id);
extern void wnd_roll_selected_menu(MENUWND * p_menu_wnd);
extern void wnd_menu_set_select(MENUWND * p_menu_wnd);
extern unsigned char choose_menu_option(char *** temp_menus, unsigned char max_size, unsigned char * def_pos);
extern unsigned char choose_menu_option_ex(char *** temp_menus, unsigned char max_size, unsigned char * def_pos, unsigned char flag_read_only, unsigned char ex_style);
extern unsigned char app_graph_meme( unsigned char * icons,char *** menus,unsigned char icon_nums,unsigned char cursor );
extern void app_run_books( void );
extern void app_run_recent_calls( void );
extern void app_run_messages( void );
extern unsigned char app_set_time( TIME * pret_time);
extern void app_run_security_settings( void );
extern void app_run_super_settings( void );
extern void app_run_factory_settings();
extern void app_run_all_settings( void );
extern void app_run_display_settings( void );
extern void app_select_ring_music();
extern void app_run_voice_settings( void );
extern void app_run_call_ctrl_settings( void );
extern void app_hardware_test( void );
extern void app_keyboard_test(void);

extern unsigned char check_if_no_ext_power(unsigned char * pid);
extern void app_process_alarm_comming_background();




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++RECORD SHOW+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern unsigned char get_record_string(RECORDWND* p_record_wnd, char *rec_buf, unsigned short rec_pos);
extern void wnd_roll_selected_record(RECORDWND* p_record_wnd);
extern void wnd_record_move(RECORDWND * p_record_wnd,unsigned char key_code);
extern void wnd_record_redraw(RECORDWND * p_record_wnd);
extern void wnd_record_init(RECORDWND * p_record_wnd);
extern unsigned char wnd_record_show( RECORDWND * p_record_wnd);
extern void app_records_show( unsigned char table_idx, char * pcap);
extern void set_records_list_soft_key(RECORDWND* p_record_wnd);
extern unsigned char run_records_list_softkey_action(RECORDWND* p_record_wnd, unsigned char soft_key);
extern void view_call_log( RECORDWND* p_record_wnd );
extern void app_show_record_capacity(unsigned char table_idx);
extern void sms_box_option(RECORDWND * p_record_wnd);
extern void wnd_record_delete(RECORDWND * p_record_wnd);
extern unsigned short wnd_get_record_used(RECORDWND* p_record_wnd);
extern unsigned long wnd_get_record_used_and_index(RECORDWND* p_record_wnd, unsigned short rec_no);
extern unsigned char wnd_record_read(RECORDWND* p_record_wnd, unsigned short  rec_no, unsigned char * pbuf );
extern void wnd_load_table_index(RECORDWND* p_record_wnd);




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++TIME +++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern unsigned char  YearIsRunNian(   int year );
extern unsigned short GetGongYearDays( int year );
extern unsigned char  GongmonIsLarge(  int mon );
extern unsigned char  GetGongmonDays(  int year,char mon );
extern unsigned char  GetWeekInfo();
extern unsigned char  get_week_from_date(DATE * pdate);
extern unsigned char  time24_to_time12(TIME * time24, TIME * time12);
extern void time12_to_time24(TIME * time12, TIME * time24, unsigned char flag_am_pm);
extern unsigned char app_set_time( TIME * pret_time);
extern void app_set_alarm_clock( void );
extern void get_alarm_time_str(TIME * p_time, char * ret_str);
extern void get_normal_time_str(TIME * p_time, char * ret_str);
extern void app_set_date( DATE * p_date );
extern void app_set_date_time();
extern void get_date_str(DATE * p_date, char * ret_str);
extern void get_time_str_ex(TIME * p_time, char * ret_str,unsigned char bfixed_len, unsigned char is_alarm);

















///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////MODULE Functions/////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
extern unsigned char mu_user_enter_pin( char * pin_out );
extern unsigned char mu_user_enter_puk( void );
extern unsigned char mu_is_pin_active( void ) ;
extern unsigned char mu_change_pin( char *sOld,char *sNew ) ;
extern unsigned char mu_active_pin( char *sPin )  ;
extern unsigned char mu_cancel_pin( char *sPin ) ;
extern unsigned char mu_user_active_pin( char * spin ,int length);
extern unsigned char mu_user_cancel_pin( void );

extern unsigned char mu_registering();
extern void wait_for_android_response(int wait_ms);
extern unsigned char mu_get_cimi( char *str_cimi, unsigned char cimi_len );
extern unsigned char mu_check_sim_valid(int wait_ms);
extern int get_smsinfo_from_simcard(void);	
extern int get_bookinfo_from_simcard();
extern unsigned char mu_get_csq( void ) ;
extern unsigned char mu_reg_changed( void );
extern void mu_set_spk_gain( unsigned char level );
extern void mu_set_ring_gain( unsigned char level );
extern void mu_dial_dtmf( char dtmf );  
extern unsigned char mu_dial_dtmf_str( char * dtmf_str );
extern void mu_hook_on( void );
extern void mu_answer_call( void );
extern unsigned char mu_set_voice_path( unsigned char voc_path );
extern void mu_generate_beep( void );
extern void mu_generate_dtmf( char dtmf );
extern void mu_play_dialtone( void );
extern void mu_stop_dialtone( void );
extern unsigned char mu_mute_mic(void) ;
extern unsigned char mu_cancel_mute_mic(void) ;
extern void mu_delete_message( unsigned char sm_index );
extern unsigned char mu_send_sms( char * numbers,char * context ) ;
extern void mu_set_date_time(DATE * p_date, TIME * p_time);
extern unsigned char mu_list_operator( T_NETWORKS *networks );
extern unsigned char mu_select_operator( unsigned char mode,char * name );
extern unsigned char mu_get_cgsn( char * str_cgsn,unsigned char datelen );
extern unsigned char  mu_getsimcard_sms(unsigned char sms_idx, T_SMS_RECORD * psms_record);
extern unsigned char  mu_getsimcard_book(unsigned char book_idx, T_BOOK_ENTRY* pbook_entry) ;
extern unsigned short find_char_pos(char *src, const char c, unsigned short tms);
extern unsigned char  mu_list_calls( T_CALL_STATE *call_state );
extern void mu_terminate_call( unsigned char id );
extern void mu_active_holded_call( unsigned char id );
extern void mu_shift_waiting_call( void );



extern unsigned char mu_query_callwait_status( void );
extern unsigned char mu_set_callwait( unsigned char on_off );
extern unsigned char mu_dactive_calldivert( unsigned char divert_mode );
extern unsigned char mu_query_calldivert_status( );
extern unsigned char check_ccfc_state(unsigned char * pcmd, T_CALL_DIVERT_STATUS * divert_status);
extern unsigned char mu_active_calldivert( unsigned char divert_mode,char *num );



extern void gu_set_sys_date_time(DATE *p_date, TIME * p_time);



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++DIAL OUT++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern unsigned char wnd_input_dialnumbers(char * buf,  char * pcap , unsigned char maxlen);
extern unsigned char get_max_string_width( unsigned char *width_list,char ** str1, char ** str2, char ** str3 , char ** str4 );
extern unsigned char compare_special_numbers( char *numbers );
extern unsigned char set_voice_path_by_action(unsigned char byte_action);
extern unsigned char is_emergency_number(char * numbers);
extern unsigned char pre_process_numbers(char * numbers);
extern unsigned char is_international_numbers( char * numbers);
extern unsigned char bcd2d( unsigned char dat );
extern void app_dial_out(char * init_str);
extern void app_stop_call(void);
extern unsigned long get_talked_seconds(T_CALL_RECORD * pcall_record);
extern void adjust_speech_volume(void);
extern void adjust_ring_volume();
extern void talk_phone_call( T_CALL_RECORD * pcall_record ,char * that_name);
extern unsigned long money_str_to_digit( char * money_str );
extern void money_digit_to_asc_10( unsigned long d,char * money_str );
extern void money_digit_to_asc_10_ex( unsigned long d,char * money_str , unsigned char flag_fixed_pos);
extern void app_back_ground_rating();
extern unsigned char dialout_numbers( T_BOOK_ENTRY *book_entry );





extern unsigned short crc16_ccitt( const unsigned char * data,unsigned short data_len );
extern unsigned short crc16_ccitt_ex( const unsigned char * data,unsigned short data_len, unsigned short org_crc );
extern void print_hex_string(char * caption,unsigned char * pbuf, unsigned short buf_size);
extern void print_asc_string(char * caption,unsigned char * pbuf, unsigned short buf_size);
extern unsigned short get_short_value(unsigned char * pbuf);
extern unsigned long get_long_value(unsigned char * pbuf);	



extern unsigned char app_lock_phone_and_card();
extern void get_new_simpin_from_esn(char * new_pin);
extern unsigned char app_cancel_lock_phone_and_card();
extern unsigned char app_pass_lock_phone_and_card();
extern void app_do_unlock_phone_failed(unsigned char unlock_type);
extern void app_reg_cimis(char * str_cimi);
extern void app_show_cimis();
extern unsigned char app_check_soft_wake_up();




extern unsigned short recursion_gbpos(unsigned short code , unsigned short st, unsigned short et);
extern unsigned short recursion_unipos(unsigned short code , unsigned short st, unsigned short et);
extern unsigned short MultiByte_WideChar(unsigned short * pshort , char * pbuf);
extern unsigned short WideChar_MultiByte( char * pbuf, unsigned short * pshort);



extern unsigned char get_charset_cmp();
extern int wtoi(unsigned short * pshort);
extern unsigned short  wstrlen(unsigned short * pstr);
extern void wstrcpy(unsigned short * dest, unsigned short * src);
extern void wstrncpy(unsigned short * dest, unsigned short * src,unsigned short length);
extern char * w2a(char * dst, unsigned short * src);
extern unsigned short * a2w(unsigned short * dst, char * src);

extern unsigned short  recursion_uni_fontpos(unsigned short code , unsigned short st, unsigned short et);
extern unsigned char * get_uni_font(unsigned short code);





extern char *get_gbcode(char *strInput_py_str);
extern void clear_chs_rect();
extern void clear_bx_rect();
extern unsigned short make_lianxiang_5d(EDITWND * p_edit_wnd);
extern unsigned char make_spell_5d(EDITWND * p_edit_wnd,char * KeyBuf, char *spell_str ,char  org_char );









extern unsigned short build_crc_buffer(unsigned char cmd_type, unsigned char * pdst ,unsigned char * pdt, unsigned short length );

extern void set_handfree_led(unsigned char onoff);




//2018-05-06 new protocols without at commands;

extern char * andr_build_431_callout(char * str_cmd, char * phone_number);
extern char * andr_build_432_hook_onoff(char * str_cmd, unsigned char flag_accept);
extern char * andr_build_434_sendsms(char * str_cmd,char * phone_number, char * str_context);
extern char * andr_build_435_play_voice(char * str_cmd,int voc_id);
extern char * andr_build_436_stop_voice(char * str_cmd);
extern char * andr_build_437_talking_ctrl(char * str_cmd, char * ctrl_type, char cmd);
extern char * andr_build_438_voc_path(char * str_cmd,char vocpath);
extern char * andr_build_43A_voc_level(char * str_cmd,unsigned char voc_type, unsigned char voc_lvl);
extern char * andr_build_43B_shut_phone(char * str_cmd, unsigned char flag_reboot);
extern char * andr_build_43C_play(char * str_cmd, char dtmf);
extern char * andr_build_43D1_set_ring_music(char * str_cmd, int  voc_id);
extern char * andr_build_43D2_set_sms_music(char * str_cmd, char sms_voc);


extern char * andr_build_442_query_bluetooth_devices(char * str_cmd);
extern char * andr_build_443_query_wifi_devices(char * str_cmd);
extern char * andr_build_444_read_simbook_count(char * str_cmd);
extern char * andr_build_444_read_simbook_user(char * str_cmd, char * user_pos);
extern char * andr_build_445_read_simsms_count(char * str_cmd);
extern char * andr_build_445_read_simbook_user(char * str_cmd, char * sms_pos);
extern char * andr_build_446_query_call_waiting(char * str_cmd);
extern char * andr_build_446_setup_call_waiting(char * str_cmd, unsigned char flag_enable);
extern char * andr_build_447_query_call_divert(char * str_cmd);
extern char * andr_build_447_setup_call_divert(char * str_cmd,   char divert_type, unsigned char flag_enable, char * phone_number);
extern char * andr_build_448_query_simpin_enabled(char * str_cmd);
extern char * andr_build_448_setup_simpin_enabled(char * str_cmd,char * str_pin, unsigned char flag_enable);
extern char * andr_build_448_input_simpin(char * str_cmd, char * str_pin);
extern char * andr_build_448_input_simpuk(char * str_cmd, char * str_puk, char * str_new_pin);
extern char * andr_build_448_input_simpin2(char * str_cmd, char * str_pin2);
extern char * andr_build_448_input_simpuk2(char * str_cmd, char * str_puk2, char * str_new_pin2);
extern char * andr_build_448_change_pin(char * str_cmd, char * str_old_pin, char * str_new_pin);
extern char * andr_build_449_query_bluetooth_enabled(char * str_cmd);
extern char * andr_build_449_setup_bluetooth_enabled(char * str_cmd,unsigned char flag_enable);
extern char * andr_build_44A_query_wifi_enabled(char * str_cmd);
extern char * andr_build_44A_setup_wifi_enabled(char * str_cmd,unsigned char flag_enable);
extern char * andr_build_44B_get_imei(char * str_cmd);
extern char * andr_build_44C_get_imsi(char * str_cmd);
extern char * andr_build_44D_get_iccid(char * str_cmd);
extern char * andr_build_44E_get_csq(char * str_cmd);
extern char * andr_build_44F_get_andr_ver(char * str_cmd);
extern char * andr_build_44G_set_sleep(char * str_cmd, char sleep_type);



extern int  get_blue_wifi_list(char ** plist,  char * str_msg);
extern int  get_divert_list(T_CALL_DIVERT_STATUS * pdivt,  char * str_msg);
extern void analyse_android_message(FlSignalBuffer  * p_evt, char * str_msg, int msg_len);
extern int ex_atoi(unsigned char * p , int len);
extern unsigned char get_sms_from_msg(char * str_msg);
extern unsigned char get_book_from_msg(char * str_msg);

extern int UnicodeToUtf8(unsigned char* pInput, unsigned char *pOutput);  
extern int Utf8ToUnicode(unsigned char* pInput, unsigned char* pOutput); 






//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC



#define FLASH_CELL_LENGTH_1K   (1024)
#define FLASH_CELL_LENGTH_2K   (2048)


#define TEMP_FILES_CELL_LEN  (1024)




extern unsigned char  gpcommbuf[STBUF_MAX_SIZE];
extern unsigned short gpcommbuf_len;



extern unsigned char lcd_buf[SCREEN_WIDTH+1][8];
extern DATABASEINFO * g_table_info;
extern APPSYS appsys;
extern SYSPROP * sysprop;



extern unsigned  long  gsyn_seq;
extern unsigned char  g_ot_buffer[TFNET_FILE_SIZE];
extern int  g_ot_id_len[TFNET_ID_MAX];


extern DATABASEINFO g_db_info[DATABASEINFO_CNT];
extern unsigned char * prop_file;

extern unsigned char gbook_buf[HUGEBUF_SIZE];



extern const char soft_version[];
extern const char soft_ver[];
extern const char soft_time[];
extern const char phone_type[];
extern const char database_ver[];


extern const unsigned short FONT_TX_10_ARRAY[];
extern const unsigned char  FONT_TX_10_BIN_TXT[];
extern const unsigned short FONT_BX_10_ARRAY[];
extern const unsigned char  FONT_BX_10_BIN_TXT[];
extern const unsigned short FONT_BX_16_ARRAY[];
extern const unsigned char  FONT_BX_16_BIN_TXT[];

extern const unsigned short BITMAP_ARRAY[];
extern const unsigned char BITMAP_BIN_TXT[];
extern  const char hex_digits[];
extern  const char hex_dtmf[];
extern  const unsigned char y_dem_of_row[];
extern  const unsigned char backlight_delay[];
extern  const unsigned char main_bmp_menu[];
extern  const unsigned char main_bmp_menu_nocdma[];
extern  const unsigned long sand_glasses[];
extern  const unsigned char music_array[];
extern const unsigned char  alarm_array[];
extern const char  smsring_array[];

extern const unsigned char music_volume[];
extern const unsigned char sms_volume[];
extern const unsigned char speech_volume[];



extern unsigned char uni_buf[26];
extern unsigned short * ctw_buf;
extern unsigned char  * wtc_buf;
extern const unsigned char  py_bx_code[];








//#define DebugPrintf ALOGD

#define DebugPrintf(fmt, ...) ALOGD("[%s]  "fmt, app_get_datetime_str(), ##__VA_ARGS__)

#endif









