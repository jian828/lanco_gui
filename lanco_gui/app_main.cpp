

#include "inc/app_def.h"


unsigned char lcd_buf[SCREEN_WIDTH+1][8];  
unsigned char gpcommbuf[STBUF_MAX_SIZE];




unsigned char gbook_buf[HUGEBUF_SIZE];



unsigned  long  gsyn_seq;
unsigned  short gpcommbuf_len;


APPSYS    appsys;




const unsigned char music_array[]=
{
    FL_AUDIO_1,
   	FL_AUDIO_2,
   	FL_AUDIO_3,
   	FL_AUDIO_4,   
    FL_AUDIO_5,   
   	FL_AUDIO_6,
   	FL_AUDIO_7,
   	FL_AUDIO_8,
   	FL_AUDIO_9,
    FL_AUDIO_10,
};

const unsigned char alarm_array[]=
{
    FL_ALARM_1,
   	FL_ALARM_2,
   	FL_ALARM_3,
   	FL_ALARM_4,   
    FL_ALARM_5,   
};

const char smsring_array[]=
{
    FL_SMSRING_1,
   	FL_SMSRING_2,
   	FL_SMSRING_3,
   	FL_SMSRING_4,   
    FL_SMSRING_5,  
};



const unsigned char music_volume[]=
{
    1,
   	3,
   	4,
   	5,   
    7,   
};

const unsigned char sms_volume[]=
{
    1,
   	4,
   	7,
   11,   
   14,   
};

const unsigned char speech_volume[]=
{
    1,
   	2,
   	3,
   	4,   
    5,   
};




const unsigned char seg_num[] =
{
	BMP_SEG0_BIN,
	BMP_SEG1_BIN,
	BMP_SEG2_BIN,
	BMP_SEG3_BIN,
	BMP_SEG4_BIN,
	BMP_SEG5_BIN,
	BMP_SEG6_BIN,
	BMP_SEG7_BIN,
	BMP_SEG8_BIN,
	BMP_SEG9_BIN,
	BMP_SEGCOLON_BIN,
};


const unsigned char main_bmp_menu[] =
{
	BMP_TOOL1_BIN,
	BMP_TOOL2_BIN,
	BMP_TOOL3_BIN,
	BMP_TOOL4_BIN,
	BMP_TOOL5_BIN,
	BMP_TOOLB_BIN,
	BMP_TOOLC_BIN,
	BMP_TOOL6_BIN,
};

void app_check_valid_paras()
{
    if(sysprop->byte_ring_volume >4)
    {
        sysprop->byte_ring_volume =4;
	}

	if(sysprop->byte_sms_volume >4)
    {
        sysprop->byte_sms_volume =4;
	}

	if(sysprop->byte_talk_volume >4)
    {
        sysprop->byte_talk_volume =4;
	}

	if( sysprop->byte_income_ring >=10)
	{
        sysprop->byte_income_ring =1;
	}

	if(    sysprop->byte_sms_ring >=5)
	{
        sysprop->byte_sms_ring =1;
	}

	if( sysprop->struct_alarm.alarm_ring>=5)
	{
        sysprop->struct_alarm.alarm_ring =1;
	}
}




#define ASK_CSQ_INTERVAL (15)
#define ASK_REG_INTERVAL (2)
#define ASK_GPRS_INTERVAL (12)

#define DEFAULT_SCREEN_REFRESH_TIME_WFBLU 0x01
#define DEFAULT_SCREEN_REFRESH_NETWORK  0x02
#define DEFAULT_SCREEN_REFRESH_ALARM    0x04
#define DEFAULT_SCREEN_REFRESH_SOFTKEY  0x08
#define DEFAULT_SCREEN_REFRESH_BATARRY  0x10
#define DEFAULT_SCREEN_REFRESH_AC_POWER 0x20


