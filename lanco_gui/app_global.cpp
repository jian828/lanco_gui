
#include "inc/app_def.h"
#include "inc/KeyInput.h"
#include "inc/DataFile.h"



#define __SYN_SEQ__  (unsigned char)((gsyn_seq++)%15 +1)



extern CSystemUIManager* pUI;



unsigned long min_heap=MAX_INT32_FFFFFFFF;
unsigned long min_stck=MAX_INT32_FFFFFFFF;


unsigned long max_heap=MAX_INT32_FFFFFFFF;
unsigned long max_stck=MAX_INT32_FFFFFFFF;

void app_test_heap()
{
    char * pheap= (char *)app_alloc_memory(2);
	unsigned long bb=0;

    if(MAX_INT32_FFFFFFFF == min_heap)
    {
        min_heap= max_heap = (unsigned long)pheap;
	}
	else
	{
        if(min_heap > (unsigned long)pheap)
        {
             min_heap = (unsigned long)pheap;
		}

		if(max_heap < (unsigned long)pheap)
		{
             max_heap = (unsigned long)pheap;
		}
	}

	if(MAX_INT32_FFFFFFFF == min_stck)
	{
        min_stck=max_stck = (unsigned long)&bb;
	}
	else
	{
        if(min_stck > (unsigned long)&bb)
        {
             min_stck = (unsigned long)&bb;
		}

		if(max_stck < (unsigned long)&bb)
		{
             max_stck = (unsigned long)&bb;
		}

	}


	
	DebugPrintf("------>heap address=0x%.8X stk address=0x%.08X  ,DLT heap=  0X%.8X/0X%.8X , DLT stck=0X%.8X/0X%.8X\r\n ", (unsigned long)pheap,  (unsigned long)&bb,  (unsigned long)pheap- min_heap, max_heap-min_heap, (unsigned long)&bb-min_stck, max_stck-min_stck);



	
	app_free_memory(pheap);pheap=NULL;
}





///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////////////////////////////DATA AND TIME//////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

unsigned long asc_to_digit(  char * str_asc,unsigned char d_cnt )
{
    if(d_cnt<10)
    {
        char tmp_buf[10];
	    memset(tmp_buf,0,10);
	    memcpy(tmp_buf,str_asc, d_cnt);
	    return (long)atoi(tmp_buf);   // in 32bit or 16bit or 8 bit system , this value range must be change!
    }
    return 0;
}


