
#include "inc/app_def.h"


unsigned long next;




unsigned short myrand( void )
{
    unsigned short tmp1;
    unsigned short tmp2;
    next = next * 1103515245 + 12345;
    tmp1 = (unsigned short)(next/65536) % 32768;
    tmp2 = (tmp1 / 10);
    tmp2 = tmp2 * 10;
    return( tmp1 - tmp2);  // numbers 0 to 9
    //return(unsigned short)(next/65536) % 32768;
}



unsigned char  mu_enter_pin(  char * sPin )    
{
    unsigned char nret=0;
    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_input_simpin(cmd_buf,sPin));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_pin, succeed \r\n");
			   nret =1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_pin, faled01 \r\n");
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_pin ,failed02! \r\n");
	return nret;
}


unsigned char mu_is_pin_active( void )  
{
    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_query_simpin_enabled(cmd_buf));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_is_pin_active, enabled \r\n");
			   return 1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_is_pin_active, disabled\r\n");
			   return 0;
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_is_pin_active ,failed! \r\n");
	return 0;

}

unsigned char mu_change_pin( char *sOld,char *sNew )  
{
    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_change_pin(cmd_buf,sOld,sNew));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_change_pin, succeed \r\n");
			   return 1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_change_pin, failed01 \r\n");
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_change_pin ,failed02 \r\n");
    return 0;
}

unsigned char  mu_user_enter_pin( char * pin_out )
{
    unsigned char i=0;
	char tmp_buf[10];
	memset(tmp_buf,0,10);

	for(i=0;i<3;i++)
	{
        if (1 == wnd_input_password(tmp_buf, get_multi_string((char * *)text_input_simpin), 8))
        {
            msg(get_multi_string((char * *) text_please_wait));
			
            if(1 == mu_enter_pin(tmp_buf))
            {
                msgok(get_multi_string((char * *) text_pin_code_pass));
				delay_ms(1500);

				if(NULL !=pin_out )
				{
                     strcpy(pin_out, tmp_buf);
				}
                return 1;
			}
		}
		else
		{
            break;
		}
	}
	return 0;
}


unsigned char mu_enter_puk(  char *sPuk, char *sPin )         //输入PUK码,和新的pin?
{
    unsigned char nret=0;
    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_input_simpuk(cmd_buf, sPuk, sPin));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_puk, succeed \r\n");
			   nret =1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_puk, faled01 \r\n");
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_enter_puk ,failed02! \r\n");
	return nret;
}

unsigned char mu_active_pin( char *sPin )   
{

    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_setup_simpin_enabled(cmd_buf,sPin,1));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_active_pin, enabled \r\n");
			   mu_is_pin_active();
			   return 1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_active_pin, disabled\r\n");
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_active_pin ,failed! \r\n");

    return 0;
}

unsigned char mu_cancel_pin( char *sPin ) 
{
    unsigned long st_tick = app_get_tick_count();
	char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
	

    appsys.flag_got_458=0;
	send_android_command(andr_build_448_setup_simpin_enabled(cmd_buf,sPin,0));
	
	while(get_duration_tick(st_tick) < 1000)
	{
	   if(appsys.flag_got_458 >0)
	   {
	       if(appsys.byte_pinret_state == '1')
	       {
		       if(1 == appsys.flag_genie_trace)DebugPrintf("mu_cancel_pin, enabled \r\n");
			   
			   mu_is_pin_active();
			   
			   return 1;
	       }
		   else
		   {
               if(1 == appsys.flag_genie_trace)DebugPrintf("mu_cancel_pin, disabled\r\n");
		   }
		   break;
	   }
       delay_ms(100);
	}

	if(1 == appsys.flag_genie_trace)DebugPrintf("mu_cancel_pin ,failed! \r\n");

    return 0;
}



unsigned char mu_user_active_pin( char * spin ,int length)
{
	memset(spin,0,length);

    if (0== wnd_input_password(spin, get_multi_string((char * *) text_please_enter_pin), length))
    {
        return 0;
	}
	msg(get_multi_string((char * *) text_please_wait));
	if(1 == mu_active_pin(spin))
	{
        msgok(get_multi_string((char * *)text_pin_protect_actived));
		delay_ms(1500);
		return 1;
	}
	else
	{
         msg_warn(get_multi_string((char * *)text_pin_code_error));
		 delay_ms(1500);

	} 
	return 0;
}