void * app_main(void * p)
{
    char tmp_emergency[EMERGENCY_NUM_LEN];
    FlSignalBuffer EventPara;
	unsigned char ask_csq_inerval=0;
    unsigned char ask_reg_inerval=0;
 	unsigned char ask_gprs_interval=0;
	unsigned char ask_gprs_times=0;
	unsigned char old_minute=0;
	unsigned char em_numcnt=0;
    char input_code=0;
	
    ALOGD("--------------LANCO_GUI V1.005 2018-05-26-----------\r\n");

    app_initial_system();

	app_check_valid_paras();

    appsys.byte_reg_state = REG_STATE_NOT_REG;
	appsys.byte_net_state = NET_STATE_UNKNOWN;

    appsys.flag_sim_encypted =0;
    appsys.byte_power_service_delay=BAT_SERVICE_DELAY/2;
	appsys.byte_need_redraw  =   0xFF;

  
	wait_for_android_response(20000);
    mu_check_sim_valid(8000);
    mu_registering();

	if(0 == appsys.power_service.flag_extern_power)
	{
         appsys.power_service.dword_no_ext_power_st= app_get_tick_count();
	}


	

	mu_set_voice_path(VOICE_PATH_HANDFREE);

    while(1)
	{	
	    appsys.byte_need_exit=0;
	
	    if(appsys.byte_need_redraw>0)
		{
           if(0xFF == appsys.byte_need_redraw)
		   {
		       set_handfree_led(0);
               lcd_clear_screen();
		   }
		   
		    app_refresh_default_screen();   

		    appsys.byte_need_redraw =0;
		}

		
		if(1 == app_get_message( &EventPara) )
		{
	        if(EVENT_KEY == EventPara.eventTyp )
			{
				if(1 == EventPara.sig_p.key_evt.isPressed)
				{
                    if(1 == appsys.flag_keypad_locked)
                    {
                        unsigned char flag_need_show_err=1;
						
                        if(TFKEY_STAR == EventPara.sig_p.key_evt.key_val)
                        {
                            msg(get_multi_string((char * *)text_keypad_unlocked_welcome_to_use));
                            appsys.flag_keypad_locked=0;
							delay_ms(1500);
							flag_need_show_err =0;
						}
						else
						{
	                        input_code =get_key_value(EventPara.sig_p.key_evt.key_val);
							if(input_code !=0)
							{
	                            if(em_numcnt  <EMERGENCY_NUM_LEN-2)
	                            {
	                                unsigned char flag_is_emergency=1;
									
	                                tmp_emergency[ em_numcnt++]= input_code;
									tmp_emergency[ em_numcnt ] = 0;

									switch(em_numcnt)
									{
                                        case 1:
										{
                                            if('1' != tmp_emergency[0])
                                           	{
                                                flag_is_emergency =0;
											}
										}
										break;
										
										case 2:
                                            if('2' != tmp_emergency[1] &&  '1' != tmp_emergency[1] )
                                           	{
                                                flag_is_emergency =0;
											}
										break;
										
										case 3:
	                                        if( !((0 == memcmp(tmp_emergency, "110", 3))
	                                        ||    (0 == memcmp(tmp_emergency, "119", 3))
	                                        ||    (0 == memcmp(tmp_emergency, "120", 3)))
											)
	                                        {
	                                             flag_is_emergency =0;
	                                        }
										default:
											break;
									}
									
									if( 1 == flag_is_emergency)
									{
									    if(em_numcnt >=3 )
									    {
								            if(HOOK_STATE_ONHOOK == appsys.flag_hook_state)appsys.flag_hand_free =1;
											appsys.flag_fast_dial=1;
											app_dial_out(tmp_emergency);
											appsys.flag_fast_dial=0;
											appsys.byte_need_redraw=0xFF;
											memset(tmp_emergency,0,sizeof(tmp_emergency));
											em_numcnt = 0;
									    }
										flag_need_show_err =0;
									}
								}
							}
						}

						if(1 == flag_need_show_err)
						{
						    memset(tmp_emergency,0,sizeof(tmp_emergency));
							em_numcnt = 0;
							appsys.flag_wish_input_star=1;
			                msg_warn(get_multi_string((char * *)text_keypad_locked_please_enter_star_to_unlock));
						    delay_ms(1500);
							appsys.flag_wish_input_star=0;
						}
						
						appsys.byte_need_redraw=0xFF;
					}


					else if(TFKEY_SMS==  EventPara.sig_p.key_evt.key_val)
					{
                        app_run_messages();
					}	
				    else if(TFKEY_MUTE==  EventPara.sig_p.key_evt.key_val)
				    {
                        if(0 == appsys.flag_ring_mute)
                        {
                             app_set_voice_volumn(VOCTYPE_INCOMING_CALL, 0);
                 		     app_set_voice_volumn(VOCTYPE_SMS_INCOMING, 0);
							 appsys.flag_ring_mute=1;
						}
						else
						{
                             app_set_voice_volumn(VOCTYPE_INCOMING_CALL, music_volume[sysprop->byte_ring_volume]);
                 		     app_set_voice_volumn(VOCTYPE_SMS_INCOMING,  music_volume[sysprop->byte_sms_volume]);
							 appsys.flag_ring_mute=0;
						}

						appsys.byte_need_redraw |= DEFAULT_SCREEN_REFRESH_SOFTKEY;
					}
				    else if(TFKEY_ALARM==  EventPara.sig_p.key_evt.key_val)
					{
                        app_set_alarm_clock();
						appsys.byte_need_redraw=0xFF;
					}
				 	else if(TFKEY_MUSIC==  EventPara.sig_p.key_evt.key_val)
					{
                        app_select_ring_music();
						appsys.byte_need_redraw=0xFF;
					}

				 	else if(TFKEY_REDIAL== EventPara.sig_p.key_evt.key_val)
					{
				        if(strlen(trim(appsys.str_dialed_numbers))>0)
				        {
				            if(HOOK_STATE_ONHOOK == appsys.flag_hook_state)appsys.flag_hand_free =1;
							appsys.flag_fast_dial=1;
							app_dial_out(appsys.str_dialed_numbers);
							appsys.flag_fast_dial=0;
							appsys.byte_need_redraw=0xFF;
				        }
						else
						{
                            T_CALL_RECORD call_record;
							memset(&call_record,0,sizeof(call_record));
							if(1 == app_record_read(TABLEINFO_CALL_OUT, 1, (unsigned char *)&call_record))
							{
							    if(strlen(trim(call_record.num)) >0)
							    {
							        if(HOOK_STATE_ONHOOK == appsys.flag_hook_state)appsys.flag_hand_free =1;
									appsys.flag_fast_dial=1;
									app_dial_out(call_record.num);
									appsys.flag_fast_dial=0;
									appsys.byte_need_redraw=0xFF;
							    }
							}
						}
					}						
					else if(TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val  
					|| TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val
					|| TFKEY_CONFIRM== EventPara.sig_p.key_evt.key_val
					 )
					{
                        process_soft_key_on_def_screen(EventPara.sig_p.key_evt.key_val);
						appsys.byte_need_redraw=0xFF;
					}
					else if(TFKEY_HANDFREE== EventPara.sig_p.key_evt.key_val)
					{

					}
					else if(TFKEY_SEND == EventPara.sig_p.key_evt.key_val)
					{
                        app_records_show(TABLEINFO_CALL_OUT, get_multi_string((char * *)text_dialed_calls) );
					}
                    else if((TFKEY_UP==EventPara.sig_p.key_evt.key_val)|| ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
                     ||(TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)  || (TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val)
					 )
                    {
						 switch(EventPara.sig_p.key_evt.key_val)
						 {
                             case TFKEY_UP:
							 {
                                 app_run_books();
							 }
							 break;

							 case TFKEY_LEFT:
							 {
                                 app_records_show(TABLEINFO_SMS_INBOX, get_multi_string((char * *)text_inbox_list));
							 }
							 break;

							 case TFKEY_DOWN:
							 {
                                 app_run_recent_calls();
							 }
							 break;

							 case TFKEY_RIGHT:
							 {
				                T_SMS_RECORD sms_record;
								memset(&sms_record,0, sizeof(T_SMS_RECORD));
								edit_sms_message(&sms_record);
			            	 }
							 break;

							 default:
							 break;
						 }
						 appsys.byte_need_redraw=0xFF;
					}
					else 
					{
                        input_code =get_key_value(EventPara.sig_p.key_evt.key_val);
						if(input_code !=0)
						{
						     unsigned long st_tick=app_get_tick_count();
							 unsigned char flag_special=0;

							 appsys.byte_input_key_when_delay =0;
							 
							 if('0' == input_code)
							 {
                                 flag_special=0; delay_ms(90);
							 }
							 else
							 {
	                             while(KEY_STATE_PRESSED == appsys.flag_key_state)
	                             {
	                                 delay_ms(100);
								 	 if(get_duration_tick(st_tick)> 1500)
									 {
		                                 flag_special=1;
		                                 if(1 ==appsys.flag_genie_trace)DebugPrintf("special functions\r\n",NULL);
                                         break;
									 }
									 
								     if(appsys.byte_input_key_when_delay >0)
									 {
                                         break;
									 }
								 }
							 }

							 if(0 == flag_special)
							 {
                                  char init_str[8];
								  memset(init_str,0,sizeof(init_str));
								  init_str[0]=input_code;
								  init_str[1]=appsys.byte_input_key_when_delay;
								  
								  appsys.flag_hand_free =1; // press any dtmf key as handfree
								  
							      if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
								  {
								      appsys.flag_pre_dial =1;
                                      app_dial_out(init_str);
									  appsys.flag_pre_dial =0;
								  }
							 }
							 else // fast dial
							 {
                                   unsigned char rec_pos=0;
								   T_BOOK_ENTRY temp_entry;
								   char tmp_buf[1+1];
								   memset(&temp_entry, 0,sizeof(T_BOOK_ENTRY));
								   tmp_buf[0]=input_code ;
								   tmp_buf[1]=0;
								   rec_pos= find_book_entry_by_speed_location( tmp_buf, &temp_entry);
								   if(rec_pos>0)
								   {
								        if(HOOK_STATE_ONHOOK == appsys.flag_hook_state)appsys.flag_hand_free =1;
								        appsys.flag_fast_dial=1;
                                        app_dial_out(temp_entry.num);
										appsys.flag_fast_dial=0;
								   }
							 }
						}
					}
				}
	        }
			else if(EVENT_MODULE== EventPara.eventTyp )
			{
                //don't know why come this automatically, maybe some error
			}
			else if(EVENT_TIMER== EventPara.eventTyp )
			{

                if(1 == appsys.flag_got_424)
                {
                    appsys.flag_got_424=0;
					appsys.byte_need_redraw |= DEFAULT_SCREEN_REFRESH_BATARRY;
				}

				
			
                if(     (0 == appsys.flag_showing_lowbat)  
					&& !((   (0 == appsys.flag_nouim_emergency_call)
                           &&(1 == sysprop->flag_encyped_only)
                           &&(0 == appsys.flag_sim_encypted))
				        )
				)
                {
				    process_auto_redraw();// half second redraw
                }

				if(appsys.flag_time_changed >0)
				{  
				    appsys.flag_time_changed=0;

                    if(em_numcnt >0)
                    {
                        if(get_duration_tick(appsys.dword_last_input_tick) >1800)
                        {					
						    memset(tmp_emergency,0,sizeof(tmp_emergency));
							em_numcnt = 0;
						}
					}

					
				    if(old_minute !=appsys.struct_time.min)
					{
                        appsys.byte_need_redraw |=DEFAULT_SCREEN_REFRESH_TIME_WFBLU;
						
                        old_minute=appsys.struct_time.min;
						if( (old_minute%10) ==0 && 1 == sysprop->flag_lock_base)
						{
						   //get lock base info
						}
					}

					if(0 == appsys.flag_is_sleeping)
					{
						if(++ask_csq_inerval>=ASK_CSQ_INTERVAL)
						{
						     unsigned short old_csq=appsys.byte_csq_level;
							 ask_csq_inerval=0;

							 
	                       //  mu_get_csq();

							 
							 if(old_csq !=appsys.byte_csq_level )
							 {
	                             appsys.byte_need_redraw |=DEFAULT_SCREEN_REFRESH_NETWORK;
								 if(1 ==appsys.flag_genie_trace)DebugPrintf("csq status changed\r\n",NULL);
							 }
						}
					}

					if(++ask_reg_inerval>=ASK_REG_INTERVAL)
					{
					     ask_reg_inerval=0;
#if 1
					     if( 1 == mu_reg_changed())
					     { 
							  appsys.byte_need_redraw =0xFF;
					     }
#endif
					}
	

					if(appsys.power_service.gui_flag_extern_power !=  appsys.power_service.flag_extern_power)
					{
						appsys.byte_need_redraw |= DEFAULT_SCREEN_REFRESH_AC_POWER;
					}

					if(appsys.power_service.gui_flag_having_battery!=  appsys.power_service.flag_having_battery)
					{
                        appsys.byte_need_redraw |= DEFAULT_SCREEN_REFRESH_BATARRY;
					}

                    if( (0 == appsys.power_service.flag_extern_power)  &&  (1 == appsys.power_service.flag_having_battery))
                    {
						if(appsys.power_service.gui_byte_battery_lvl!= get_battery_lvl())
						{
							 appsys.byte_need_redraw |= DEFAULT_SCREEN_REFRESH_BATARRY;
						}
                    }

					if(1 == sysprop->flag_auto_redial  && (0 == appsys.flag_nouim_emergency_call))
					{
                        if(appsys.byte_redial_cnt >0)
                        {
                            if( get_duration_tick(sysprop->auto_redial.redial_tick)/1000 > sysprop->auto_redial.redial_interval)
                            {
                                if(strlen(sysprop->auto_redial.num) >0)
                                {
                                    if(HOOK_STATE_ONHOOK == appsys.flag_hook_state)appsys.flag_hand_free =1;
									appsys.flag_doing_auto_redial=1;
	  						        appsys.flag_fast_dial=1;
	                                app_dial_out(sysprop->auto_redial.num);
									appsys.flag_fast_dial=0;  
									appsys.flag_doing_auto_redial=0;
									sysprop->auto_redial.redial_tick= app_get_tick_count();
									
									if(appsys.byte_redial_cnt>0 )appsys.byte_redial_cnt--;
                                }
								else
								{
                                    appsys.byte_redial_cnt=0;
								}
							}
						}
					}
				}
			}
		}
    }
	return 0;
}