unsigned char app_check_password( unsigned char pin_type)
{
    char * str_pin=0;
	char * str_show=0;
	if(PASSWORD_TYPE_USER_PIN== pin_type)
    {
         str_pin= sysprop->str_keypad_pin;
		 str_show= get_multi_string((char * *)text_enter_keypad_pin);
	}

	else if(PASSWORD_TYPE_MASTER_PIN== pin_type)
	{
         str_pin= sysprop->str_master_pin;
		 str_show= get_multi_string((char * *)text_enter_master_pin);
	}	
    if( 0  != str_pin )
    {
	    if( 0 == strlen(str_pin))
		{
	         return 1;
		}
		else
		{
		    char str_unlock[10];
		    memset(str_unlock,0,10);
		    if(1 ==wnd_input_password(str_unlock,(char *)str_show, 8))
		    {
		        if( 0 == strncmp( str_pin , str_unlock, strlen(str_pin)))
		        {
	                return 1;
				}
				else
				{
		            msg_warn(get_multi_string((char * *)text_password_error));
					delay_ms(2000);
				}
			}
		}
	}
	return 0;
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////////////////////////////GLOBLE FUNCTIONS///////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void app_init_global_vars()
{
	prop_file = app_alloc_memory(FLASH_CELL_LENGTH_1K);			// 在堆中分配1K内存
	memset(prop_file,0,FLASH_CELL_LENGTH_1K);					// 1k内存的数据全部置零

    memset(&appsys, 0 ,sizeof(APPSYS));							// appsys结构变量的数据全部置零

    appsys.byte_last_music_id =   0xFF;
    sysprop     =(SYSPROP *)&prop_file[0];						// 

	appsys.byte_power_service_delay=BAT_SERVICE_DELAY;

	appsys.power_service.flag_extern_power=0xFF;
	appsys.power_service.flag_having_battery=0xFF;

	appsys.power_service.gui_byte_battery_lvl=0xFF;
	appsys.power_service.gui_flag_extern_power=0xFF;
	appsys.power_service.gui_flag_having_battery=0xFF;
	appsys.flag_voice_path=0xFF;

    ctw_buf = (unsigned short *)app_alloc_memory(ANSI_GB_BUF_SIZE *2);
    wtc_buf = (unsigned char  *)app_alloc_memory(ANSI_GB_BUF_SIZE);



    memset(gpcommbuf,0, sizeof(gpcommbuf));
	gpcommbuf_len =0;


	
	sysprop->byte_language_type =LANGUAGE_SPANISH;


	gsyn_seq =1;
	appsys.byte_alarm_clock_hour=appsys.byte_alarm_clock_min=0xFF;
	appsys.power_service.dword_no_ext_power_st=MAX_INT32_FFFFFFFF;



}









void app_system_reset()
{
    app_shut_down(1);
}



void app_feed_dog()
{

}



void app_init_gpio()
{	
     

}
void app_enable_speaker()
{
	appsys.flag_speaker_on=1;
}
void app_disable_speaker()
{
	appsys.flag_speaker_on=0;
}


void app_signal_led_on()
{
 
	appsys.flag_signal_led_on=1;
}
void app_signal_led_off()
{
	appsys.flag_signal_led_on=0;
}

void set_handfree_led(unsigned char onoff)
{
    appsys.flag_hdfree_led_on=onoff;
}















unsigned char get_battery_lvl()
{
    unsigned char lvl= appsys.power_service.battary_status.adc /20;
	if(lvl >4){lvl=4;}
	return lvl;
}




void app_get_power_info()
{
    if( (appsys.power_service.gui_flag_extern_power != appsys.power_service.flag_extern_power) )
    {
        appsys.power_service.gui_flag_extern_power= appsys.power_service.flag_extern_power;
		
        if(1 == appsys.flag_genie_trace)DebugPrintf("external power changed-----> %s\r\n", (1== appsys.power_service.flag_extern_power)?" external power connected++++":" external power moved away----");
		
		appsys.power_service.dword_charge_tot_time =0;

		if(1 == appsys.power_service.flag_extern_power)
		{		
		    if(1 == appsys.flag_showing_lowbat )
			{
                 appsys.flag_showing_lowbat =0;
			}
			
			if(sysprop->byte_backlightoff_timeout == 3)
			{
		        lcd_backlight_on();
			}
		}  

    	if(0 == appsys.power_service.flag_extern_power)
		{
            appsys.power_service.dword_no_ext_power_st = app_get_tick_count();
		}
		else
		{
            appsys.power_service.dword_no_ext_power_st = MAX_INT32_FFFFFFFF;
		}

		if(0xFF == appsys.byte_sysrun_state)
		{
		    appsys.byte_need_redraw=0xFF;
		}
	}
	
    if(appsys.power_service.gui_flag_having_battery != appsys.power_service.flag_having_battery)
    {	
        appsys.power_service.gui_flag_having_battery= appsys.power_service.flag_having_battery;
		
			
		if(0xFF == appsys.byte_sysrun_state)
		{
		    appsys.byte_need_redraw=0xFF;
		}
	}
}





void app_power_service()
{
    //app_test_heap();
	
    if( 1 == appsys.power_service.flag_having_battery  )
    {
	    if(1 == appsys.power_service.flag_extern_power)
	    {
		    if(0 == appsys.power_service.flag_battery_full)
		    {
				 appsys.power_service.dword_charge_tot_time += BAT_SERVICE_DELAY;
		    }
	    }
		else
		{
			if(  0 == appsys.flag_showing_lowbat && appsys.power_service.battary_status.adc<=10)
			{
			     if(app_get_tick_count() >45 * 1000 )
			     {
	                 appsys.flag_showing_lowbat =1;
					 if(0xFF == appsys.byte_sysrun_state){mu_generate_beep();}
					 appsys.byte_need_redraw=0xFF;
			     }
			}
			else if(1 == appsys.flag_showing_lowbat && appsys.power_service.battary_status.adc>=20)
			{
                 appsys.flag_showing_lowbat=0;
				 appsys.byte_need_redraw=0xFF;
			}
		}
	}
	else
	{
        ;// having external power only
	}
}

void app_query_hw_info(unsigned short wait_ms)
{
   char tmp_cmd[128];

   memset(tmp_cmd,0,sizeof(tmp_cmd));
   send_android_command(andr_build_44B_get_imei(tmp_cmd));
   
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   send_android_command(andr_build_44C_get_imsi(tmp_cmd));
   
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   send_android_command(andr_build_44D_get_iccid(tmp_cmd));
   
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   send_android_command(andr_build_44F_get_andr_ver(tmp_cmd));

   delay_ms(wait_ms);
}





unsigned char app_wait_simcard_ready(int wait_ms)
{
    unsigned long st_tick = app_get_tick_count();

    appsys.flag_got_421=0;

	while(get_duration_tick(st_tick) < wait_ms)
	{
	   if(appsys.flag_got_421 >0)
	   {
	       if(1 == appsys.flag_genie_trace)DebugPrintf("app_wait_simcard_ready,ok \r\n");
		   return 1;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("app_wait_simcard_ready ,failed! \r\n");
	return 0;
}



void app_initial_system()
{
	app_init_global_vars();
    app_init_gpio();
    app_get_system_datetime();

    appsys.flag_genie_trace=1;  //JWL

    gu_check_lanco_folder();
	
	lcd_init_my();

    lcd_clear_screen();

    app_check_database();
	
    lcd_goto_xy(0, 0);
	show_bitmap(BMP_START_UNICOM_BIN);
	delay_ms(300);
	
	app_enable_speaker();
    lcd_backlight_on();	




	appsys.flag_key_tone=sysprop->flag_key_tone;
    lcd_set_contrast_my(sysprop->byte_contrast_volume);


    InitLXIndexTbl();
    InitPYIndexTbl();
    InitBHIndexTbl();
	app_test_function();


    appsys.flag_flash_checked=1;
	appsys.flag_having_missed_call=check_if_having_missed_call();	
	appsys.flag_having_unread_sms= check_if_having_new_sms();	

    //repaire some parameters
	if( 2 != sysprop->byte_decimal_pos)
	{
        sysprop->byte_decimal_pos=2;
	}

	set_handfree_led(0); 
}

void refresh_interface_registering()
{
	static unsigned char st_pos=36;
	unsigned char tmp_bmp[4];
	unsigned char i=0;

	tmp_bmp[0]=0x00;
	tmp_bmp[1]=0x90;
	tmp_bmp[2]=0x60;
	tmp_bmp[3]=0x00;
    lcd_fill_area(0x00, 36,36,50,4);

	//if(st_pos<36)
	{
    //    st_pos=36;
	}

	if(++st_pos>38)
	{
	    st_pos=36;
	}

	for(i=st_pos;i<=82;i+=4)
	{
         lcd_goto_xy(i, 36);
		 lcd_display_data(4, 4, tmp_bmp);
	}	
}



unsigned char convert_csq_level_grade(  unsigned char csq_level )
{
    if(appsys.byte_csq_level >5)
    {
        return 5;
	}
	else if(appsys.byte_csq_level >0)
	{
        return appsys.byte_csq_level+1;
	}
	
    return appsys.byte_csq_level;
}

char get_key_value(unsigned char key_code)
{
   switch(key_code)
   {
		case TFKEY_1:
			return '1';
			break;
		case TFKEY_2:
			return '2';
			break;			
		case TFKEY_3:
			return '3';
			break;			
		case TFKEY_4:
			return '4';
			break;			
		case TFKEY_5:
			return '5';
			break;			
		case TFKEY_6:
			return '6';
			break;			
		case TFKEY_7:
			return '7';
			break;			
		case TFKEY_8:
			return '8';
			break;			
		case TFKEY_9:
			return '9';
			break;			
		case TFKEY_STAR:
			return '*';
			break;			
		case TFKEY_0:
			return '0';
			break;			
		case TFKEY_SHARP:
			return '#';
			break;
		default:
		break;
   }
   return 0;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////////////////////////////STRING COMMANDS////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

unsigned char is_digit(char c)
{
   return  (c>='0' && c<='9')? 1:0;
}
unsigned char is_dtmf(char c)
{
   return  ( 1== is_digit(c) || '*'==c || '#' == c);
}

unsigned char is_digit_str(char * p, unsigned short len)
{
   unsigned short i=0;
   for(i=0;i<len;i++)
   {
       if(0 == is_digit(p[i]))
       {
           return 0;
	   }
   }
   return 1;
}
unsigned char is_dtmf_str(char * p, unsigned short len)
{
   unsigned short i=0;
   for(i=0;i<len;i++)
   {
       if(0 == is_dtmf(p[i]))
       {
           return 0;
	   }
   }
   return 1;
}

unsigned char is_ip_str(char * p, unsigned short len)
{
   unsigned short i=0;
   for(i=0;i<len;i++)
   {
       if( (0 == is_digit(p[i])) &&  ('.' != p[i] ))
       {
           return 0;
	   }
   }
   return 1;
}

 

unsigned char is_space ( char c )
{
   return c == ' '  || c == '\t' ||
          c == '\v' || c == '\f'  ;  
}

char * trim_left ( char * p )
{
    while ( p && is_space(*p) ) ++p ;	
    return p ;
}

char * trim_right ( char * p )
{
    char * temp;

    if (!p) return p;
    temp = (char *)(p + strlen(p)-1);
    while ( (temp>=p) && is_space(*temp) ) --temp;
    *(temp+1) = '\0';
    return p;
}

char * trim ( char * p )
{
    return trim_left(trim_right(p));
}

char * str_pad_left(char * pstr,unsigned char max_len, char c)
{
	unsigned char cur_len= strlen(pstr);
    if(cur_len<max_len)
	{
	    unsigned char delta= max_len- cur_len;
		unsigned char i=0;
		for(i=max_len-1; i>=delta;i--)
		{
		    pstr[i]=pstr[ i-delta];
		}
		memset(pstr,c, delta);
	}
	return pstr;
}

char * str_pad_right(char * pstr,unsigned char max_len, char c)
{
	unsigned char cur_len= strlen(pstr);
    if(cur_len<max_len)
	{
	    unsigned char delta= max_len- cur_len;
		memset(&pstr[cur_len],c, delta);
	}
	return pstr;
}

unsigned char app_get_fast_dial_number(char * fast_dial)
{
    RECORDWND * p_record_wnd = appsys.fst_dial.p_record_wnd ;
	unsigned char  rec_pos  =appsys.fst_dial.rec_pos;
	
    if(rec_pos >0 && NULL != p_record_wnd)
    {
	    if(TABLEINFO_PHONEBOOK== appsys.fst_dial.p_record_wnd->table_idx)
		{
	        T_BOOK_ENTRY book_entry;
			memset(&book_entry,0,sizeof(T_BOOK_ENTRY));

		    if(1 ==wnd_record_read(p_record_wnd, rec_pos, (unsigned char *)&book_entry))
		    {
		        strncpy(fast_dial, book_entry.num, BOOK_ENTRY_NUM_FIELD_SIZE);
				return 1;
 			}
		}
	    else if( TABLEINFO_CALL_IN == p_record_wnd->table_idx
	      ||TABLEINFO_CALL_OUT     == p_record_wnd->table_idx
	      ||TABLEINFO_MISSED_CALL  == p_record_wnd->table_idx
		)
	    {
	        T_CALL_RECORD call_record;
			memset(&call_record,0,sizeof(T_CALL_RECORD));
		    if(1 ==wnd_record_read(p_record_wnd, rec_pos, (unsigned char *)&call_record))
		    {
                 strncpy(fast_dial, call_record.num, BOOK_ENTRY_NUM_FIELD_SIZE);
				 return 1;
			}
		}
		else if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx
	          ||TABLEINFO_SMS_OUTBOX == p_record_wnd->table_idx
		)
		{
	        T_SMS_RECORD sms_record;
			T_BOOK_ENTRY temp_entry;
			
			memset(&temp_entry,0, sizeof(T_BOOK_ENTRY));		
			memset(&sms_record,0,sizeof(T_SMS_RECORD));

		    if(1 ==wnd_record_read(p_record_wnd, rec_pos, (unsigned char *)&sms_record))
		    {
			    strncpy(fast_dial, sms_record.str_num, BOOK_ENTRY_NUM_FIELD_SIZE);
				return 1;
			}
		}

	}
	return 0;
}



unsigned char check_if_too_fast_key(void)
{
    unsigned char nret=0;
    if(KEY_STATE_PRESSED == appsys.flag_key_state)
    {
        unsigned long cur_tick =app_get_tick_count();
		unsigned long time_delta=0;
		unsigned char  buf_max_cnt=0;
		unsigned long  slot =0;
		
        if(1 == appsys.flag_record_showing)
        {
			buf_max_cnt=15;
			time_delta = 120;
        }
		else if(1 == appsys.flag_editing_sms)
		{
			buf_max_cnt=12;
			time_delta = 110;
		}
		else
		{
			buf_max_cnt=12;
			time_delta = 130;
		}


		
        if(appsys.byte_fast_key_times<buf_max_cnt)
        {
            if(cur_tick- appsys.dword_last_key_tick <time_delta)
            { 
                 appsys.byte_fast_key_times++;
			     //if(1 ==appsys.flag_genie_trace)DebugPrintf("----byte_fast_key_times  =%d\r\n",appsys.byte_fast_key_times);
			}
		}
		else
		{
			//if(1 ==appsys.flag_genie_trace)DebugPrintf("!!!!!!!!!!!!key_ignored!! cnt=%d mxcnt=%d  delta=%d  slot=%dms!!!!!!!!!!!!!!!!!!\r\n",appsys.byte_fast_key_times,buf_max_cnt, time_delta,(cur_tick- appsys.dword_last_key_tick));
			nret= 1;
		}	

        
		slot= get_duration_tick(appsys.dword_last_key_tick);

        if(slot >time_delta*2)
        { 
             int sub_cnt =(cur_tick- appsys.dword_last_key_tick)/time_delta;
		  
             if(appsys.byte_fast_key_times>=sub_cnt)
             {
                 appsys.byte_fast_key_times-=sub_cnt;
             }
			 else
			 {
			     appsys.byte_fast_key_times=0;
			 }
			 //if(1 ==appsys.flag_genie_trace)DebugPrintf("----byte_fast_key_times = %d -----slot=%d-----(-=%d)\r\n",appsys.byte_fast_key_times,slot, sub_cnt);
		}
		
	    appsys.dword_last_key_tick=cur_tick;	
    }
	
	return nret;
}



void app_shut_down(unsigned char flag_reboot)
{
    char tmp_cmd[128];
	app_stop_music();
    wnd_show_notice(get_multi_string((char * *)text_prompt), get_combined_string(get_multi_string((char * *) text_power_down), ",", get_multi_string((char * *)text_please_wait) ), NOTICE_TYPE_NULL, IGNORE_SOFT_KEY_INPUT);

	memset(tmp_cmd,0,sizeof(tmp_cmd));
	send_android_command(andr_build_43B_shut_phone(tmp_cmd, (1 == flag_reboot)?1:0));

	while(1){delay_ms(1000);}
}

unsigned char is_valid_key(unsigned char key_code)
{
  //居家卫士 总共有28个按键
  switch(key_code)
  {
	case TFKEY_SOFT_LEFT:
	case TFKEY_SOFT_RIGHT:
	case TFKEY_POWER:
	case TFKEY_SMS:
	case TFKEY_UP:
	case TFKEY_LEFT:
	case TFKEY_DOWN:
	case TFKEY_RIGHT:
	case TFKEY_1:
	case TFKEY_2:
	case TFKEY_3:
	case TFKEY_4:
	case TFKEY_5:
	case TFKEY_6:
	case TFKEY_7:
	case TFKEY_8:
	case TFKEY_9:
	case TFKEY_STAR:
	case TFKEY_0:
	case TFKEY_SHARP:
	case TFKEY_SEND:
	case TFKEY_HANDFREE:
	{   
	    return 1;  
	}   
	break;
	
	   default:
	   break;
	}
	
	return 0;
}



unsigned char app_get_message(FlSignalBuffer  * p_evt )
{
     CMessage* pMsg=NULL;
	 memset(p_evt,0,sizeof(FlSignalBuffer));
	
     if(  (pMsg = pUI->app_get_message()) !=NULL)
	 {	  
		  if(pMsg->mMsgCode == EVENT_TIMER)
          {
			 p_evt->eventTyp =EVENT_TIMER;

			 if(get_duration_tick(appsys.dword_one_second_tick)>950)
			 {
			     app_get_system_datetime();
	             appsys.dword_one_second_tick=app_get_tick_count();
				 appsys.flag_time_changed=0xFF;
			 }

			 if(get_duration_tick(appsys.dword_half_second_tick)>450)
			 {
	             appsys.dword_half_second_tick=app_get_tick_count();
				 appsys.flag_half_second=0xFF;
			 }
		  }
		  else if(pMsg->mMsgCode == EVENT_KEY)
		  {
                input_event * pkey = (input_event *)pMsg->mMsgBuf;

				appsys.dword_last_input_tick= app_get_tick_count();


			    if( TFKEY_ON_HOOK == pkey->code )
			    {
			        if(1 == pkey->value)
					{
				    	p_evt->sig_p.handset_evt=1;
						p_evt->eventTyp =EVENT_HANDSET;
			        }
				}
				else if(TFKEY_OFF_HOOK == pkey->code)
				{
				    if(1 == pkey->value)
					{
					    p_evt->sig_p.handset_evt=0;
						p_evt->eventTyp =EVENT_HANDSET;
				    }
				}
				else
				{

					p_evt->eventTyp =EVENT_KEY;
					p_evt->sig_p.key_evt.key_val=pkey->code;

					if(1 == pkey->value)
					{
	                     p_evt->sig_p.key_evt.isPressed=1;

						 if(1 == appsys.flag_any_key_stop_music)
						 {
	                         app_stop_music();
						 }
					}
					else if(0 == pkey->value)
					{		
	                     p_evt->sig_p.key_evt.isPressed=0;
					}
					else
					{
	                     p_evt->sig_p.key_evt.isPressed=0xFF;
					}
			    }
		  }	
		  else if(pMsg->mMsgCode == EVENT_ANDROID_TO_UI)
		  {
		       char  ansi_buf[1023];
			   int   ansi_len =0;

	  		   char * uni_buf = new char[pMsg->mMsgBufLen+2];
			   memset(uni_buf,0, pMsg->mMsgBufLen +2);
			   memcpy(uni_buf,pMsg->mMsgBuf, pMsg->mMsgBufLen);
			   memset(ansi_buf,0,sizeof(ansi_buf));



			   if(1 == appsys.flag_genie_trace)
			   {
				   print_hex_string("ANDROID MSG UNIC",pMsg->mMsgBuf, pMsg->mMsgBufLen);
               }


               ansi_len = WideChar_MultiByte(ansi_buf, (unsigned short *) uni_buf);

			   
		       if( ansi_len>=5)
		       {
		           analyse_android_message(p_evt, ansi_buf,  ansi_len);
		       }
			   else
			   {
                  if(1 == appsys.flag_genie_trace)
				  {
				      DebugPrintf("ERROR ANDROID MSG ANSI: %s\r\n", ansi_buf);
					  print_hex_string("ERRORANDROID MSG UNIC",pMsg->mMsgBuf, pMsg->mMsgBufLen);
                  }
			   }

			   free(uni_buf);
		  }
		  
		  CMessage::Free(pMsg);
		  	
          if(p_evt->eventTyp >0)
          {
		      return  (1 ==app_pre_process_msg(p_evt ))? 0:1;
          }
		 
	 }

    return 0;
}












	

unsigned char app_pre_process_msg(FlSignalBuffer  * p_evt )
{
    unsigned short pos=0xFFFF;
    unsigned char flag_handled=0;
    unsigned char flag_need_wakeup=0;

#if 1
    if(1 == appsys.flag_is_sleeping)
    {
      
	    if( (EVENT_KEY== p_evt->eventTyp) 
          ||(EVENT_MODULE== p_evt->eventTyp)
          ||(EVENT_HANDSET== p_evt->eventTyp)
          ||(EVENT_USART1== p_evt->eventTyp)
          ||(EVENT_USART2== p_evt->eventTyp)
		)
	    {
	        if(1== appsys.flag_genie_trace)DebugPrintf("wake up by event  %d\r\n",  p_evt->eventTyp);
			flag_need_wakeup=1;
	    }
	    if(1 == app_check_soft_wake_up())   
        {
  	        if(1== appsys.flag_genie_trace)DebugPrintf("wake up by soft wake up\r\n",NULL);
			flag_need_wakeup=1;
			appsys.byte_need_exit|=EXIT_BY_SOFT_AWAKE;
	    } 

		if(1 == flag_need_wakeup)
		{   
            app_run_wakeup();
		}
    }





#endif	

	if(gpcommbuf_len >0)
	{


	}

 

		
    // short msg and incoming call can be pop out automatically.
	if( EVENT_MODULE == p_evt->eventTyp )
	{
        if(appsys.flag_got_423 >0)
        {
             switch(appsys.byte_tlk_state)
			 {
			     //1	来电
	             case '1':
				 	if(0 == appsys.flag_call_comming)
				 	{
				 	    appsys.flag_got_423=0;
                        if(4 != sysprop->byte_backlightoff_timeout ) lcd_backlight_on();
		                process_recv_call();
					}
				 	break;


					
                 //2	通话结束
				 case '2':
				 	appsys.flag_no_carrier=1;
				 	break;
                 //3	正在呼出
				 case '3':
				 	break;
                 //4	接通
				 case '4':
	                break;
                 //5	呼叫保持中
				 case '5':
				 	break;
                 //5	三方通话中
				 case '6':
				 	break;					
				 default:
				 	break;
			 }

		}


	}
#if 1	
	else if(EVENT_KEY == p_evt->eventTyp)
	{
	    if(p_evt->sig_p.key_evt.isPressed)
	    {
		    appsys.byte_need_exit=0;
	    }

		if(0xFF == p_evt->sig_p.key_evt.isPressed)
		{
		    //除了按下和松开的其他逻辑状态
            return 1;
		}
	    appsys.flag_key_state=(p_evt->sig_p.key_evt.isPressed)? KEY_STATE_PRESSED : KEY_STATE_RELEASED;



		
        if(1 == appsys.flag_genie_trace)DebugPrintf(" key pressed (%d):   0x%.2X  %s\r\n",p_evt->sig_p.key_evt.key_val, p_evt->sig_p.key_evt.key_val, (p_evt->sig_p.key_evt.isPressed)? "KEY_STATE_PRESSED++" : "KEY_STATE_RELEASED--" );

        if(4 != sysprop->byte_backlightoff_timeout)lcd_backlight_on();
        app_feed_dog();


		if(TFKEY_POWER==p_evt->sig_p.key_evt.key_val &&  KEY_STATE_PRESSED == appsys.flag_key_state )
		{
			if(0 == appsys.flag_keypad_locked)
			{
				appsys.flag_keypower_down=1;
				appsys.dword_key_power_tick=app_get_tick_count();
			}
		}
		else
		{ 
			appsys.flag_keypower_down=0;
		}

        if(p_evt->sig_p.key_evt.isPressed)
		{
	         if( TFKEY_SOFT_RIGHT==  p_evt->sig_p.key_evt.key_val)
			 {

				  
			 }
        }


        if(1 == appsys.flag_dialing ||  1== appsys.flag_talk_rating)
        {
            
        }
		else
		{
			if(1 == check_if_too_fast_key())
			{
	            return 1;
			}
		}	
	


        //to check if need play beep or dtmf voc
		if(1 == p_evt->sig_p.key_evt.isPressed)
		{
		    unsigned char flag_need_beep=0;
			unsigned char flag_beep_done=0;
		    char input_code =get_key_value(p_evt->sig_p.key_evt.key_val);

		    if(0 != input_code)
		    {
		        if(1 == appsys.flag_dialing )
                {
                    // if number is full ,then don't generate dtmf sound
			    }
				else if( 1 == appsys.flag_key_tone)
				{
                    flag_need_beep=1;
				}
		    }
			else if( 1 == appsys.flag_key_tone)
			{
                flag_need_beep=1;
			}
			
		

			if( (1 ==flag_need_beep)  &&  ( (TFKEY_HANDFREE != p_evt->sig_p.key_evt.key_val  )  || 1  == appsys.flag_hardware_test)  )
			{
			    if(0 == appsys.flag_sending_sms 
				&& 0 == appsys.flag_call_comming 
                && 0 == appsys.flag_dialing
                && 0 == appsys.flag_selecting_ring
                && (TFKEY_POWER != p_evt->sig_p.key_evt.key_val)
				)
			    {  
                    mu_generate_beep();
					flag_beep_done =1;
			    }
			}

			if(0 == flag_beep_done)
			{
				if(1 == appsys.flag_genie_trace)DebugPrintf("flag_need_beep=%d, flag_selecting_ring=%d\r\n",flag_need_beep,appsys.flag_selecting_ring);
				if(1 == appsys.flag_genie_trace)DebugPrintf("p_evt->sig_p.key_evt.key_val=0x%02X\r\n",p_evt->sig_p.key_evt.key_val);
				if(1 == appsys.flag_genie_trace)DebugPrintf("appsys.flag_hardware_test=%d\r\n",appsys.flag_hardware_test);
				if(1 == appsys.flag_genie_trace)DebugPrintf("appsys.flag_sending_sms=%d\r\n",appsys.flag_sending_sms);
				if(1 == appsys.flag_genie_trace)DebugPrintf("appsys.flag_call_comming=%d\r\n",appsys.flag_call_comming);
				if(1 == appsys.flag_genie_trace)DebugPrintf("appsys.flag_dialing=%d\r\n",appsys.flag_dialing);
			}
		}



		if((1 ==appsys.flag_enable_handfree) && (TFKEY_HANDFREE == p_evt->sig_p.key_evt.key_val)  && ( 1== p_evt->sig_p.key_evt.isPressed) )
		{
		     if(0 == appsys.flag_sending_numbers)
		     {
	             appsys.flag_hand_free = (1 ==appsys.flag_hand_free)?0:1;
			     if(1 ==appsys.flag_genie_trace)DebugPrintf("hand free flag changed to %s\r\n" ,(1 ==appsys.flag_hand_free)? "true":"false");
		     }
			 else
			 {
                 appsys.flag_handfree_pressed=1;
			 }
		}


		if(TFKEY_HANDFREE == p_evt->sig_p.key_evt.key_val  )
		{

             if(   0 == appsys.flag_is_sleeping  
			 	    && 1 == p_evt->sig_p.key_evt.isPressed
				 	&& 0 == appsys.flag_talk_rating
				 	&& 0 == appsys.flag_sending_sms
				    && 0 == appsys.flag_call_comming
				    && 0 == appsys.flag_dialing
                    && 0 == appsys.flag_low_layer_dial
                    && 0xFF== appsys.byte_sysrun_state
                    && 0 == appsys.flag_hardware_test
                    && 0 == appsys.flag_doing_auto_redial
				 )
             {
                 appsys.flag_low_layer_dial =1;
                 if(0 == appsys.flag_keypad_locked)
                 {
                     appsys.flag_hand_free =1;
	                 set_voice_path_by_action(VOICE_ACTION_KEY);
					 if(1)
	                 {
	                     char str_fast_dial[BOOK_ENTRY_NUM_FIELD_SIZE+1];
						 memset(str_fast_dial,0,sizeof(str_fast_dial));
						 if(1 ==appsys.flag_genie_trace)DebugPrintf("++++make call by flag_low_layer_dial\r\n",NULL);

						 if( 1 == app_get_fast_dial_number(str_fast_dial))
						 {
							 app_dial_out(str_fast_dial);
						 }
						 else
						 {
	                         app_dial_out(NULL);
						 }

						 appsys.byte_need_redraw=0xFF;
	                 }
                 }
				 else
				 {
				     appsys.flag_wish_input_star=1;
                     msg_warn(get_multi_string((char * *)text_keypad_locked_please_enter_star_to_unlock));
					 delay_ms(1500);
					 appsys.flag_wish_input_star=0;
				 }
				 appsys.flag_low_layer_dial=0;
			}
		}

	

	}
	else if(EVENT_HANDSET == p_evt->eventTyp)
	{
		appsys.byte_need_exit=0;


        if(4 != sysprop->byte_backlightoff_timeout )lcd_backlight_on();

        appsys.flag_hook_state = (0 == p_evt->sig_p.handset_evt)? HOOK_STATE_OFFHOOK : HOOK_STATE_ONHOOK;

		if(appsys.flag_hook_state == HOOK_STATE_OFFHOOK)
		{
		    appsys.flag_hand_free=0;
			app_disable_speaker();
			
			if(1 == appsys.flag_dialing && 0 == appsys.flag_hardware_test)
			{
                mu_set_voice_path(VOICE_PATH_HANDSET);
			}
		}
		else
		{
            appsys.dword_prepare_enspk_tick = app_get_tick_count();
		}


        if(1)
		{
			 if(   0 == appsys.flag_is_sleeping  
				 	&& 0 == appsys.flag_talk_rating
				 	&& 0 == appsys.flag_sending_sms
				    && 0 == appsys.flag_call_comming
				    && 0 == appsys.flag_dialing
				    && 0 == appsys.flag_low_layer_dial
				    && 0xFF== appsys.byte_sysrun_state
				    && 0 == appsys.flag_hardware_test
				    && 0 == appsys.flag_doing_auto_redial
				 )
			 {
			        appsys.flag_low_layer_dial =1;
				    if(0 == appsys.flag_keypad_locked)
				    {
			            if(1 ==set_voice_path_by_action(VOICE_ACTION_HANDSET))
			            {
	                         char str_fast_dial[BOOK_ENTRY_NUM_FIELD_SIZE+1];
							 memset(str_fast_dial,0,sizeof(str_fast_dial));
							 
							 if( 1 == app_get_fast_dial_number(str_fast_dial))
							 {
								 app_dial_out(str_fast_dial);
							 }
							 else
							 {
		                         app_dial_out(NULL);
							 }
						}
				    }
					else
					{
					     appsys.flag_wish_input_star=1;
	                     msg_warn(get_multi_string((char * *)text_keypad_locked_please_enter_star_to_unlock));
						 delay_ms(1500);
						 appsys.flag_wish_input_star=0;
					}
					appsys.flag_low_layer_dial=0;
			 }	
		 }
	}
	else 
#endif
		if(EVENT_TIMER  == p_evt->eventTyp)
	{      
	    if(MAX_INT32_FFFFFFFF != appsys.dword_prepare_enspk_tick)
	    {
            if(get_duration_tick(appsys.dword_prepare_enspk_tick) >180)
            {
                appsys.dword_prepare_enspk_tick =MAX_INT32_FFFFFFFF;
				app_enable_speaker();
			}
		}

		if(appsys.byte_delay_cnt_handfree >0)
		{
		    appsys.byte_delay_cnt_handfree--;
			if(0 == appsys.byte_delay_cnt_handfree)
			{
			     if(1 == appsys.flag_genie_trace)DebugPrintf("[delay] for set voice path to handfree\r\n");
			     mu_set_voice_path(VOICE_PATH_HANDFREE);
			}
		}



		if(MAX_INT32_FFFFFFFF != appsys.power_service.dword_no_ext_power_st && 0xFF== appsys.byte_sysrun_state)
	    {
             if(get_duration_tick(appsys.power_service.dword_no_ext_power_st) >60 * 1000 * 3)
             {
                 appsys.power_service.dword_no_ext_power_st =    MAX_INT32_FFFFFFFF;
                 
			 }
		}





	    if(1 == appsys.flag_keypower_down)
	    {
		    if(get_duration_tick(appsys.dword_key_power_tick) >3500)
            {
                if( 0 == appsys.flag_waiting_power_down)
                {
                    appsys.flag_waiting_power_down=1;
                    app_shut_down(0);
                }
			}	
		}

		
		
        //process some back ground things
		if (  (appsys.flag_half_second& 0x80)>0 )
		{
		     appsys.flag_half_second &=  (unsigned char)(~0x80);
			 if(appsys.flag_talk_rating >0)
			 {
			     app_back_ground_rating();
			 }
		     if(1 == appsys.byte_sysrun_state)
		     {  
		         refresh_interface_registering();
		     }



			 if(1== appsys.flag_having_missed_call  ||  1 == appsys.flag_having_unread_sms)
			 {
                 if(1 == appsys.flag_signal_led_on)
                 {
                     app_signal_led_off();
				 }
				 else
				 {
                     app_signal_led_on();
				 }
			 }
			 else
			 {
                 if(1 == appsys.flag_signal_led_on)
                 {
                     app_signal_led_off();
				 }
			 }
		}

		if(1 == appsys.flag_playing_tone)
		{
             if(get_duration_tick(appsys.dword_dialtone_st) >20000)
             {
                  mu_stop_dialtone();
			 }
		}
		

		
		if (  (appsys.flag_time_changed & 0x80)>0 )
		{
		     appsys.flag_time_changed &=  (unsigned char)(~0x80);
			 app_feed_dog();
			 app_get_power_info();

			 if(1 == appsys.flag_backlight_on  && 0 == appsys.flag_alarm_ringing)
			 { 
				 if(3 == sysprop->byte_backlightoff_timeout )
				 {

                      if(1 ==appsys.power_service.flag_extern_power)
                      {
                          // keep it always on
					  }
					  else if( get_duration_tick(appsys.dword_last_input_tick)/1000> (unsigned char )backlight_delay[0])
					  {
                          lcd_backlight_off();
					  }
				 }
				 else if(4 == sysprop->byte_backlightoff_timeout )
				 {
                      lcd_backlight_off();
				 }
				 else if( get_duration_tick(appsys.dword_last_input_tick)/1000> backlight_delay[sysprop->byte_backlightoff_timeout])
                 {
                      lcd_backlight_off();
				 }
			 }

			 if( (1 == appsys.flag_dialing || 1 == appsys.flag_talk_rating) && (1 ==appsys.flag_hand_free)  )
			 {
			     if(0 == appsys.flag_pre_dial)
			     {
		             if(0 == appsys.flag_hdfree_led_on)
					 {
	                    if(1 == appsys.flag_genie_trace)DebugPrintf("(01)set_handfree_led=1 \r\n",NULL);
					    set_handfree_led(1);
		             }
			     }
			 }
			 else
			 {
			    if(1 == appsys.flag_hdfree_led_on)
			    {
				    if(1 == appsys.flag_genie_trace)DebugPrintf("(02)set_handfree_led=0 \r\n",NULL);
		           set_handfree_led(0);
			 	}
			 }

			 if(1 == sysprop->flag_auto_lock_keypad)
			 {
                 if(0 == appsys.flag_keypad_locked)
                 {
                      if( get_duration_tick(appsys.dword_last_input_tick)/1000/60 >= (0 == sysprop->byte_auto_lock_keypad_timeout?1:sysprop->byte_auto_lock_keypad_timeout))
                      {
                          appsys.flag_keypad_locked =1;
						  appsys.byte_need_exit=0xFF;
					  }
				 }
			 }

			 //auto back to last window

		     if(   (0   == appsys.flag_sending_sms)  
			 	&& (0 == appsys.flag_voice_doing)
			 	&& (0xFF== appsys.byte_sysrun_state) 
			 	&& (0 == appsys.flag_talk_rating)
			 	&& (0 == appsys.flag_call_comming)
			 	&& (0 == appsys.flag_dialing)
			 	&& (0 == appsys.flag_alarm_ringing)
			 	&& (0 == appsys.flag_sending_sms)
			 	&& (0 == appsys.flag_doing_fast)
			 )
		     {
				 if( get_duration_tick(appsys.dword_last_input_tick)/1000/60 >= WINDOW_TIME_OUT)
				 {
				     appsys.byte_need_exit|=EXIT_BY_TIMEOUT;
				 }
		     }

			 if(appsys.byte_music_max_time>1)
			 {
                 if( (-- appsys.byte_music_max_time) ==1 )  
                 {
                      if(1 ==appsys.flag_genie_trace)DebugPrintf("stop playing the music by max time out\r\n",NULL);
                      app_stop_music();
				 }
			 }


			 if(appsys.byte_last_minute!=appsys.struct_time.min)
			 {
			     appsys.byte_last_minute=appsys.struct_time.min;
			 }
			
            
             if( (1 == appsys.flag_flash_checked) && ( appsys.byte_sysrun_state >0 )  &&  (  appsys.byte_power_service_delay  >0))
             {
                 if( -- appsys.byte_power_service_delay == 0)
                 {
	                 appsys.byte_power_service_delay=BAT_SERVICE_DELAY;
	                 app_power_service();
                 }
             }

			 if( (0  == appsys.power_service.flag_extern_power)  &&   (0xFF== appsys.byte_sysrun_state))
			 {
				 if(sysprop->byte_sleep_timeout>0 
				 	&& 0 == appsys.flag_is_sleeping  
				 	&& 0 == appsys.flag_talk_rating
				 	&& 0 == appsys.flag_sending_sms
				    && 0 == appsys.flag_call_comming
				    && 0xFF == appsys.byte_sysrun_state
				    && 0 == appsys.flag_dialing
				    && 0 == appsys.flag_alarm_ringing
				    && 0 == appsys.flag_genie_trace
				 )
				 {
	                 unsigned char tm_array[]={0,10,20,30};
					 sysprop->byte_sleep_timeout %=4;
					 if(get_duration_tick(appsys.dword_last_input_tick)/1000 > tm_array[sysprop->byte_sleep_timeout]  )
					 {
	                     app_run_sleeping();
					 }
				 }
			 }
		}



	    if( sysprop->struct_alarm.alarm_on>0
		 	&& 0 == appsys.flag_talk_rating
		 	&& 0 == appsys.flag_sending_sms
		    && 0 == appsys.flag_call_comming
		    && 0 == appsys.flag_dialing
		    && 0 == appsys.flag_alarm_ringing
		    && (
                  appsys.byte_alarm_clock_hour != sysprop->struct_alarm.alarm_time.hour
               && appsys.byte_alarm_clock_min  != sysprop->struct_alarm.alarm_time.min
               && get_duration_tick(appsys.dword_last_clock_alarm_tick) >62000
		    )
		)
	    {
		    if(sysprop->struct_alarm.alarm_time.hour == appsys.struct_time.hour
	        && sysprop->struct_alarm.alarm_time.min  == appsys.struct_time.min
			)
			{
				appsys.flag_need_clock_alarm =1;
				appsys.byte_alarm_clock_hour =sysprop->struct_alarm.alarm_time.hour;
				appsys.byte_alarm_clock_min  =sysprop->struct_alarm.alarm_time.min;
			}
		}

		if(  1 == appsys.flag_need_clock_alarm
	      && 0 == appsys.flag_alarm_ringing
		  && appsys.byte_alarm_clock_hour == appsys.struct_time.hour
	      && appsys.byte_alarm_clock_min  == appsys.struct_time.min
		)
		{
             process_alarm_ring();
		}


        flag_handled=0;// timer goes to every interface anyhow
	}
	return flag_handled;
}
void app_run_sleeping(void)
{

    appsys.flag_is_sleeping=1;
	app_disable_speaker();
}
void app_run_wakeup()
{
    appsys.flag_is_sleeping=0;


	appsys.dword_last_input_tick=app_get_tick_count();
}


unsigned long app_get_tick_count()// return ms
{
    struct timespec ts;  
    clock_gettime(CLOCK_MONOTONIC, &ts);  
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}

#define MAX_TICK_CYCLE MAX_INT32_FFFFFFFF

unsigned long get_duration_tick_ex(unsigned long st_tick, unsigned long et_tick)
{
    if(et_tick < st_tick)
    {
        return (MAX_TICK_CYCLE- st_tick) + et_tick; 
	}
	return  et_tick- st_tick;
}
unsigned long get_duration_tick(unsigned long st_tick)
{
    return get_duration_tick_ex( st_tick, app_get_tick_count());
}


unsigned short findsubstr(char *src,const char *sub ,unsigned short st_pos, unsigned short et_pos)
{
    unsigned short i=0, j=0, s_len;


    s_len = strlen((char *)sub);

    while ( src[i] && ( i>=st_pos && i<et_pos) )
    {
        if ( src[i] == sub[j] )
        {
            ++i;
            ++j;
            if ( j >= s_len )
            {
                return i - j;
            }
        }
        else
        {
            ++i;
            i -= j;
            j = 0;
        }
    }

    return 0xffff;
}
unsigned short findsubdata(unsigned char *src, unsigned short src_len,unsigned char * sub,  unsigned short sub_len ,unsigned short st_pos, unsigned short et_pos)
{
    unsigned short i=0, j=0, k=0;

    while ( k< src_len && ( i>=st_pos && i<et_pos) )
    {
        if ( src[i] == sub[j] )
        {
            ++i;
            ++j;
            if ( j >= sub_len )
            {
                return i - j;
            }
        }
        else
        {
            ++i;
            i -= j;
            j = 0;
        }
		k++;
    }

    return 0xffff;
}

unsigned short dumpstr( char *src,unsigned short src_len,char *dest,unsigned short deslen,const char *begin,const char *end )
{
    unsigned short loc_begin=0, loc_end=0, i=0;


    if ( 0xffff == (loc_begin = findsubstr(src, begin, 0, src_len)) )
    {
        return 0;
    }

    loc_begin += strlen((char *)begin);
    if ( 0xffff == (loc_end = findsubstr(src+loc_begin, end  ,0, src_len-loc_begin)) )
    {
        return 0;
    }


    if ( loc_end> deslen )
    {
        return 0;
    }

    loc_end += loc_begin;

    for ( i=loc_begin; i<loc_end; ++i )
    {
        *dest++ = src[i];
    }
    *dest = 0;

    return loc_end - loc_begin;
}

unsigned char get_crc_value(unsigned char * pdata, unsigned short length)
{
    unsigned char ret_val =0;
	unsigned short i=0;

	for(i=0;i<length;i++)
	{
        ret_val^= pdata[i];    
	}
	return ret_val;
}


void delay_ms(unsigned short timeout_ms)
{
    FlSignalBuffer EventPara;
	unsigned long cur_tick= app_get_tick_count();

	while(1)
	{
        if(1 == app_get_message(&EventPara) )
        {
            if(EVENT_KEY == EventPara.eventTyp)
            {
                if(EventPara.sig_p.key_evt.isPressed)
				{
			         if( TFKEY_STAR ==  EventPara.sig_p.key_evt.key_val)
			         {
                          if(1 == appsys.flag_wish_input_star)
                          {
                               appsys.flag_wish_input_star=0;
							   appsys.flag_keypad_locked=0;
						       msg(get_multi_string((char * *)text_keypad_unlocked_welcome_to_use));
							   delay_ms(1200);
							   appsys.byte_need_redraw=0xFF;
                               return;
						  }
					 }
				  	 else if( get_key_value(EventPara.sig_p.key_evt.key_val) >0)
					 {
                         appsys.byte_input_key_when_delay = get_key_value(EventPara.sig_p.key_evt.key_val);
					 }
                }
			}
		
            if(EVENT_MODULE == EventPara.eventTyp)
            {
                // msg that we don't need!
			}else if(EVENT_TIMER== EventPara.eventTyp)
			{
                if(get_duration_tick(cur_tick) >= timeout_ms)
                {
                    break;
				}
			}
			else
			{
                // msg that we don't need!
			}
		}
	}
	//if(1 ==appsys.flag_genie_trace)DebugPrintf("delayed %.6d milliseconds\r\n",timeout_ms);
}


unsigned char get_module_line(unsigned short timeout_ms)
{
	FlSignalBuffer EventPara;
	unsigned long cur_tick= app_get_tick_count();


	while(1)
	{
	    if( 1 ==app_get_message(&EventPara) )
	    {
	        if(EVENT_MODULE == EventPara.eventTyp)
	        {
                return 1;
			}else if(EVENT_TIMER== EventPara.eventTyp)
			{
	            if(get_duration_tick(cur_tick) >= timeout_ms)
	            {
	                if(1 ==appsys.flag_genie_trace)DebugPrintf("time out for getting module line\r\n",NULL);
	                return 0;
					break;
				}
			}
		}
	}
	return 0;
}







const unsigned char days_mon[12]={31,28,31,30,31,30,31,31,30,31,30,31};
void app_time_span_add(unsigned long seconds)
{
   unsigned long cur_s=appsys.struct_time.hour;
   DATE date;
   unsigned char flag_month_changed=0;
   
   memcpy(&date, &(appsys.struct_date),sizeof(DATE));

   cur_s *=60;
   cur_s+=appsys.struct_time.min;
   cur_s *=60;
   cur_s+=appsys.struct_time.sec;

   cur_s+=seconds;

   if(cur_s>86400)
   {
       cur_s-=86400;
		
       if ( ++date.week>6 )
       {
           date.week=0;
       }   
		
       if ( ++date.day>days_mon[date.mon-1] )
       {
           if ( date.mon==2 )
           {
                if(YearIsRunNian(date.year))
                {
                    if(date.day>29)
                    {
                       flag_month_changed=1;
					}
				}
				else
				{
				    if(date.day>28)
				    {
                       flag_month_changed=1;
					}
				}           
           }
           else
           {
                flag_month_changed=1;
           }   

		   if(1 == flag_month_changed)
		   {
                date.day = 1;
				if(++ date.mon>12)
				{
                    date.mon = 1;
                    ++date.year;
				}
		   }
       }
   }

   appsys.struct_time.hour=(unsigned char) (cur_s/3600);
   appsys.struct_time.min= (unsigned char) ( cur_s%3600/60);
   appsys.struct_time.sec= (unsigned char) (cur_s%3600%60);

   memcpy(&(appsys.struct_date), &date,sizeof(DATE));
}


void app_get_system_datetime()
{
	time_t timep;
	struct tm *p;
	
 	time(&timep);   
	p = localtime(&timep);

	if( (1900 + p->tm_year) >=2018 )
	{
	    appsys.struct_date.year   =   1900 + p->tm_year;
	    appsys.struct_date.mon    =   1 + p->tm_mon;
	    appsys.struct_date.day    =   p->tm_mday;
	    appsys.struct_date.week   =   1 + p->tm_wday; 

	    appsys.struct_time.hour  =    p->tm_hour;
	    appsys.struct_time.min   =    p->tm_min;
	    appsys.struct_time.sec   =    p->tm_sec;
	}
	else
	{
	    appsys.struct_date.year   =   2018;
	    appsys.struct_date.mon    =   5;
	    appsys.struct_date.day    =   5;
	    appsys.struct_date.week   =   6; 

	    appsys.struct_time.hour  =    12;
	    appsys.struct_time.min   =    0;
	    appsys.struct_time.sec   =    0;
		mu_set_date_time(&appsys.struct_date, &appsys.struct_time);
	}


}

















unsigned char edit_sms_message( T_SMS_RECORD * psms_record)
{
     unsigned char pre_crc= get_crc_value((unsigned char *)psms_record->context, strlen(psms_record->context));
	 unsigned char flag_can_send_sms;

	 
     appsys.flag_editing_sms=1;
	 
__INPUT_SMS_CONTEXT:
	flag_can_send_sms =1;
    if (1 == wnd_input_text(psms_record->context, get_multi_string((char * *) text_input),  SMS_CONTEXT_LENGTH, (LANGUAGE_CHINESE == sysprop->byte_language_type)?IN_PY5D:IN_ABC, EDIT_STYLE_SMS_FEATURE))
    {
        if( strlen(psms_record->context)>0)
        {
__INPUT_SMS_NUMBER:
             if (1 == wnd_input_text(psms_record->str_num, get_multi_string((char * *) text_enter_number),  BOOK_ENTRY_NUM_FIELD_SIZE, IN_FIXED, EDIT_STYLE_DIGITALONLY))
             {
                   if( 0 == strlen(psms_record->str_num))
                   {
			            msg_warn(get_multi_string((char * *) text_phone_number_input_needed));
						delay_ms(2000);
						goto __INPUT_SMS_NUMBER;
				   }
			 }
			 else
			 {
                 goto __INPUT_SMS_CONTEXT;
			 }
		}
		else
		{
            msg_warn( get_multi_string((char * *) text_message_text_input_needed));
			delay_ms(2000);
			goto __INPUT_SMS_CONTEXT;
		}
	}
	else
	{
	    flag_can_send_sms =0;
	}
	
	appsys.flag_editing_sms=0;

	if(1 == flag_can_send_sms)
	{
		memcpy( &(psms_record->date), &(appsys.struct_date),sizeof(DATE));
		memcpy( &(psms_record->time), &(appsys.struct_time),sizeof(TIME));
	   
		switch (app_menu_select((char ***)mem1_3_4_1, 4, NULL))
		{
	        case 1:
				{
					unsigned char nret=0;
					msg(get_multi_string((char * *) text_sending_message));

					if(1 == mu_send_sms(psms_record->str_num, psms_record->context))
					{
	                    psms_record->sms_flag.flag_sent=1;
						msgok( get_combined_string(get_multi_string((char * *)text_send), " ", get_multi_string((char * *)text_complete)));
						nret = 1;
					}
					else 
					{
					    msg_warn( get_combined_string(get_multi_string((char * *)text_send), " ", get_multi_string((char * *)text_failed)));
	                    nret = 0;
					}
				    app_record_append(TABLEINFO_SMS_OUTBOX, (unsigned char *)psms_record, REC_PREFIX_APPEND);
					delay_ms(1200);
				    appsys.byte_need_redraw=0xFF;
					return nret;
	        	}
				break;

	        case 2:
				{
					unsigned char nret=0;

					msg(get_multi_string((char * *) text_sending_message));

					if(1 == mu_send_sms(psms_record->str_num, psms_record->context))
					{
	                    psms_record->sms_flag.flag_sent=1;
						msgok( get_combined_string(get_multi_string((char * *)text_send), " ", get_multi_string((char * *)text_complete)));
						nret = 1;
					}
					else 
					{
					    msg_warn( get_combined_string(get_multi_string((char * *)text_send), " ", get_multi_string((char * *)text_failed)));
	                    nret = 0;
					}
					delay_ms(1200);
				    appsys.byte_need_redraw=0xFF;
					return nret;
	        	}
				break;

				
			case 3:
		        psms_record->date.year = appsys.struct_date.year;
				psms_record->date.mon= appsys.struct_date.mon;
				psms_record->date.day= appsys.struct_date.day;

				psms_record->time.hour= appsys.struct_time.hour;
				psms_record->time.min= appsys.struct_time.min;
				psms_record->time.sec= appsys.struct_time.sec;
				psms_record->sms_flag.flag_sent=0;
				
	            app_record_append(TABLEINFO_SMS_DRAFT, (unsigned char *)psms_record, REC_PREFIX_APPEND);
			    appsys.byte_need_redraw=0xFF;
				return 1;				
				break;
			case 4:
	            goto __INPUT_SMS_NUMBER;
				break;
				
			default:
				flag_can_send_sms =0;
				break;
		}
	}

    if(0 == flag_can_send_sms)
    {
        if(0 == appsys.flag_editing_draft)
	    {
		    if(strlen( psms_record->context) >0)
		    {    
		         psms_record->date.year = appsys.struct_date.year;
				 psms_record->date.mon= appsys.struct_date.mon;
				 psms_record->date.day= appsys.struct_date.day;

				 psms_record->time.hour= appsys.struct_time.hour;
				 psms_record->time.min= appsys.struct_time.min;
				 psms_record->time.sec= appsys.struct_time.sec;
				 psms_record->sms_flag.flag_sent=0;

			     app_record_append(TABLEINFO_SMS_DRAFT, (unsigned char *)psms_record, REC_PREFIX_APPEND);
			}
	    }
		else 
		{
		    if(strlen( psms_record->context) >0 &&  (pre_crc != get_crc_value((unsigned char *)psms_record->context, strlen(psms_record->context))))
		    {    
		         psms_record->date.year = appsys.struct_date.year;
				 psms_record->date.mon= appsys.struct_date.mon;
				 psms_record->date.day= appsys.struct_date.day;

				 psms_record->time.hour= appsys.struct_time.hour;
				 psms_record->time.min= appsys.struct_time.min;
				 psms_record->time.sec= appsys.struct_time.sec;
				 psms_record->sms_flag.flag_sent=0;

			     app_record_edit(TABLEINFO_SMS_DRAFT,appsys.byte_cur_draft_rec_no, (unsigned char *)psms_record);
			}
		}
	}

    appsys.byte_need_redraw=0xFF;
	return 0;
}

char * app_get_datetime_str()
{
    memset(appsys.str_sys_time,0,sizeof(appsys.str_sys_time));
    sprintf(appsys.str_sys_time,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d", appsys.struct_date.year,appsys.struct_date.mon,appsys.struct_date.day, appsys.struct_time.hour,appsys.struct_time.min,appsys.struct_time.sec);
	return appsys.str_sys_time;
}


void show_sms_info( T_SMS_RECORD * psms_record ,unsigned char table_idx )
{
    FlSignalBuffer EventPara;
    T_BOOK_ENTRY book_entry;
	

    memset(&book_entry, 0, sizeof(T_BOOK_ENTRY));
    find_book_entry_by_numbers(psms_record->str_num, &book_entry);

    lcd_clear_screen();
	show_caption(get_multi_string((char * *) text_details_info));
	lcd_goto_xy(0, 12);	lcd_draw_horizontal(SCREEN_WIDTH-1);

    appsys.byte_font_type=FONT_T_X_10;
	lcd_goto_x_line(0, 1);
	lcd_put_string(get_multi_string((char * *) text_date));
	lcd_put_string(":");
	{
	    char str_date_time[20];
		memset(str_date_time,0,20);
		sprintf(str_date_time,"%.4d-%.2d-%.2d %.2d:%.2d",psms_record->date.year,psms_record->date.mon, psms_record->date.day, psms_record->time.hour, psms_record->time.min);
		lcd_put_string(str_date_time);
	}

	lcd_goto_x_line(0, 2);
	lcd_put_string( (TABLEINFO_SMS_INBOX==table_idx)? get_combined_string(get_multi_string((char * *)text_from), ":", NULL) :get_combined_string(get_multi_string((char * *)text_to), ":", NULL));

	lcd_show_to_colum(  strlen(book_entry.name)>0? book_entry.name : psms_record->str_num, lcd_get_x(), lcd_get_y(),  SCREEN_WIDTH-2);
	
    show_soft_key_prompt(get_multi_string((char * *)text_null ),  get_multi_string((char * *)text_back));
  
    while ( 1 )
    {
	    if(1 == app_get_message( &EventPara) )
		{
	        if(EVENT_KEY == EventPara.eventTyp )
			{
				if(EventPara.sig_p.key_evt.isPressed)
				{
				    if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
					{
					    return;
				    }
				}
	        }
			else if(EVENT_TIMER== EventPara.eventTyp )
			{
                if(appsys.byte_need_exit > 0)
                {
                    return;
				}
			}
	    }
    }
}






void app_show_record_capacity(unsigned char table_idx)
{
    FlSignalBuffer EventPara;
    unsigned short rec_used = app_get_record_used(table_idx);
	unsigned char percent=0;
    char tmp_buf[32];

	memset(tmp_buf,0,32);
    strcpy(tmp_buf, get_multi_string((char * *)g_table_info[table_idx].p_flh));
	percent = rec_used *100 / g_table_info[table_idx].tb_info.rec_max_cnt;
	if(0 == percent)
	{
         percent= (rec_used >0)?1:0;
	}
	appsys.byte_need_redraw=0xFF;
	while(1)
    {
        if(appsys.byte_need_redraw>0)
        {
            lcd_clear_screen();
            show_caption(tmp_buf);
			lcd_put_rect(1, 12, SCREEN_WIDTH-1, 50);
			show_soft_key_prompt(get_multi_string((char * *)text_null),  get_multi_string((char * *) text_back ));
			lcd_put_percent_pillar(percent);

            appsys.byte_font_type=FONT_T_X_10;
            lcd_goto_xy(45, 20);
			lcd_put_string(get_multi_string((char * *)text_record_nums));
			lcd_put_string(":");
			memset(tmp_buf,0,32);
			sprintf(tmp_buf,"%d/%d", rec_used, g_table_info[table_idx].tb_info.rec_max_cnt);
			lcd_put_string(tmp_buf);

            lcd_goto_xy(45, 35);
			lcd_put_string(get_multi_string((char * *) text_capacity));
			lcd_put_string(":");
			memset(tmp_buf,0,32);
			sprintf(tmp_buf,"%d%%", percent);
			lcd_put_string(tmp_buf);

			
			appsys.byte_need_redraw=0;
		}
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						    return;
					    }						
					}
		        }
		    	else if( EVENT_TIMER == EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    return ;
					}
				}
		    }
		}
	}
}















