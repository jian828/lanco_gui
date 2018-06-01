
#include "inc/app_def.h"



const char * const * const shortcut_key_prompt[] =
{
	text_book,
	text_logs,
	text_sms,
	text_ring,
	text_callctrl,
};

const char * const * const xinqi_week[]=
{
	text_sun,
	text_mon,
	text_tue,
	text_wed,
	text_thu,
	text_fri,
	text_sat,
	text_sun,
};

const char * const * const   month_for_short[] =
{
	text_null,
	text_jan,
	text_feb,
	text_mar,
	text_apr,
	text_may,
	text_jun,
	text_jul,
	text_aug,
	text_sep,
	text_oct,
	text_nov,
	text_dec,
};

const char * const * const mem_simcard[]=
{
	text_simcard,
	text_simcard_locking,
	text_change_sim_pin,
};








const char * const * const  mem_emergency_num[] =
{
	text_emergency_number,
	text_number_1,
	text_number_2,
	text_number_3,

};






const char * const * const mem_dy_item[]=
{   
	text_choose_phrase,
	text_happy_birthday,
	text_happy_new_year,
	text_god_bless_you,
	text_thats_all_right,
	text_ill_be_there_right_away,
	text_see_you_at_the_same_place,
	text_wish_you_a_happy_family,
	text_have_a_pleasant_journey,
	text_how_are_you_going,
	text_dont_mention_it,
	text_where_are_you,
	text_missing_you,
	text_i_agree,
	text_i_dont_agree,
	text_congratulations,
	text_best_wishes,
	text_good_luck,
	text_call_me_while_free,
	text_im_sorry,
	text_i_love_you,
	text_thank_you,
};



const char * const * const mem_main[]=
{
	text_menu,
	text_phonebook,
	text_call_logs,
	text_messaging,
	text_alarm_clock,
	text_security,
	text_wifi,
	text_bluetooth,
	text_settings,
};



const char * const * const mem_main_pay[]=
{
	text_menu,
	text_security,
	text_settings,
};


const char * const * const mem_messages[]=
{  
	text_messaging,
	text_create_new_message,
	text_inbox,
	text_outbox,
	text_draft,
	text_message_totals,
	text_delete_all_sms,
	text_message_setting,
}; 


const char * const * const mem_del_all_sms[] =
{
	text_delete_sms,
	text_delete_inbox,
	text_delete_outbox,
	text_delete_all,
};

const char * const * const mem_book[]=
{
	text_phone_book,
	text_view,
	text_new_entry,
	text_check_memory,	
	text_clear_book,
	text_simcard,
	
};

const char * const * const mem_search_book_opt[] = 
{
	text_phone_book_options,
    text_edit_entry,
    text_new_entry,
	text_delete_entry,
	text_dial,
	text_capacity,
};



const char * const * const mem_view_call_log_opt[] =
{
	text_call_entry_options,
	text_delete,
	text_copy_to_phonebook,
	text_capacity,
};

const char * const * const mem_list_in_sms_opt[] =
{
	text_message_options,
	text_view,
	text_delete,
	text_details,
	text_call_back,
	text_capacity,
	text_simcard_sms,
};





const char * const * const mem_list_out_sms_opt[] =
{
	text_message_options,
	text_view,
	text_delete,
	text_details,
	text_capacity
};

const char * const * const mem_in_sm_detial_opt[] =
{
	text_message_options,
	text_reply,
	text_forward,
	text_delete,
	text_call_back,
	text_add_to_phonebook,	
	text_details,

};


const char * const * const mem_simbook_opt[] =
{
	text_simbook_options,
	text_call_back,
	text_add_to_phonebook,	
};

const char * const * const mem_out_sm_detial_opt[] =
{
	text_message_options,
	text_resend,
	text_forward,
	text_delete,
	text_details,
};

const char * const * const mem_draft_detial_opt[] =
{
	text_message_options,
	text_edit,
	text_delete,
	text_details,
};





const char * const * const mem_call_ctrl_settings[] =
{
	text_call_control,
	text_automatic_redial,
	text_call_waiting,
	text_call_forwarding,
	text_call_barring,
	text_dial_delay,
		
};

const char * const * const mem_call_barring_settings[] =
{
	text_call_barring,
	text_bar_all_outgoing,
	text_outgoing_international,
	text_all_incoming,
};