void process_alarm_ring()
{
    FlSignalBuffer EventPara;
    unsigned long st_time= app_get_tick_count();
	unsigned char tmp_cnt=0;


    app_play_alarm_ring(sysprop->struct_alarm.alarm_ring, 60);

	
	appsys.byte_need_redraw=0xFF;
	appsys.flag_alarm_ringing=1;
	appsys.dword_last_clock_alarm_tick = app_get_tick_count();

	
    lcd_backlight_on();
	
	if(1 == sysprop->struct_alarm.alarm_on)
	{
        sysprop->struct_alarm.alarm_on =0;   
		app_save_prop();
	}

	while(1)
	{
         if(appsys.byte_need_redraw>0)
         {
             if(0xFF == appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
			     show_caption(get_multi_string((char * *) text_scheduled_time));
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);
				 lcd_goto_xy(14, 13);
                 show_bitmap(BMP_ALARM_CLOCK_BIN);
				 lcd_goto_xy(72, 13);
				 show_bitmap(BMP_ALARM_CLOCK_BIN);
				 
                 show_soft_key_prompt(get_multi_string((char * *) text_delay), get_multi_string((char * *) text_end));
			 }
             // 15,3
             {
                unsigned char x1,y1,x2,y2;
				x1=14+15;
				y1=13+2;
				x2=72+15;
				y2=13+2;
				lcd_goto_xy(x1,  y1);
				lcd_clear_line_ex(8, x1+11);
				lcd_goto_xy(x2,  y2);
				lcd_clear_line_ex(8, x2+11);
					
				if(0 == tmp_cnt)
                {
				    lcd_goto_xy(x1+3,  y1);
					show_bitmap(BMP_MAIN_ALARM_BIN);
				  	lcd_goto_xy(x2+3,  y2);
					appsys.flag_color_reverse=1;
					show_bitmap(BMP_MAIN_ALARM_BIN);
					appsys.flag_color_reverse=0;
	                tmp_cnt=1;
                }
				else
				{
				    lcd_goto_xy(x1+3,  y1);
				    appsys.flag_color_reverse=1;
					show_bitmap(BMP_MAIN_ALARM_BIN);
					appsys.flag_color_reverse=0;
				  	lcd_goto_xy(x2+3,  y2);
					show_bitmap(BMP_MAIN_ALARM_BIN);
					tmp_cnt=0;
				}
			 }
			 appsys.byte_need_redraw=0;
		 }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {	
				     if(1 ==EventPara.sig_p.key_evt.isPressed)
				     {
				          if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				          {
				               appsys.flag_need_clock_alarm=0;
							   appsys.byte_alarm_clock_hour=appsys.byte_alarm_clock_min=0xFF;
                               break;
						  }
				          if( (TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val) ||(TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
				          {
								appsys.byte_alarm_clock_min+=10;
								
								if(appsys.byte_alarm_clock_min>59)
								{
                                    appsys.byte_alarm_clock_min-=60;
									appsys.byte_alarm_clock_hour++;
									
									if(appsys.byte_alarm_clock_hour>23)
									{
                                        appsys.byte_alarm_clock_hour =0;
									}
								}
								appsys.flag_need_clock_alarm=1;
                                break;
						  }
				     }
		         }
				 else if(EVENT_TIMER == EventPara.eventTyp )
				 {
				     if( appsys.flag_half_second>0)
				     {
				         appsys.flag_half_second=0;
				         appsys.byte_need_redraw=1;
				     }
                     if(get_duration_tick(st_time)>(60 *1000))
                     {
                          appsys.flag_need_clock_alarm=0;
                          appsys.byte_alarm_clock_hour=appsys.byte_alarm_clock_min=0xFF;
                          break;
                     }
				 }
		     }
		 }
	}
	
    app_stop_music();
	
	app_save_prop();
	appsys.byte_need_redraw=0xFF;
	appsys.flag_alarm_ringing=0;
}