void app_volumn_setting(unsigned char lvl_nums, unsigned char cur_lvl)
{
    unsigned char i=0;
	unsigned char box_width= (SCREEN_WIDTH-10)/lvl_nums/2;
	unsigned char box_grade= (SCREEN_HEIGHT-24-10)/lvl_nums;

	lcd_put_rect(1, 12, SCREEN_WIDTH-1, 50);

    for(i=0;i<lvl_nums;i++)
    {
        unsigned char left=(i*box_width*2 +box_width +((SCREEN_WIDTH-10)%lvl_nums) );
		unsigned char top =48-(i*box_grade);
		unsigned char right =left+box_width;
		unsigned char bottom=49;

        if(i<= cur_lvl)
        {    
            lcd_fill_area(0xFF, left, top , right-left+1, bottom-top+1);
		}
		else
		{		
		    lcd_fill_area(0x00, left, top , right-left+1, bottom-top+1);
            lcd_put_rect( left, top  , right ,bottom );
		}
	}
}

void app_set_lcd_contrast()
{
    FlSignalBuffer EventPara;
	unsigned char  lvl_nums=(MAX_LCD_CONTRAST-MIN_LCD_CONTRAST)/LCD_CONTRAST_STEP+1 ;
	unsigned char  cur_lvl =0;
    unsigned char  temp_volume=0;


    if( sysprop->byte_contrast_volume < MIN_LCD_CONTRAST  || sysprop->byte_contrast_volume>MAX_LCD_CONTRAST)
    {
        sysprop->byte_contrast_volume = DEFAULT_LCD_CONTRAST;
		app_save_prop();
	}

	cur_lvl =(sysprop->byte_contrast_volume-MIN_LCD_CONTRAST)/LCD_CONTRAST_STEP;
    temp_volume=sysprop->byte_contrast_volume;
	appsys.byte_need_redraw=0xFF;

    while(1)
    {
         if( appsys.byte_need_redraw>0)
         {
            if(0xFF == appsys.byte_need_redraw)
            {
				lcd_clear_screen();
				show_caption(get_multi_string((char * *) text_contrast_adjust));
				show_soft_key_prompt(get_multi_string((char * *) text_save), get_multi_string((char * *) text_back)  );
			}

			app_volumn_setting(lvl_nums, cur_lvl);
			

			temp_volume =MIN_LCD_CONTRAST+ LCD_CONTRAST_STEP*cur_lvl;
	        lcd_set_contrast_my(temp_volume);			
	        appsys.byte_need_redraw=0;
		    if(1 ==appsys.flag_genie_trace)DebugPrintf("cur_lvl =%d ,lcd contrast changed -->%d\r\n",cur_lvl, temp_volume); 
		 }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						    lcd_set_contrast_my(sysprop->byte_contrast_volume);
						    return;
					    }
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val  || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if(temp_volume != sysprop->byte_contrast_volume)
						    {
	                            sysprop->byte_contrast_volume =temp_volume;
								app_save_prop();
								msgok(get_multi_string((char * *)text_changed_successful));
								delay_ms(1000);
						    }
							return;
						}
						else if(TFKEY_UP==EventPara.sig_p.key_evt.key_val || TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val  )
						{
	                        if(cur_lvl<(lvl_nums-1))
	                        {
	                            cur_lvl++;
	                            appsys.byte_need_redraw=1;
							}
						}
						else if(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val || TFKEY_DOWN==EventPara.sig_p.key_evt.key_val )
						{
	                        if(cur_lvl>0)
	                        {
	                            cur_lvl--;
								appsys.byte_need_redraw=1;
							}						
						}
					}
		        }
				else if(EVENT_TIMER== EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    return;
					}
				}
		     }
		 }
	}
}



void app_set_ring_volume()
{
    FlSignalBuffer EventPara;
	unsigned char  lvl_nums=5;
	unsigned char  cur_lvl =sysprop->byte_ring_volume;



 	appsys.byte_need_redraw=0xFF;
    while(1)
    {
         if( appsys.byte_need_redraw>0)
         {
            if(0xFF == appsys.byte_need_redraw)
            {
			    lcd_clear_screen();
				show_caption(get_multi_string((char * *) text_ring_volume));
				show_soft_key_prompt(get_multi_string((char * *) text_save), get_multi_string((char * *) text_back));
			}
            
	        app_volumn_setting(lvl_nums, cur_lvl);



		    app_play_income_ring(sysprop->byte_income_ring, 0);
			
			
            if(cur_lvl < MAX_VOLUM_LEVEL)
            {
			    app_set_voice_volumn(VOCTYPE_INCOMING_CALL, music_volume [cur_lvl]);
            }
			appsys.byte_need_redraw=0;
		 }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						    break;
					    }
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if(cur_lvl!= sysprop->byte_ring_volume)
						    {
	                            sysprop->byte_ring_volume=cur_lvl;
								app_save_prop();
								msgok(get_multi_string((char * *)text_changed_successful));
								delay_ms(1000);
						    }
							break;
						}
						else if(TFKEY_UP==EventPara.sig_p.key_evt.key_val || TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val )
						{
	                        if(cur_lvl<(lvl_nums-1))
	                        {
	                            cur_lvl++;
	                            appsys.byte_need_redraw=1;
							}
						}
						else if(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val  || TFKEY_DOWN==EventPara.sig_p.key_evt.key_val )
						{
	                        if(cur_lvl>0)
	                        {
	                            cur_lvl--;
								appsys.byte_need_redraw=1;
							}						
						}
					}
		        }
				else if(EVENT_TIMER== EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    break;;
					}
				}
		     }
		 }
	}
	app_stop_music();	

}




void app_set_sms_volume()
{
    FlSignalBuffer EventPara;
	unsigned char  lvl_nums=5;
	unsigned char  cur_lvl =sysprop->byte_sms_volume;



 	appsys.byte_need_redraw=0xFF;
    while(1)
    {
         if( appsys.byte_need_redraw>0)
         {
            if(0xFF == appsys.byte_need_redraw)
            {
			    lcd_clear_screen();
				show_caption(get_multi_string((char * *) text_sms_volume));
				show_soft_key_prompt(get_multi_string((char * *) text_save), get_multi_string((char * *) text_back));
			}
            
	         app_volumn_setting(lvl_nums, cur_lvl);

			 if(cur_lvl < MAX_VOLUM_LEVEL)
			 {
				 app_set_voice_volumn(VOCTYPE_SMS_INCOMING, sms_volume [cur_lvl]);
			 }

             app_play_sms_ring(   sysprop->byte_sms_ring);
			 
			appsys.byte_need_redraw=0;
		 }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						    break;
					    }
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if(cur_lvl!= sysprop->byte_sms_volume)
						    {
	                            sysprop->byte_sms_volume=cur_lvl;
								app_save_prop();
								msgok(get_multi_string((char * *)text_changed_successful));
								delay_ms(1000);
						    }
							break;
						}
						else if(TFKEY_UP==EventPara.sig_p.key_evt.key_val  || TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val)
						{
	                        if(cur_lvl<(lvl_nums-1))
	                        {
	                            cur_lvl++;
	                            appsys.byte_need_redraw=1;
							}
						}
						else if(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val || TFKEY_DOWN==EventPara.sig_p.key_evt.key_val )
						{
	                        if(cur_lvl>0)
	                        {
	                            cur_lvl--;
								appsys.byte_need_redraw=1;
							}						
						}
					}
		        }
				else if(EVENT_TIMER== EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    break;;
					}
				}
		     }
		 }
	}
	app_stop_music();	
	
}




void app_set_speech_volume()
{
    FlSignalBuffer EventPara;
	unsigned char  lvl_nums=5;
	unsigned char  cur_lvl =sysprop->byte_talk_volume;



 	appsys.byte_need_redraw=0xFF;
    while(1)
    {
         if( appsys.byte_need_redraw>0)
         {
            if(0xFF == appsys.byte_need_redraw)
            {
			    lcd_clear_screen();
				show_caption(get_multi_string((char * *) text_speech_volume));
				show_soft_key_prompt(get_multi_string((char * *) text_save), get_multi_string((char * *) text_back));
			}
            
	        app_volumn_setting(lvl_nums, cur_lvl);

			if(cur_lvl < MAX_VOLUM_LEVEL)	
			{
			    app_set_voice_volumn(VOCTYPE_TALKING, speech_volume [cur_lvl]);
			}
			appsys.byte_need_redraw=0;
		 }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						    break;
					    }
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if(cur_lvl!= sysprop->byte_talk_volume)
						    {
	                            sysprop->byte_talk_volume=cur_lvl;
								app_save_prop();
								msgok(get_multi_string((char * *)text_changed_successful));
								delay_ms(1000);
						    }
							break;
						}
						else if(TFKEY_UP==EventPara.sig_p.key_evt.key_val || TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val )
						{
	                        if(cur_lvl<(lvl_nums-1))
	                        {
	                            cur_lvl++;
	                            appsys.byte_need_redraw=1;
							}
						}
						else if(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val  || TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)
						{
	                        if(cur_lvl>0)
	                        {
	                            cur_lvl--;
								appsys.byte_need_redraw=1;
							}						
						}
					}
		        }
				else if(EVENT_TIMER== EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    break;;
					}
				}
		     }
		 }
	}
}








void run_set_call_wait( void )
{
    unsigned char status=0;
	unsigned char def_pos=0;
	unsigned char flag_can_save =0;


    msg(get_combined_string(get_multi_string((char * *)text_query_status), "\n", get_multi_string((char * *)text_please_wait)));
    status = mu_query_callwait_status();
    def_pos = (1 ==status)?0:1;
	if (1 == choose_menu_option((char***)active_or_cancel, 2, &def_pos))
	{
        msg(get_multi_string((char * *)text_please_wait));
        if(0 == def_pos )
        {
			 if(1 ==mu_set_callwait(1))
			 {
                 msgok(get_multi_string((char * *)text_activate_successfully));  
				 flag_can_save=1;
			 }
			 else
			 {
                 msg_warn(get_multi_string((char * *) text_activate_failure));
			 }
		}
		else
		{
			 if(1 ==mu_set_callwait(0))
			 {
                 msgok(get_multi_string((char * *)text_deactivate_successfully));  
				 flag_can_save=1;
			 }
			 else
			 {
                 msg_warn(get_multi_string((char * *) text_deactivate_failure));
			 }
		}
		
		if(1 == flag_can_save)
		{
		    app_save_prop();
		}	
		delay_ms(1500);
	}
}






void run_set_call_divert( void )
{
    unsigned char  divert_mode;
    T_CALL_DIVERT_STATUS * divert_status;
	unsigned short pos_info =0;
	unsigned char def_pos=0;



	mu_query_calldivert_status();
  
    while ( 1 )
    {
        switch ( app_menu_select((char ***)call_diverting, 4, &pos_info) )
        {
            case 1:
				
                divert_mode = DIVERT_ALWAYS;
                break;
            case 2:
                divert_mode = DIVERT_BUSY;
                break;
            case 3:
                divert_mode = DIVERT_NO_REPLY;
                break;
            case 4:
                divert_mode = DIVERT_UNREACHABLE;
                break;

            default:
                return;
                break;
        }

        divert_status = &appsys.DIVT_LIST[divert_mode -1];
		

        def_pos = (1== divert_status->status)?0:1;
	    if (1 == choose_menu_option((char *** )active_or_cancel, 2, &def_pos))
	    {
             if(0 == def_pos)
             {
                  if (1 == wnd_input_text(divert_status->num, get_multi_string((char * *) text_enter_number),BOOK_ENTRY_NUM_FIELD_SIZE , IN_FIXED, EDIT_STYLE_DIGITALONLY))
                  {
                      msg(get_multi_string((char * *)text_please_wait));
				      if ( mu_active_calldivert(divert_mode, divert_status->num) )
	                  {
	                      msgok(get_multi_string((char * *)text_activate_successfully));   
	                      delay_ms(1500);
	                  }
	                  else
	                  {
	                      msg_warn(get_multi_string((char * *)text_activate_failure));
	                      delay_ms(1500);
	                  }
				  }
			 }
			 else
			 {
                  msg(get_multi_string((char * *)text_please_wait));
                  if ( mu_dactive_calldivert(divert_mode) )
                  {
                      msgok(get_multi_string((char * *)text_deactivate_successfully));        
                      delay_ms(1500);
                  }
                  else
                  {
                      msg_warn(get_multi_string((char * *)text_deactivate_failure));
                      delay_ms(1500);
                  }
			 }
	    }
    }
}




void run_call_barring_set( void )
{
    unsigned char  bk_val=0, * p_var=0;
 	unsigned short pos_info =0;
	unsigned char  def_pos=0;

    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_call_barring_settings,3,&pos_info) )
        {
            case 1:
                p_var = &(sysprop->flag_outcall_allow);
                break;

            case 2:
                p_var = &(sysprop->flag_internationalcall_allow);
                break;

            case 3:
                p_var = &(sysprop->flag_incomecall_allow);
                break;

            default:
                return;
                break;
        }


	    bk_val =  def_pos= *p_var;

	    if (1 == choose_menu_option((char ***)forbit_or_allow, 2, &def_pos))
	    {
            if( bk_val  != def_pos )
            {
                  *p_var = def_pos;
				  app_save_prop();
				  msgok(get_multi_string((char * *)text_changed_successful));
				  delay_ms(1500);
			}
	    }
    }
}

unsigned char get_dial_delay()
{
    unsigned char nret =0;
    switch(sysprop->byte_call_delay_s)
    {
       case 0:
	   	    nret=0;
			break;
	   case 1:
	     	nret=5;
			break;
	   case 2:
	   	    nret=6;
			break;
	   case 3:
	   	    nret=7;
			break;
	   case 4:
	     	nret=8;
		    break;

	   default:
	   	    nret =5;
	   	break;
	}
    return nret;
}







void app_set_baseinfo()
{
	  unsigned char def_pos=(sysprop->flag_lock_operator==1)?0:1;

	  if (1 == choose_menu_option((char *** )active_or_cancel, 2, &def_pos))
	  {
	      if(def_pos  != ((sysprop->flag_lock_operator ==1)?0:1))
	      {
	           sysprop->flag_lock_operator  = (1 ==def_pos)?0:1;
			   app_save_prop();
			   msgok(get_multi_string((char * *)text_changed_successful));
			   delay_ms(1500);			   
		  }
	  }
}




void app_view_version( void )
{
    FlSignalBuffer EventPara;

    appsys.byte_need_redraw=0xFF;

    while(1)
    {
         if(appsys.byte_need_redraw>0)
         {
		     lcd_clear_screen();
			 show_caption(get_multi_string((char * *) text_software_version));
             show_soft_key_prompt( get_multi_string((char * *) text_others), get_multi_string((char * *) text_back));

			 lcd_goto_xy(0, 11);
			 lcd_draw_horizontal(SCREEN_WIDTH-1);


		     appsys.byte_font_type= FONT_T_X_10;
		     lcd_goto_x_line(0, 1);
			 lcd_put_string((char *)phone_type);
		     lcd_goto_x_line(0, 2);
			 lcd_put_string("OS:");
			 lcd_put_string((char *)soft_version);
			 lcd_put_string(" ");
			 lcd_put_string((char *)(soft_ver+4));
			 lcd_put_string(" ");
			 lcd_put_string((char *)soft_time);	

		     lcd_goto_x_line(0, 3);
			 lcd_put_string("Module:");

             lcd_put_string("SC9820");
			 appsys.byte_need_redraw=0;
         }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {
					 if(EventPara.sig_p.key_evt.isPressed)
					 {
					     if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						 {
						     return;
					     }
						 else if( (TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val))
						 {
						     msg(get_multi_string((char * *) text_please_wait));
						     app_query_hw_info(500);
						     wnd_show_notice(get_multi_string((char * *)text_prompt), appsys.str_andr_ver, NOTICE_TYPE_INFO,NEED_SOFT_KEY_INPUT);
					     }
					 }
		         }
				 else  if(EVENT_TIMER== EventPara.eventTyp )
				 {
	                 if(appsys.byte_need_exit > 0)
	                 {
	                      return;
					 }
				 }
		     }
         }
    }
}

unsigned char  app_user_change_simpin( void )
{
    char  sOld[10];
	char sNew1[10];
	char sNew2[10];
	unsigned char nret=0;
	memset(sOld,0,10);
	memset(sNew1,0,10);
	memset(sNew2,0,10);

    if (0== wnd_input_password(sOld, get_multi_string((char * *)text_enter_old_pin), 8))
    {
        return 0;
	}

    if (0== wnd_input_password(sNew1,get_multi_string((char * *)text_enter_new_pin), 8))
    {
        return 0;
	}
	
    if (0== wnd_input_password(sNew2,get_multi_string((char * *)text_enter_new_again), 8))
    {
        return 0;
	}	

    if ( 0 != strcmp(sNew1,sNew2))
    {
        msg_warn(get_multi_string((char * *)text_pin_disaccord));  
        delay_ms(1000);
		return 0;
    }
    msg(get_multi_string((char * *) text_please_wait));
	
    if (1 == mu_change_pin(sOld,sNew1) )
    {
        msgok(get_multi_string((char * *)text_pin_code_changed));
		strncpy(sysprop->str_simpin_1, sNew1,8);
		app_save_prop();
		delay_ms(1000);
		nret = 1;
    }
    else
    {
        msg_warn(get_multi_string((char * *)text_failed));
		nret = 0;
    } 
	delay_ms(1500);

	return nret;
}

void app_pin_setting( void )
{
	unsigned char flag_active=0;
	unsigned short pos_info=0;

    char *temp_menu[3][4];
	char ** pmenus[3];
	unsigned char i=0;

	temp_menu[0][0]=(char *)text_select[0];
	temp_menu[0][1]=(char *)text_select[1];
    temp_menu[0][2]=(char *)text_select[2];
    temp_menu[0][3]=(char *)text_select[3];
	
	temp_menu[1][0]=(char *)text_set_pin_lock[0];
	temp_menu[1][1]=(char *)text_set_pin_lock[1];
    temp_menu[1][2]=(char *)text_set_pin_lock[2];
    temp_menu[1][3]=(char *)text_set_pin_lock[3];	
	
	temp_menu[2][0]=(char *)text_cancel_pin_protect[0];
	temp_menu[2][1]=(char *)text_cancel_pin_protect[1];
    temp_menu[2][2]=(char *)text_cancel_pin_protect[2];
    temp_menu[2][3]=(char *)text_cancel_pin_protect[3];

	
	for(i=0;i<3;i++)
	{
        pmenus[i]=temp_menu[i];
	}


    pos_info = ( 1== (flag_active= mu_is_pin_active()))?0:1;  //(0<<8+0<<0):(0<<8+1<<0)
	
	switch( app_menu_select((char ***)pmenus, 2, &pos_info))
	{
        case 1:
			if(0 == flag_active)
			{
			     char tmp_pin[10];
				 memset(tmp_pin,0,sizeof(tmp_pin));
                 mu_user_active_pin(tmp_pin, 8);
			}
			else
			{
               msg(get_multi_string((char * *)text_pin_is_already_actived));
			   delay_ms(1000); 
			}			
			break;
		case 2:
			if(1 == flag_active)
			{
               mu_user_cancel_pin();
			}
			else
			{
               msg(get_multi_string((char * *)text_pin_is_already_canceled));
			   delay_ms(1000); 
			}
			break;
		default:
			return ;
			break;

	}

}