unsigned char mu_user_cancel_pin( void )
{
    char  temp_pin[10];
	memset(temp_pin,0,10);

    if (0== wnd_input_password(temp_pin, get_multi_string((char * *)text_please_enter_pin), 8))
    {
        return 0;
	}
	msg(get_multi_string((char * *) text_please_wait));
    if ( mu_cancel_pin(temp_pin) )
    {
        msgok(get_multi_string((char * *)text_pin_protect_has_been_canceled));
        delay_ms(2000);
		return 1;
    }
    else
    {
        msg_warn(get_multi_string((char * *)text_failed));
        delay_ms(2000);
    }
 	return 0;
}

unsigned char mu_user_enter_puk( void )
{

	char spuk[10];
	char spin[10];
	char spin2[10];


	memset(spuk,0,10);
	memset(spin,0,10);
	memset(spin2,0,10);



    ENTER_PUK_AGAIN:
    if (0 == wnd_input_password(spuk, get_multi_string((char * *)text_input_puk), 8))
    {
        return 0;
    }
    if (0 == wnd_input_password(spin, get_multi_string((char * *)text_enter_new_pin), 8))
    {
        return 0;
    }
	
    if (0 == wnd_input_password(spin2, get_multi_string((char * *)text_enter_new_again), 8))
    {
        return 0;
    }
	

    
    if ( strncmp(spin,spin2,8)!=0 )
    {
	 	msg_warn(get_multi_string((char * *)text_pin_disaccord_please_input_again));
        delay_ms(2000); 
		goto ENTER_PUK_AGAIN;
    }   
 	msg(get_multi_string((char * *) text_please_wait));

    if (1 == mu_enter_puk(spuk,spin) )
    {
		msg(get_multi_string((char * *)text_succeed_keep_your_pin));
        delay_ms(2000);
        return 1;
    }
    else
    {
		msg_warn(get_multi_string((char * *)text_unlock_puk_failed));
        delay_ms(2000);
        return 0;
    }
}


unsigned char mu_check_lockcard( void )
{
    if ( 1 == mu_user_enter_pin(NULL) )
    {
	    return 1;
	}
    return 0; 
}






unsigned char  mu_get_cimi( char *str_cimi, unsigned char cimi_len ) //ESN?
{
    
    return 0;
}




unsigned char mu_check_sim_valid(int wait_ms)
{
	if(1 == appsys.flag_genie_trace) DebugPrintf("start check simcard\r\n");
 
	msg(get_combined_string(get_multi_string((char * *)text_checking_simcard), "\n", get_multi_string((char * *)text_please_wait)));

	if(1 == app_wait_simcard_ready(wait_ms))
	{
         switch(appsys.byte_simcard_state)
         {
              //0 提示SIM状态未知
              case '0':
			  //1	提示没有SIM卡	
			  case '1':
			  {
		          app_show_no_simcard();
				  appsys.flag_nouim_emergency_call=1;
			  }
			  	break;
				
			  //2	提示开机需要用户输入PIN码
			  case '2':
			  		if(mu_check_lockcard())
			  		{
                        return 1;
					}
			  	break;

			  //3	提示开机需要用户输入PUK码
			  case '3':
			  	{
				    msg_warn(get_multi_string((char * *)text_simcard_locked_puk_needed));
	                delay_ms(2000);
	                if(1 == mu_user_enter_puk())
	                {
                         return 1;
					}
			  	}
			  	break;

		      //4	提示开机需要网络PIN码
			  case '4':
			  	break;

			  //5	提示SIM卡就绪
			  case '5':
			  	{
			  		return 1;
			  	}
			  	break;

			  default:
			  	break;

		 }
	}

	if(1 == appsys.flag_genie_trace) DebugPrintf("check simcard failed!!!!\r\n");
	return 0;
}



unsigned char mu_get_csq( void ) 
{
 
    return 0; 
}






int get_smsinfo_from_simcard()
{
    unsigned long st_tick= app_get_tick_count();
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));

    msg(get_multi_string((char * *)text_please_wait));
    appsys.flag_got_455=0;
	send_android_command(andr_build_445_read_simsms_count(tmp_cmd));

	while(get_duration_tick(st_tick) <1000)
	{
        if(appsys.flag_got_455 >0)
        {
            return appsys.byte_simcard_sms_cnt;
		}
		else
		{
            delay_ms(100);
		}
	}
	return 0;
}