void show_time_colon(unsigned char flag_show)
{
	unsigned char x_colon=0;
	unsigned char y_delta=5;
    unsigned char y_lvl=34 - y_delta;

	flag_show=(0==flag_show)?1:0;

    if ( CLOCK_FORMAT_24 ==  sysprop->flag_clock_format)
    {
		x_colon=61;
    }
    else
    {
        unsigned char temp_hour= appsys.struct_time.hour;

        if ( temp_hour > 12 )
        {
            temp_hour -=12;
        }
		else if(0 == temp_hour)
		{
            temp_hour =12;
		}
	
        if( (temp_hour /10) >0)
        {
            x_colon=59;
		}
		else
		{
            x_colon=55;
		}
    }
	lcd_goto_xy(x_colon, y_lvl);

	
	
	if(1== flag_show)
	{
	     show_bitmap(seg_num[10]);
	}
	else
	{
		 lcd_fill_area(0x00, x_colon,y_lvl, 7, 19);
	}
}

void process_auto_redraw()
{
    if(appsys.flag_half_second >0 )
    {
        static unsigned char flag_show=0;
		unsigned char x_colon=0;
		unsigned char y_delta=5;
        appsys.flag_half_second=0;

		flag_show=(0==flag_show)?1:0;


         show_time_colon(flag_show);
  

		if(1 == appsys.flag_having_missed_call)
		{
			if(1== flag_show)
			{
			    lcd_goto_xy(SCREEN_WIDTH-17, 44- y_delta);
			    show_bitmap(BMP_MAIN_CALLIN_BIN);
			}
			else
			{
                lcd_fill_area(0x00, SCREEN_WIDTH-17,44-y_delta, 16, 10);
			}
		}
		if(1 == appsys.flag_having_unread_sms)
		{	
		    if(1== flag_show)
			{
			    lcd_goto_xy(SCREEN_WIDTH-16, 24+9 - y_delta);
			    show_bitmap(BMP_MAIN_MAILBOX_BIN);
			}
			else
			{
                lcd_fill_area(0x00, SCREEN_WIDTH-16,24+9 -y_delta, 16, 10);
			}			
		}



    	if(1 == appsys.flag_keypad_locked)
		{
			if(1== flag_show)
			{
		         lcd_goto_xy(0, 24+9-y_delta);
			     show_bitmap(BMP_MAIN_KEYLOCK_BIN);
			}
			else
			{
                lcd_fill_area(0x00, 0 ,24+9-y_delta, 16, 8);
			}				 
		}
		

		if( sysprop->struct_alarm.alarm_on >0)
		{
			if(1== flag_show)
			{
		         lcd_goto_xy(0, 45-y_delta);
			     show_bitmap(BMP_MAIN_ALARM_BIN);
			}
			else
			{
                lcd_fill_area(0x00, 0 ,45-y_delta, 7, 8);
			}	
		}

        if((1 == appsys.power_service.flag_extern_power) && (1 == appsys.power_service.flag_having_battery))
        {
             if( 0 == appsys.power_service.flag_battery_full)
             {
                 app_show_battery(1);
			 }
		}
	}
}