unsigned char app_bluetooth_enabled_query()
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_459=0;

   send_android_command(andr_build_449_query_bluetooth_enabled(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_459 >0)
       {
            return 1;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }
   return 0;
}


unsigned char app_bluetooth_device_query(unsigned char * def_pos)
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_452=0;
   appsys.byte_blue_cnt=0;
   memset(appsys.BLUE_LIST,0,sizeof(appsys.BLUE_LIST));
   send_android_command(andr_build_442_query_bluetooth_devices(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_452 >0)
       {
            break;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   if(appsys.byte_blue_cnt >0)
   {
	   char * temp_menus[MAX_BLUE_WIFI_CNT][4];
	   char ** pmenus[MAX_BLUE_WIFI_CNT];
	
	   unsigned char i=0;
	   
	   temp_menus[0][0]=(char *)(text_bluetooth_select[0]);
	   temp_menus[0][1]=(char *)(text_bluetooth_select[1]);   
	   temp_menus[0][2]=(char *)(text_bluetooth_select[2]);  
	   temp_menus[0][3]=(char *)(text_bluetooth_select[3]);   

	   
	   for(i =0;i< appsys.byte_blue_cnt;i++)
	   {
	       temp_menus[ i+1][0]=temp_menus[i+1][1]= temp_menus[i+1][2]= temp_menus[i+1][3]= appsys.BLUE_LIST[i];

		   if(0 == strcmp(sysprop->str_cur_bluetooth,appsys.BLUE_LIST[i] ))
		   {
               *def_pos = i;
		   }
	   }


	   for(i=0;i< (appsys.byte_blue_cnt+1);i++)
	   {
	       pmenus[i]=(char **)temp_menus[i];
	   }


	   if(1 == choose_menu_option((char ***)pmenus, appsys.byte_blue_cnt, def_pos))
	   {
	       strcpy(sysprop->str_cur_bluetooth, appsys.BLUE_LIST[ *def_pos]);
		   app_save_prop();
	       return 1;
	   }   
   }

   return 0;
}


void app_bluetooth_setting( void )
{
	unsigned short pos_info=0;

    if(1 == app_bluetooth_enabled_query())
    {
        sysprop->flag_blue_enabled = appsys.byte_blueret_state=='1'?1:0;

		switch( app_menu_select((char ***)mem_bluetooth_setting, (sysprop->flag_blue_enabled==1)?2:1, &pos_info))
		{
	        case 1:
			{
				unsigned char def_pos= (0 ==sysprop->flag_blue_enabled)?1:0;
	            if(1 == choose_menu_option( (char ***)active_or_cancel, 2, &def_pos))
	            {	
	       	          if(def_pos  != ((sysprop->flag_blue_enabled ==1)?0:1))
	                  {
	                       char tmp_cmd[128];
						   memset(tmp_cmd,0,sizeof(tmp_cmd));
	                       sysprop->flag_blue_enabled  = (1 ==def_pos)?0:1;
	                       send_android_command(andr_build_449_setup_bluetooth_enabled(tmp_cmd, sysprop->flag_blue_enabled));
						  
						   msgok(get_multi_string((char * *)text_changed_successful));
						   app_save_prop();
						   delay_ms(1000);
					  }
	            }
	        }
				break;
			case 2:
				{
					unsigned char def_pos=0;
				    app_bluetooth_device_query(&def_pos);
				}
				break;
			default:
				return ;
				break;

		}
    }

}




unsigned char app_wifi_enabled_query()
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_45A=0;

   send_android_command(andr_build_44A_query_wifi_enabled(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_45A >0)
       {
            return 1;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }
   return 0;
}


unsigned char app_wifi_device_query(unsigned char * def_pos)
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_453=0;
   appsys.byte_wifi_cnt=0;
   memset(appsys.WIFI_LIST,0,sizeof(appsys.WIFI_LIST));
   send_android_command(andr_build_443_query_wifi_devices(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_453 >0)
       {
            break;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   if(appsys.byte_wifi_cnt >0)
   {
	   char * temp_menus[MAX_BLUE_WIFI_CNT][4];
	   char ** pmenus[MAX_BLUE_WIFI_CNT];
	
	   unsigned char i=0;
	   
	   temp_menus[0][0]=(char *)(text_wifi_select[0]);
	   temp_menus[0][1]=(char *)(text_wifi_select[1]);   
	   temp_menus[0][2]=(char *)(text_wifi_select[2]);   
       temp_menus[0][3]=(char *)(text_wifi_select[3]);  
	   
	   for(i =0;i< appsys.byte_wifi_cnt;i++)
	   {
	       temp_menus[ i+1][0]=temp_menus[i+1][1] =temp_menus[i+1][2]= temp_menus[i+1][3]= appsys.WIFI_LIST[i];

		   if(0 == strcmp(sysprop->str_cur_wifi,appsys.WIFI_LIST[i] ))
		   {
               *def_pos = i;
		   }
	   }


	   for(i=0;i< (appsys.byte_wifi_cnt+1);i++)
	   {
	       pmenus[i]=(char **)temp_menus[i];
	   }


	   if(1 == choose_menu_option((char ***)pmenus, appsys.byte_wifi_cnt, def_pos))
	   {
	       strcpy(sysprop->str_cur_wifi, appsys.WIFI_LIST[ *def_pos]);
		   app_save_prop();
	       return 1;
	   }   
   }

   return 0;
}


void app_wifi_setting( void )
{
	unsigned short pos_info=0;

    if(1 == app_wifi_enabled_query())
    {
        sysprop->flag_wifi_enabled = appsys.byte_wifiret_state=='1'?1:0;
	
		switch( app_menu_select((char ***)mem_wifi_setting, sysprop->flag_wifi_enabled?2:1, &pos_info))
		{
	        case 1:
			{
				unsigned char def_pos= (0 ==sysprop->flag_wifi_enabled)?1:0;
	            if(1 == choose_menu_option( (char ***)active_or_cancel, 2, &def_pos))
	            {	
	       	          if(def_pos  != ((sysprop->flag_wifi_enabled ==1)?0:1))
	                  {
	                       char tmp_cmd[128];
						   memset(tmp_cmd,0,sizeof(tmp_cmd));
	                       sysprop->flag_wifi_enabled  = (1 ==def_pos)?0:1;
	                       send_android_command(andr_build_44A_setup_wifi_enabled(tmp_cmd, sysprop->flag_wifi_enabled));
						  
						   msgok(get_multi_string((char * *)text_changed_successful));
						   app_save_prop();
						   delay_ms(1000);
					  }
	            }
	        }
				break;
			case 2:
				{
					unsigned char def_pos=0;
				    app_wifi_device_query(&def_pos);
				}
				break;
			default:
				return ;
				break;

		}
    }

}





unsigned char hex_to_byte(char hex)
{
    if( hex>='0' && hex<='9')
    {
        return hex-0x30;
	}
	else if(hex>='A' && hex<='F')
	{
        return hex-'A'+10;
	}
	else if(hex>='a' && hex<='f')
	{
        return hex-'a'+10;
	}
	return 0;
}
unsigned long hex_str_to_long(char * hex_str)
{
    unsigned char hex_len= strlen(hex_str);
	return hex_str_to_long_ex(hex_str,  hex_len);
}
unsigned long hex_str_to_long_ex(char * hex_str, unsigned char hex_len)
{
	unsigned char i=0;
	unsigned long ret_long=0;
	hex_len= (hex_len>8)? 8: hex_len;
    hex_len= (hex_len>strlen(hex_str))?strlen(hex_str): hex_len;
	
	for(i=0;i<hex_len;i++)
	{
        if(0 ==i)
        {
            ret_long = hex_to_byte(hex_str[i]);
		}
		else
		{
            ret_long<<=4;
			ret_long += hex_to_byte(hex_str[i]);
		}
	}
	return ret_long;
}




unsigned char check_if_having_new_sms()
{
	signed char table_idx = TABLEINFO_SMS_INBOX;
	T_SMS_RECORD sms_record;
	unsigned char flag_over=0;
    unsigned short i=0,cell_id=0, tmp_cnt=0, cmp_cnt=0;
	unsigned char * huge_buf;
	unsigned char * tbidx_buf;
	unsigned short  rec_cnt =0;
    unsigned long temp_st=app_get_tick_count();



	tbidx_buf =app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);       
    if(1 ==flash_cell_read(table_idx  + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, tbidx_buf))
	{
	    unsigned short * pshort_buf=(unsigned short *) tbidx_buf;
	    rec_cnt= pshort_buf[0];
        if(1 ==appsys.flag_genie_trace)DebugPrintf("check_if_having_new_sms tot rec_cnt=%d\r\n",rec_cnt);
	    if(rec_cnt>0)
	    {
			huge_buf=app_alloc_memory(g_table_info[table_idx].cell_info.cell_len);       
			for(cell_id=0; cell_id <g_table_info[table_idx].cell_info.cell_cnt; cell_id++)
	        {
	             unsigned short st,et;			 
	             memset(huge_buf,0, g_table_info[table_idx].cell_info.cell_len );
			     flash_cell_read(cell_id +g_table_info[table_idx].cell_info.cell_start, g_table_info[table_idx].cell_info.cell_len, huge_buf);

				 st=cell_id *  (g_table_info[table_idx].cell_info.cell_max_rec_cnt);
				 et=((cell_id +1)*(g_table_info[table_idx].cell_info.cell_max_rec_cnt));
	 

				 for ( i=st; i<et; i++ )
			     {
			         if(1 == check_if_rec_in_table_index(pshort_buf,rec_cnt ,  (i+1) ) )
			         {
					     memcpy(&sms_record,  &huge_buf[  ( i % (g_table_info[table_idx].cell_info.cell_max_rec_cnt)) *g_table_info[table_idx].tb_info.rec_len   ], g_table_info[table_idx].tb_info.rec_len);

						 if(0 == sms_record.sms_flag.flag_read)
                         {	
						     tmp_cnt++;
                         }
						 cmp_cnt++;
						 if(cmp_cnt >= rec_cnt)
						 {
                             flag_over =1;
							 break;
						 }
					 }
				 }

				 if(1 == flag_over)
				 {
                      break;
				 }
	        }
		    app_free_memory(huge_buf);huge_buf=NULL;
	    }
	}
	app_free_memory(tbidx_buf);tbidx_buf=NULL;

	if(1 ==appsys.flag_genie_trace)DebugPrintf("unread sms cnt=%d TM: %dms\r\n",tmp_cnt,get_duration_tick(temp_st));
    return tmp_cnt>0?1:0;
}


unsigned char check_if_having_missed_call()
{
	signed char table_idx = TABLEINFO_MISSED_CALL;
	T_CALL_RECORD call_record;
	unsigned char flag_over=0;
    unsigned short i=0,cell_id=0, tmp_cnt=0,cmp_cnt=0;
	unsigned char * huge_buf;
	unsigned char * tbidx_buf;
	unsigned short  rec_cnt =0;
    unsigned long temp_st=app_get_tick_count();



	tbidx_buf =app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);    

 

    if(1 ==flash_cell_read(table_idx  + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, tbidx_buf))
	{

	    unsigned short * pshort_buf=(unsigned short *) tbidx_buf;
	    rec_cnt= pshort_buf[0];
		if(1 ==appsys.flag_genie_trace)DebugPrintf("check_if_having_missed_call rec_cnt=%d\r\n",rec_cnt);
	    if(rec_cnt>0)
	    {
    
			huge_buf=app_alloc_memory(g_table_info[table_idx].cell_info.cell_len);       
			for(cell_id=0; cell_id <g_table_info[table_idx].cell_info.cell_cnt; cell_id++)
	        {

	             unsigned short st,et;			 
	             memset(huge_buf,0, g_table_info[table_idx].cell_info.cell_len );
			     flash_cell_read(cell_id +g_table_info[table_idx].cell_info.cell_start, g_table_info[table_idx].cell_info.cell_len, huge_buf);

				 st=cell_id *  (g_table_info[table_idx].cell_info.cell_max_rec_cnt);
				 et=((cell_id +1)*(g_table_info[table_idx].cell_info.cell_max_rec_cnt));

				 for ( i=st; i<et; i++ )
			     {

			         if(1 == check_if_rec_in_table_index(pshort_buf,rec_cnt ,  (i+1) ) )
			         {
					     memcpy(&call_record,  &huge_buf[  ( i % (g_table_info[table_idx].cell_info.cell_max_rec_cnt)) *g_table_info[table_idx].tb_info.rec_len   ], g_table_info[table_idx].tb_info.rec_len);
						 if(1 == call_record.not_answer)
						 {
                             tmp_cnt++;
						 }
						 cmp_cnt ++;
						 if(cmp_cnt >= rec_cnt)
						 {
                             flag_over =1;
							 break;
						 }
					 }
				 }
				 if(1 == flag_over)
				 {
                     break;
				 }
	        }
		    app_free_memory(huge_buf);huge_buf=NULL;
	    }
	}
	app_free_memory(tbidx_buf);tbidx_buf=NULL;


	if(1 ==appsys.flag_genie_trace)DebugPrintf("missed call cnt=%d TM: %dms\r\n",tmp_cnt,get_duration_tick(temp_st));

    return tmp_cnt>0?1:0;
}



void app_play_dial_tone(unsigned char tone_id,  unsigned char max_time)
{
    char tmp_cmd[128];

	memset(tmp_cmd,0,sizeof(tmp_cmd));
	
	app_enable_speaker();
 	if(1 ==appsys.flag_genie_trace)DebugPrintf("play dial tone: music_id=%d max_time=%d\r\n", tone_id,  max_time );

	send_android_command( andr_build_435_play_voice(tmp_cmd, tone_id));

	
	appsys.byte_last_music_id= tone_id;
	appsys.byte_music_max_time=max_time;

}

void app_play_income_ring(unsigned char ring_id,  unsigned char max_time)
{
    if(ring_id < MAX_INCOME_RING_CNT)
    {
	    char tmp_cmd[128];

		memset(tmp_cmd,0,sizeof(tmp_cmd));
		
		app_enable_speaker();
	 	if(1 ==appsys.flag_genie_trace)DebugPrintf("play income music_id=%d max_time=%d\r\n", music_array[ring_id],  max_time );

		send_android_command( andr_build_435_play_voice(tmp_cmd, music_array[ring_id]));

		
		appsys.byte_last_music_id= music_array[ring_id];
		appsys.byte_music_max_time=max_time;
    }
	else
	{
          if(1 == appsys.flag_genie_trace)DebugPrintf("error ring id : %d\r\n", ring_id);
	}

}

void app_play_alarm_ring(unsigned char alarm_id,  unsigned char max_time)
{
    if(alarm_id < MAX_ALARM_RING_CNT)
    {
	    char tmp_cmd[128];

		memset(tmp_cmd,0,sizeof(tmp_cmd));
		
		app_enable_speaker();
	 	if(1 ==appsys.flag_genie_trace)DebugPrintf("play alarm music_id =%d max_time=%d\r\n", alarm_id,  max_time );

		send_android_command( andr_build_435_play_voice(tmp_cmd,  alarm_array[alarm_id]));

		
		appsys.byte_last_music_id= alarm_array[alarm_id];
		appsys.byte_music_max_time=max_time;
    }
	else
	{
         if(1 == appsys.flag_genie_trace)DebugPrintf("error alarm_id : %d\r\n", alarm_id);
	}

}


void app_play_sms_ring(unsigned char smsvoc_id)
{
    if(smsvoc_id < MAX_SMS_RING_CNT)
    {
	    char tmp_cmd[128];

		memset(tmp_cmd,0,sizeof(tmp_cmd));
		
		app_enable_speaker();
	 	if(1 ==appsys.flag_genie_trace)DebugPrintf("play smsring music_id=%c\r\n", smsring_array[smsvoc_id] );
		send_android_command( andr_build_43C_play(tmp_cmd, smsring_array[smsvoc_id]));

	 	appsys.byte_last_music_id= smsring_array[smsvoc_id];
		appsys.byte_music_max_time=0;
    }
	else
	{
        if(1 == appsys.flag_genie_trace)DebugPrintf("error smsvoc_id : %d\r\n", smsvoc_id);
	}
}




void app_set_voice_volumn(unsigned char voice_type,  unsigned char volumn)
{
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
 	if(1 ==appsys.flag_genie_trace)DebugPrintf("play volumn: voice_type=%d volumn=%d\r\n", voice_type, volumn);
	send_android_command(andr_build_43A_voc_level(tmp_cmd,voice_type, volumn));
}



void app_stop_music()
{
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
    if(1 ==appsys.flag_genie_trace)DebugPrintf("stop music: music_id=%d \r\n", appsys.byte_last_music_id);

	send_android_command(andr_build_436_stop_voice(tmp_cmd));
	appsys.byte_last_music_id=0xFF;
	appsys.byte_music_max_time=0;

}




void process_recv_call()
{
    FlSignalBuffer EventPara;
    T_CALL_RECORD call_record;

    unsigned char  idx_pos=0,last_x=0xFF;
	unsigned short cnt=0;
	
    unsigned char xpos[]={36,32,28,24};

	unsigned short tmp_pos=0;
	unsigned short cur_pos=0;
	unsigned char flag_accept=0;

	unsigned char flag_played_music_incom=0;


	
    memset(&call_record,0,sizeof(T_CALL_RECORD));
    appsys.flag_call_comming=1;
    appsys.flag_no_carrier =0;

	appsys.byte_need_redraw=0xFF;

    strcpy(call_record.num,appsys.str_incom_number);
	
	if(1)
	{
	     T_BOOK_ENTRY book_entry;
		 call_record.year=appsys.struct_date.year;
		 call_record.month=appsys.struct_date.mon;
		 call_record.day=appsys.struct_date.day;
		 call_record.hour=appsys.struct_time.hour;
		 call_record.minute=appsys.struct_time.min;
		 call_record.second=appsys.struct_time.sec;

		
		 if(1 ==appsys.flag_playing_tone)
		 {
             mu_stop_dialtone();
		 }

		 memset(&book_entry,0, sizeof(T_BOOK_ENTRY));
		 find_book_entry_by_numbers(call_record.num, &book_entry);
		 
		 app_enable_speaker();


         flag_played_music_incom=1;
		 
		 
		 while(1)
	     {
             if(appsys.byte_need_redraw>0)
             {
                 appsys.byte_need_redraw=0;
				 appsys.byte_font_type=FONT_T_X_10;
				 
				     lcd_clear_screen();
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);

			
				 show_caption(get_multi_string((char * *)text_incoming_call));
				 lcd_goto_xy(5, 25);
				 show_bitmap( BMP_MAIN_CALLIN_BIN);
				 lcd_goto_xy(45, 25);

				 if(strlen(call_record.num))
				 {
				     lcd_put_string(call_record.num);
				 }
				 else
				 {
                     lcd_put_string(get_multi_string((char * *)text_no_name));
				 }
				 
				 if(strlen(book_entry.name)>0)
				 {
				     char tmp_buf[32];
					 memset(tmp_buf,0,sizeof(tmp_buf));
					 sprintf(tmp_buf," (%s)", book_entry.name);
					 if(get_str_dots_width(tmp_buf) >100)
					 {
					     show_caption(book_entry.name);
					 }
					 else
					 {
                         show_caption(tmp_buf);
					 }
				 }
				 else
				 {
                     show_caption(get_multi_string((char * *)text_incoming_call));
				 }
				 
				 show_soft_key_prompt(get_multi_string((char * *) text_accept) , get_multi_string((char * *) text_reject));
			 }
			 else
			 {
			     if(1 == app_get_message( &EventPara) )
				 {
			         if(EVENT_KEY == EventPara.eventTyp )
					 {
					     if( 1 ==EventPara.sig_p.key_evt.isPressed)
					     {
                             if( TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val || TFKEY_EXIT== EventPara.sig_p.key_evt.key_val )
                             {
                                  //stop music 
                                  mu_hook_on();
                                  call_record.not_answer=1;
								  app_record_append(TABLEINFO_CALL_IN, (unsigned char *)&call_record, REC_PREFIX_APPEND);
								  flag_accept=0;
								  break;
							 }
							 else if(TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
							 {
                                  appsys.flag_hand_free=1;
								  if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
								  {
								       flag_accept=1;
								  }
								  break;
 						     }
							 else if(TFKEY_HANDFREE== EventPara.sig_p.key_evt.key_val)
							 {
							      appsys.flag_hand_free=1;
						          if(1 ==set_voice_path_by_action(VOICE_ACTION_HANDSET))
						          {
									  flag_accept=1;
						          }
								  break;
							 }
						 }
			         }
					 else if( EVENT_HANDSET == EventPara.eventTyp )
					 {
						 if(1 ==set_voice_path_by_action(VOICE_ACTION_HANDSET))
						 {
                              flag_accept=1;
							  break;
						 }
					 }
					 else if( EVENT_MODULE== EventPara.eventTyp )
					 {
					  
					 }
					 else if( EVENT_TIMER== EventPara.eventTyp )
					 {

					     if( (++cnt%2 ==0))
					     {
							 if(0xFF !=last_x)
							 {
							    lcd_goto_xy(last_x, 27);
								lcd_clear_line(last_x+8);
							 }

							 lcd_goto_xy(xpos[idx_pos], 27);
							 last_x =xpos[idx_pos];
							 show_bitmap(BMP_ARROW_LEFT_BIN);
							 idx_pos++;

							 if(idx_pos>3)
							 {
							    idx_pos =0;
							 }
					     }


					     if(1 == appsys.flag_no_carrier )
                         {
                              call_record.not_answer=1;
						      app_record_append(TABLEINFO_MISSED_CALL, (unsigned char *)&call_record, REC_PREFIX_APPEND);
                              appsys.flag_having_missed_call=1;
							  app_stop_music();
							  break;
						 }
					 }
				 } 
			 }
	   	 }

		 //app_stop_music();  //removed by JEOWILL 20180521
		 
	 	 if(1 == flag_accept)
		 {
		      mu_answer_call();
			  call_record.len_hour =call_record.len_minute = call_record.second =0;
			  
			  memset(&(appsys.back_rating),0,sizeof(T_BACKGROUND_RATING));
			  appsys.back_rating.pcall_record=&call_record;
			  appsys.back_rating.flag_end_dial=0;
			  appsys.back_rating.flag_start_rating=0;
			  appsys.back_rating.flag_last_duration=0;
			  appsys.back_rating.flag_money_minder=0;
			  appsys.back_rating.dword_last_minter_tick=0;		


			  call_record.year=appsys.struct_date.year;
			  call_record.month=appsys.struct_date.mon;
			  call_record.day=appsys.struct_date.day;
			  call_record.hour=appsys.struct_time.hour;
			  call_record.minute=appsys.struct_time.min;
			  call_record.second=appsys.struct_time.sec;


			  
			  appsys.flag_talk_rating=1;
			  talk_phone_call(&call_record, book_entry.name);
			  appsys.flag_talk_rating=0;
			  call_record.not_answer=0;
		      app_record_append(TABLEINFO_CALL_IN, (unsigned char *)&call_record, REC_PREFIX_APPEND);
		 }	
    }
	appsys.flag_no_carrier =0;
	appsys.flag_call_comming=0;


	appsys.flag_talk_rating=0;
	appsys.byte_need_redraw=0xFF;
}





void app_run_super_settings( void )
{
    unsigned short pos_info=0;
    appsys.flag_super_mode=1;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_super_set,4,&pos_info) )
        {
            case 1:
				app_view_version();
                break;      
            case 2:
                app_run_security_settings();
                break;
            case 3:
                app_run_factory_settings();
                break;	
            case 4:
				appsys.flag_hardware_test =1;
                app_hardware_test();
				appsys.flag_hardware_test =0;
                break;	
			case 5:
				app_show_cimis();
				break;	

			default:

				appsys.flag_super_mode=0;
				return;
				break;
        }
    }
}

void app_run_factory_settings( void )
{
 	if(1 == choose_yes_no(get_multi_string((char * *) text_confirm), get_multi_string((char * *)text_restore_factory_settings)))
	{
	    msg(get_multi_string((char * *)text_please_wait));
	    load_from_factory();

		reset_table_index(TABLEINFO_CALL_OUT);
		reset_table_index(TABLEINFO_CALL_IN);
		reset_table_index(TABLEINFO_MISSED_CALL);
		reset_table_index(TABLEINFO_PHONEBOOK);
		reset_table_index(TABLEINFO_SMS_INBOX);
		reset_table_index(TABLEINFO_SMS_OUTBOX);
		reset_table_index(TABLEINFO_SMS_DRAFT);		
		
		

        msgok(get_multi_string((char * *)text_restore_successful));		
		delay_ms(2000);
		app_system_reset();
	}
}


#define MUSIC_NUMS 10
unsigned char app_music_show(unsigned char * def_pos)
{
   char * temp_menus[12][4];
   char ** pmenus[12];
   unsigned char i=0;
   
   temp_menus[0][0]=(char *)(text_music_select[0]);
   temp_menus[0][1]=(char *)(text_music_select[1]);   
   temp_menus[0][2]=(char *)(text_music_select[2]);  
   temp_menus[0][3]=(char *)(text_music_select[3]);  
   
   temp_menus[1][0]=temp_menus[1][1]=temp_menus[1][2]=temp_menus[1][3]="AUDIO_1";
   temp_menus[2][0]=temp_menus[2][1]=temp_menus[2][2]=temp_menus[2][3]="AUDIO_2";
   temp_menus[3][0]=temp_menus[3][1]=temp_menus[3][2]=temp_menus[3][3]="AUDIO_3";
   temp_menus[4][0]=temp_menus[4][1]=temp_menus[4][2]=temp_menus[4][3]="AUDIO_4";
   temp_menus[5][0]=temp_menus[5][1]=temp_menus[5][2]=temp_menus[5][3]="AUDIO_5";
   temp_menus[6][0]=temp_menus[6][1]=temp_menus[6][2]=temp_menus[6][3]="AUDIO_6";
   temp_menus[7][0]=temp_menus[7][1]=temp_menus[7][2]=temp_menus[7][3]="AUDIO_7";
   temp_menus[8][0]=temp_menus[8][1]=temp_menus[8][2]=temp_menus[8][3]="AUDIO_8";
   temp_menus[9][0]=temp_menus[9][1]=temp_menus[9][2]=temp_menus[9][3]="AUDIO_9";
   temp_menus[10][0]=temp_menus[10][1]=temp_menus[10][2]=temp_menus[10][3]="AUDIO_10";



   for(i=0;i<MUSIC_NUMS+1;i++)
   {
       pmenus[i]=(char **)temp_menus[i];
   }


   if(1 == choose_menu_option_ex((char ***)pmenus, MUSIC_NUMS, def_pos, 0, MENU_STYLE_MUSIC_VOICE))
   {
       return 1;
   }   
   return 0;
}