int get_bookinfo_from_simcard()
{
    unsigned long st_tick= app_get_tick_count();
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));

    msg(get_multi_string((char * *)text_please_wait));
    appsys.flag_got_454=0;
	send_android_command(andr_build_444_read_simbook_count(tmp_cmd));

	while(get_duration_tick(st_tick) <1000)
	{
        if(appsys.flag_got_454 >0)
        {
            return appsys.byte_simcard_book_cnt;
		}
		else
		{
            delay_ms(100);
		}
	}
	return 0;
}

void wait_for_android_response(int wait_ms)
{
    unsigned long st_tick= app_get_tick_count();
	
	if(1 == appsys.flag_genie_trace) DebugPrintf("start wait 1st response.\r\n");
	
  	while(get_duration_tick(st_tick) <wait_ms)
	{
        if(appsys.flag_got_andr_message>0)
        {
			 break;
		}
		else
		{
            delay_ms(100);
		}
	}
	if(1 == appsys.flag_genie_trace) DebugPrintf("wait 1st response finished.\r\n");
}



unsigned char mu_registering()
{
	lcd_goto_xy(0, 0);
	show_bitmap(BMP_REGISTERING_BIN);
	appsys.byte_sysrun_state=1;	

    app_query_hw_info();
	delay_ms(200);

	appsys.flag_enable_handfree=1;

	appsys.byte_sysrun_state=0xFF;

	appsys.flag_sms_full =(app_get_record_used(TABLEINFO_SMS_INBOX) == g_table_info[TABLEINFO_SMS_INBOX].tb_info.rec_max_cnt)?1:0;
	
    appsys.dword_last_input_tick = app_get_tick_count();

	return 1;
}



unsigned char mu_reg_changed( void )
{
    if(appsys.flag_got_422 >0)
    {
        appsys.flag_got_422=0;
        return 1;
	}

    return 0;
}




void mu_set_spk_gain( unsigned char level )
{
    if(level <MAX_VOLUM_LEVEL)
    {
	    app_set_voice_volumn(VOCTYPE_TALKING, speech_volume [level]);
    }
}




void mu_set_date_time(DATE * p_date, TIME * p_time)
{
   char tmp_buf[32];
   memset(tmp_buf,0,sizeof(tmp_buf));
   //date -s "yyyymmdd.hhmmss"
   sprintf(tmp_buf,"date -s \"%.4d%.2d%.2d.%.2d%.2d%.2d\"", p_date->year,p_date->mon,p_date->day, p_time->hour,p_time->min,p_time->sec);
   if(1 == appsys.flag_genie_trace)DebugPrintf("mu_set_date_time :%s",tmp_buf);
   system(tmp_buf);
}




void mu_dial_dtmf( char dtmf )  
{
   char cmd_buf[128];
   char ctrl_type[2];
   
   memset(cmd_buf,0,sizeof(cmd_buf));
   memset(ctrl_type,0,sizeof(ctrl_type));
   ctrl_type[0]='3';

   send_android_command(andr_build_437_talking_ctrl(cmd_buf, ctrl_type, dtmf));

}
unsigned char mu_dial_dtmf_str( char * dtmf_str )  
{
    char str_dial[24];
	char cmd_buf[128];
	unsigned char flag_succeed =0;

	unsigned long st_tick = app_get_tick_count();

	memset(cmd_buf,0,sizeof(cmd_buf));

    appsys.flag_handfree_pressed=0;
	
    appsys.flag_sending_numbers=1;
	
	memset(str_dial, 0, sizeof(str_dial));
	
    strcpy(appsys.str_dialed_numbers, dtmf_str);

    strcpy(str_dial, dtmf_str);

    appsys.flag_got_423=0;
	
    send_android_command(andr_build_431_callout(cmd_buf, str_dial));

	while(get_duration_tick( st_tick) < 1500)
	{
	    if(appsys.flag_got_423 >0)
	    {
	        if(appsys.byte_tlk_state == '3')
	        {
	            flag_succeed =1;
			}
			appsys.flag_got_423 =0;
			if(1 == appsys.flag_genie_trace)DebugPrintf("mu_dial_dtmf_str , dialing out...\r\n");
	    }
		else
	    {
            delay_ms(100);
		}
	}
	
    appsys.flag_sending_numbers=0;


	if(HOOK_STATE_OFFHOOK == appsys.flag_hook_state )
	{
        appsys.flag_hand_free=0;
		set_handfree_led(0);
	}
	else
	{
        if( 0 ==  appsys.flag_hand_free || 1== appsys.flag_handfree_pressed)
        {
             flag_succeed=0;
			 set_handfree_led(0);
		}
	}
	
	appsys.flag_handfree_pressed=0;

    return flag_succeed;
}