void app_show_battery(unsigned char flag_cycle)
{
    static unsigned char fake_gade=0;
    unsigned char bat_x,bat_y;
	unsigned char cur_grade=0;
	unsigned char i=0;
	
	bat_y=0;
	bat_x=SCREEN_WIDTH-22;
    lcd_goto_xy(bat_x ,bat_y);


    appsys.power_service.gui_byte_battery_lvl= get_battery_lvl();

    show_bitmap(BMP_BATTARY_BIN);
    if(1 != appsys.power_service.flag_extern_power)
    {
        lcd_goto_xy(bat_x ,bat_y);
		lcd_clear_line_ex(10, bat_x+5);
	}

	
    lcd_goto_xy(bat_x+2+6, bat_y+3);
	lcd_clear_line_ex(4, bat_x+13+6);
	
	if(1 != appsys.power_service.flag_having_battery)
	{
        unsigned char tmp_bmp[5];
		tmp_bmp[0]=0x11 <<3;
		tmp_bmp[1]=0x0A <<3;
		tmp_bmp[2]=0x04 <<3;
		tmp_bmp[3]=0x0A <<3;
		tmp_bmp[4]=0x11 <<3; 

		lcd_goto_xy(bat_x +6 +5 , bat_y+3);
		lcd_display_data(5, 5, tmp_bmp);
		
		return;	    
	}

	

	if(0 ==flag_cycle)
	{
        fake_gade = ( (1 == appsys.power_service.flag_battery_full) || (appsys.power_service.dword_charge_tot_time >BAT_CHARGE_MAX_UI_TIME) )?4: appsys.power_service.gui_byte_battery_lvl;
	}
	else
	{
	    if(appsys.power_service.dword_charge_tot_time <BAT_CHARGE_MAX_UI_TIME)
	    {
            fake_gade= (fake_gade<cur_grade)?cur_grade:(fake_gade+0);
	    }
		else
		{
            fake_gade =4;
		}
	}

    for(i=0;i< fake_gade;i++)
    {
        lcd_goto_xy(bat_x +6 +3*(i+1)-1 , bat_y+3);
        lcd_draw_vertical(0xFF, 4);
		lcd_add_x(1);
		lcd_draw_vertical(0xFF, 4);
		
	}
	if(1 ==flag_cycle)
	{
		// battery have 4 grade
		if(fake_gade <4)
		{
	        fake_gade++;
		}
		else
		{
	        fake_gade= cur_grade;
		}
	}
}