unsigned char app_alarm_show(unsigned char * def_pos)
{
   char * temp_menus[12][4];
   char ** pmenus[12];
   unsigned char i=0;
   
   temp_menus[0][0]=(char *)(text_music_select[0]);
   temp_menus[0][1]=(char *)(text_music_select[1]);   
   temp_menus[0][2]=(char *)(text_music_select[2]);    
   temp_menus[0][3]=(char *)(text_music_select[3]);   
   
   temp_menus[1][0]=temp_menus[1][1]=temp_menus[1][2]=temp_menus[1][3]="ALARM_1";
   temp_menus[2][0]=temp_menus[2][1]=temp_menus[2][2]=temp_menus[2][3]="ALARM_2";
   temp_menus[3][0]=temp_menus[3][1]=temp_menus[3][2]=temp_menus[3][3]="ALARM_3";
   temp_menus[4][0]=temp_menus[4][1]=temp_menus[4][2]=temp_menus[4][3]="ALARM_4";
   temp_menus[5][0]=temp_menus[5][1]=temp_menus[5][2]=temp_menus[5][3]="ALARM_5";




   for(i=0;i<MAX_ALARM_RING_CNT+1;i++)
   {
       pmenus[i]=(char **)temp_menus[i];
   }


   if(1 == choose_menu_option_ex((char ***)pmenus, MAX_ALARM_RING_CNT, def_pos, 0, MENU_STYLE_ALARM_VOICE))
   {
       return 1;
   }   
   return 0;
}


unsigned char app_smsring_show(unsigned char * def_pos)
{
   char * temp_menus[12][4];
   char ** pmenus[12];
   unsigned char i=0;
   
   temp_menus[0][0]=(char *)(text_music_select[0]);
   temp_menus[0][1]=(char *)(text_music_select[1]);   
   temp_menus[0][2]=(char *)(text_music_select[2]);  
   temp_menus[0][3]=(char *)(text_music_select[3]);  
   
   temp_menus[1][0]=temp_menus[1][1]=temp_menus[1][2]=temp_menus[1][3]="SMS_VOC_1";
   temp_menus[2][0]=temp_menus[2][1]=temp_menus[2][2]=temp_menus[2][3]="SMS_VOC_2";
   temp_menus[3][0]=temp_menus[3][1]=temp_menus[3][2]=temp_menus[3][3]="SMS_VOC_3";
   temp_menus[4][0]=temp_menus[4][1]=temp_menus[4][2]=temp_menus[4][3]="SMS_VOC_4";
   temp_menus[5][0]=temp_menus[5][1]=temp_menus[5][2]=temp_menus[5][3]="SMS_VOC_5";




   for(i=0;i<MAX_SMS_RING_CNT+1;i++)
   {
       pmenus[i]=(char **)temp_menus[i];
   }


   if(1 == choose_menu_option_ex((char ***)pmenus, MAX_SMS_RING_CNT, def_pos, 0, MENU_STYLE_SMSRING_VOICE))
   {
       return 1;
   }   
   return 0;
}









void app_free_table_info(T_TABLE_INFO * ptable_info)
{
    if( (NULL != ptable_info->precords) &&  (ptable_info->precords != gbook_buf) )
    {
         app_free_memory(ptable_info->precords);ptable_info->precords=NULL;
	}
	
    ptable_info->precords=NULL;

    ptable_info->rec_cnt=0;
	ptable_info->flag_loaded=0;
}

unsigned char check_if_rec_in_table_index(unsigned short * pshort_buf, unsigned short length, unsigned short rec_no)
{
    unsigned short i=0;
	for(i=1;i<= length;i++)
	{
        if(pshort_buf[i] == rec_no)
        {
             return 1;
		}
	}
	return 0;
}

unsigned short app_load_table_info(unsigned char table_idx,  T_TABLE_INFO * ptable_info)
{
    unsigned short i=0,cell_id=0, tmp_cnt=0;
	unsigned char * huge_buf;
	unsigned char * tbidx_buf;
	unsigned char flag_over=0;
    unsigned long temp_st=app_get_tick_count();

    app_free_table_info(ptable_info);
	tbidx_buf =app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);       
    if(1 ==flash_cell_read(table_idx  + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, tbidx_buf))
	{
	    unsigned short * pshort_buf=(unsigned short *) tbidx_buf;
	    ptable_info->rec_cnt= pshort_buf[0];
		ptable_info->flag_loaded=1;
		
	    if(ptable_info->rec_cnt>0)
	    {
            unsigned char flag_can_deal =1;
            if(TABLEINFO_PHONEBOOK == table_idx)
            {
                 memset(gbook_buf,0,sizeof(gbook_buf));
                 ptable_info->precords = gbook_buf;
			}
			else 
			{
                flag_can_deal =0;
			}

            if(1 == flag_can_deal)
            {
				huge_buf=app_alloc_memory(g_table_info[table_idx].cell_info.cell_len);       
				for(cell_id=0; cell_id <g_table_info[table_idx].cell_info.cell_cnt; cell_id++)
		        {
		             unsigned short st,et;			 
		             memset(huge_buf,0, g_table_info[table_idx].cell_info.cell_len );
				     flash_cell_read(cell_id +g_table_info[table_idx].cell_info.cell_start, g_table_info[table_idx].cell_info.cell_len, huge_buf);

					 st=cell_id *  (g_table_info[table_idx].cell_info.cell_max_rec_cnt);
					 et=((cell_id +1)*(g_table_info[table_idx].cell_info.cell_max_rec_cnt));

					 for ( i=st; i<et; i++ )
				     {
				         if(1 == check_if_rec_in_table_index(pshort_buf, ptable_info->rec_cnt ,  (i+1) ) )
				         {
						     memcpy((unsigned char *)(&(ptable_info->precords[g_table_info[table_idx].tb_info.rec_len * tmp_cnt])),  &huge_buf[  ( i % (g_table_info[table_idx].cell_info.cell_max_rec_cnt)) *g_table_info[table_idx].tb_info.rec_len   ], g_table_info[table_idx].tb_info.rec_len);
	                         tmp_cnt++;
							 if(tmp_cnt >= ptable_info->rec_cnt)
							 {
                                 flag_over=1;
								 break;
							 }
						 }
					 }

					 if(1 == flag_over)
					 {
                         break;
					 }
		        }
			    app_free_memory(huge_buf);huge_buf=NULL;
            }
	    }
	}
	app_free_memory(tbidx_buf);tbidx_buf=NULL;


	if(1 ==appsys.flag_genie_trace)DebugPrintf("load table [%s]: rec_cnt:%d ,tmp_cnt:%d, TM: %dms\r\n",get_multi_string((char * *)g_table_info[table_idx].p_flh),ptable_info->rec_cnt,tmp_cnt,get_duration_tick(temp_st));

#if 0
	if(TABLEINFO_PHONEBOOK == table_idx)
	{
        T_BOOK_ENTRY * pbook_entry = (T_BOOK_ENTRY *)(&ptable_info->precords[g_table_info[table_idx].tb_info.rec_len* (ptable_info->rec_cnt-1)]);
		if(1 ==appsys.flag_genie_trace)DebugPrintf("-------%d  user name :%s  user number: %s  location %s\r\n",i, pbook_entry->name,pbook_entry->num,pbook_entry->location);
	}

#endif	

    return tmp_cnt;
}







void app_show_no_simcard()
{
   unsigned char idx_pos=0,last_x=0xFF,cycle_time=0;
   unsigned char xpos[]={84,76,68,60,52,44,38,30};
   FlSignalBuffer EventPara;

   appsys.byte_need_redraw =0xFF;
   while(1)
   {
        if(appsys.byte_need_redraw>0)
        {

            lcd_clear_screen();
			show_caption(get_multi_string((char * *)text_insert_simcard));
			

            lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
	        lcd_goto_xy(2, 18);
			show_bitmap(BMP_PHONE_BIN);
			lcd_goto_xy(93, 27);
			show_bitmap(BMP_SIMCARD_BIN);

			show_soft_key_prompt(get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));

			appsys.byte_need_redraw=0;

		}
		else
		{
            if(1 == app_get_message( &EventPara) )
			{
			    if(EVENT_KEY == EventPara.eventTyp )
			    {
                    if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
                    {
                        return ;
					}
				}
                else if(EVENT_TIMER== EventPara.eventTyp )
				{
                    if(0xFF !=last_x)
                    {
	                    lcd_goto_xy(last_x, 33);
						lcd_clear_line(last_x+8);
                    }

					
					lcd_goto_xy(xpos[idx_pos], 33);
					last_x =xpos[idx_pos];
					show_bitmap(BMP_ARROW_LEFT_BIN);
					idx_pos++;
					
					if(idx_pos>7)
					{
                        idx_pos =0;
					}
		        }
            }
		}
    }
}


char * left_trim_for_base64( char * p )
{
    while ( p && (!isbase64(*p)) ) ++p ;
    return p ;
}

char * right_trim_for_base64( char * p )
{
    char * temp;
    if (!p) return p;
    temp = (char *)(p + strlen(p)-1);
    while ( (temp>=p) && (!isbase64(*temp)) ) --temp;
    *(temp+1) = '\0';
    return p;
}

char * trim_for_base64( char * p )
{
    return left_trim_for_base64(right_trim_for_base64(p));
}





unsigned char calcuate_verify_value(unsigned char *data, unsigned int data_size)
{
    char dat_par = 0x55;


    while (data_size--)
    {
        dat_par ^= *data++;
    }

    return dat_par;
}



void app_view_call_meter_total( void )
{
    FlSignalBuffer EventPara;

    appsys.byte_need_redraw=0xFF;
    while(1)
    {
        if(appsys.byte_need_redraw>0)
        {
            char tmp_buf1[16],tmp_buf2[16];
			unsigned char align_len=0,align_left=0;
			memset(tmp_buf1,0, 16);
			memset(tmp_buf2,0, 16);
            lcd_clear_screen();
			show_caption(get_multi_string((char * *) text_call_timer_totals));
            show_soft_key_prompt(get_multi_string((char * *) text_clear) , get_multi_string((char * *) text_back));
            lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51); 

			appsys.byte_font_type =FONT_T_X_10;

			sprintf(tmp_buf1, "%.2d:%.2d:%.2d",  sysprop->dword_call_in_tm_total/3600,sysprop->dword_call_in_tm_total%3600/60, sysprop->dword_call_in_tm_total%3600%60);
			sprintf(tmp_buf2, "%.2d:%.2d:%.2d",  sysprop->dword_call_out_tm_total/3600,sysprop->dword_call_out_tm_total%3600/60, sysprop->dword_call_out_tm_total%3600%60);
            align_len = (strlen(tmp_buf1)>strlen(tmp_buf2))? get_str_dots_width(tmp_buf1):get_str_dots_width(tmp_buf2);

			align_left =(get_str_dots_width((get_multi_string((char * *) text_received)))>get_str_dots_width((get_multi_string((char * *) text_dialed))))? get_str_dots_width((get_multi_string((char * *) text_received))):get_str_dots_width((get_multi_string((char * *) text_dialed)));
            align_left+=get_str_dots_width(": ");

            lcd_goto_xy(3, 20);
			lcd_put_string(get_multi_string((char * *) text_received));
			lcd_put_string(": ");

			lcd_goto_xy(align_left+ align_len - get_str_dots_width(tmp_buf1),20);
			lcd_put_string(tmp_buf1);

            lcd_goto_xy(3, 35);
			lcd_put_string(get_multi_string((char * *) text_dialed));
			lcd_put_string(": ");
			lcd_goto_xy(align_left+ align_len - get_str_dots_width(tmp_buf2),35);
			lcd_put_string(tmp_buf2);

			appsys.byte_need_redraw=0;
		}
		else
		{
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {
					 if(EventPara.sig_p.key_evt.isPressed)
					 {
					     if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)|| (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						 {
						     return;
					     }
						 else  if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						 {
						 	 if ( choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_do_you_want_to_clear_calltime)) )
                             {
                                  sysprop->dword_call_in_tm_total=0;
								  sysprop->dword_call_out_tm_total=0;
								  app_save_prop();
								  msgok(get_multi_string((char * *)text_call_time_cleared ));
								  delay_ms(2000);
							 }
						 }
					 }
		         }
				 else if(EVENT_TIMER== EventPara.eventTyp )
				 {
	                 if(appsys.byte_need_exit > 0)
	                 {
	                     return;
					 }
				 }
		     }
		 }
	 }
}


void app_view_sms_total( void )
{
    FlSignalBuffer EventPara;

    appsys.byte_need_redraw=0xFF;
    while(1)
    {
        if(appsys.byte_need_redraw>0)
        {
            char tmp_buf1[16],tmp_buf2[16],tmp_buf3[16];
			unsigned char align_len=0,align_left=0;
			memset(tmp_buf1,0, sizeof(tmp_buf1));
			memset(tmp_buf2,0, sizeof(tmp_buf2));
			memset(tmp_buf3,0, sizeof(tmp_buf3));
			
            lcd_clear_screen();
			show_caption(get_multi_string((char * *) text_message_totals));
            show_soft_key_prompt(get_multi_string((char * *) text_null) , get_multi_string((char * *) text_back));
            lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51); 

            appsys.byte_font_type =FONT_T_X_10;
			
			sprintf(tmp_buf1, "%d/%d",  app_get_record_used(TABLEINFO_SMS_INBOX) , g_table_info[TABLEINFO_SMS_INBOX].tb_info.rec_max_cnt);
			sprintf(tmp_buf2, "%d/%d",  app_get_record_used(TABLEINFO_SMS_OUTBOX), g_table_info[TABLEINFO_SMS_OUTBOX].tb_info.rec_max_cnt);
			sprintf(tmp_buf3, "%d/%d",  app_get_record_used(TABLEINFO_SMS_DRAFT) , g_table_info[TABLEINFO_SMS_DRAFT].tb_info.rec_max_cnt);

            align_len = (strlen(tmp_buf1)>strlen(tmp_buf2))? get_str_dots_width(tmp_buf1):get_str_dots_width(tmp_buf2);

			align_left =(get_str_dots_width((get_multi_string((char * *) text_inbox_list)))>get_str_dots_width((get_multi_string((char * *) text_outbox_list))))? get_str_dots_width((get_multi_string((char * *) text_inbox_list))):get_str_dots_width((get_multi_string((char * *) text_outbox_list)));
            align_left+=get_str_dots_width(": ");

            lcd_goto_xy(3, 14);
			lcd_put_string(get_multi_string((char * *) text_inbox_list));
			lcd_put_string(": ");
			lcd_goto_xy(align_left+ align_len - get_str_dots_width(tmp_buf1),14);
			lcd_put_string(tmp_buf1);			

            lcd_goto_xy(3, 26);
			lcd_put_string(get_multi_string((char * *) text_outbox_list));
			lcd_put_string(": ");
			lcd_goto_xy(align_left+ align_len - get_str_dots_width(tmp_buf2),26);
			lcd_put_string(tmp_buf2);	

            lcd_goto_xy(3, 38);
			lcd_put_string(get_multi_string((char * *) text_draft_list));
			lcd_put_string(": ");
			lcd_goto_xy(align_left+ align_len - get_str_dots_width(tmp_buf3),38);
			lcd_put_string(tmp_buf3);				

			appsys.byte_need_redraw=0;
		}
		else
		{
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {
					 if(EventPara.sig_p.key_evt.isPressed)
					 {
                         if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						 {
						     return;                   
						 }
					 }
		         }
				 else if(EVENT_TIMER== EventPara.eventTyp )
				 {
	                 if(appsys.byte_need_exit > 0)
	                 {
	                     return;
					 }
				 }
		     }
		 }
	 }
}













void app_handset_test()
{
    FlSignalBuffer EventPara;
    char * pstr_show=NULL;

	appsys.byte_need_redraw=0xFF;
    mu_set_spk_gain(3);
	appsys.flag_hand_free =0;
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
             if(0xFF ==  appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
				 show_caption(get_multi_string((char * *)text_handset_test));
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);
                 show_soft_key_prompt(get_multi_string((char * *) text_null) , get_multi_string((char * *) text_back));
             }
			 pstr_show =  (HOOK_STATE_OFFHOOK == appsys.flag_hook_state)? get_multi_string((char * *) text_off_hook):get_multi_string((char * *) text_on_hook);

			 appsys.byte_font_type=FONT_B_X_10;
			 lcd_goto_xy((SCREEN_WIDTH-get_str_dots_width(pstr_show))/2, 25);
			 lcd_clear_line(SCREEN_WIDTH-3);
			 lcd_goto_xy((SCREEN_WIDTH-get_str_dots_width(pstr_show))/2, 25);
			 
			 lcd_put_string(pstr_show);
			 appsys.byte_need_redraw=0;
    	}
		else
		{
			if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
				             mu_stop_dialtone();
				             mu_set_voice_path(VOICE_PATH_HANDFREE);
						     mu_set_spk_gain(sysprop->byte_talk_volume);
							 return;
						}      
				 	}
		        }
				else if( EVENT_HANDSET == EventPara.eventTyp)
				{
                    if(HOOK_STATE_OFFHOOK == appsys.flag_hook_state)
                    {
                        mu_set_voice_path(VOICE_PATH_HANDSET);		
                        mu_play_dialtone();
					}
					else
					{
                        mu_stop_dialtone();
					}
					appsys.byte_need_redraw=1;
				}
			}
		}		
	}
}

void app_handfree_test()
{
    FlSignalBuffer EventPara;
	char * pstr_show=NULL;

	appsys.byte_need_redraw=0xFF;
    appsys.flag_hand_free=0;

    mu_set_spk_gain(3);

	
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
             if(0xFF ==  appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
				 show_caption(get_multi_string((char * *)text_handfree_test));
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);
                 show_soft_key_prompt( get_multi_string((char * *) text_null) , get_multi_string((char * *) text_back));
             }

			 pstr_show =( (1 == appsys.flag_hand_free)? get_multi_string((char * *)text_open):get_multi_string((char * *)text_close));

			 appsys.byte_font_type=FONT_B_X_10;
			 lcd_goto_xy(( SCREEN_WIDTH - get_str_dots_width(pstr_show))/2, 25);

            
			 lcd_clear_line(SCREEN_WIDTH-3);
			 lcd_goto_xy(( SCREEN_WIDTH - get_str_dots_width(pstr_show))/2, 25);

			 
			 lcd_put_string(pstr_show);
			 appsys.byte_need_redraw=0;
    	}
		else
		{
			if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
				             mu_stop_dialtone();
				             mu_set_voice_path(VOICE_PATH_HANDFREE);
						     mu_set_spk_gain(sysprop->byte_talk_volume);
							 return;
						}  
						else if( TFKEY_HANDFREE== EventPara.sig_p.key_evt.key_val)
						{
		                     if(1 ==appsys.flag_hand_free)
		                     {
		                         mu_set_voice_path(VOICE_PATH_HANDFREE);
		                         mu_play_dialtone();
							 }
							 else
							 {
		                         mu_stop_dialtone();
							 }
							 appsys.byte_need_redraw=0xFF; 
						}
				 	}
		        }
			}
		}		
	}
}


void app_simpin_test()
{
    unsigned char i=0;
    FlSignalBuffer EventPara;

	appsys.byte_need_redraw=0xFF;

	
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
		   msg("SIM PIN /PUK TEST\n1,2,3");
        
		   appsys.byte_need_redraw=0;
		}
		else
		{
			if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
							 return;
						}  
						else if( (TFKEY_1== EventPara.sig_p.key_evt.key_val ))
				        {
							 fake_android_message("2,1,1");
							 appsys.byte_need_redraw=0xFF;
						}  
						else if( (TFKEY_2== EventPara.sig_p.key_evt.key_val))
				        {
							 fake_android_message("2,1,2");
							 appsys.byte_need_redraw=0xFF;
						}  
						else if( (TFKEY_3== EventPara.sig_p.key_evt.key_val))
				        {
							 fake_android_message("2,1,3");
							 appsys.byte_need_redraw=0xFF;
						}  

						if(1 == mu_check_sim_valid(200))
						{

						}
				 	}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
				    
				}
			}	
		}
	}
}



void app_protocols_test()
{
    unsigned char i=0;
    FlSignalBuffer EventPara;

	appsys.byte_need_redraw=0xFF;

	
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
		   msg("1->SIMPIN (disabled)\n 2->NEWSMS 3->INCALL");
        
		   appsys.byte_need_redraw=0;
		}
		else
		{
			if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
							 return;
						}  
						else if( (TFKEY_1== EventPara.sig_p.key_evt.key_val ))
				        {
							 //app_simpin_test();
							 appsys.byte_need_redraw=0xFF;
						}  
						else if( (TFKEY_2== EventPara.sig_p.key_evt.key_val))
				        {
				             char tmp_cmd[128];
							 memset(tmp_cmd,0,sizeof(tmp_cmd));
							 sprintf(tmp_cmd,"2,5,\"13925255166\";\"20180508182636\";\"this is test sms from 周大大 ,时间%s\"\r\n", app_get_datetime_str());
							 fake_android_message(tmp_cmd);

							 appsys.byte_need_redraw=0xFF;
							 return ;
						}  
						else if( (TFKEY_3== EventPara.sig_p.key_evt.key_val))
				        {
							 fake_android_message("2,3,1,13925255166");
							 appsys.byte_need_redraw=0xFF;
							 
						}  
						  
				 	}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
				    
				}
			}	
		}
	}
}



void app_signal_test_resp()
{
    unsigned char i=0;
    FlSignalBuffer EventPara;

	appsys.byte_need_redraw=0xFF;

	
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
		   msg("reg:0123 DT:4567  RM:89");
        
		   appsys.byte_need_redraw=0;
		}
		else
		{
			if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
							 return;
						}  
						else if( (TFKEY_0== EventPara.sig_p.key_evt.key_val ))
				        {
                            fake_android_message("2,2,0,3,4,0,China_Unicom");return;
					 
						}  
						else if( (TFKEY_1== EventPara.sig_p.key_evt.key_val ))
				        {
                            fake_android_message("2,2,1,3,4,0,null");return;
						}  
						else if( (TFKEY_2== EventPara.sig_p.key_evt.key_val))
				        {
			                fake_android_message("2,2,2,3,4,0,emergency");return;
						}  
						else if( (TFKEY_3== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,3,3,4,0,no atena");return;
						}  


						

						else if( (TFKEY_4== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,0,0,4,0,NO G");return;
						}  
						else if( (TFKEY_5== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,0,1,4,0,2G");return;
						}  
						else if( (TFKEY_6== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,0,2,4,0,3G");return;
						}  
						else if( (TFKEY_7== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,0,3,4,0,4G");return;
						} 

						

						
						else if( (TFKEY_8== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,2,3,4,0,Local");return;
						}  
						else if( (TFKEY_9== EventPara.sig_p.key_evt.key_val))
				        {
						    fake_android_message("2,2,2,3,4,1,Roaming");return;
							
						}  
						
						  
				 	}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
				    
				}
			}	
		}
	}
}



void app_lcd_test()
{

    unsigned char i=0;
    FlSignalBuffer EventPara;

	appsys.byte_need_redraw=0xFF;
	lcd_clear_screen();
	
    while(1)
    {
		if(1 == app_get_message( &EventPara) )
		{
	        if(EVENT_KEY == EventPara.eventTyp )
			{
			 	if(EventPara.sig_p.key_evt.isPressed)
				{
			        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
			        {
						 return;
					}  
			 	}
	        }
			else if(EVENT_TIMER == EventPara.eventTyp )
			{
			    lcd_put_rect(i*2, i*2, SCREEN_WIDTH-1-i*2, SCREEN_HEIGHT-1-i*2);

			    if(i++>15)
			    {
					lcd_clear_screen();
					i=0;
			    }
			}
		}	
	}
}

void app_battery_test()
{
    FlSignalBuffer EventPara;

	appsys.byte_need_redraw=0xFF;
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
             if(0xFF ==  appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
				 show_caption(get_multi_string((char * *)text_battery));
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);
                 show_soft_key_prompt( get_multi_string((char * *) text_check), get_multi_string((char * *) text_back));
             }

			 appsys.byte_font_type=FONT_T_X_10;

			 lcd_goto_xy(3, 14);
			 lcd_clear_line(SCREEN_WIDTH-3);
			 lcd_goto_xy(3, 14);
             lcd_put_string((1 == appsys.power_service.flag_extern_power)?get_multi_string((char * *)text_external_power_on): get_multi_string((char * *)text_external_power_off));


             if(1 == appsys.power_service.flag_having_battery)
             {
				 lcd_goto_xy(3, 26);
				 lcd_clear_line(SCREEN_WIDTH-3);
				 lcd_goto_xy(3, 26);
			 }
			 else
			 {
				 lcd_goto_xy(3, 34);
				 lcd_clear_line(SCREEN_WIDTH-3);
				 lcd_goto_xy(3, 34);
			 }

             lcd_put_string((1 == appsys.power_service.flag_having_battery)?get_multi_string((char * *)text_battary_connected): get_multi_string((char * *)text_battary_missing));

			 if(1 == appsys.power_service.flag_having_battery)
			 {
				 lcd_goto_xy(3, 38);
				 lcd_clear_line(SCREEN_WIDTH-3);
				 lcd_goto_xy(3, 38);
	             lcd_put_string(get_multi_string((char * *)text_battary_level));lcd_put_string(": ");
				 {
	                 unsigned char lvl= get_battery_lvl();
					 if(lvl<=2)
					 {
	                     lcd_put_string(get_multi_string((char * *)text_low));
					 }
					 else if (lvl<=3)
					 {
	                     lcd_put_string(get_multi_string((char * *)text_middle));
					 }
					 else
					 {
	                     lcd_put_string(get_multi_string((char * *)text_high));
					 }
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
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
				        if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
							 return;
						}  
						else if( TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						     msg(get_multi_string((char * *)text_please_wait));
							 delay_ms(280);
							 appsys.byte_need_redraw=0xFF;
						}
				 	}
		        }
			}
		}		
	}
}