void mu_hook_on( void )
{
    char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
    send_android_command(andr_build_432_hook_onoff(cmd_buf, 0));
}

void mu_answer_call( void )
{
    char cmd_buf[128];
	memset(cmd_buf,0,sizeof(cmd_buf));
    send_android_command(andr_build_432_hook_onoff(cmd_buf, 1));
}

unsigned char mu_set_voice_path( unsigned char voc_path )  
{	//SPEAKER AND MIC PATH  1=handset, 0= handfree

    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
	if(1 ==appsys.flag_genie_trace)DebugPrintf("set voice path to %s\r\n" , (VOICE_PATH_HANDSET== voc_path)? "handset" : "handfree");

    app_disable_speaker();
	
    if (VOICE_PATH_HANDSET== voc_path )
    {  
        send_android_command(andr_build_438_voc_path(tmp_cmd, '1'));
    }
    else
    {	
	    send_android_command(andr_build_438_voc_path(tmp_cmd, '2'));
    }


	if(VOICE_PATH_HANDFREE== voc_path )
	{
     	app_enable_speaker();          //防止挂机的时候拨号音从喇叭放出；
	}



    appsys.flag_voice_path =voc_path;


	return 1;
}




void mu_generate_beep( void )
{
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
	app_enable_speaker();
 	if(1 ==appsys.flag_genie_trace)DebugPrintf("))))) play beep ))))\r\n");
	send_android_command( andr_build_43C_play(tmp_cmd, 'K'));
}

void mu_generate_dtmf( char dtmf )
{
    char tmp_cmd[128];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
	app_enable_speaker();
 	if(1 ==appsys.flag_genie_trace)DebugPrintf("~~~~~ play dtmf =%c ~~~~~\r\n",dtmf);
	send_android_command( andr_build_43C_play(tmp_cmd, dtmf));
}


void mu_play_dialtone( void )
{

    appsys.flag_playing_tone=1;
	appsys.dword_dialtone_st=app_get_tick_count();

	app_play_dial_tone(DIAL_TONE_ASIA, 45);

	if(0== appsys.flag_hand_free && HOOK_STATE_OFFHOOK == appsys.flag_hook_state)
	{
        app_disable_speaker();
	}
}

void mu_stop_dialtone( void )
{
     appsys.flag_playing_tone=0;
     app_stop_music();
}



unsigned char  mu_list_calls( T_CALL_STATE *call_state )
{
    return 0;
}




void mu_shift_waiting_call( void )
{
  
}



void mu_terminate_call( unsigned char id )
{

}




void mu_active_holded_call( unsigned char id )
{

}


unsigned char mu_mute_mic( ) 
{
    appsys.flag_muted=1;




    return 0;
}
unsigned char mu_cancel_mute_mic(  ) 
{
    appsys.flag_muted=0;



    return 0;
}

void mu_delete_message( unsigned char sm_index )    
{

}


unsigned char mu_send_sms( char * numbers,char * context ) 
{
    char tmp_cmd[512];
	unsigned long st_tick =app_get_tick_count();
	
	memset(tmp_cmd,0,sizeof(tmp_cmd));

	appsys.flag_got_426=0;
	send_android_command(andr_build_434_sendsms(tmp_cmd, numbers, context));

    while( get_duration_tick(st_tick) <5000)
    {
         if(appsys.flag_got_426 >0)
         {
             return appsys.byte_smsret_state=='1'?1:0;
		 }
		 else
		 {
             delay_ms(200);
		 }
	}
	
	return 0;
}














unsigned char mu_query_callwait_status( void )
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_456=0;

   send_android_command(andr_build_446_query_call_waiting(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_456 >0)
       {
            return appsys.byte_callwaitret_state=='1'?1:0;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   return 0;
}

unsigned char mu_set_callwait( unsigned char on_off )
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_456=0;

   send_android_command(andr_build_446_setup_call_waiting(tmp_cmd,on_off));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_456 >0)
       {
            return appsys.byte_callwaitret_state=='1'?1:0;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   return 0;
}



unsigned char check_ccfc_state(unsigned char * pcmd, T_CALL_DIVERT_STATUS * divert_status)
{
    
	return 0;
}

unsigned char mu_query_calldivert_status( )
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_457=0;
   memset(appsys.DIVT_LIST,0,sizeof(appsys.DIVT_LIST));
   send_android_command(andr_build_447_query_call_divert(tmp_cmd));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_457 >0)
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