void process_soft_key_on_def_screen(unsigned char soft_key)
{ 
   if(TFKEY_SOFT_LEFT == soft_key || (TFKEY_CONFIRM == soft_key))
   {
        run_main_menu();
   }
   else if(TFKEY_SOFT_RIGHT == soft_key)
   {
        app_run_books();
   }
}



void show_soft_key_prompt(  char *left,  char *right )
{
    appsys.byte_font_type= FONT_B_X_10;
	
	lcd_goto_xy(0, 52);
	lcd_clear_line(SCREEN_WIDTH-1);

    if(left != get_multi_string((char * *)text_null))
 	{
	    lcd_goto_xy(0, 52);
	    lcd_put_string( left);
    }

    if(right !=get_multi_string((char * *)text_null))
    {
	    lcd_goto_xy(SCREEN_WIDTH - get_str_dots_width(right), 52);
		lcd_put_string(right);
    }
	appsys.byte_font_type= FONT_T_X_10;
}


void show_network_prompt( void )
{
    unsigned char tmp_pos=0;
	unsigned char fak_val=0;
	unsigned short bmp_para=0;
	unsigned char x0= 0;
	unsigned char y0= 0;

    char tmp_oper[32];
	
    lcd_goto_xy(x0, y0);
	
    if(1)//show singal anyway
    { 
        fak_val =convert_csq_level_grade(appsys.byte_csq_level);

		//if(fak_val<3){fak_val=3;} this would be a pretty joke

	    tmp_pos=13 + fak_val*2;


        switch(appsys.byte_net_state)
        {
             case 1:
			 	 bmp_para = show_bitmap(BMP_MAIN_ANTENNA_2G_BIN);
				 break;
			 case 2:
			 	bmp_para = show_bitmap(BMP_MAIN_ANTENNA_3G_BIN);
				 break;
			 case 3:
			 	bmp_para = show_bitmap(BMP_MAIN_ANTENNA_4G_BIN);
				 break;

				 
			 case 0:
			 default:
			 	 bmp_para = show_bitmap(BMP_MAIN_ANTENNA_UNKN_BIN);
			 	break;
		}
     
       

	    lcd_goto_xy(x0+ tmp_pos, y0);
		tmp_pos=(unsigned char)( (bmp_para&0xFF00)>>8 ) +x0;// clear to 
		lcd_clear_line_ex( (unsigned char)(bmp_para&0x00FF) , tmp_pos);

    }

    memset(tmp_oper,0,sizeof(tmp_oper));
	
	if(0 == appsys.flag_nouim_emergency_call)
	{
	     if(strcmp(appsys.str_operator_name,"null")==0 )
	     {
 
		 }
		 else
		 {
		     if(appsys.byte_csq_level == 0)
		     {

		     }
             strcpy(tmp_oper, appsys.str_operator_name);		
		 }
	}
	else
	{
	     strcpy(tmp_oper,  get_multi_string((char * *) text_insert_simcard));
	}

    appsys.byte_font_type = FONT_B_X_10;
    tmp_pos=(SCREEN_WIDTH - get_str_dots_width(tmp_oper))/2;
    lcd_goto_xy(tmp_pos, 0);
	lcd_put_string(tmp_oper);

}