const char * const * const mem_del_all_call[] =
{
	text_delete_all_calls,
	text_delete_received_log,
	text_delete_dialed_log,
	text_delete_missed_log,
	text_delete_all_log,
};

const char * const * const mem_calls[]=
{ 
	text_call_logs,
	text_dialed_calls,
	text_received_calls,
	text_missed_calls,
	text_call_timer_totals,
	text_delete_all_calls,
};  

	const char * const * const mem_settings[]=
	{
		text_settings,
		text_display,
		text_date_and_time,
		text_voice_settings,
		text_call_control,
		text_software_version,
		text_factory_settings,
	};



const char * const * const mem_display_set[]=
{
	text_display,
	text_language,
	//text_backlight,
	text_sleep_setting,
};



const char * const * const mem_voice_set[]=
{
	text_voice_settings,
	text_incoming_ring,
	text_alarm_tone,
	text_new_message_tone,

    text_ring_volume,
    text_sms_volume,
    text_speech_volume,
 	
	text_keybeep_tones,
};







const char * const * const mem_bluetooth_setting[] =
{
	text_bluetooth_settings,
	text_bluetooth_switch,
	text_bluetooth_query,
};

const char * const * const mem_wifi_setting[] =
{
	text_wifi_settings,
	text_wifi_switch,
	text_wifi_query,
};




const char * const * const mem_time_and_date[] =
{
	text_time_and_date,
	text_set_date_and_time,
	text_time_format,
};

const char * const * const mem_time_format[] =
{
	text_time_format,
	text_am_pm,
	text_24_hour,
};

const char * const * const mem_date_format[] =
{
	text_date_format,
	text_mm_dd_yy,
	text_dd_mm_yy,
	text_yy_mm_dd,
};

const char * const * const mem_super_set[]=
{
	text_super_settings,
	text_software_version,
	text_security_settings,
	text_factory_settings,
	text_hardware_test,
};



const char * const * const mem_test[]=
{
	text_hardware_test,
	text_keypad_test,	
	text_handset_test,
	text_handfree_test,
	text_lcd_test,
	text_bat_test,
	text_expw_test,
	text_signal_test,
    text_echo_test,	
};



const char * const * const mem_security_set[]=
{
	text_security,
    text_lock_keypad,
	text_auto_lock_keypad,
	text_simcard_locking,
	text_change_sim_pin,
	text_phone_card_lock_mutual,
};

const char * const * const mem1_3_4_1[]=
{
	text_sms_operate,
	text_send_and_save,
	text_send,
	text_save,
	text_cancel,
};

const char * const * const mem_3_8_1[]=
{
	text_delete,
	text_resend,
	text_edit_and_resend,
};

const char * const * const mem_backlight[]=
{
	text_backlight,
	text_wait_10_seconds,
	text_wait_30_seconds,
	text_wait_120_seconds,
	text_always_on_if_ac,
	text_always_off,
};



const char * const * const mem_sleep_delay[]=
{
	text_sleep_setting,
	text_disable,
	text_10s,
	text_15s,
	text_30s,
	text_01m,
	text_03m,
	text_05m,
	text_10m,
    text_30m,
    text_01h, 
};


const char * const * const mem_call_delay[]=
{
    text_dial_delay,
	text_disable,
	text_wait_5s,
	text_wait_6s,
	text_wait_7s,
	text_wait_8s,	
};


const char * const * const call_diverting[] =
{
	text_call_forwarding,
	text_all_calls,
	text_if_busy,
	text_on_no_reply,
	text_if_not_reachable,
	text_cancel_all,
};
		

const char * const * const active_or_cancel[] =
{
	text_select,
	text_activate,
	text_cancel,
};

const char * const * const forbit_or_allow[] =
{
	text_select,
	text_forbit,
	text_allow,
};


const char * const * const alarm_clock_set[] =
{
	text_alarm_clock,
	text_close,
	text_once,
	text_everyday,
};

const char * const * const open_or_close[] =
{
	text_select,
	text_open,
	text_close,
};







const char * const * const mem_soft_keys[] =
{
	text_functions,
	text_search,
	text_call_logs,
	text_create_new_message,
	text_ring_tone,
	text_call_control,
};