void setup_auto_redial()
{
	FlSignalBuffer EventPara;
	char tmp_buf[8];
	T_AUTO_REDIAL_STATUS rd_stat;	
	unsigned char cursor=1;


	memcpy(&rd_stat, &sysprop->auto_redial, sizeof(T_AUTO_REDIAL_STATUS));
	appsys.byte_need_redraw=0xFF;

	while(1)
	{
        if(appsys.byte_need_redraw>0)
        {
             if(0xFF == appsys.byte_need_redraw)
             {
	             lcd_clear_screen();
				 show_caption(get_multi_string((char * *) text_redail_setting));
				 show_soft_key_prompt(get_multi_string((char * *) text_edit), get_multi_string((char * *) text_back));
	             lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
             }

             appsys.byte_font_type=FONT_T_X_10;

			 if(1 == cursor)appsys.flag_color_reverse=1;

		     lcd_goto_x_line(3, 1);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-2);
			 lcd_goto_x_line(3, 1);	

			 
			 lcd_put_string("1.");
			 lcd_put_string(get_multi_string((char * *)text_redial_times));
			 lcd_put_string(": ");
			 memset(tmp_buf, 0, sizeof(tmp_buf));
			 sprintf(tmp_buf,"%d", rd_stat.redial_cnt);
			 lcd_put_string(tmp_buf);
			 appsys.flag_color_reverse=0;

	
			 if(2 == cursor)appsys.flag_color_reverse=1;
		     lcd_goto_x_line(3, 2);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-2);
			 lcd_goto_x_line(3, 2);	
			 
			 lcd_put_string("2.");
			 lcd_put_string(get_multi_string((char * *)text_redial_interval));
			 lcd_put_string(": ");
			 memset(tmp_buf, 0, sizeof(tmp_buf));
			 sprintf(tmp_buf,"%d", rd_stat.redial_interval);
			 lcd_put_string(tmp_buf);
			 appsys.flag_color_reverse=0;
			 appsys.byte_need_redraw=0;
		}
		else
		{
            if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				 	if(EventPara.sig_p.key_evt.isPressed)
					{
                         switch(EventPara.sig_p.key_evt.key_val)
                         {
                              case TFKEY_SOFT_RIGHT:
							  {
							  	  if(sysprop->auto_redial.redial_cnt != rd_stat.redial_cnt  ||  sysprop->auto_redial.redial_interval != rd_stat.redial_interval)
							  	  {
                                      if(1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_ask_save_or_not)) )
                                      {
                                            sysprop->auto_redial.redial_cnt = rd_stat.redial_cnt ;
											sysprop->auto_redial.redial_interval = rd_stat.redial_interval;
											app_save_prop();
									  }
								  }  	
						          
								  return;
                              }
							  break;
							  case TFKEY_EXIT:
							  {
								  return;
							  }
							  break;

							  case TFKEY_1:
							  case TFKEY_2:
							  {
								  if(TFKEY_1 == EventPara.sig_p.key_evt.key_val)
								  {
	                                  cursor =1;
								  }
								  else if(TFKEY_2 == EventPara.sig_p.key_evt.key_val)
								  {
	                                  cursor =2;
								  }
								  else 
								  {
	                                  break;
								  }
							  }
							  case TFKEY_SOFT_LEFT:
	                          {
								   if(1 == cursor)
								   {
									    memset(tmp_buf, 0, sizeof(tmp_buf));
									    sprintf(tmp_buf,"%d", rd_stat.redial_cnt);
	                                    if (1 == wnd_input_text(tmp_buf, get_multi_string((char * *)text_redial_times),   2, IN_FIXED, EDIT_STYLE_DIGITALONLY))	
	                                    {
	                                        rd_stat.redial_cnt= atoi(tmp_buf);
									    }
								   }
								   else if(2 == cursor)
								   {
										 memset(tmp_buf, 0, sizeof(tmp_buf));
										 sprintf(tmp_buf,"%d", rd_stat.redial_interval);
		                                 if (1 == wnd_input_text(tmp_buf, get_multi_string((char * *)text_redial_interval),  3, IN_FIXED, EDIT_STYLE_DIGITALONLY))	
		                                 {
		                                      rd_stat.redial_interval= (atoi(tmp_buf)>255)? 255: atoi(tmp_buf);
										 }
								   }
							  }
							  break;
							  default:
							  break;
						}

                        if( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT == EventPara.sig_p.key_evt.key_val))
						{
                             if(cursor>1)
							 {
							     cursor--;
							 }
							 else
							 {
                                 cursor =2;
							 }
							 appsys.byte_need_redraw=1;
						}
						else if( (TFKEY_DOWN== EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
                             if(cursor <2)
							 {
							      cursor++;
                             }
							 else
							 {
                                  cursor =1;
							 }
							 appsys.byte_need_redraw=1;
						}
				 	}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                   break;;
					}
				}
			}
		}
	}
	appsys.byte_need_redraw=0xFF;
}






void show_device_serial_number()
{
    FlSignalBuffer EventPara;
    char str_cgsn[16];
    memset(str_cgsn,0,16);

    appsys.byte_need_redraw=0xFF;
	
	while(1)
	{
        if(appsys.byte_need_redraw>0)
        {
			appsys.byte_need_redraw=0;
			lcd_clear_screen();
			
			show_caption(get_multi_string((char * *)text_device_serial_no));
		    lcd_goto_xy(0, 11);
			lcd_draw_horizontal(SCREEN_WIDTH-1);
			show_soft_key_prompt( get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));

		    appsys.byte_font_type=FONT_T_X_10;
            lcd_goto_x_line(0, 1);
		    lcd_clear_line(SCREEN_WIDTH-1);

		    lcd_goto_x_line(0, 1);
		    lcd_put_string("CIMI:");
		    lcd_put_string(appsys.str_simcimi_number);
		   	
            lcd_goto_x_line(0, 2);
		    lcd_clear_line(SCREEN_WIDTH-1);	
            lcd_goto_x_line(0, 2);		   
		    lcd_put_string("IMEI:");
		    lcd_put_string(appsys.str_imei_no); 
			  
			appsys.byte_need_redraw=0;
        }
		else
		{
	        if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if( (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val) || (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val))
						{
						    return;
					    }						
					}
		        }
				else  if(EVENT_TIMER== EventPara.eventTyp )
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    return;
					}
				}
		    }
		}
	}
}


void show_device_csq_level()
{
    FlSignalBuffer EventPara;
	unsigned char flag_need_clear=0;
    char str_show[32];
	unsigned long st_tick=app_get_tick_count();
    memset(str_show,0,sizeof(str_show));

    appsys.byte_need_redraw=0xFF;
    msg(get_multi_string((char * *)text_please_wait));
	mu_get_csq();
	
	while(0 == appsys.byte_need_exit)
	{
        if(appsys.byte_need_redraw>0)
        {
			appsys.byte_need_redraw=0;
			lcd_clear_screen();
			
			show_caption("信号与网络");

            lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);

			show_soft_key_prompt( get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));

		    appsys.byte_font_type=FONT_T_X_10;
            lcd_goto_x_line(3, 1);
		    lcd_clear_line(SCREEN_WIDTH-3);
		    lcd_goto_x_line(3, 1);

			lcd_put_string("运营商: ");lcd_put_string(appsys.str_operator_name);
		
            lcd_goto_x_line(3, 2);
		    lcd_clear_line(SCREEN_WIDTH-3);	
            lcd_goto_x_line(3, 2);		   
			sprintf(str_show,"CSQ等级: %d 网络: ",  appsys.byte_csq_level );
		    lcd_put_string(str_show);
		

	        switch(appsys.byte_net_state)
	        {
	             case 1:
				 	lcd_put_string("2G");
					 break;
				 case 2:
				 	lcd_put_string("3G");
					 break;
				 case 3:
				 	lcd_put_string("4G");
					 break;
				 case 0:
				 default:
				 	lcd_put_string("未知");
				 	break;
			}

			if(1)
			{
				 char * pbuf = "[每2秒刷新]";
				 unsigned char str_width = get_str_dots_width(pbuf);
				 lcd_goto_xy((SCREEN_WIDTH- str_width)/2, 52);
				 lcd_put_string(pbuf);
				 flag_need_clear=1;
			}
			appsys.byte_need_redraw=0;
        }
		else
		{
	        if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if( (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val) || (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val))
						{
						    return;
					    }						
					}
		        }
				else  if(EVENT_TIMER== EventPara.eventTyp )
				{
				 	if(1 == flag_need_clear && get_duration_tick(st_tick)>900 )
					{
					     flag_need_clear=0;
						 lcd_goto_xy(3, 52);
						 lcd_clear_line(SCREEN_WIDTH-27);
					}
					
                    if(get_duration_tick(st_tick) >=1900)
                    {
                         st_tick= app_get_tick_count();
						 mu_get_csq();
						 appsys.byte_need_redraw=0xFF;
					}
					
	                if(appsys.byte_need_exit > 0)
	                {
	                    return;
					}
				}
		    }
		}
	}
}

void show_device_power_info()
{
    FlSignalBuffer EventPara;
	unsigned char flag_need_clear=0;
    char str_show[32];
	unsigned long st_tick=app_get_tick_count();
    memset(str_show,0,sizeof(str_show));

    appsys.byte_need_redraw=0xFF;
    msg(get_multi_string((char * *)text_please_wait));
	mu_get_csq();
	
	while(0 == appsys.byte_need_exit)
	{
        if(appsys.byte_need_redraw>0)
        {
			appsys.byte_need_redraw=0;
			lcd_clear_screen();
			
			show_caption(get_multi_string((char * *) text_check));

            lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);

			show_soft_key_prompt( get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));

		    appsys.byte_font_type=FONT_T_X_10;
            lcd_goto_x_line(3, 1);
		    lcd_clear_line(SCREEN_WIDTH-3);

		
		    lcd_goto_x_line(3, 1);
			sprintf(str_show,"外电:%s",   (appsys.power_service.flag_extern_power==1)?"有":"无");
		    lcd_put_string(str_show);
		
		   	
            lcd_goto_x_line(3, 2);
		    lcd_clear_line(SCREEN_WIDTH-3);	
            lcd_goto_x_line(3, 2);		   
			sprintf(str_show,"电池检测:%s ADC=%d", (appsys.power_service.flag_having_battery==1)?"有":"无", appsys.power_service.battary_status.adc);
		    lcd_put_string(str_show);


            lcd_goto_x_line(3, 3);
		    lcd_clear_line(SCREEN_WIDTH-3);	
            lcd_goto_x_line(3, 3);		   

			

			if(1)
			{
				 char * pbuf = "[每2秒刷新]";
				 unsigned char str_width = get_str_dots_width(pbuf);
				 lcd_goto_xy((SCREEN_WIDTH- str_width)/2, 52);
				 lcd_put_string(pbuf);
				 flag_need_clear=1;
			}
		
			  
			appsys.byte_need_redraw=0;
        }
		else
		{
	        if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if( (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val) || (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val))
						{
						    return;
					    }						
					}
		        }
				else  if(EVENT_TIMER== EventPara.eventTyp )
				{
				 	if(1 == flag_need_clear && get_duration_tick(st_tick)>900 )
					{
					     flag_need_clear=0;
						 lcd_goto_xy(3, 52);
						 lcd_clear_line(SCREEN_WIDTH-27);
					}
					
                    if(get_duration_tick(st_tick) >=1900)
                    {
                         st_tick= app_get_tick_count();
						 appsys.byte_need_redraw=0xFF;
					}
					
	                if(appsys.byte_need_exit > 0)
	                {
	                    return;
					}
				}
		    }
		}
	}
}






unsigned char app_lock_phone_and_card()
{
    char str_pin[16];
	char str_old[16];

	unsigned long tmp_long=0;

	msg(get_multi_string((char * *)text_please_wait));	
	memset(str_old,0, sizeof(str_old));
	memset(str_pin,0, sizeof(str_pin));

    get_new_simpin_from_esn(str_pin);

	if(1 == sysprop->flag_lock_phone)
	{
	    msgok(get_multi_string((char * *)text_activate_successfully));
		delay_ms(1500);
	    return 1;
	}

    if (0== wnd_input_password(str_old, get_multi_string((char * *)text_enter_old_pin), 8))
    {
         return 0;
	}	

	if(1 == appsys.flag_genie_trace)DebugPrintf("user input old pin is:--->%s\r\n", str_old);

    msg(get_multi_string((char * *)text_please_wait));
	
	if (0 == mu_is_pin_active() )
	{
        if(0 == mu_active_pin(str_old))
        {
             // warning...
             msg_warn(get_multi_string((char * *)text_activate_failure));
			 delay_ms(1500);
			 return 0;
		}	 
	}
	if(1 == appsys.flag_genie_trace)DebugPrintf("sim pin is already activated\r\n",NULL);
	



    if( 0 == mu_change_pin(str_old, str_pin))
	{
         msg_warn(get_multi_string((char * *)text_pin_code_error));
		 delay_ms(1500);
		 return 0;
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("old pin has been changed to new pin----> %s\r\n",str_pin);
	
    msgok(get_multi_string((char * *)text_activate_successfully));
	if(1 == appsys.flag_genie_trace)DebugPrintf("lock phone to sim card number---->%s\r\n",appsys.str_simcimi_number);
	
	sysprop->flag_lock_phone=1;
    app_save_prop();
	delay_ms(1500);
	
    return 1;
}

void get_new_simpin_from_esn(char * new_pin)
{
    unsigned long tmp_long = hex_str_to_long( appsys.str_imei_no);
	tmp_long %=10000;
	sprintf(new_pin,"%.4d", tmp_long);	
}

unsigned char app_cancel_lock_phone_and_card()
{
    char * str_def="1234";
    char str_pin[16];


 	if (0 == mu_is_pin_active() )
 	{
        msgok(get_multi_string((char * *)text_deactivate_successfully));
		delay_ms(1500);
		return 1;
	}

    msg(get_multi_string((char * *)text_please_wait));


	memset(str_pin,0, sizeof(str_pin));
    get_new_simpin_from_esn(str_pin);




    if( 0== mu_change_pin(str_pin, str_def))
    {
         msg_warn(get_multi_string((char * *)text_deactivate_failure));
		 delay_ms(1500);
		 return 0;
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("changed pin to 1234 anyway\r\n",NULL);

	if( 0 == mu_cancel_pin(str_def))
	{
         msg_warn(get_multi_string((char * *)text_deactivate_failure));
		 delay_ms(1500);
		 return 0;
	}

    if(1 == appsys.flag_genie_trace)DebugPrintf("deactive pin succeed\r\n",NULL);

	sysprop->flag_lock_phone =0;

	msgok(get_multi_string((char * *)text_deactivate_successfully));
	app_save_prop();
	
	delay_ms(1500);	
    return 1;
}


unsigned char app_pass_lock_phone_and_card()
{
    char str_pin[16];


    msg(get_multi_string((char * *)text_please_wait));

	memset(str_pin,0, sizeof(str_pin));
    get_new_simpin_from_esn(str_pin);

    //will lock

    return 0;
}

void app_do_unlock_phone_failed(unsigned char unlock_type)
{
    FlSignalBuffer EventPara;	


      
	if(UNLOCK_TYPE_SIMPIN == unlock_type )
	{
	    wnd_show_notice(  get_multi_string((char * *)text_simpin_locked), get_multi_string((char * *)text_please_use_correct_card), NOTICE_TYPE_ALARM, IGNORE_SOFT_KEY_INPUT);
	}
	else if( UNLOCK_TYPE_CIMI == unlock_type)
	{
	    wnd_show_notice( get_multi_string((char * *)text_phone_locked), get_multi_string((char * *)text_please_use_correct_card), NOTICE_TYPE_ALARM, IGNORE_SOFT_KEY_INPUT);  
	}

		   
	while(1)
	{
        if(1 == app_get_message( &EventPara) )
		{
	        if(EVENT_KEY == EventPara.eventTyp )
			{
				if(EventPara.sig_p.key_evt.isPressed)
				{
				    if(TFKEY_CONFIRM==EventPara.sig_p.key_evt.key_val)
					{
                         char  tmp_buf[10];
						 unsigned char nret=0;
 ___INPUT_PASSWORD:
						 memset(tmp_buf,0,sizeof(tmp_buf));
					     if (wnd_input_password(tmp_buf, get_multi_string((char * *) text_super_password), 5) >0 )
					     {
					         if(0 == strncmp(tmp_buf, "74604",5 ))
					         {
							     if(UNLOCK_TYPE_SIMPIN == unlock_type ||  UNLOCK_TYPE_CIMI == unlock_type)
							     {
									 sysprop->flag_lock_phone =0;
							     }

								 app_save_prop();

								 if(UNLOCK_TYPE_CIMI== unlock_type )
								 {
								    app_show_cimis();
								 }
								 app_system_reset();	 
					         }
							 else
							 {
                                 msg_warn(get_multi_string((char * *)text_password_error));
								 delay_ms(1500);
								 goto ___INPUT_PASSWORD;
							 }
						 }
					}
			    }					
			}
        }
    }
}


void app_reg_cimis(char * str_cimi)
{
    //we can save 50 cimis,every records with 20 bytes
    unsigned char * huge_buf =NULL;

	if(0 == strlen (str_cimi))
	{
        return;
	}
	
    huge_buf = app_alloc_memory(g_db_info[DBINFO_LOCAL_FILES].cell_info.cell_len);

	if(NULL != huge_buf)
	{
         if(load_file_from_flash(DBINFO_LOCAL_FILES, DB_LOCAL_ALL_CIMI_POS, huge_buf))
         {
			 int i=0;
			 unsigned char flag_new=1;
			 unsigned char * pbuf =&huge_buf[8];			 
             if(  !(0x55 == huge_buf[0] && 0xAA == huge_buf[1]))
             {
                memset(huge_buf,0,g_db_info[DBINFO_LOCAL_FILES].cell_info.cell_len);
                huge_buf[0] =0x55;
				huge_buf[1] =0xAA;		
			 }

			 if(huge_buf[2] >50)//counter
			 {
                 huge_buf[2]=50;
			 }
			 

             for(i=0;i<huge_buf[2];i++)
             {
                 if( 0 == memcmp(&pbuf[i *20],  str_cimi, strlen(str_cimi)))
                 {
                     flag_new=0;
					 break;
				 }
			 }

			 if(1 == flag_new)
			 {
				 if(50 ==huge_buf[2]) 
				 {
	                 for(i=0;i<49;i++)
	                 {
	                     memcpy(&pbuf[i*20],  &pbuf[ (i+1)*20], 20);
					 }
				 }
				 else
				 {
	                 huge_buf[2]++;
				 }

				 i= huge_buf[2]-1;
				 i*=20;

	             memcpy(&pbuf[i],  str_cimi, strlen(str_cimi)); 
				 save_file_to_flash(DBINFO_LOCAL_FILES, DB_LOCAL_ALL_CIMI_POS, huge_buf);
			 }
		 }
		 app_free_memory(huge_buf);huge_buf=NULL;
	}
}

void app_show_cimis()
{
    unsigned char * huge_buf =NULL;
	unsigned char   npos=0;
	unsigned char one_page_num=0;
	char tmp_buf[16];
    huge_buf = app_alloc_memory(g_db_info[DBINFO_LOCAL_FILES].cell_info.cell_len);
	if(NULL != huge_buf)
	{
         if(load_file_from_flash(DBINFO_LOCAL_FILES, DB_LOCAL_ALL_CIMI_POS, huge_buf))
         {
			 int i=0;
			 unsigned char * pbuf =&huge_buf[8];	
             if(  !(0x55 == huge_buf[0] && 0xAA == huge_buf[1]))
             {
                memset(huge_buf,0,g_db_info[DBINFO_LOCAL_FILES].cell_info.cell_len);
                huge_buf[0] =0x55;
				huge_buf[1] =0xAA;		
				save_file_to_flash(DBINFO_LOCAL_FILES, DB_LOCAL_ALL_CIMI_POS, huge_buf);
			 }
			 else if(huge_buf[2] >0)
			 {
                 FlSignalBuffer EventPara;	

				 appsys.byte_need_redraw=0xFF;
				 while(1)
				 {
                      if(appsys.byte_need_redraw>0)
                      {
                           if(0xFF ==appsys.byte_need_redraw)
                           {
                                lcd_clear_screen();
    							show_caption("Used simcards");
                                lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);	
								show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_back)); 
                           }
						   if(npos>=huge_buf[2])
						   {
                               npos=0;
						   }

						   if(npos<huge_buf[2])
						   {
                               if(huge_buf[2]-npos>=4 )
                               {
                                   one_page_num=4;
							   }
							   else
							   {
                                   one_page_num=huge_buf[2]- npos;
							   }
						   }

						   for(i=0;i<4;i++)
						   {
						      
								lcd_goto_xy(3, y_dem_of_row[i+1]-1); 

								if(i<one_page_num)
								{
									memset(tmp_buf,0,sizeof(tmp_buf));
									sprintf(tmp_buf,"%.2d--",npos+i +1);

									if(0 == memcmp(appsys.str_simcimi_number, (char *)&pbuf[ (npos+i)*20]  , strlen(appsys.str_simcimi_number)))
									{
                                        appsys.flag_color_reverse=1;
									}
									lcd_put_string(tmp_buf);
									lcd_put_string((char *)&pbuf[ (npos+i)*20]);

									if(1== appsys.flag_color_reverse)
									{
                                        appsys.flag_color_reverse=0;
									}
								}
								else
								{
                                    lcd_clear_line(SCREEN_WIDTH-2);
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
									if(EventPara.sig_p.key_evt.isPressed)
									{
									    if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val) ||  (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
										{
										     app_free_memory(huge_buf);huge_buf=NULL;
										     return;
									    }
										else if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
										{
										     if(npos>one_page_num)
										     {
										          npos -= one_page_num;
										     }
											 else
											 {
                                                  npos=0;
											 }
											 appsys.byte_need_redraw=1;
										}
										else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
										{
										     if(4 != one_page_num)
										     {
										          npos =0;
										     }
											 else
											 {
                                                  npos+=4;
											 }
											 appsys.byte_need_redraw=1;
										}
									}
						        }
					        }
					  }
				 }
			 }
		 }
	}
}

unsigned char app_check_soft_wake_up()
{
    unsigned char flag_ret=0;
	
	if( sysprop->struct_alarm.alarm_on >0)
	{
	    if(sysprop->struct_alarm.alarm_time.hour == appsys.struct_time.hour
        && sysprop->struct_alarm.alarm_time.min == appsys.struct_time.min
		)
		{
		    appsys.dword_last_input_tick = app_get_tick_count();
            flag_ret=1;
		}
	}

	if(1 == sysprop->flag_auto_redial)
	{
        if( (appsys.byte_redial_cnt>0))
        {
            if( get_duration_tick(sysprop->auto_redial.redial_tick)/1000 > sysprop->auto_redial.redial_interval)
            {
                 appsys.dword_last_input_tick = app_get_tick_count();
                 flag_ret=1;
			}
		}
	}

	return flag_ret;
}
























































/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////SMS  PDU  PROTOCOLS//////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////





/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//the table is for changing from 03.38 alphabet table to ASCII alphabet table  
//add by sunsome.ju
const unsigned char tfdefault_to_ascii_table[128] =
{
    0x40,0xA3,0x24,0xA5,0xE8,0xE9,0xF9,0xEC,0xF2,0xC7,0x0A,0xD8,0xF8,0x0D,0xC5,0xE5,
    0x20,0x5F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xC6,0xE6,0xDF,0xC9,
    0x20,0x21,0x22,0x23,0xA4,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0xA1,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xC4,0xD6,0xD1,0xDC,0xA7,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0xE4,0xF6,0xF1,0xFC,0xE0
};

const unsigned char tfdefault_to_ascii_extension_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x5e,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7B,0x7D,0x20,0x20,0x20,0x20,0x20,0x5c,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x5B,0x7e,0x5D,0x20,
    0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20
};

//the table is for changing from ASCII alphabet table to 03.38 alphabet table  
//add by sunsome.ju
const unsigned char tfascii_to_default_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x20,0x20,0x0D,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x21,0x22,0x23,0x02,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x20,0x20,0x20,0x20,0x11,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x20,0x20,0x20,0x20,0x20
};



unsigned char TFAPI_CHECK_IF_NEED_EXT(unsigned char  ascii_char,  unsigned char * default_char_ptr) 
{
    unsigned char  result = 1;

    switch (ascii_char) 
    {
    case '^':
        *default_char_ptr = 0x14;
        break;

    case '{':
        *default_char_ptr = 0x28;
        break;

    case '}':
        *default_char_ptr = 0x29;
        break;

    case '\\':
        *default_char_ptr = 0x2f;
        break;

    case '[':
        *default_char_ptr = 0x3c;
        break;

    case '~':
        *default_char_ptr = 0x3d;
        break;

    case ']':
        *default_char_ptr = 0x3e;
        break;
        
    case '|':
        *default_char_ptr = 0x40;
        break;
        
    default:
        *default_char_ptr = 0x20;
        result = 0;
        break;
    }

    return (result);
}




unsigned short TFAPI_GET_7BITS_STR_FROM_8BITS( unsigned char * pdata,  int length, char * ret_str )
{
    int  ret_len = 0;
    int  i       = 0;

    for (i=0; i<length; i++)
    {
        if (pdata[i] >= 0x80)
        {
            break;
        }
        else
        {
            if (pdata[i] == 0x1b)
            {
                i++;
                if (pdata[i] >= 0x80)
                {
                    break;
                }
                else
                {
                    ret_str[ret_len] = tfdefault_to_ascii_extension_table[pdata[i]];
                    ret_len++;
                }
            }
            else
            {
                 ret_str[ret_len] = tfdefault_to_ascii_table[pdata[i]];
                 ret_len++;
            }
        }
    }

    return (ret_len);
}

unsigned short TFAPI_GET_7BITS_ESCAPE(char * ascii_ptr, unsigned short length,   unsigned char * str_escape)
{
    unsigned short  default_len = 0;
    int   i           = 0;
    unsigned char   temp_char   = 0;
	
    for (i=0; i<length; i++)
    {
        if (ascii_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            // according to GSM 7bit default alphabet extension table
            if (TFAPI_CHECK_IF_NEED_EXT(ascii_ptr[i], &temp_char))
            {
                str_escape[default_len] = 0x1b;
                default_len++;
                str_escape[default_len] = temp_char;
                default_len++;
            }
            else
            {
                str_escape[default_len] = tfascii_to_default_table[ascii_ptr[i]];
                default_len++;
            }
        }
    }

    return (default_len);
}









void  f_memset( void *s,int c,unsigned short n )
{
    unsigned char *p;

    p = (unsigned char *)s;
    while ( n-- )
    {
        *p++= c;
    }
}

void  f_memcpy( void *s1,const void *s2, unsigned short n )
{
    unsigned char *p1;
    const unsigned char  *p2;

    p1 = (unsigned char *)s1;
    p2 = (unsigned char *)s2;
    while ( n-- )
    {
        *p1++ = *p2++;
    }
}

unsigned short f_strlen( char *s1 )
{
    unsigned short len=0;
    while ( *s1++ )
    {
        len++;
    }
    return len;
}
void f_strcpy( char *s1,const char *s2 )
{
    do
    {
        *s1++ = *s2++;
    }
    while ( *s2 );
}