void show_seg_clock()
{
    unsigned char y_delta=5;
    unsigned y_lvl=34-y_delta;
    if ( CLOCK_FORMAT_24 ==  sysprop->flag_clock_format)
    {
        lcd_goto_xy(61,y_lvl);
		show_bitmap(seg_num[10]);

		lcd_goto_xy(42,y_lvl);
		show_bitmap(seg_num[ appsys.struct_time.hour /10 ]);
		lcd_goto_xy(52,y_lvl);
		show_bitmap(seg_num[ appsys.struct_time.hour %10 ]);
		lcd_goto_xy(68,y_lvl);
		show_bitmap(seg_num[ appsys.struct_time.min/10 ]);
		lcd_goto_xy(78,y_lvl);
		show_bitmap(seg_num[ appsys.struct_time.min%10 ]);
    }
    else
    {
        unsigned char temp_hour= appsys.struct_time.hour;

        if ( temp_hour > 12 )
        {
            temp_hour -=12;
        }
		else if(0 == temp_hour)
		{
            temp_hour =12;
		}

        lcd_goto_xy(40,y_lvl);
        lcd_clear_line_ex(19, SCREEN_WIDTH -18);
      
        if ( temp_hour / 10 )
        {
            lcd_goto_xy(40,y_lvl);
            show_bitmap(seg_num[ temp_hour /10 ]);

	        lcd_goto_xy(50,y_lvl);
			show_bitmap(seg_num[ temp_hour %10 ]);
	        lcd_goto_xy(66,y_lvl);
			show_bitmap(seg_num[ appsys.struct_time.min/10 ]);
	        lcd_goto_xy(76,y_lvl);
			show_bitmap(seg_num[ appsys.struct_time.min%10 ]);


	        lcd_goto_xy(86,43-y_delta);

	        if ( (temp_hour=appsys.struct_time.hour) >= 12 )
	        {
				show_bitmap(BMP_TIME_P_BIN);
	        }
	        else
	        {
				show_bitmap(BMP_TIME_A_BIN);
	        }
        }
        else
        {
	        lcd_goto_xy(46,y_lvl);
			show_bitmap(seg_num[ temp_hour %10 ]);
	        lcd_goto_xy(62,y_lvl);
			show_bitmap(seg_num[ appsys.struct_time.min/10 ]);
	        lcd_goto_xy(72,y_lvl);
			show_bitmap(seg_num[ appsys.struct_time.min%10 ]);


	        lcd_goto_xy(82,43);

	        if ( (temp_hour=appsys.struct_time.hour) >= 12 )
	        {
				show_bitmap(BMP_TIME_P_BIN);
	        }
	        else
	        {
				show_bitmap(BMP_TIME_A_BIN);
	        }			
        }
    }
}




void show_clock_time( void )
{
    char tmp_buf[32];
    appsys.byte_font_type= FONT_T_X_10;
    appsys.struct_date.week = GetWeekInfo();

    lcd_goto_xy(0,14);
	lcd_clear_line_ex(11, SCREEN_WIDTH-1);

    memset(tmp_buf,0,sizeof(tmp_buf));
	
    if( LANGUAGE_ENGLISH == sysprop->byte_language_type  || LANGUAGE_SPANISH== sysprop->byte_language_type)
    {
		sprintf(tmp_buf,"%.4d %s %.2d %s",appsys.struct_date.year, get_multi_string_ex((char * * *)month_for_short, appsys.struct_date.mon), appsys.struct_date.day , get_multi_string_ex((char * * *)xinqi_week, appsys.struct_date.week));
    }
	else
	{
		sprintf(tmp_buf,"%.4d-%d-%d %s",appsys.struct_date.year,appsys.struct_date.mon,appsys.struct_date.day, get_multi_string_ex((char * * *)xinqi_week, appsys.struct_date.week));
	}

    lcd_goto_xy((SCREEN_WIDTH - get_str_dots_width(tmp_buf)) / 2, 14);
    lcd_put_string(tmp_buf);

    if(1 ==sysprop->flag_wifi_enabled)
    {
	    lcd_goto_xy(0,14);
		show_bitmap(BMP_MAIN_WIFI_BIN);
    }

    if(1 ==sysprop->flag_blue_enabled)
    {
	    lcd_goto_xy(SCREEN_WIDTH-1 - 16 ,14);
		show_bitmap(BMP_MAIN_BLUETOOTH_BIN);
    }

	
    show_seg_clock();

}