unsigned char mu_active_calldivert( unsigned char divert_mode,char *num )
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_457=0;
   send_android_command(andr_build_447_setup_call_divert(tmp_cmd, divert_mode+0x30,  1, num));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_457 >0)
       {
            return  appsys.byte_divtret_state=='1'?1:0;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   return 0;
}

unsigned char mu_dactive_calldivert( unsigned char divert_mode )
{
   char tmp_cmd[128];
   unsigned long st_tick= app_get_tick_count();
   memset(tmp_cmd,0,sizeof(tmp_cmd));
   msg(get_multi_string((char * *)text_please_wait));
   appsys.flag_got_457=0;
   send_android_command(andr_build_447_setup_call_divert(tmp_cmd, divert_mode+0x30,  0, "0"));

   while(get_duration_tick(st_tick) < 10000)
   {
       if( appsys.flag_got_457 >0)
       {
            return  appsys.byte_divtret_state=='1'?1:0;
	   }
	   else
	   {
           delay_ms(300);
	   }
   }

   return 0;
}





unsigned char mu_list_operator( T_NETWORKS *networks )
{
   
    return 0;
}


unsigned char mu_select_operator( unsigned char mode,char * name )
{
   return 0;
}

unsigned char mu_get_cgsn( char * str_cgsn,unsigned char datelen )
{

	
    return 0;
}

unsigned short get_ucs2_value(char * pdata)
{
    unsigned short ret_val=0;
    ret_val += hex_to_byte(pdata[0]);
    ret_val <<=4;
	ret_val += hex_to_byte(pdata[1]);
	ret_val <<=4;
	ret_val += hex_to_byte(pdata[2]);
	ret_val <<=4;
	ret_val += hex_to_byte(pdata[3]);

    return ret_val;
}
void ucs2_to_ascii_my(char * pdata ,unsigned short data_len ,char * pout, unsigned char out_len )
{
    unsigned short i=0;
	for(i=0;i<data_len;i+=4)
	{
	    if(i/4  < out_len)
	    {
            pout[i/4]= get_ucs2_value(&pdata[i]);
	    }
		else
		{
            break;
		}
	}
}
unsigned short find_char_pos(char *src, const char c, unsigned short tms)
{
    unsigned short i,cnt;
	unsigned short len = strlen(src);

	i=cnt=0;

	if(len >0)
	{
		if(tms !=0xFFFF)
		{
		    while ( src[i] )
		    {
		        if ( c ==src[i] )
		        {
				   if(++cnt == tms)
				   {
		              return i;
				   }
		        }
				i++;
		    }
		}
		else
		{
		    i=len;
			while(i >0)
 			{
 			    i--;
				
				if(src[i] == c)
				{
                    return i;
				}
				
			}
	 
		}
	}
    return 0xffff;
}





unsigned char  mu_getsimcard_sms(unsigned char sms_idx, T_SMS_RECORD * psms_record) 
{
    unsigned long st_tick= app_get_tick_count();
    char tmp_cmd[128];
	char str_idx[6];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
	memset(str_idx,0,sizeof(str_idx));
	sprintf(str_idx,"%d", sms_idx);

    appsys.flag_got_455=0;
	send_android_command(andr_build_445_read_simbook_user(tmp_cmd, str_idx));

	while(get_duration_tick(st_tick) <1000)
	{
        if(appsys.flag_got_455 >0)
        {
            memcpy(psms_record,&appsys.tp_sms, sizeof(T_SMS_RECORD));
            return 1;
		}
		else
		{
            delay_ms(100);
		}
	}
	return 0;
}

unsigned char  mu_getsimcard_book(unsigned char book_idx, T_BOOK_ENTRY* pbook_entry) 
{
    unsigned long st_tick= app_get_tick_count();
    char tmp_cmd[128];
	char str_idx[6];
	memset(tmp_cmd,0,sizeof(tmp_cmd));
	memset(str_idx,0,sizeof(str_idx));
	sprintf(str_idx,"%d", book_idx);

    appsys.flag_got_454=0;
	send_android_command(andr_build_444_read_simbook_user(tmp_cmd, str_idx));

	while(get_duration_tick(st_tick) <1000)
	{
        if(appsys.flag_got_454 >0)
        {
            memcpy(pbook_entry,&appsys.tp_book, sizeof(T_BOOK_ENTRY));
            return 1;
		}
		else
		{
            delay_ms(100);
		}
	}
	return 0;
}