unsigned char is_hex(char c)
{
    return  ((c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f'))?1:0 ;
}

unsigned char is_hex_str(char * pstr)
{
   int length = strlen(pstr);
   int i=0;
   for(i=0;i<length;i++)
   {
       if( 0 == is_hex(pstr[i]))
       {
           return 0;
	   }
   }
   return 1;
}

unsigned char is_ansi(char c)
{
    return  ( c > 0 && c<= 0x7F)?1:0 ;
}

unsigned char is_ansi_str(char * pstr)
{
   int length = strlen(pstr);
   int i=0;
   for(i=0;i<length;i++)
   {
       if( 0 == is_ansi(pstr[i]))
       {
           return 0;
	   }
   }
   return 1;
}



unsigned char get_hex(char c)
{
   if(c>='0' && c<='9')
   {
       return c-0x30;
   }
   else if(c>='A' && c<='F')
   {
       return  c-'A' +10;
   }
   else if(c>='a' && c<='f')
   {
       return  c-'a' +10;
   }

   return 0;
}


unsigned char get_send_dcs(char * pdt)
{
     int i=0;
	 int length= f_strlen(pdt);
     for(i=0;i<length;i++)
     {
         if((unsigned char)pdt[i] >=0x80)
         {
             return DCS_MODE_UCS2;
		 }
	 }
	 return DCS_MODE_DEFA;
}









































































unsigned char ReadMaBiao(unsigned long nAddr, unsigned char *pBuffer, unsigned short  bReadLen)
{
#define MABIAO_SIZE  145021
    
    if(nAddr + bReadLen < MABIAO_SIZE)
    {
	    memcpy(pBuffer, &py_bx_code[nAddr], bReadLen);
		return TRUE;
    }

	return FALSE;
}





T_PYINPUTCODE  gPYCode;
unsigned char make_spell_5d(EDITWND * p_edit_wnd,char * KeyBuf, char *spell_str ,char  org_char )
{
    FlSignalBuffer EventPara;
	char tmp_spell[8];

	unsigned char i,py_index,py_max,py_cursor,disp_width;
	unsigned char key_max,py_page,char_max=21,keyValEx;
	unsigned char team_idx=0;
	
	key_max=0;

    memset(&gPYCode,0,sizeof(gPYCode));

    if((org_char>='2')&&(org_char<='9') )
	{
	    KeyBuf[key_max]=org_char;
	    KeyBuf[key_max+1]='\0';
		key_max++;

		if(ConvPYInpCode(DAN_PIN,(char *)KeyBuf,&gPYCode))
		{
			py_index=0;					
			py_cursor=0;
			py_page=0;
			for(py_max=0;gPYCode.ssPinYun[py_max][0]!='\0';py_max++);
		}
		else
		{
		    key_max--;
		    KeyBuf[key_max+1]='\0';
		}
	}
	
	appsys.byte_need_redraw=1;
	

    while(1)
    {
        if(appsys.byte_need_redraw>0)
        {
            if(0xFF ==  appsys.byte_need_redraw)
            {					
                return 0;
			}
		
            appsys.byte_need_redraw=0;
			disp_width=0;

            clear_chs_rect();
	        lcd_goto_xy(CHS_POS_LEFT, CHS_POS_TOP);
            while(1)
			{
                memset(tmp_spell,0,sizeof(tmp_spell));

				for(i=0;i< strlen(gPYCode.ssPinYun[py_index]);i++)
				{
                    if( !(' ' == gPYCode.ssPinYun[py_index][i] || 0 == gPYCode.ssPinYun[py_index][i]))
                    {
                         tmp_spell[i] = gPYCode.ssPinYun[py_index][i];
					}
					else
					{
                         break;
					}
				}
			
				if(tmp_spell[0]==0)
				{
				    if(team_idx >0)
				    {
						lcd_goto_xy(lcd_get_x(), CHS_POS_TOP+2);
						show_bitmap(BMP_ARROW_LEFT_BIN);
						lcd_add_x(8);
					}
					break;
				}	
				if((disp_width+strlen((char *)tmp_spell)+1)> char_max)
				{
                    team_idx = py_index;
					lcd_goto_xy(lcd_get_x(), CHS_POS_TOP+3);
					show_bitmap(BMP_ARROW_RIGHT_BIN);
					lcd_add_x(8);
					break;
				}						
				if(py_cursor==py_index)
				{
					appsys.flag_color_reverse=1;
				}
				lcd_put_string((char *)tmp_spell);
				appsys.flag_color_reverse=0;
				lcd_add_x(lcd_put_char(' '));
				
				disp_width+=strlen((char *)tmp_spell)+1;
				py_index++;
			}
		}
		else
		{
	        if(1 == app_get_message( &EventPara) )
		    {
		        if(EventPara.eventTyp ==EVENT_KEY)
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
                        if(TFKEY_SOFT_LEFT==  EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
                        {
							for(i=0;gPYCode.ssPinYun[py_cursor][i]!=0;i++)
							{   	
				            	spell_str[i]=gPYCode.ssPinYun[py_cursor][i];
				            }	
				            spell_str[i]=0;
				           
				            return (py_cursor+1);
						}
						else if(TFKEY_SOFT_RIGHT==  EventPara.sig_p.key_evt.key_val)
						{
			            	if(key_max>0)
				        	{
					            for(key_max=0;KeyBuf[key_max]!='\0';key_max++);
					            KeyBuf[--key_max]='\0';

                                team_idx=0;
								
                                if(0 == key_max)
                                {
                                    clear_chs_rect();
                                    return 0;
								}
								
								if(ConvPYInpCode(DAN_PIN,(char *)KeyBuf,&gPYCode))
								{
									py_index=0;					
									py_cursor=0;
									py_page=0;
									for(py_max=0;gPYCode.ssPinYun[py_max][0]!='\0';py_max++);

								}

								appsys.byte_need_redraw=1;
							}
						}
						else if(TFKEY_EXIT==  EventPara.sig_p.key_evt.key_val)
						{
						    clear_chs_rect();
                            return 0;
						}
				 	    else if( TFKEY_SHARP == EventPara.sig_p.key_evt.key_val)
						{
						    clear_chs_rect();
					        p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
							return 0;
						}
					    else if( TFKEY_STAR ==  EventPara.sig_p.key_evt.key_val)
						{
							unsigned short ret_wchar=wnd_get_sign_option(p_edit_wnd);
							appsys.byte_need_redraw=0xFF;
						    return ret_wchar;
						}	
				        else if ( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)) // Move highlight left
				        {
				         	if(py_cursor>0)
				            {
				            	py_cursor--;          	
				            	if(py_cursor<py_page)
				            	{   //change pages
				            		disp_width=0;
				            		for(py_page=py_cursor;py_page>0;py_page--)
				            		{
				            			disp_width=disp_width+strlen((char *)gPYCode.ssPinYun[py_page])+1;
				            			if(char_max<disp_width)
				            			{
				            				py_page++;
				            				break;
				            			}
				            		}
				            	}
				            	py_index=py_page;
				            	appsys.byte_need_redraw=1;
				            }        
				        }
				        else if ( (TFKEY_DOWN== EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val)) // Move highlight right
				        {
				            if((py_cursor+1)<py_max)
				            {
				            	py_cursor++;            	
				            	if(py_cursor>=py_index)
				            	{	
				            		py_page=py_cursor;
				            	}
				            	py_index=py_page;
				            	appsys.byte_need_redraw=1;
				            }            
				        }
						else
						{
                            keyValEx=get_key_value(EventPara.sig_p.key_evt.key_val);

                            if((key_max<6)&&((keyValEx>='2')&&(keyValEx<='9')))
						    {		    	
							    KeyBuf[key_max]=keyValEx;
							    KeyBuf[key_max+1]='\0';
								key_max++;	  


								if(ConvPYInpCode(DAN_PIN,(char *)KeyBuf,&gPYCode))
								{ 
						            appsys.byte_need_redraw=1;
									py_index=0;					
									py_cursor=0;
									py_page=0;
									for(py_max=0;gPYCode.ssPinYun[py_max][0]!='\0';py_max++);
							    }
								else
								{
								    key_max--;
								    KeyBuf[key_max+1]='\0';
								}
							}
						}
					}
		       	}
				else if(EVENT_TIMER == EventPara.eventTyp )
				{

				}
	        }
		}
	}	
	return 0;
}


unsigned short make_lianxiang_5d(EDITWND * p_edit_wnd)
{
    #define CNT_PER_PAGE  6
    char hz_buf[127];

    FlSignalBuffer EventPara;
    unsigned char  KeyVal;
	unsigned short gb_buf[128];
	unsigned char  uni_index,uni_page,uni_max,sel_idx;	

	uni_index=0;					
	uni_page=0;
	uni_max =0;
	uni_index=0;
	sel_idx=0;
	memset(hz_buf,0,sizeof(hz_buf));

    if(ReadLXCode((unsigned char *)p_edit_wnd->lx_buf, (unsigned char *)hz_buf))
    {
		memset(gb_buf,0,sizeof(gb_buf));
		a2w(gb_buf,hz_buf);
		uni_max = (unsigned char)wstrlen(gb_buf);	
    }
	else
	{
   		return 0;
	}

		
		
    appsys.byte_need_redraw=1;
    while(1)
    {
        if(appsys.byte_need_redraw>0)
        { 
            if(0xFF ==  appsys.byte_need_redraw)
            {					
                return 0;
			}
		
            appsys.byte_need_redraw=0;
			uni_index =0;

            clear_chs_rect();
			lcd_goto_xy(CHS_POS_LEFT, CHS_POS_TOP);

			if(uni_page>0)
			{
			    lcd_goto_xy(CHS_POS_LEFT, CHS_POS_TOP+2);
				show_bitmap(BMP_ARROW_LEFT_BIN);
				lcd_goto_xy(lcd_get_x()+7, CHS_POS_TOP);
			}

            while(1)
			{
				if(gb_buf[ uni_page * CNT_PER_PAGE + uni_index]==0)
				{
					break;
				}

				
				lcd_add_x(lcd_put_char(uni_index+1 +0x30) +1);
				
                if(uni_index == sel_idx) {appsys.flag_color_reverse=1;}
				lcd_add_x(lcd_put_wchar(gb_buf[uni_page * CNT_PER_PAGE + uni_index]));

				appsys.flag_color_reverse=0;
				
				uni_index++;
				if(( CNT_PER_PAGE == uni_index )&&(gb_buf[ uni_page * CNT_PER_PAGE + uni_index ]!=0))
				{
				    lcd_goto_xy(lcd_get_x(), CHS_POS_TOP+3);
					show_bitmap(BMP_ARROW_RIGHT_BIN);
					break;
				}
			}	
		
		}
		else
		{
	        if(1 == app_get_message( &EventPara) )
		    {
		        if(EventPara.eventTyp ==EVENT_KEY)
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
    			   	    if (TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
				        {
				            clear_chs_rect();
							p_edit_wnd->lx_buf[0] =(gb_buf[uni_page *CNT_PER_PAGE +sel_idx]&0xFF00)>>8  ;
							p_edit_wnd->lx_buf[1] =(gb_buf[uni_page *CNT_PER_PAGE +sel_idx]&0x00FF)>>0  ;
							p_edit_wnd->lx_buf[2] =0 ;
							
							return gb_buf[uni_page *CNT_PER_PAGE +sel_idx];
				        }
				        else if ((TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
				        {
				            clear_chs_rect();
				        	return 0;
				        }
						else if ( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) ) // Move highlight left
				        {
				         	if(uni_page>0)
				            {
				            	uni_page--;    
								sel_idx =0;
								appsys.byte_need_redraw=1;
				            }   
				        }
				        else if ( (TFKEY_DOWN== EventPara.sig_p.key_evt.key_val)) // Move highlight right
				        {
				            if( (uni_page+1) * CNT_PER_PAGE <uni_max)
				            {
				            	uni_page++;       
								sel_idx=0;
                                appsys.byte_need_redraw=1;
				            }  
				        }
						else if  ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
						{
                            if(sel_idx >0)
                            {
                                sel_idx --;
								appsys.byte_need_redraw=1;
							}
							else
							{
					         	if(uni_page>0)
					            {
					            	uni_page--;          	
									appsys.byte_need_redraw=1;
									sel_idx =(CNT_PER_PAGE-1);
					            }   
							}
						}
						else if(TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val)
						{
                            if(uni_index >0)
                            {
                                if(sel_idx < (uni_index-1) )
                                {
                                     sel_idx++;
									 appsys.byte_need_redraw=1;
								}
								else
								{
						            if( (uni_page+1) * CNT_PER_PAGE <uni_max)
						            {
						            	uni_page++;            	
		                                appsys.byte_need_redraw=1;
										sel_idx=0;
						            }  
								}
							}
						}
					    else if( TFKEY_SHARP == EventPara.sig_p.key_evt.key_val)
						{
						    clear_chs_rect();
					        p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
							return 0;
						}
					    else if( TFKEY_STAR ==  EventPara.sig_p.key_evt.key_val)
						{
							unsigned short ret_wchar=wnd_get_sign_option(p_edit_wnd);
							appsys.byte_need_redraw=0xFF;
						    return ret_wchar;
						}	
						else 
						{
                              KeyVal=get_key_value(EventPara.sig_p.key_evt.key_val);
							  if(KeyVal>='1' && KeyVal<='6')
							  {
								  clear_chs_rect();
								  p_edit_wnd->lx_buf[0] =(gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31]&0xFF00)>>8  ;
							      p_edit_wnd->lx_buf[1] =(gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31]&0x00FF)>>0  ;
							      p_edit_wnd->lx_buf[2] =0 ;
                                  return gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31];
							  }
						}
					}
		        }
	        }
		}
    }
	return 0;
}
































































////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////GPRS FUNCTIONS///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////






unsigned char check_packet_valid(unsigned char * pbuf, int length)
{
   	unsigned short crc16,tmpcrc;
	crc16=pbuf[0];
	crc16<<=8;
	crc16+=pbuf[1];

	tmpcrc = crc16_ccitt(&pbuf[2], length-2);
	if(tmpcrc == crc16 )
	{
	    return 1;
	}
    return 0;
}
































































void remove_simsms_id(unsigned char id_pos)
{
 
}

void app_show_sim_sms_array()
{
    FlSignalBuffer EventPara;
	T_SMS_RECORD   sms_record;
	T_BOOK_ENTRY   book_entry;
	char tmp_buf[40];

    appsys.byte_need_redraw=0xFF;
	appsys.byte_simsms_idpos=0;

    while(1)
    {
         if(appsys.byte_need_redraw>0)
         {
		     lcd_clear_screen();

			 show_caption(get_multi_string((char * *)text_simcard_sms));
			
             show_soft_key_prompt( get_multi_string((char * *) text_details), get_multi_string((char * *) text_back));

			 memset(&sms_record,0,sizeof(sms_record));
			 memset(tmp_buf,0,sizeof(tmp_buf));

             if(appsys.byte_simcard_sms_cnt>0)
             {
				 if(1 == mu_getsimcard_sms(appsys.byte_simsms_idpos +1 , &sms_record))
				 {			 
					 lcd_goto_x_line(3, 1);lcd_clear_line(SCREEN_WIDTH-3);
					 lcd_goto_x_line(3, 2);lcd_clear_line(SCREEN_WIDTH-3);	
					 lcd_goto_x_line(3, 3);lcd_clear_line(SCREEN_WIDTH-3);	

					 lcd_goto_x_line(3, 1);
					 
	                 sprintf(tmp_buf,"第%d条, 共%d条", (appsys.byte_simsms_idpos +1), appsys.byte_simcard_sms_cnt);
					 lcd_put_string(tmp_buf);
					 
					 memset(&book_entry,0,sizeof(book_entry));

					 lcd_goto_x_line(3, 2);
					 if(find_book_entry_by_numbers(sms_record.str_num, &book_entry))
					 {
				          lcd_put_string(book_entry.name);
					 }	
					 else
					 {
				          lcd_put_string(sms_record.str_num);
					 }
					 
				     lcd_goto_x_line(3, 3);
					 sprintf(tmp_buf,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d", sms_record.date.year,sms_record.date.mon, sms_record.date.day, sms_record.time.hour,sms_record.time.min,sms_record.time.sec);
					 lcd_put_string(tmp_buf);
				 }
				 else
				 { 
					 lcd_goto_x_line(3, 2);lcd_clear_line(SCREEN_WIDTH-3);
					 lcd_goto_x_line(3, 2);			 
	                 strcpy(tmp_buf,"  此短信息不可显示");
					 lcd_put_string(tmp_buf);
				 }
             }
			 else 
			 {
                 break;
			 }

			 lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
			 appsys.byte_need_redraw=0;
         }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {
					 if(EventPara.sig_p.key_evt.isPressed)
					 {
					     if( (TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val)||(TFKEY_CONFIRM==EventPara.sig_p.key_evt.key_val))
						 {
						     app_show_sms_record(&sms_record, appsys.byte_simsms_idpos);
							 appsys.byte_need_redraw=0xFF;
							 if(0 == appsys.byte_simcard_sms_cnt)
							 {
                                 return ;
							 }
					     }
					     if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						 {
						     appsys.byte_need_redraw=0xFF;
						     return;
					     }
						 else if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val)||(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val))
						 {
                             if(appsys.byte_simsms_idpos>0)
                             {
                                 appsys.byte_simsms_idpos--;
							 }
							 else
							 {
                                 if(appsys.byte_simcard_sms_cnt>0)
                                 {
                                     appsys.byte_simsms_idpos=appsys.byte_simcard_sms_cnt-1;
								 }
							 }
							 appsys.byte_need_redraw=0xFF;
						 }
						 else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)||(TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val))
						 {
						     if(appsys.byte_simcard_sms_cnt>0)
						     {
                                 if(appsys.byte_simsms_idpos<appsys.byte_simcard_sms_cnt-1)
	                             {
	                                 appsys.byte_simsms_idpos++;
								 }
								 else
								 {
                                     appsys.byte_simsms_idpos=0;
								 }
								 appsys.byte_need_redraw=0xFF;
						     }
						 }
					 }
		         }
				 else  if(EVENT_TIMER== EventPara.eventTyp )
				 {
	                 if(appsys.byte_need_exit > 0)
	                 {
	                      return;
					 }
				 }
		     }
         }
    }
}



void app_show_sim_book_array()
{
    FlSignalBuffer EventPara;
	T_BOOK_ENTRY   book_entry;
	char tmp_buf[40];
	unsigned char flag_valid_book=0;

    appsys.byte_need_redraw=0xFF;
	appsys.byte_simbook_idpos=0;

    while(1)
    {
         if(appsys.byte_need_redraw>0)
         {
		     lcd_clear_screen();

	
			
       

			

             if(appsys.byte_simcard_book_cnt>0)
             {

                 memset(tmp_buf,0,sizeof(tmp_buf));

			     sprintf(tmp_buf,"%s (%d/%d)", get_multi_string((char * *)text_simcard_book),(appsys.byte_simbook_idpos +1), appsys.byte_simcard_book_cnt);
	        	 show_caption(tmp_buf);
			 
             	 memset(&book_entry,0,sizeof(book_entry));
				 if(1 == mu_getsimcard_book((appsys.byte_simbook_idpos +1) , &book_entry))
				 {			 
					 lcd_goto_x_line(3, 1);lcd_clear_line(SCREEN_WIDTH-3);
					 lcd_goto_x_line(3, 2);lcd_clear_line(SCREEN_WIDTH-3);	
					 lcd_goto_x_line(3, 3);lcd_clear_line(SCREEN_WIDTH-3);	

					 lcd_goto_x_line(3, 1);
					 
	    		     lcd_put_string(book_entry.name);
					 
					 lcd_goto_x_line(3, 2);

				     lcd_put_string(book_entry.num);
					
					 
				     lcd_goto_x_line(3, 3);

					 flag_valid_book =1;
				 }
				 else
				 { 
					 lcd_goto_x_line(3, 2);lcd_clear_line(SCREEN_WIDTH-3);
					 lcd_goto_x_line(3, 2);			 
	                 strcpy(tmp_buf," ");
					 lcd_put_string(tmp_buf);
					 flag_valid_book=0;
				 }

				  show_soft_key_prompt( flag_valid_book ==1? get_multi_string((char * *) text_option):get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));
             }
			 else 
			 {
                 break;
			 }

			 lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
			 appsys.byte_need_redraw=0;
         }
		 else
		 {
		     if(1 == app_get_message( &EventPara) )
			 {
		         if(EVENT_KEY == EventPara.eventTyp )
				 {
					 if(EventPara.sig_p.key_evt.isPressed)
					 {
					     if( (TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val)||(TFKEY_CONFIRM==EventPara.sig_p.key_evt.key_val))
						 {
						    if(1 == flag_valid_book)
						    {

								    switch ( app_menu_select((char ***)mem_simbook_opt, 2, NULL) )
				                    {
				                        case 1:
					                        appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
											if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
											{
				                                 app_dial_out(book_entry.num);
											}
				                            break;

				                        case 2:
											new_book_entry(book_entry.num);
				                            break;
											
				                        default:
				                            break;
				                    }
							}
					     }
					     if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						 {
						     appsys.byte_need_redraw=0xFF;
						     return;
					     }
						 else if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val)||(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val))
						 {
                             if(appsys.byte_simbook_idpos>0)
                             {
                                 appsys.byte_simbook_idpos--;
							 }
							 else
							 {
                                 if(appsys.byte_simcard_book_cnt>0)
                                 {
                                     appsys.byte_simbook_idpos=appsys.byte_simcard_book_cnt-1;
								 }
							 }
							 appsys.byte_need_redraw=0xFF;
						 }
						 else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)||(TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val))
						 {
						     if(appsys.byte_simcard_book_cnt>0)
						     {
                                 if(appsys.byte_simbook_idpos<appsys.byte_simcard_book_cnt-1)
	                             {
	                                 appsys.byte_simbook_idpos++;
								 }
								 else
								 {
                                     appsys.byte_simbook_idpos=0;
								 }
								 appsys.byte_need_redraw=0xFF;
						     }
						 }
						 else if(TFKEY_SEND==EventPara.sig_p.key_evt.key_val)
						 {
			                 appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
						     if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
						     {
						          appsys.flag_fast_dial=1;
			                      app_dial_out(book_entry.num);
								  appsys.flag_fast_dial=0;
						     }
						 }
					 }
		         }
				 else  if(EVENT_TIMER== EventPara.eventTyp )
				 {
	                 if(appsys.byte_need_exit > 0)
	                 {
	                      return;
					 }
				 }
		     }
         }
    }
}


void app_show_sms_from_simcard()
{
	if(get_smsinfo_from_simcard()>0)
	{
        app_show_sim_sms_array();
	}
	else
	{
        msg(get_multi_string((char * *) text_no_sms_on_sim));
		delay_ms(1600);
	}
}


void app_show_book_from_simcard()
{
	if(get_bookinfo_from_simcard()>0)
	{
        app_show_sim_book_array();
	}
	else
	{
        msg(get_multi_string((char * *) text_no_book_on_sim));
		delay_ms(1600);
	}
}







//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-------------------------------------UI---TO--ANDROID -----------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

char * andr_build_431_callout(char * str_cmd, char * phone_number)
{
   strcat(str_cmd,"3,1,"); 
   strcat(str_cmd,phone_number);	
   return str_cmd;
}

char * andr_build_432_hook_onoff(char * str_cmd, unsigned char flag_accept)
{
   strcat(str_cmd,"3,2,");  
   strcat(str_cmd, (flag_accept>0)?"1":"0");	
   return str_cmd;
}



char * andr_build_434_sendsms(char * str_cmd,char * phone_number, char * str_context)
{
   strcat(str_cmd,"3,4,\""); 
   strcat(str_cmd, phone_number);  strcat(str_cmd,"\";\""); 
   strcat(str_cmd, str_context); 
   strcat(str_cmd,"\"");
   return str_cmd;
}

char * andr_build_435_play_voice(char * str_cmd, int  voc_id)
{
   strcat(str_cmd,"3,5,"); 

   sprintf(&str_cmd[strlen(str_cmd)],"%d",voc_id); 
   return str_cmd;
}

char * andr_build_436_stop_voice(char * str_cmd)
{   
   strcat(str_cmd,"3,6,0"); 
   return str_cmd;
}

char * andr_build_437_talking_ctrl(char * str_cmd, char * ctrl_type, char cmd)
{
   strcat(str_cmd,"3,7,"); 
   strcat(str_cmd, ctrl_type); strcat(str_cmd,",");
   str_cmd[strlen(str_cmd)] = cmd; 
   return str_cmd;
}


char * andr_build_438_voc_path(char * str_cmd,char vocpath)
{
   strcat(str_cmd,"3,8,"); 
   str_cmd[strlen(str_cmd)] = vocpath; 
   return str_cmd;
}



char * andr_build_43A_voc_level(char * str_cmd,unsigned char voc_type, unsigned char voc_lvl)
{
   strcat(str_cmd,"3,A,"); 
   str_cmd[strlen(str_cmd)] = 0x30 +voc_type;    strcat(str_cmd,",");
   sprintf(&str_cmd[strlen(str_cmd)],"%d" ,voc_lvl);
   
   return str_cmd;
}

char * andr_build_43B_shut_phone(char * str_cmd, unsigned char flag_reboot)
{
   strcat(str_cmd,"3,B,"); 
   str_cmd[strlen(str_cmd)] = (flag_reboot == 1)?'1':'2'; 
   return str_cmd;
}

char * andr_build_43C_play(char * str_cmd, char dtmf)
{
   strcat(str_cmd,"3,C,"); 
   str_cmd[strlen(str_cmd)] = dtmf; 
   return str_cmd;
}

char * andr_build_43D1_set_ring_music(char * str_cmd, int  voc_id)
{
   strcat(str_cmd,"3,D,1,"); 
   sprintf(&str_cmd[strlen(str_cmd)],"%d",voc_id); 
   return str_cmd;
}

char * andr_build_43D2_set_sms_music(char * str_cmd, char sms_voc)
{
   strcat(str_cmd,"3,D,2,"); 
   str_cmd[strlen(str_cmd)] = sms_voc; 
   return str_cmd;
}



//--------------------------------------------------------------------



char * andr_build_442_query_bluetooth_devices(char * str_cmd)
{
   strcat(str_cmd,"4,2,0"); 
   return str_cmd;
}

char * andr_build_443_query_wifi_devices(char * str_cmd)
{
   strcat(str_cmd,"4,3,0"); 
   return str_cmd;
}

char * andr_build_444_read_simbook_count(char * str_cmd)
{
   strcat(str_cmd,"4,4,0"); 
   return str_cmd;
}

char * andr_build_444_read_simbook_user(char * str_cmd, char * user_pos)
{
   strcat(str_cmd,"4,4,"); 
   strcat(str_cmd, user_pos); 
   return str_cmd;
}

char * andr_build_445_read_simsms_count(char * str_cmd)
{
   strcat(str_cmd,"4,5,0"); 
   return str_cmd;
}

char * andr_build_445_read_simbook_user(char * str_cmd, char * sms_pos)
{
   strcat(str_cmd,"4,5,"); 
   strcat(str_cmd, sms_pos); 
   return str_cmd;
}