char *  get_multi_string(char ** p_str)
{
	return (char *)(p_str[sysprop->byte_language_type]);
}

char *  get_multi_string_ex(char *** p_str, unsigned char line)
{
    return (char *)(p_str[line][sysprop->byte_language_type]);
}




void app_refresh_default_screen( void )
{
    unsigned char temp_val=0;
	unsigned char y_delta=5;
    char tmp_buf[16];
	
    if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_NETWORK)>0)
    {
        show_network_prompt();
    }
    if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_BATARRY)>0)
    {
        app_show_battery(0);
	}	
    if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_TIME_WFBLU)>0)
    {
        show_clock_time();
    }

	
	if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_AC_POWER)>0)
	{
        if(0 == appsys.power_service.flag_extern_power && ( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_BATARRY)==0) )
        {
             app_show_battery(0);
		}
	}


		
	
    if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_ALARM)>0)
    {
	    if(sysprop->struct_alarm.alarm_on >0  ||  1 == appsys.flag_need_clock_alarm)
	    {
			appsys.byte_font_type =FONT_T_X_10;
			memset(tmp_buf,0,16);

			if(0 == appsys.flag_need_clock_alarm  )
			{
		        if ( CLOCK_FORMAT_12 ==  sysprop->flag_clock_format )
		        {
					get_alarm_time_str(&(sysprop->struct_alarm.alarm_time), tmp_buf);
				}
		        else
		        {
		 		    sprintf(tmp_buf,"%.2d:%.2d", sysprop->struct_alarm.alarm_time.hour,sysprop->struct_alarm.alarm_time.min);
		        }
			}
			else
			{
		        if ( CLOCK_FORMAT_12 ==  sysprop->flag_clock_format )
		        {
		            TIME tmp_tm;
					tmp_tm.hour = appsys.byte_alarm_clock_hour;
					tmp_tm.min  = appsys.byte_alarm_clock_min;
					tmp_tm.sec  =0;
					get_alarm_time_str(&tmp_tm, tmp_buf);
				}
		        else
		        {
		 		    sprintf(tmp_buf,"%.2d:%.2d", appsys.byte_alarm_clock_hour,appsys.byte_alarm_clock_min);
		        }
			}
			
			lcd_goto_xy(0, 45-y_delta);
			show_bitmap(BMP_MAIN_ALARM_BIN);
			lcd_goto_xy(7, 44-y_delta);
			lcd_put_string(tmp_buf);

		}
    }




	
    if( (appsys.byte_need_redraw & DEFAULT_SCREEN_REFRESH_SOFTKEY)>0)
    {
	    appsys.byte_font_type= FONT_B_X_10;

        lcd_goto_xy(0, 54);
		lcd_clear_line(SCREEN_WIDTH-1);		
	    lcd_goto_xy(0, 54);

        show_soft_key_prompt(get_multi_string((char **) text_menu), get_multi_string((char **) text_book));

        if(1 == appsys.flag_ring_mute)
        {
		    lcd_goto_xy((SCREEN_WIDTH-12)/2, 54);
	        show_bitmap(BMP_MAIN_MUTE_BIN);
        }
    }




}


void run_main_menu( void )
{
    unsigned char cursor_pos=1;


    unsigned char * pbmp_menu =(unsigned char *)main_bmp_menu;
    const char * const * const*   ppmem_main =mem_main;

    while(0 == appsys.byte_need_exit)
	{
	    switch ( (cursor_pos = app_graph_meme(pbmp_menu, (char ***)ppmem_main,8, cursor_pos)) )
	    {
		    case 1:
	            app_run_books();
	            break;
	        case 2:
	            app_run_recent_calls();
	            break;
	        case 3:
	            app_run_messages();
	            break;
	        case 4:
	            app_set_alarm_clock();
	            break;
	        case 5:
	            app_run_security_settings();
	            break;
			case 6:
                 app_wifi_setting();
			    break;	
			case 7:
				 app_bluetooth_setting();
				break;
	        case 8:
	            app_run_all_settings();  
	            break;
	
			case 0:
			default:
				appsys.byte_need_redraw=0xFF;
				return;
				break;
	    } 
    }
    appsys.byte_need_redraw=0xFF;

    
}