char * andr_build_446_query_call_waiting(char * str_cmd)
{
   strcat(str_cmd,"4,6,2"); 
   return str_cmd;
}

char * andr_build_446_setup_call_waiting(char * str_cmd, unsigned char flag_enable)
{
   strcat(str_cmd,"4,6,"); 
   strcat(str_cmd, (flag_enable >0) ? "1":"0"); 
   return str_cmd;
}

char * andr_build_447_query_call_divert(char * str_cmd)
{
   strcat(str_cmd,"4,7,5,0"); 
   return str_cmd;
}

char * andr_build_447_setup_call_divert(char * str_cmd,   char  divert_type, unsigned char flag_enable, char * phone_number)
{
   strcat(str_cmd,"4,7,"); 
   str_cmd[strlen(str_cmd)] =divert_type; strcat(str_cmd,",");
   strcat(str_cmd,(flag_enable >0) ? "1":"0"); strcat(str_cmd,",");
   strcat(str_cmd,phone_number);
   return str_cmd;
}


char * andr_build_448_query_simpin_enabled(char * str_cmd)
{
   strcat(str_cmd,"4,8,8"); 
   return str_cmd;
}

char * andr_build_448_setup_simpin_enabled(char * str_cmd, char * str_pin, unsigned char flag_enable)
{
   strcat(str_cmd,"4,8,"); 
   strcat(str_cmd, (flag_enable >0) ? "5":"6");
   return str_cmd;
}

char * andr_build_448_input_simpin(char * str_cmd, char * str_pin)
{
   strcat(str_cmd,"4,8,1,"); 
   strcat(str_cmd, str_pin);
   return str_cmd;
}

char * andr_build_448_input_simpuk(char * str_cmd, char * str_puk, char * str_new_pin)
{
   strcat(str_cmd,"4,8,2,"); 
   strcat(str_cmd, str_puk);strcat(str_cmd,",");
   strcat(str_cmd, str_new_pin);
   return str_cmd;
}

char * andr_build_448_input_simpin2(char * str_cmd, char * str_pin2)
{
   strcat(str_cmd,"4,8,3,"); 
   strcat(str_cmd, str_pin2);
   return str_cmd;
}

char * andr_build_448_input_simpuk2(char * str_cmd, char * str_puk2, char * str_new_pin2)
{
   strcat(str_cmd,"4,8,4,"); 
   strcat(str_cmd, str_puk2);strcat(str_cmd,",");
   strcat(str_cmd, str_new_pin2);
   return str_cmd;
}

char * andr_build_448_change_pin(char * str_cmd, char * str_old_pin, char * str_new_pin)
{
   strcat(str_cmd,"4,8,7,"); 
   strcat(str_cmd, str_old_pin);strcat(str_cmd,",");
   strcat(str_cmd, str_new_pin);
   return str_cmd;
}


char * andr_build_449_query_bluetooth_enabled(char * str_cmd)
{
   strcat(str_cmd,"4,9,2"); 
   return str_cmd;
}

char * andr_build_449_setup_bluetooth_enabled(char * str_cmd,unsigned char flag_enable)
{
   strcat(str_cmd,"4,9,"); 
   strcat(str_cmd, (flag_enable >0) ? "1":"0");
   return str_cmd;
}


char * andr_build_44A_query_wifi_enabled(char * str_cmd)
{
   strcat(str_cmd,"4,A,2"); 
   return str_cmd;
}

char * andr_build_44A_setup_wifi_enabled(char * str_cmd,unsigned char flag_enable)
{
   strcat(str_cmd,"4,A,"); 
   strcat(str_cmd, (flag_enable >0) ? "1":"0");
   return str_cmd;
}

char * andr_build_44B_get_imei(char * str_cmd)
{
   strcat(str_cmd,"4,B,1"); 
   return str_cmd;
}

char * andr_build_44C_get_imsi(char * str_cmd)
{
   strcat(str_cmd,"4,C,1"); 
   return str_cmd;
}

char * andr_build_44D_get_iccid(char * str_cmd)
{
   strcat(str_cmd,"4,D,1"); 
   return str_cmd;
}

char * andr_build_44E_get_csq(char * str_cmd)
{
   strcat(str_cmd,"4,E,1"); 
   return str_cmd;
}

char * andr_build_44F_get_andr_ver(char * str_cmd)
{
   strcat(str_cmd,"4,F,1"); 
   return str_cmd;
}

char * andr_build_44G_set_sleep(char * str_cmd, char sleep_type)
{
   strcat(str_cmd,"4,G,"); 
   str_cmd[strlen(str_cmd)] =sleep_type;
   return str_cmd;
}







int  get_blue_wifi_list(char plist[MAX_BLUE_WIFI_CNT][BLUE_WIFI_NAME_LEN],  char * str_msg)
{

   char temp_blue[BLUE_WIFI_NAME_LEN];
   int lst_cnt=0;
   char * pmsg= str_msg;

   unsigned short npos = 0xFFFF;

   while((npos = find_char_pos(pmsg,';',1)) !=0xFFFF)
   {
	   memset(temp_blue,0,sizeof(temp_blue));
       memcpy(temp_blue, pmsg,npos);

	   if(dumpstr( temp_blue, npos , plist[lst_cnt], BLUE_WIFI_NAME_LEN-1, "\"","\"" ))
	   {
	      lst_cnt++;
	   }
	   pmsg = & pmsg[npos +1];

	   if(lst_cnt >= (MAX_BLUE_WIFI_CNT-1))
	   {
           pmsg =NULL;
		   break;
	   }
   }

   if(pmsg !=NULL)
   {
       memset(temp_blue,0,sizeof(temp_blue));
       strcpy(temp_blue, pmsg);
	   if(dumpstr( temp_blue, npos , plist[lst_cnt], BLUE_WIFI_NAME_LEN-1, "\"","\"" ))
	   {
	      lst_cnt++;
	   }
   }
   return lst_cnt;
}



int  get_divert_list(T_CALL_DIVERT_STATUS * pdivt,  char * str_msg)
{
   char temp_buffer[80];
   char divt_buffer[80];
   int lst_cnt=0;
   unsigned char divt_mode =0;
   char * pmsg= str_msg;

   unsigned short npos = 0xFFFF;

   //"1,1, 13633061236";"2,0,0";"3,0,0";"4,0,0"

   while((npos = find_char_pos(pmsg,';',1)) !=0xFFFF)
   {
       memset(divt_buffer,0,sizeof(divt_buffer));
	   memset(temp_buffer,0,sizeof(temp_buffer));
       memcpy(temp_buffer, pmsg,npos);

	   if(dumpstr( temp_buffer, strlen(temp_buffer) , divt_buffer, sizeof(divt_buffer)-1, "\"","\"" ))
	   {
	      //1,1,13633061236
	      //^ ^ ^
          //0123456789
	  
		  divt_mode = ex_atoi(&divt_buffer[0], 1);
		  if(divt_mode >=1 && divt_mode <=4)
		  {
		       pdivt[divt_mode -1].status = ex_atoi(&divt_buffer[2], 1);
			   if(1 == pdivt[divt_mode -1].status)
			   {
			       strncpy(pdivt[divt_mode -1].num, &divt_buffer[4], BOOK_ENTRY_NUM_FIELD_SIZE-1);			   
			   }
               lst_cnt++;
		  }
	   }
	   pmsg = &pmsg[npos +1];

	   if(lst_cnt >= MAX_DIVT_CNT)
	   {
           pmsg =NULL;
		   break;
	   }
   }

   if(pmsg !=NULL)
   {
       memset(divt_buffer,0,sizeof(divt_buffer));
	   memset(temp_buffer,0,sizeof(temp_buffer));
       strcpy(temp_buffer, pmsg);

	   if(dumpstr( temp_buffer, strlen(temp_buffer) , divt_buffer, sizeof(divt_buffer)-1, "\"","\"" ))
	   {
	      //4,0,0
	      //^ ^ ^
          //0123456789
	  
		  divt_mode = ex_atoi(&divt_buffer[0], 1);
		  if(divt_mode >=1 && divt_mode <=4)
		  {	  
		       pdivt[divt_mode -1].status = ex_atoi(&divt_buffer[2], 1);
			   if(1 == pdivt[divt_mode -1].status)
			   {
			       strncpy(pdivt[divt_mode -1].num, &divt_buffer[4], BOOK_ENTRY_NUM_FIELD_SIZE-1);				   
			   }
               lst_cnt++;
		  }
	   }
   }
   return lst_cnt;
}


int ex_atoi(unsigned char * p , int len)
{
    char tmp_buf[32];
	memset(tmp_buf,0,sizeof(tmp_buf));
	memcpy(tmp_buf,p,len);
	return atoi(tmp_buf);
}

unsigned char get_sms_from_msg(char * str_msg)
{
	 unsigned short dh_pos=0xFFFF;
	 unsigned short et_pos=0xFFFF;
	 char * pmsg = str_msg;
	 char  str_big[320];
	 char  tmp_buf[32];

	 memset(&appsys.tp_sms,0,sizeof(appsys.tp_sms));

	 //"1008611";"yyyyMMddhhmmss";"Hello"
	 dh_pos=find_char_pos(pmsg, ';', 1);

	 if(dh_pos !=0xFFFF)
	 {
	     memset(str_big,0,sizeof(str_big));
	     memcpy(str_big, pmsg, dh_pos);

		 if(dumpstr(str_big, strlen(str_big), appsys.tp_sms.str_num, sizeof(appsys.tp_sms.str_num)-1, "\"", "\"") >0)
		 {

              pmsg= &pmsg[dh_pos +1];

		  	  dh_pos=find_char_pos(pmsg, ';', 1);
			  if(dh_pos !=0xFFFF)
			  {

			     memset(str_big,0,sizeof(str_big));
			     memcpy(str_big, pmsg, dh_pos);

				 if(dumpstr(str_big, strlen(str_big), tmp_buf, sizeof(appsys.tp_sms.str_num)-1, "\"", "\"") >0)
				 {
	
                      appsys.tp_sms.date.year= ex_atoi(&tmp_buf[0],4);
					  appsys.tp_sms.date.mon=  ex_atoi(&tmp_buf[4],2);
					  appsys.tp_sms.date.day=  ex_atoi(&tmp_buf[6],2);

					  appsys.tp_sms.time.hour= ex_atoi(&tmp_buf[8] ,2);
					  appsys.tp_sms.time.min = ex_atoi(&tmp_buf[10],2);
					  appsys.tp_sms.time.sec = ex_atoi(&tmp_buf[12],2);
				  

					  pmsg= &pmsg[dh_pos +1];
	  

					  dh_pos = find_char_pos(pmsg, '\"', 1);
				  
					  et_pos = find_char_pos(pmsg, '\"', 0xFFFF);
	
					  if(dh_pos !=0xFFFF && et_pos != 0xFFFF && dh_pos < et_pos)
					  {

					      memset(str_big,0,sizeof(str_big));
						  memcpy(str_big, &pmsg[dh_pos+1], et_pos - dh_pos - 1);

                          strncpy(appsys.tp_sms.context, str_big, SMS_CONTEXT_LENGTH);

						  appsys.tp_sms.sms_flag.flag_read=0;
						  if(1 == appsys.flag_genie_trace)DebugPrintf("-----------got a new message-----------\r\n");
						  return 1;
					  }
					  else
					  {
                         if(1 == appsys.flag_genie_trace)DebugPrintf("get_sms_from_msg err4 : dh_pos =%d, et_pos=%d\r\n", dh_pos,et_pos);
					  }

				 }	
				 else
				 {
                     if(1 == appsys.flag_genie_trace)DebugPrintf("get_sms_from_msg err5 : buf0 =%s\r\n", str_big);
				 }
			  }
			  else
			  {
                 if(1 == appsys.flag_genie_trace)DebugPrintf("get_sms_from_msg err6 \r\n");
			 }  
	     }
		 else
		 {
            if(1 == appsys.flag_genie_trace)DebugPrintf("get_sms_from_msg err7 : buf0 =%s\r\n", str_big);
		 }
	 }
	 else
	 {
           if(1 == appsys.flag_genie_trace)DebugPrintf("get_sms_from_msg err8 \r\n");
	 }
	 return 0;
}


unsigned char get_book_from_msg(char * str_msg)
{
	 unsigned short dh_pos=0xFFFF;
	 unsigned short et_pos=0xFFFF;
	 char * pmsg = str_msg;
	 char  str_big[128];

	 memset(&appsys.tp_book,0,sizeof(appsys.tp_book));

	 //"Tom";"13925255166"
	 dh_pos=find_char_pos(pmsg, ';', 1);

	 if(dh_pos !=0xFFFF)
	 {
	     memset(str_big,0,sizeof(str_big));
	     memcpy(str_big, pmsg, dh_pos);

		 if(dumpstr(str_big, strlen(str_big), appsys.tp_book.name, sizeof(appsys.tp_book.name)-1, "\"", "\"") >0)
		 {
              pmsg= &pmsg[dh_pos +1];

			 if(dumpstr(pmsg, strlen(pmsg), appsys.tp_book.num, sizeof(appsys.tp_book.num)-1, "\"", "\"") >0)
			 {
				  if(1 == appsys.flag_genie_trace)DebugPrintf("-----------get book,user:%s  phone=%s-----------\r\n", appsys.tp_book.name, appsys.tp_book.num);
				  return 1;
			  }
			  else
			  {
                 if(1 == appsys.flag_genie_trace)DebugPrintf("get_book_from_msg err1 \r\n");
			 }  
	     }
		 else
		 {
            if(1 == appsys.flag_genie_trace)DebugPrintf("get_book_from_msg err2 : buf0 =%s\r\n", str_big);
		 }
	 }
	 else
	 {
           if(1 == appsys.flag_genie_trace)DebugPrintf("get_book_from_msg err3 \r\n");
	 }
	 return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------- ANYLYSE MESSAGES---------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void analyse_android_message(FlSignalBuffer  * p_evt, char * str_msg, int msg_len)
{
    //head  ,  type  ,  paras....
    //[0]  [1]  [2] [3] [4]       

	if(1 == appsys.flag_genie_trace)DebugPrintf("analyse_android_message len=%d msg = %s\r\n", msg_len,str_msg);
	if(0 == appsys.flag_got_andr_message){appsys.flag_got_andr_message=1;}
    
    switch(str_msg[0])
   	{
   	    //4.2系统状态更新上报('2')
        case '2':
		{
			  p_evt->eventTyp =EVENT_MODULE;
			  
              switch(str_msg[2])
              {
                  //4.2.1	PIN状态上报('1')
                  case '1':
				  {
					appsys.byte_simcard_state = str_msg[4];
					appsys.flag_got_421=1;
                  }
				  	break;

				  //4.2.2	注册状态上报('2') (包含信号强度RSSI)
				  case '2':
				  {
				  	//2,2,0,3,4,0,China_Unicom
				  	//0123456789012
				  	//    ^ ^ ^ ^

					if(str_msg[4] =='0'  || str_msg[4] =='1' || str_msg[4] =='2'  || str_msg[4] =='3'  )
					{
				  	    appsys.byte_reg_state= str_msg[4]-0x30;
					}
					else{appsys.byte_reg_state = REG_STATE_NOT_REG;}

				 	if(str_msg[6] =='0'  || str_msg[6] =='1' || str_msg[6] =='2'  || str_msg[6] =='3'  )
					{
				  	    appsys.byte_net_state= str_msg[6]-0x30;
					}
					else{appsys.byte_net_state = NET_STATE_UNKNOWN;}


					if(str_msg[8] =='0'  || str_msg[8] =='1' || str_msg[8] =='2'  || str_msg[8] =='3' || str_msg[8] =='4' )
					{
                        appsys.byte_csq_level= str_msg[8]-0x30;
					}
					else {appsys.byte_csq_level =0;}

				 	if(str_msg[10] =='0'  || str_msg[10] =='1' )
					{
                        appsys.byte_roam_state= str_msg[10]-0x30;
					} else {appsys.byte_roam_state = 0;}

					memset(appsys.str_operator_name,0,sizeof(appsys.str_operator_name));
					
					strncpy(appsys.str_operator_name,&str_msg[12], sizeof(appsys.str_operator_name) -1);
					

					if(1 == appsys.flag_genie_trace)DebugPrintf("GOT OPER: %s\r\n", appsys.str_operator_name);
					appsys.flag_got_422=1;
				  }
				  	break;

				  //4.2.3	通话状态上报('3')
				  case '3':
				  {
					 appsys.byte_tlk_state = str_msg[4];

					 if(appsys.byte_tlk_state == '1')
					 {
					     memset(appsys.str_incom_number,0,sizeof(appsys.str_incom_number));
                         strcpy(appsys.str_incom_number,&str_msg[6]);
					 }
					 appsys.flag_got_423=1;
				  }
				  	break;

                  //4.2.4	电源状态上报('4')
				  case '4':
				  {
				  	//2,4,3,2,100
				  	//2,4,2,0,100
				  	//01234567890123456789
				  	//    ^ ^ 
				  	memset(appsys.str_debug_msg,0,sizeof(appsys.str_debug_msg));

					strncpy(appsys.str_debug_msg,str_msg, sizeof(appsys.str_debug_msg) -1);
					
					switch(str_msg[4])
					{
                        case '1':
							appsys.power_service.flag_extern_power=1;
						    appsys.power_service.flag_having_battery=0;
						break;
						
						case '2':
							appsys.power_service.flag_extern_power=0;
						    appsys.power_service.flag_having_battery=1;
						break;
						case '3':
							appsys.power_service.flag_extern_power=1;
						    appsys.power_service.flag_having_battery=1;
						break;
					    default:
							break;
					}


					if(str_msg[6] =='0')
					{
                         appsys.power_service.flag_battery_full=0xFF;
					}
					else if(str_msg[6] =='1')
					{
                         appsys.power_service.flag_battery_full=0;
					}
					else if(str_msg[6] =='2')
					{
                         appsys.power_service.flag_battery_full=1;
					}

					appsys.power_service.battary_status.adc = atoi(&str_msg[8]);

					appsys.flag_got_424=1;
				  }
				  	break;

				  //4.2.5	收到新短信上报('5')
				  case '5':
				  {
					if(1 == get_sms_from_msg (&str_msg[4]))
					{
						 app_record_append(TABLEINFO_SMS_INBOX, (unsigned char *)&appsys.tp_sms, REC_PREFIX_APPEND);
						 
						 appsys.flag_having_unread_sms=1;

						 if( (0 == appsys.flag_call_comming  &&  0 == appsys.flag_talk_rating))
				         {
				             if(sysprop->byte_sms_ring < MAX_SMS_RING_CNT)
				             {
						        // app_play_sms_ring(sysprop->byte_sms_ring);   //由ANDROID后台播放；
				             }
				         }
						 
					     appsys.flag_got_425=1;
					 }
				  }
				  break;

				  //4.2.6	短信息发送结果('6')
				  case '6':
				  {
					 appsys.byte_smsret_state= str_msg[4];
					 appsys.flag_got_426=1;
				  }
				  
                  default:
				  	break;
			  }
		}
		break;

	    //4.4	 读取/返回系统数据('4'/'5')
        case '5':
		{
		 	  p_evt->eventTyp =EVENT_MODULE;
              switch(str_msg[2])
              {

				  //4.4.2	查询蓝牙列表('2')
				  case '2':
				  	{
                       //5,2,"BL Nordic 01";"BL F920_M301";"BL JWL_TEST"
					   appsys.byte_blue_cnt = get_blue_wifi_list(appsys.BLUE_LIST, &str_msg[4]);
					   appsys.flag_got_452=1;
				    }
				  	break;

				  //4.4.3	查询WIFI列表('3')
				  case '3':
				    {
                       appsys.byte_wifi_cnt = get_blue_wifi_list(appsys.WIFI_LIST, &str_msg[4]);
				    }

					appsys.flag_got_453=1;
				  	break;

                  //4.4.4	读取SIM卡联系人('4')
				  case '4':
				  	{
                        if(str_msg[4] == '\"')
                        {
                           if(1 ==get_book_from_msg (&str_msg[4]))
                           {
                                  
						   }
						}
						else
						{
                            appsys.byte_simcard_book_cnt = atoi(&str_msg[4]);
							if(1 == appsys.flag_genie_trace)DebugPrintf("++there are %d book on simcard\r\n",appsys.byte_simcard_book_cnt);
						}

						appsys.flag_got_454=1;
				  	}
				  	break;

				  //4.4.5	读取SIM卡短消息(0x05)
				  case '5':
				  	{
                        if(str_msg[4] == '\"')
                        {
                           if(1 ==get_sms_from_msg (&str_msg[4]))
                           {
                                  
						   }
						}
						else
						{
                            appsys.byte_simcard_sms_cnt = atoi(&str_msg[4]);
							if(1 == appsys.flag_genie_trace)DebugPrintf("++there are %d messages on simcard\r\n",appsys.byte_simcard_sms_cnt);
						}

						appsys.flag_got_455=1;
				  	}
				  	break;
					
                  //4.4.6	呼叫等待设置命令（'6'）
				  case '6':
				    {
	                    appsys.byte_callwaitret_state= str_msg[4];
						appsys.flag_got_456=1;
				    }
				  	break;
					
                  //4.4.7	呼叫转移设置命令（'7'）
				  case '7':

                    if(str_msg[4] == '\"')
                    {
						get_divert_list(appsys.DIVT_LIST, &str_msg[4]);
                    }
					else
					{
					    //5,7,1,1
					    //      ^
					    //0123456789
					    appsys.byte_divtret_state = str_msg[6];
					}
					appsys.flag_got_457=1; 
				  	break;
					
                  //4.4.8	PIN码设置命令（'8'）
				  case '8':
				  	//5,8,8,1
				  	//      ^
                    appsys.byte_pinret_state =str_msg[6];
					appsys.flag_got_458=1;
				  	break;
					
                  //4.4.9	蓝牙开关设置命令（'9'）
				  case '9':
					{
						appsys.byte_blueret_state=str_msg[4];
						appsys.flag_got_459=1;
					}
				  	break;
					
                  //4.4.10	WIFI开关设置命令（'A'）
				  case 'A':
					{
					   appsys.byte_wifiret_state=str_msg[4];
					   appsys.flag_got_45A=1;
				  	}
				  	break;

                  //4.4.11   读取IMEI（'B'）
				  case 'B':
					{
						memset(appsys.str_imei_no,0,sizeof(appsys.str_imei_no));
                        strcpy(appsys.str_imei_no,&str_msg[4]);
				  	}
				  	break;

				  //4.4.12	 读取IMSI（'C'）
				  case 'C':
					{
					   memset(appsys.str_simcimi_number,0,sizeof(appsys.str_simcimi_number));
                       strcpy(appsys.str_simcimi_number,&str_msg[4]);
				  	}
				  	break;

				  //4.4.13	 读取SIM卡的CCID（'D'）
				  case 'D':
					{
					   memset(appsys.str_iccid,0,sizeof(appsys.str_iccid));
                       strcpy(appsys.str_iccid,&str_msg[4]);
				  	}
				  	break;

				  //4.4.14   读取信号强度级别（'E'）
				  case 'E':
					{
                       	if(str_msg[4] =='0'  || str_msg[4] =='1' || str_msg[4] =='2'  || str_msg[4] =='3' || str_msg[4] =='4' )
						{
	                        appsys.byte_csq_level= str_msg[4]-0x30;
						}
					else {appsys.byte_csq_level =0;}
				  	}
				    break;
				  
				  //4.4.15	 查询软件版本（'F'）
				  case 'F':
					{
						int length,i;
						memset(appsys.str_andr_ver,0,sizeof(appsys.str_andr_ver));
                        strcpy(appsys.str_andr_ver,&str_msg[5]);

						length = strlen(appsys.str_andr_ver);
						for(i=0;i<length;i++)
						{
                           if(appsys.str_andr_ver[i]=='\"' || appsys.str_andr_ver[i]==';')
                           {
                               appsys.str_andr_ver[i]=' ';
						   }
						}
				  	}
				  	break; 
				  
                  default:
				  	break;
			  }

		}
		break;	

		default:
			break;
	}
}

//syw add 20180521
int UnicodeToUtf8(unsigned char* pInput, unsigned char *pOutput)    
{
    int len = 0; //记录转换后的Utf8字符串的字节数
    while (*pInput)
    {
        //处理一个unicode字符
        unsigned char low = *pInput;//取出unicode字符的低8位
        pInput++;
        unsigned char high = *pInput;//取出unicode字符的高8位
        unsigned short wchar = high;
		wchar = ((wchar << 8) & 0xff00);
		wchar |= low;
		printf("\n UnicodeToUtf8:%4x.\n", wchar);
        if (wchar <= 0x7F ) //英文字符
        {
            pOutput[len] = (char)wchar;  //取wchar的低8位
            len++;
        }
        else if (wchar >=0x80 && wchar <= 0x7FF)  //可以转换成双字节pOutput字符
        {
            pOutput[len] = 0xc0 |((wchar >> 6)&0x1f);  //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f);  //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
            len++;
        }
        else if (wchar >=0x800 && wchar < 0xFFFF)  //可以转换成3个字节的pOutput字符
        {
            pOutput[len] = 0xe0 | ((wchar >> 12)&0x0f);  //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
            len++;
			int mid = (0x80 | ((wchar >> 6) & 0x3f));
			printf("\n UnicodeToUtf8 mid :%4x.\n", mid);
            pOutput[len] = mid;  //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f);  //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz  
            len++;
        } 

        else //对于其他字节数的unicode字符不进行处理
        {
            return -1;
        }
        pInput ++;//处理下一个unicode字符
    }
    //utf8字符串后面，有个\0
    pOutput [len]= 0;
    return len;
}

int Utf8ToUnicode(unsigned char* pInput, unsigned char* pOutput)  
{
    int outputSize = 0; //记录转换后的Unicode字符串的字节数

    while (*pInput)
    {
        if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
        {
            *pOutput = *pInput;
             pOutput++;
            *pOutput = 0; //小端法表示，在高地址填补0
        }
        else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
        {
            unsigned char high = *pInput;
            pInput++;
            unsigned char low = *pInput;
            if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
            {
                return -1; //如果不是则报错
            }

            *pOutput = (high << 6) + (low & 0x3F);
            pOutput++;
            *pOutput = (high >> 2) & 0x07;
        }
        else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
        {
            unsigned char high = *pInput;
            pInput++;
            unsigned char middle = *pInput;
            pInput++;
            unsigned char low = *pInput;
            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
            {
                return -1;
            }
            *pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
            pOutput++;
            *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
        }
        else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }
        pInput ++;//处理下一个utf8字符
        pOutput ++;
        outputSize += 2;
    }
    //unicode字符串后面，有两个\0  
    *pOutput = 0;
     pOutput++;
    *pOutput = 0;
    return outputSize;
}
