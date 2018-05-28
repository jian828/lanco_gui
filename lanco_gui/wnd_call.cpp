
#include "inc/app_def.h"


unsigned char compare_special_numbers( char *numbers )
{
    if (0 == strncmp("*#738364*", numbers ,9) )
    {
        return DIALING_RET_SUPERSETTING;
    }
	else if(0 == strncmp("#*07525839133*", numbers,14) )
	{
        return DIALING_RET_GET_MAINMENU;
	}
    else if ( 0 == strncmp("*#06#", numbers, 5) )
    {
        return DIALING_RET_GET_IMEI;
    }
    else if (0 == strncmp("*#0000#", numbers ,7) )
    {
        return DIALING_RET_GET_VERISON;
    }	
    else if ( 0 == strncmp("*#8133#", numbers, 7) )
    {
        return DIALING_RET_EN_DEBUG;
    }	
    else if ( 0 == strncmp("*#6666#", numbers, 7) )
    {
	   return DIALING_RET_SPECIAL_TEST;
    }	
    else if ( 0 == strncmp("*#882201#", numbers, 9) )
    {
	   return DIALING_RET_PROJECT_TEST;
    }	
    else if( 0 == strncmp("*#188#5858#", numbers, 11) )
    {
	   return DIALING_RET_LOCK_OPER;
	}
    else if( 0 == strncmp("*#1004#", numbers, 7) )
    {
	   return DIALING_RET_LOCK_BASE;
	}
    else if( 0 == strncmp("*#1006#", numbers, 7) )
    {
	   return DIALING_RET_SIM_ENCYPT;
	}	
    else if( 0 == strncmp("*#737#", numbers, 6) )
    {
	   return DIALING_RET_TDKEY_QUERY;
	}	
    else if( 0 == strncmp("*#3166#", numbers, 7) )
    {
	   return DIALING_RET_OPEN_USB;
	}	
    else if( 0 == strncmp("*#3155#", numbers, 7) )
    {
	}	
    else if( 0 == strncmp("*#3177#", numbers, 7) )
    {
	   return DIALING_RET_NORMAL_WORK;
	}	
    else if( 0 == strncmp("*#3188#", numbers, 7) )
    {
	   
	}
    else if ( 0 == strncmp("*#3199#", numbers, 7) )
    {
    }

    else if ( 0 == strncmp("*#4444#", numbers, 7) )
    {
	   return DIALING_RET_LOCK_PHONE;
	}	

    else if ( 0 == strncmp("*#3106#", numbers, 7) )
    {
	   return DIALING_RET_SET_IP_PARA;
	}	
    else if ( 0 == strncmp("*#3107#", numbers, 7) )
    {
	   return DIALING_RET_SET_LIFE_SRV;
	}	
    else if ( 0 == strncmp("*#3108#", numbers, 7) )
    {
	   return DIALING_RET_GET_CSQ_VAL;
	}	
    else if ( 0 == strncmp("*#3109#", numbers, 7) )
    {
	   return DIALING_RET_GET_POWER_INFO;
	}	
    else if ( 0 == strncmp("*#3211#", numbers, 7) )
    {
	   return DIALING_RET_SET_P2P_SNSR_ERR;
	}	
    else if ( 0 == strncmp("*#3212#", numbers, 7) )
    {
	   return DIALING_RET_SET_P2P_LOW_BAT;
	}	
	

	
	
	

    else if ( 0xFFFF !=findsubstr(numbers, "*#001", 0, strlen(numbers)) && '#' == numbers[strlen(numbers)-1] )
    {
        char tmp_buf[BOOK_ENTRY_NUM_FIELD_SIZE+1];
		memset(tmp_buf,0,sizeof(tmp_buf));
		if(dumpstr(numbers, strlen(numbers), tmp_buf, sizeof(tmp_buf)-1, "*#001", "#"))
		{
             strcpy(sysprop->str_additional_prefix, tmp_buf);
		}
		else
		{
             memset(sysprop->str_additional_prefix, 0, sizeof(sysprop->str_additional_prefix));
		}
		app_save_prop();
		msgok(get_multi_string((char * *) text_changed_successful));
		delay_ms(800);
	    return DIALING_RET_PREFIX_RW;
    }	
    else if ( 0xFFFF !=findsubstr(numbers, "*#002", 0, strlen(numbers)) && '#' == numbers[strlen(numbers)-1] )
    {
		msg(sysprop->str_additional_prefix);
		delay_ms(800);
	    return DIALING_RET_PREFIX_RW;
    }	


  
	
	return DIALING_RET_NO_SENSE;
}

unsigned char set_voice_path_by_action(unsigned char byte_action)
{
    //byte_action  0: handset   1: handfree key,   
    //VOICE_ACTION_HANDSET      0
    //VOICE_ACTION_KEY          1
    
    if(VOICE_ACTION_KEY== byte_action)
    {
         if(1 == appsys.flag_hand_free)
         {
             mu_set_voice_path(VOICE_PATH_HANDFREE);
		 }
		 else 
		 {
             if( (HOOK_STATE_ONHOOK== appsys.flag_hook_state))
             {
                  mu_set_voice_path(VOICE_PATH_HANDFREE);
                  return 0;
			 }
			 else
			 {
                  mu_set_voice_path(VOICE_PATH_HANDSET);
			 }
		 }
	}
	else if( VOICE_ACTION_HANDSET == byte_action)
	{
        if( (HOOK_STATE_OFFHOOK== appsys.flag_hook_state))
        {
             mu_set_voice_path(VOICE_PATH_HANDSET);
		}
		else
		{  
		     if( (1==appsys.flag_playing_tone) && (0 == appsys.flag_hand_free) )
			 {
			     mu_stop_dialtone();
		     }
		     mu_set_voice_path(VOICE_PATH_HANDFREE);
			 
             if(0 == appsys.flag_hand_free)
			 {	 
                  return 0;
			 }
		}
	}

	return 1;
}


unsigned char wnd_input_dialnumbers(char * buf,  char * pcap , unsigned char maxlen)
{
	EDITWND m_edit_wnd;
	EDITWND * p_edit_wnd =&m_edit_wnd;
    FlSignalBuffer EventPara;

	unsigned char cur_line_cnt=0;
	unsigned char tmp=0;
	unsigned char zero_2_plus   =0;
	char input_code=0;
    unsigned char update_cursor_cnt=0;
	
	memset(p_edit_wnd,0,sizeof(EDITWND));


	p_edit_wnd->frame_left =2;
	p_edit_wnd->frame_top =24;
	p_edit_wnd->frame_right =SCREEN_WIDTH-3;
	p_edit_wnd->frame_bottom =42;
	//p_edit_wnd->style= EDIT_STYLE_ALIGE_RIGHT|EDIT_STYLE_HIDE_CURSOR;
	p_edit_wnd->style= EDIT_STYLE_ALIGE_RIGHT;


    a2w(p_edit_wnd->data_buf, buf);
	p_edit_wnd->data_len=wstrlen(m_edit_wnd.data_buf);
	p_edit_wnd->max_len=maxlen;

    p_edit_wnd->pcaption=pcap;
	p_edit_wnd->data_font=FONT_B_X_16;

    p_edit_wnd->data_pos=strlen(buf);
 	p_edit_wnd->cursor_x_last=0;
	p_edit_wnd->cursor_y_last=0;
    p_edit_wnd->input_mode=IN_FIXED;	
 	p_edit_wnd->soft_key_type=SOFT_KEY_TYPE_DEL_CANCLE_OK;

    if(1 == p_edit_wnd->data_len  && KEY_STATE_PRESSED == appsys.flag_key_state)
    {
        input_code =p_edit_wnd->data_buf[0];
	}

	m_edit_wnd.body_left=m_edit_wnd.frame_left;
	m_edit_wnd.body_top=m_edit_wnd.frame_top;
	m_edit_wnd.body_right=m_edit_wnd.frame_right;
	m_edit_wnd.body_bottom=m_edit_wnd.frame_bottom;
	wnd_get_lines_info(p_edit_wnd);
	
    if(p_edit_wnd->line_cnt>2)
    {
      	p_edit_wnd->frame_left =2;
		p_edit_wnd->frame_top =18;
		p_edit_wnd->frame_right =SCREEN_WIDTH-3;
		p_edit_wnd->frame_bottom =48;
	    p_edit_wnd->style &= ~EDIT_STYLE_ALIGE_RIGHT;
	}
	appsys.byte_need_redraw=0xFF;

    while(1)
    {
	    if(appsys.byte_need_redraw>0)
		{
			if(0xFF == appsys.byte_need_redraw)
			{
		        lcd_clear_screen();

				p_edit_wnd->body_left=p_edit_wnd->frame_left;
				p_edit_wnd->body_top=p_edit_wnd->frame_top;
				p_edit_wnd->body_right=p_edit_wnd->frame_right;
				p_edit_wnd->body_bottom=p_edit_wnd->frame_bottom;

				appsys.byte_font_type=FONT_B_X_16;
				p_edit_wnd->body_lines=  (p_edit_wnd->body_bottom-p_edit_wnd->body_top+1)/get_cursor_height();
			    lcd_put_rect(1,14, SCREEN_WIDTH-2,48);
		        lcd_put_rect(0,13, SCREEN_WIDTH-1,49);
			}
			if( (appsys.byte_need_redraw & EDIT_REDRAW_CAPTION) >0)
			{
				 show_caption(p_edit_wnd->pcaption);
			}
			if( (appsys.byte_need_redraw & EDIT_REDRAW_BOTTOM) >0)
			{
			     if(p_edit_wnd->data_len >0)
			     {
			         show_soft_key_prompt(get_multi_string((char * *)text_dial), get_multi_string((char * *)text_del));
			     }
				 else
				 {
                     show_soft_key_prompt(get_multi_string((char * *)text_dial), get_multi_string((char * *)text_back));
				 }
			}
			if( (appsys.byte_need_redraw & EDIT_REDRAW_DATA) >0)
			{
			    appsys.byte_font_type=FONT_B_X_16;
			    wnd_get_lines_info(p_edit_wnd);
				wnd_data_redraw(p_edit_wnd);
			}
		    appsys.byte_need_redraw=0;
		    
		}
		else
		{
		    if( 1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{
                    input_code =get_key_value(EventPara.sig_p.key_evt.key_val);
					
					if(EventPara.sig_p.key_evt.isPressed)
					{
						if(input_code != 0)
						{
						    //if(1 == appsys.flag_genie_trace)DebugPrintf("++++ dial key  = %c   ++++\r\n", input_code);
								
						    if(0 == p_edit_wnd->data_len)
						    {
                                appsys.byte_need_redraw |= EDIT_REDRAW_BOTTOM;
							}
						    if(wnd_wstr_insert_char(p_edit_wnd,input_code) >0)
							{
							    if(1 == p_edit_wnd->data_len)
							    {
							        if(1 == appsys.flag_playing_tone)
							        {
                                         mu_stop_dialtone();
									}
								}
	
							    if(1 == sysprop->flag_key_tone )
							    {
							        mu_generate_dtmf(input_code);
							    }
				
								if(cur_line_cnt !=p_edit_wnd->line_cnt)
								{
								    if(p_edit_wnd->line_cnt>2)
								    {
	                                  	p_edit_wnd->frame_left =2;
										p_edit_wnd->frame_top =15;
										p_edit_wnd->frame_right =SCREEN_WIDTH-3;
										p_edit_wnd->frame_bottom =49;
										
										p_edit_wnd->style&= ~EDIT_STYLE_ALIGE_RIGHT;
									}
									else
									{
										p_edit_wnd->frame_left =2;
										p_edit_wnd->frame_top =24;
										p_edit_wnd->frame_right =SCREEN_WIDTH-3;
										p_edit_wnd->frame_bottom =42;
										p_edit_wnd->style|= EDIT_STYLE_ALIGE_RIGHT;
										
									}
									p_edit_wnd->line_start=0;
									if(0 != cur_line_cnt)
									{
								        appsys.byte_need_redraw=0xFF;
									}

									cur_line_cnt=p_edit_wnd->line_cnt;
								}
								else
								{
									appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
								}
							}
						}
						else
						{
  if(1 == appsys.flag_genie_trace)DebugPrintf("++++ other key  = %d   ++++\r\n", EventPara.sig_p.key_evt.key_val);
					        if(TFKEY_HANDFREE == EventPara.sig_p.key_evt.key_val)
					        {
					             if(0 == appsys.flag_pre_dial  || 0 == p_edit_wnd->data_len)
					             {
	                                 if (0 == set_voice_path_by_action(VOICE_ACTION_KEY))
	                                 {
	                                      return DIALING_RET_NO_SENSE;
									 }
									 else
									 {
										 if(1 == appsys.flag_playing_tone)
										 {
										     mu_stop_dialtone();
											 delay_ms(50);
									         mu_play_dialtone();
										 }
									 }
					             }
								 else 
								 {
							         set_voice_path_by_action(VOICE_ACTION_KEY);
									 w2a(buf, p_edit_wnd->data_buf);
									 appsys.flag_hand_free=1;
                                     return DIALING_RET_DIALOUT;
								 }
							}
						    else if(TFKEY_REDIAL== EventPara.sig_p.key_evt.key_val)
							{
                                if(strlen(appsys.str_dialed_numbers)>0 )
							    {
							         strcpy(buf, appsys.str_dialed_numbers) ;
									 if(1 == appsys.flag_playing_tone)
									 {
                                         mu_stop_dialtone();
									 }
							         return DIALING_RET_DIALOUT;
							    }
								else
								{
		                            T_CALL_RECORD call_record;
									memset(&call_record,0,sizeof(call_record));
									if(1 == app_record_read(TABLEINFO_CALL_OUT, 1, (unsigned char *)&call_record))
									{
									    if(strlen(call_record.num) >0)
									    {
									         strcpy(buf, call_record.num) ;
											 if(1 == appsys.flag_playing_tone)
											 {
		                                         mu_stop_dialtone();
											 }
									         return DIALING_RET_DIALOUT;
									    }
									}
								}
							}
						    else
						    {
							    if(TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)
								{
								    if( 1 == appsys.flag_key_tone){mu_generate_beep();}
									
								    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK)
								    {
								        if(p_edit_wnd->data_len >0 )
								        {
		                                    unsigned long down_tick = app_get_tick_count();
											unsigned char flag_batch=0;

											while(KEY_STATE_PRESSED == appsys.flag_key_state)
											{
		                                        delay_ms(100);

												if(get_duration_tick(down_tick) > 900)
												{
			                                        flag_batch=1;
													if(1 ==appsys.flag_genie_trace)DebugPrintf("BATCH DEL DIAL --------------------<><><>\r\n",NULL);
													break;
												}
											}

											tmp=(0 ==flag_batch)?wnd_wstr_delete(p_edit_wnd):wnd_batch_wstr_delete(p_edit_wnd);

#if 0
									        if(p_edit_wnd->data_pos== p_edit_wnd->data_len)
									        {
		                                       tmp=wnd_wstr_delete(p_edit_wnd);
											}
											else
											{
		                                       tmp=wnd_wstr_delete(p_edit_wnd);
											}
#endif
											
										    if( tmp>0 )
											{
												if(cur_line_cnt !=p_edit_wnd->line_cnt)
												{
												    if(p_edit_wnd->line_cnt>2)
												    {
					                                  	p_edit_wnd->frame_left =2;
														p_edit_wnd->frame_top =15;
														p_edit_wnd->frame_right =SCREEN_WIDTH-3;
														p_edit_wnd->frame_bottom =49;
														p_edit_wnd->style&= ~EDIT_STYLE_ALIGE_RIGHT;
													}
													else
													{
														p_edit_wnd->frame_left =2;
														p_edit_wnd->frame_top =24;
														p_edit_wnd->frame_right =SCREEN_WIDTH-3;
														p_edit_wnd->frame_bottom =42;
														p_edit_wnd->style|= EDIT_STYLE_ALIGE_RIGHT;
														
													}
													cur_line_cnt=p_edit_wnd->line_cnt;
													p_edit_wnd->line_start=0;
												    appsys.byte_need_redraw=0xFF;
												}
												else
												{
		                                            appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
												}
										    }

                                            if(0 == p_edit_wnd->data_len )
											{
											     if(0 == appsys.flag_pre_dial || 1 == flag_batch)
											     {
	                                                 appsys.byte_need_redraw =0xFF;
											     }
												 else
											     {
											         return DIALING_RET_NO_SENSE;
											     }
											}
											
								        }
										else
										{
										    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK )
										    {
										        if(1 == appsys.flag_hand_free)
										        {
										            return DIALING_RET_NO_SENSE;
										        }
										    }
										}
								    }
									else if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NO_NULL_YES)
									{
	                                    return DIALING_RET_NO_SENSE;
									}
								}
								else if(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val)
								{
								    if( 1 == appsys.flag_key_tone){mu_generate_beep();}
								    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK)
								    {
								        return DIALING_RET_NO_SENSE;
								    }
	 							}	
								else if( (TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val) 
									|| (TFKEY_SEND == EventPara.sig_p.key_evt.key_val)
                                    || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val)
								)
								{
								    if( 1 == appsys.flag_key_tone){mu_generate_beep();}
								    if(0 == appsys.flag_hardware_test)
								    {
									    if(wstrlen(p_edit_wnd->data_buf) >0 )
									    {
									        w2a(buf, p_edit_wnd->data_buf);
									        return DIALING_RET_DIALOUT;
									    }
								    }
									else
									{
										memset(p_edit_wnd->data_buf,0, p_edit_wnd->max_len);
								       	p_edit_wnd->data_len=0;
										p_edit_wnd->data_pos=0;
										p_edit_wnd->line_start=0;

										wnd_get_lines_info( p_edit_wnd);
										appsys.byte_need_redraw=0xFF;
									}
	 							}	
								else if(TFKEY_VOL_UP== EventPara.sig_p.key_evt.key_val || TFKEY_VOL_DOWN== EventPara.sig_p.key_evt.key_val)
								{
	

								}								
								else if(
										  (TFKEY_UP==EventPara.sig_p.key_evt.key_val)
										||(TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
										||(TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)
										||(TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val)
									    )
								{
	                                wnd_cursor_move(p_edit_wnd,EventPara.sig_p.key_evt.key_val);
								}
								else if( (TFKEY_FAST1 == EventPara.sig_p.key_evt.key_val) 
									|| (TFKEY_FAST2 == EventPara.sig_p.key_evt.key_val)
									|| (TFKEY_FAST3 == EventPara.sig_p.key_evt.key_val)
									|| (TFKEY_LVSRV == EventPara.sig_p.key_evt.key_val)
									|| (TFKEY_REMAINED== EventPara.sig_p.key_evt.key_val)

								)
								{
								    char fast_number[BOOK_ENTRY_NUM_FIELD_SIZE+1];
									memset(fast_number,0,sizeof(fast_number));
									
								    if( 1 == appsys.flag_key_tone){mu_generate_beep();}
									
									app_find_fast_dial(EventPara.sig_p.key_evt.key_val, fast_number);
									if(strlen(fast_number) >0 )
									{
                                        strcpy(buf, fast_number);
										mu_stop_dialtone();
										return DIALING_RET_DIALOUT;
									}
								}
						    }
						}
					}
				}
				else if( EventPara.eventTyp == EVENT_HANDSET )
				{
				     if(0 == appsys.flag_pre_dial  || 0 == p_edit_wnd->data_len)
					 {
	                     if (0 == set_voice_path_by_action(VOICE_ACTION_HANDSET))
	                     {
	                          return DIALING_RET_NO_SENSE;
						 }
				     }
					 else
					 {
                         if(1 == set_voice_path_by_action(VOICE_ACTION_HANDSET))
                         {
                              w2a(buf, p_edit_wnd->data_buf);
						      return DIALING_RET_DIALOUT;
                         }
					 }
				}
				else if(EVENT_TIMER == EventPara.eventTyp)
				{

					if( appsys.flag_time_changed>0)
					{
					    appsys.flag_time_changed =0;
						//to do:
					}

				    if( (p_edit_wnd->style & EDIT_STYLE_HIDE_CURSOR) ==0   )
				    {
				        if(++update_cursor_cnt>5)
				        {
				            update_cursor_cnt=0;
					        wnd_put_cursor(p_edit_wnd);
				        }
					}


					if('0' == input_code && KEY_STATE_PRESSED == appsys.flag_key_state   && p_edit_wnd->data_len==1  &&  p_edit_wnd->data_buf[0] == '0')
					{
                         if( ++zero_2_plus >10)
                         {
                             p_edit_wnd->data_buf[0]='+';
							 appsys.byte_font_type =FONT_B_X_16;
							 wnd_data_redraw(p_edit_wnd);
							 zero_2_plus=0;
						 }
					}


					if(sysprop->byte_call_delay_s >0)
					{
						if( get_duration_tick(appsys.dword_last_input_tick)/1000 >= get_dial_delay())
						{
						     if( (wstrlen(p_edit_wnd->data_buf) >=3) &&  (0 == appsys.flag_hardware_test))
						     {
						         w2a(buf, p_edit_wnd->data_buf);
	                             return DIALING_RET_DIALOUT;
						     }
						}
					}

					if(1)
					{
					     unsigned char cmp_ret=DIALING_RET_NO_SENSE;
						 char tmp_dialout[BOOK_ENTRY_NUM_FIELD_SIZE+5]; 
						 memset(tmp_dialout,0,sizeof(tmp_dialout));
						 w2a(tmp_dialout, p_edit_wnd->data_buf );
						 
                         if(DIALING_RET_NO_SENSE != (cmp_ret = compare_special_numbers(tmp_dialout)) )
                         {
                              return cmp_ret;
						 }
					}

					if( get_duration_tick(appsys.dword_last_input_tick)/1000 >= (DIALING_NO_INPUT_TIMEOUT_S *10 ))
					{
                         return DIALING_RET_NO_SENSE;
					}
					
	                if(appsys.byte_need_exit > 0)
	                {
	                    return DIALING_RET_NO_SENSE;
					}
				}
			}
		}
	}
	return DIALING_RET_NO_SENSE;
}

unsigned char is_emergency_number(char * numbers)
{
    if( ((0 == memcmp(numbers,"110", 3))
    ||  (0 == memcmp(numbers, "119", 3))
    ||  (0 == memcmp(numbers, "120", 3)))
	)
    {
        return 1;
    }
	return 0;
}

unsigned char pre_process_numbers(char * numbers)
{
	if( ((1 == sysprop->flag_encyped_only) &&(0 == appsys.flag_sim_encypted))
	)
	{
	    return 0;
	}
	return 1;
}


void show_dailing_static_window( T_BOOK_ENTRY *book_entry ,unsigned char flag_show_prompt)
{
    T_BOOK_ENTRY  temp_entry;
	
	lcd_clear_screen();

	show_caption((0 == flag_show_prompt)?"正在拨号":"正在呼叫");
		
	lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);

	appsys.byte_font_type= FONT_B_X_10;			
	lcd_goto_xy(4, 18);
	show_bitmap(BMP_MAIN_CALLIN_BIN);
	lcd_goto_xy(45,18);


    if ( find_book_entry_by_numbers(book_entry->num, &temp_entry) )
    {
        strncpy(book_entry->name, temp_entry.name, BOOK_ENTRY_NAME_FIELD_SIZE);
		lcd_goto_xy(45,18);
        lcd_put_string(book_entry->name);
        lcd_clear_line(SCREEN_WIDTH-3);
    }
	else
	{
		if(strlen(book_entry->num) <=13)
		{
		   lcd_put_string(book_entry->num);
		}
		else
		{
		    unsigned char i=0;
		    for(i=0;i<13;i++)
		    {
               lcd_add_x( lcd_put_char( book_entry->num[i]));
		    }
			lcd_goto_xy(45,28);
		    for(i=13;i<strlen( book_entry->num) && i<BOOK_ENTRY_NUM_FIELD_SIZE ;i++)
		    {
                lcd_add_x( lcd_put_char( book_entry->num[i]));
		    }				
		}
	}
	

	lcd_goto_xy(4,38);
	lcd_put_string(get_multi_string((char * *)text_connecting));

    if(1 == flag_show_prompt)
    {
         show_soft_key_prompt(get_multi_string((char * *)text_null),  get_multi_string((char * *)text_end));
    }
}

unsigned char dialout_numbers( T_BOOK_ENTRY *book_entry )
{
    unsigned long tick_start=0;
    T_BOOK_ENTRY  temp_entry;
    FlSignalBuffer EventPara;
    unsigned char idx_pos=0,last_x=0xFF,cnt=0;
    unsigned char xpos[]={24,28,32,36};
    appsys.flag_pre_dial=0;
	
	show_dailing_static_window(book_entry,0);

    if(0 ==mu_dial_dtmf_str(book_entry->num) )
    {
		mu_hook_on();
		return 0;
	}

	tick_start = app_get_tick_count();
	appsys.byte_need_redraw=0xFF;


    while(1)
	{
	    if(appsys.byte_need_redraw>0)
	    {
			show_dailing_static_window(book_entry,1);
		    appsys.byte_need_redraw=0;
		}
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
				    if(1 == EventPara.sig_p.key_evt.isPressed)
				    {
				        if(TFKEY_HANDFREE == EventPara.sig_p.key_evt.key_val)
				        {
		                     if (0 == set_voice_path_by_action(VOICE_ACTION_KEY))
		                     {
		                          return 0xFF;// cancled by user
							 }
						}
						else if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
						{
	                         return 0xFF;// cancled by user
						}
						else if( (TFKEY_PHONEBOOK== EventPara.sig_p.key_evt.key_val))
						{
	                         send_android_command("2,3,4");//JWL
						}
					}	
		        }
				else if( EVENT_HANDSET== EventPara.eventTyp )
				{
	                 if (0 == set_voice_path_by_action(VOICE_ACTION_HANDSET))
	                 {
	                      return 0xFF;// cancled by user
					 }
				}				
				else if( EVENT_MODULE== EventPara.eventTyp )
				{
                     if(appsys.flag_got_423 >0)
				     {
				        if(appsys.byte_tlk_state == '4')
				        {
				             appsys.flag_got_423 =0;
							 return 1;
						}
				     }
				}
				else if( EVENT_TIMER== EventPara.eventTyp )
				{					
					if(get_duration_tick(tick_start)/1000 >WAIT_CSSI_7_TIME_OUT)
					{
	                     return 0;
					}
					else if( (++cnt%3 ==0))
					{
						 if(0xFF !=last_x)
						 {
						    lcd_goto_xy(last_x, 20);
							lcd_clear_line(last_x+8);
						 }


						 lcd_goto_xy(xpos[idx_pos], 20);
						 last_x =xpos[idx_pos];
						 show_bitmap(BMP_ARROW_RIGHT_BIN);
						 idx_pos++;

						 if(idx_pos>3)
						 {
						    idx_pos =0;
						 }
					}

		     		if(cnt%15 ==0)
					{
					    appsys.dword_callwait_clcc_tick= app_get_tick_count();
					}

				    if( 1 == appsys.flag_no_carrier)
				    {
	                    return 0;
				    }
				}			
			}
		}
    }
}

unsigned char is_international_numbers( char * numbers)
{
    if ( ('+'==numbers[0]) || ('0'==numbers[0] && '0'==numbers[1]) )
    {
        if('+'==numbers[0])
        {
             if('8'==numbers[1]   && '6'==numbers[2] )
             {
                  return 0;
			 }
		}
		else if('0'==numbers[0] && '0'==numbers[1])
		{
             if('8'==numbers[2]   && '6'==numbers[3] )
             {
                  return 0;
			 }
		}
        return 1;
    }
    else
    {
        return 0;
    }
}


unsigned char get_max_string_width( unsigned char *width_list,char ** str1, char ** str2, char ** str3 , char ** str4 )
{
    unsigned char max_len;
	unsigned char temp_len;

    max_len = width_list[0] = get_str_dots_width((char *)(str1[sysprop->byte_language_type]));

	temp_len = width_list[1] = get_str_dots_width((char *)(str2[sysprop->byte_language_type]));
    if ( max_len < temp_len )
    {
        max_len = temp_len;
    }
    temp_len= width_list[2] = get_str_dots_width((char *)(str3[sysprop->byte_language_type]));
    if ( max_len < temp_len )
    {
        max_len = temp_len;
    }
    temp_len = width_list[3] = get_str_dots_width((char *)(str4[sysprop->byte_language_type]));
    if ( max_len < temp_len )
    {
        max_len = temp_len;
    }
    return max_len;
}

unsigned char bcd2d( unsigned char dat )
{
    return (dat>>4)*10 + (dat&0x0f);
}















unsigned char get_current_calls( T_CALL_STATE *call_states,unsigned char limit )
{
    unsigned char i, cnt;
    T_BOOK_ENTRY dest_entry;

    memset(call_states, 0, limit*(sizeof(T_CALL_STATE)));

   
    cnt = mu_list_calls(call_states);
	appsys.dword_callwait_clcc_tick = app_get_tick_count();

	if(cnt >1)
	{
        appsys.flag_holding_call=0;
	}

    /*
    for ( i=0; i<cnt; ++i )
    {
        if(1== appsys.flag_genie_trace)DebugPrintf("there is total=%d calls, i=%d, nums=%s\r\n",cnt, i, call_states[i].num);
	   
        if ( find_book_entry_by_numbers(call_states[i].num, &dest_entry) )
        {
            memcpy(call_states[i].name, dest_entry.name, BOOK_ENTRY_NAME_FIELD_SIZE);
        }
    }
    */

    return cnt;
}


unsigned char get_call_index( T_CALL_STATE *call_states,unsigned char cnt, unsigned char type )
{
    unsigned char i;

    for ( i=0; i<cnt; ++i )
    {
        if ( call_states[i].id && type==call_states[i].stat )
        {
            return i;
        }
    }

    return 0xff;
}


















unsigned long get_decimal_mask(unsigned char decimal_pos)
{
    if( decimal_pos == 0 )
    {
        return 1;
	}
	else
	{
	    unsigned long ret_val=1;
        unsigned char i=0;
		for(i=0;i<decimal_pos;i++)
		{
            ret_val *=10;
		}
		return ret_val;
	}
}

void money_digit_to_asc_10( unsigned long d,char * money_str )
{
    money_digit_to_asc_10_ex(d, money_str, 0);
}
#define MONEY_BASE_UNIT (100)
void money_digit_to_asc_10_ex( unsigned long d,char * money_str , unsigned char flag_fixed_pos)
{
	char tmp_buf[12];
    memset(money_str,0,10);

	if(d> MAX_MONEY_VALUE) //MAX_INT32_FFFFFFFF= 4294967295
	{
		if(1 ==appsys.flag_genie_trace)DebugPrintf("too large money value =%d\r\n", d);
		return;
	}

    if(sysprop->byte_decimal_pos>0)
    {
        unsigned long hi,lo,i;
	    memset(tmp_buf,0,sizeof(tmp_buf));
	    sprintf(tmp_buf,"%%d.%%.%dd", sysprop->byte_decimal_pos);

		hi=d/MONEY_BASE_UNIT;
		lo=d%MONEY_BASE_UNIT;

#if 0
        for(i=0;i<4-sysprop->byte_decimal_pos;i++)
        {
             lo/=10;
		}
#else
        for(i=0;i<2-sysprop->byte_decimal_pos;i++)
        {
             lo/=10;
		}
#endif
		
	    sprintf(money_str,tmp_buf, hi, lo);
    }
	else
	{
	    sprintf(money_str,"%d", d/MONEY_BASE_UNIT);
	}

	if( flag_fixed_pos>0)
	{
        unsigned char i=0;
		for(i=strlen(money_str)-1;i>=0;i--)
		{
            if('0' == money_str[i] )
            {
               money_str[i]=0;
			}
			else
			{
                if('.' == money_str[i])
                {
                    money_str[i]=0;
				}
				break;
			}
		}
	}
}







unsigned long money_str_to_digit( char * money_str )
{
   unsigned char d_pos=0;
   unsigned char i=0;
   unsigned char money_len= strlen(money_str);
   char temp_str[12];
   memset(temp_str,0,12);
   if(NULL == money_str || 0 ==strlen(money_str))
   {
       return 0;
   }

   for(i=0;i<money_len;i++)
   {
       if('.' != money_str[i])
       {
            temp_str[strlen(temp_str)]=money_str[i];
       }
	   else
	   {
	        //1234.5    money_len=6, 
            d_pos= (money_len-1) - i;
	   }
   }

#if 0   
   d_pos=(d_pos>4)?4:d_pos;
   strncpy(&temp_str[strlen(temp_str)],"0000",4-d_pos);
#else
   d_pos=(d_pos>2)?2:d_pos;
   strncpy(&temp_str[strlen(temp_str)],"00",2-d_pos);
#endif

   if(  (strlen(temp_str)>10 && '0' != temp_str[0])  || (strlen(temp_str)==10 && temp_str[0] >'4'))
   {
        return MAX_INT32_FFFFFFFF;
   }
   return atoi(temp_str);
}

void adjust_speech_volume()
{
    FlSignalBuffer EventPara;
    unsigned bar_x, bar_y, bar_height, volume,old_val, update_frame=1;
	
	unsigned long  dword_adjust_input_tick=app_get_tick_count();

    appsys.byte_need_redraw=0xFF;

    volume= sysprop->byte_talk_volume;

	old_val =volume;
	
    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
            if(0xFF == appsys.byte_need_redraw)
            {
			    lcd_clear_screen();
				show_caption(get_multi_string((char * *)text_volume));
				show_soft_key_prompt(get_multi_string((char * *)text_save), get_multi_string((char * *)text_null));
            }

			

			app_volumn_setting(5, volume);

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
					    if ( TFKEY_VOL_UP== EventPara.sig_p.key_evt.key_val ) 
					    {
					        if ( volume < 4 )
					        {
					            ++volume;
								app_volumn_setting(5, volume);

                                mu_set_spk_gain(volume);
							
					         }
					    }
					    else if(TFKEY_VOL_DOWN ==EventPara.sig_p.key_evt.key_val  )
					    {
					        if ( volume > 0 )
					        {
					            --volume;
					            app_volumn_setting(5, volume);
                                mu_set_spk_gain(volume);
					        }
					    }
					    else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val) )
					    {
						    appsys.byte_need_redraw=0xFF;
							dword_adjust_input_tick = app_get_tick_count();
							
							if(volume !=  old_val)
							{
                                sysprop->byte_talk_volume=volume;
							    
							    app_save_prop();
							}
	                        return;
					    }
					}
		        }
			    if(EVENT_HANDSET== EventPara.eventTyp )
			    {
					if(1 == appsys.flag_talk_rating)
					{
					     if(HOOK_STATE_OFFHOOK== appsys.flag_hook_state)
					     {
					         appsys.flag_hand_free=0;
					         mu_set_voice_path(VOICE_PATH_HANDSET);
					     }
					     else
					     {
					        appsys.flag_hand_free=1;
					        mu_set_voice_path(VOICE_PATH_HANDFREE);
					     }
					}
				}
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
					if(get_duration_tick(dword_adjust_input_tick)>= 1000)//ms
					{
					    appsys.byte_need_redraw=0xFF;
						if(volume !=  old_val)
						{
                            sysprop->byte_talk_volume=volume;
						    app_save_prop();
						}
						
                        return;
					}
				}
		    }
		}
	}
}


void refresh_call_record(T_CALL_RECORD * pcall_record )
{
     unsigned long st_clock=0;
	 unsigned long cr_clock=0;
	 unsigned long tmp_val=0;
 

     tmp_val=pcall_record->hour;
	 tmp_val*=3600;
	 st_clock+=tmp_val;
     tmp_val=pcall_record->minute;
	 tmp_val*=60;
	 st_clock+=tmp_val;
	 st_clock+=pcall_record->second;

     tmp_val=appsys.struct_time.hour;
	 tmp_val*=3600;
	 cr_clock+=tmp_val;
     tmp_val=appsys.struct_time.min;
	 tmp_val*=60;
	 cr_clock+=tmp_val;
	 cr_clock+=appsys.struct_time.sec;

	 if(cr_clock>= st_clock)
	 {
         tmp_val= cr_clock-st_clock;
	 }
	 else
	 {
         tmp_val=cr_clock+ 86400- st_clock;
	 }
	 tmp_val+=1; // time seconds++1 because first second must be 1

     pcall_record->len_hour=   (unsigned char)(tmp_val/3600);
	 pcall_record->len_minute= (unsigned char)(tmp_val%3600/60);
	 pcall_record->len_second= (unsigned char)(tmp_val%3600%60);

}








void talk_phone_call( T_CALL_RECORD * pcall_record,char * that_name )
{
    FlSignalBuffer EventPara;
	char call_name[BOOK_ENTRY_NUM_FIELD_SIZE+1];
    T_CALL_STATE   call_states[3];
    unsigned char  call_cnt=0;
    unsigned char  input_code=0;
    unsigned char  org_minutes=0;
    unsigned char  align_maxlen=0;
	unsigned char  i=0;
	unsigned char  flag_waiting_call=0;
	unsigned char  byte_blink_waiting=0;
	unsigned char  flag_blinking=0;
    unsigned char  flag_been_callwaited=0;


    char str_sec_dial[21];
    char str_temp_time[10];
    char str_money_asc[MONEY_DIGI_NUM+1];
    unsigned char str_width[4];
	appsys.flag_holding_call=0;






	mu_set_spk_gain(sysprop->byte_talk_volume);
    appsys.byte_need_redraw=0xFF;
    memset(str_sec_dial, 0, 21);
	
    appsys.byte_font_type= FONT_T_X_10;

	while ( 1 )
    {
        if (appsys.byte_need_redraw>0)
        {
            if( (0xFF ==appsys.byte_need_redraw))
            {		 
                 lcd_clear_screen();

                 lcd_put_rect(0, 12,SCREEN_WIDTH-1, 51);

#if 0
				 if(1 == appsys.flag_call_comming)
				 {
				     show_caption(get_multi_string((char * *)text_incoming_call));
				 }
				 else
				 {
                     show_caption(get_multi_string((char * *)text_outgoing_call));
				 }
#endif				 
		
				 show_network_prompt();
				 app_show_battery(0);
			}
			
 			
            if ( appsys.byte_need_redraw & UPDATE_CALL_NUM )
            {
                appsys.byte_font_type= FONT_T_X_10;
				
			
			    if (1)
                {
					if(0 == flag_waiting_call)
					{
					    char * pname=NULL;
						char * pnum =NULL;
	                    // Show number or name
						lcd_goto_x_line(2, 1);
						lcd_clear_line(SCREEN_WIDTH-2);
						lcd_goto_x_line(2, 1);
						

						if(0 == call_cnt )
						{
						    pname = that_name;
							pnum =  pcall_record->num;
						}
						else
						{
							pname = call_states[0].name;
						    pnum = call_states[0].num;
						}

			            if ( strlen(pname)>0 )
	                    {
							lcd_put_string(pname);
	                    }
	                    else
	                    {		
						    lcd_put_string(pnum);
	                    }

					    // Show secondery dial number
			            if ( strlen(str_sec_dial) )
			            {
			                i = SCREEN_WIDTH-6-( (strlen(pname)>0 ? get_str_dots_width(pname):get_str_dots_width(pnum))  );
			                lcd_goto_x_line( 4 + (strlen(pname)>0 ? get_str_dots_width(pname):get_str_dots_width(pnum)),1);

			                i/=get_str_dots_width("8");
			                if ( i<strlen(str_sec_dial) )
			                {
			                    lcd_put_string( &str_sec_dial[strlen(str_sec_dial)-i+1 ]  );
			                }
			                else
			                {
			                    lcd_put_string( str_sec_dial  );
			                }
			            }

					 	lcd_goto_x_line(2, 2);
						lcd_clear_line(SCREEN_WIDTH-2);
						lcd_goto_x_line(2, 2);
					}
					else
					{

					    int act_num=get_call_index(call_states, call_cnt, ACTIVE_CALL);

						lcd_goto_x_line(2, 1);
						lcd_clear_line(SCREEN_WIDTH-2);
						lcd_goto_x_line(2, 1);
						
                        if(  call_cnt >=1)
                        {
                            if( 0 == act_num)
                            {
                                appsys.byte_font_type=FONT_B_X_10;
							}
						
							lcd_put_string(call_states[0].num);
							appsys.byte_font_type=FONT_T_X_10;
						}

						lcd_goto_x_line(2, 2);
						lcd_clear_line(SCREEN_WIDTH-2);
						lcd_goto_x_line(2, 2);
					    if(  call_cnt >=2)
                        {
                            if( 1 == act_num || (  (0xff != get_call_index(call_states, call_cnt, RINGING_CALL) ) || (0xff != get_call_index(call_states, call_cnt, DIALING_CALL))))
                            {
                                appsys.byte_font_type=FONT_B_X_10;
							}
							lcd_put_string(call_states[1].num);
							appsys.byte_font_type=FONT_T_X_10;
						}
					}
                }
            }


            if ( appsys.byte_need_redraw & UPDATE_CALL_TIME )
            {
                // Show Call timing
				memset(str_temp_time,0,10);
				sprintf(str_temp_time,"%.2d:%.2d:%.2d",  pcall_record->len_hour, pcall_record->len_minute, pcall_record->len_second);
				appsys.byte_font_type= FONT_T_X_10;
				lcd_goto_x_line((SCREEN_WIDTH - 48)/2, 3);

				lcd_put_string(str_temp_time);
            }


            if ( appsys.byte_need_redraw & UPDATE_CALL_KEY_PROMPT)
            {
    			lcd_goto_x_line(0,5);
				
                if ( 0xff != get_call_index(call_states, call_cnt, WAITING_CALL)  )
                {
                    show_soft_key_prompt(get_multi_string((char * *)text_accept),  get_multi_string((char * *)text_reject));
					byte_blink_waiting =0x80;
                }
                else if ( call_cnt > 1 )
                {
                    if(  (0xff != get_call_index(call_states, call_cnt, RINGING_CALL) ) || (0xff != get_call_index(call_states, call_cnt, DIALING_CALL)))
                    {
                        show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_end));
					}
					else
					{
                        show_soft_key_prompt(get_multi_string((char * *)text_switch), get_multi_string((char * *)text_end));
					}
                }
                else
                {

			        if(  (0 == sysprop->flag_outcall_allow))
			   	    {
		                show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_end));
				    }
					else
					{
	                    if(1 == appsys.flag_holding_call)
	                    {
	                        char * pconst="[OK拨号]"; 
						    unsigned char str_width = get_str_dots_width(pconst);
	                        show_soft_key_prompt(get_multi_string((char * *)text_talk), get_multi_string((char * *)text_end));
					        lcd_goto_xy((SCREEN_WIDTH- str_width)/2, 52);
							lcd_put_string(pconst);	
	                    }
						else
						{
						    if(0 == appsys.flag_call_comming)// call in no holding...
						    {
                                show_soft_key_prompt(get_multi_string((char * *)text_hold), get_multi_string((char * *)text_end));
							}
							else
							{
	                            show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_end));
							}
						}
					}
                }

                if(0 == appsys.flag_holding_call && 0 == flag_waiting_call)
                {
	                lcd_goto_xy( (SCREEN_WIDTH - get_str_dots_width("M"))/2, 54);
	                if(1 == appsys.flag_muted)
	                {
						lcd_put_string("M");
	                }
					else
					{
	                   lcd_put_string("    ");
					}
                }
            }
            
            appsys.byte_need_redraw = 0;
        }
		else 
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{	
				    if(1 ==EventPara.sig_p.key_evt.isPressed)
				    {
					    input_code = get_key_value(EventPara.sig_p.key_evt.key_val);
						if(0 != input_code)
						{
			                if ( strlen(str_sec_dial) < 20 )
			                {
			                    str_sec_dial[strlen(str_sec_dial)] = input_code;
			                }
			                else
			                {
			                    for ( i=0; i<19; ++i )
			                    {
			                        str_sec_dial[i] = str_sec_dial[i+1];
			                    }
			                    str_sec_dial[19] = input_code;
			                    str_sec_dial[20] = 0;
			                }
							appsys.byte_need_redraw |=UPDATE_CALL_NUM;
							mu_dial_dtmf(input_code);
						}
						else
						{
	                        if(TFKEY_HANDFREE == EventPara.sig_p.key_evt.key_val )
	                        {
                                 if (0 == set_voice_path_by_action(VOICE_ACTION_KEY))
                                 {
                                      appsys.flag_talk_stop_type= TALK_STOP_BY_CALLER;
                                      goto QUIT_DIAL;
								 }
							}
							else if(TFKEY_VOL_UP== EventPara.sig_p.key_evt.key_val || TFKEY_VOL_DOWN== EventPara.sig_p.key_evt.key_val)
							{
				                adjust_speech_volume();
							}
							else if(TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val &&  1 == appsys.flag_holding_call)
							{
                               	char str_dialout[BOOK_ENTRY_NUM_FIELD_SIZE+1];
								memset(str_dialout,0,sizeof(str_dialout));
	 
	                            if(DIALING_RET_DIALOUT==wnd_input_dialnumbers(str_dialout,  get_multi_string((char * *)text_input_number), BOOK_ENTRY_NUM_FIELD_SIZE+4))
	                            {
                                    if(  (0 == sysprop->flag_outcall_allow)
					                  ||((0 == sysprop->flag_internationalcall_allow) && is_international_numbers(str_dialout))
					                  ||(0 ==pre_process_numbers(str_dialout))
								    )
                                    {
										 msg_warn(get_multi_string((char * *)text_not_available));
									}
									else
									{
						                 mu_dial_dtmf_str(str_dialout);msg(get_multi_string((char * *)text_dialing));delay_ms(1000);
										 appsys.flag_holding_call=0;
										 flag_waiting_call=1;
										 call_cnt = get_current_calls(call_states, 3);
									}
								}

								appsys.byte_need_redraw=0xFF;
							}
						    else if(TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val)
							{
							    unsigned char tmp_id =0;
								call_cnt = get_current_calls(call_states, 3);
				                if ( call_cnt > 1 )
				                {
				                    if(0 == flag_waiting_call )
				                    {
                                         flag_waiting_call =1;
									}   

									if(  (0xff != get_call_index(call_states, call_cnt, RINGING_CALL) ) || (0xff != get_call_index(call_states, call_cnt, DIALING_CALL)))
									{
                                         //need to do nothing;
									}
									else
									{
									    mu_shift_waiting_call();   
										delay_ms(300);
										call_cnt = get_current_calls(call_states, 3);   
									}
										
				                }
								else
								{  
								    if(0 == appsys.flag_call_comming) // call in no holding...
								    {
						                if ( 0xff == get_call_index(call_states, call_cnt, ACTIVE_CALL))
						                {
						                     if(1 == appsys.flag_genie_trace)DebugPrintf("0xFF ====== ACTIVE_CALL\r\n",NULL);
						                     mu_shift_waiting_call(); 
											 appsys.flag_holding_call=0;
						                }
										else
										{
										     if(1 == appsys.flag_genie_trace)DebugPrintf("HOLDING CALL=1\r\n",NULL);
											 mu_shift_waiting_call(); 
											 appsys.flag_holding_call=1;
										}
								    }
								}

								if(1 == call_cnt)
								{
                                    flag_waiting_call =0;
								}
								
								appsys.byte_need_redraw |= UPDATE_CALL_NUM | UPDATE_CALL_KEY_PROMPT;
                                byte_blink_waiting =0x00;
 
							}
							else if(TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)
							{
							    unsigned char tmp_id =0;
                                byte_blink_waiting =0x00;
								{
                                     lcd_goto_x_line(2, 2);
						             lcd_clear_line(SCREEN_WIDTH-2);
								}

								appsys.flag_talk_stop_type= TALK_STOP_BY_CALLER;
								
								call_cnt = get_current_calls(call_states, 3);

								if(2 == call_cnt)
								{
	                                if(0 == flag_waiting_call)
	                                {
	                                    tmp_id =get_call_index(call_states, call_cnt, WAITING_CALL);
						                if ( 0xff != tmp_id)
						                {
						                    // Reject
						                    mu_terminate_call(0); 
						                }
										flag_waiting_call =0;
									}
									else
									{
										if (  (0xff != get_call_index(call_states, call_cnt, RINGING_CALL) ) || (0xff != get_call_index(call_states, call_cnt, DIALING_CALL)))
										{

										    mu_shift_waiting_call();
										}
										else
										{
		                                    tmp_id =get_call_index(call_states, call_cnt, ACTIVE_CALL);
							                if ( 0xff != tmp_id)
							                {
							                    // end current call;
							                    mu_terminate_call(call_states[tmp_id].id); 
												
												
												flag_waiting_call =0;

												if( call_cnt >= 1 )
												{
		                                            mu_shift_waiting_call();
												}
												else
												{
		                                            break;
												}
							                }
										}
									}

	                                call_cnt = get_current_calls(call_states, 3);
									
									if(call_cnt >0)
									{
									   appsys.flag_no_carrier =0;
									}
								    appsys.byte_need_redraw |= UPDATE_CALL_NUM | UPDATE_CALL_KEY_PROMPT;
									
								}
								else
								{
                                    break;
								}
							}
							else if(TFKEY_PHONEBOOK== EventPara.sig_p.key_evt.key_val)
							{
				                fake_android_message("2,3,2");
							}
							else if(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val)
							{
				                break;
							}
						}
				    }
		        }
				else if(EventPara.eventTyp ==EVENT_HANDSET)
				{
                    if (0 == set_voice_path_by_action(VOICE_ACTION_HANDSET))
                    {
                          appsys.flag_talk_stop_type= TALK_STOP_BY_CALLER;
                          goto QUIT_DIAL;
					}
				}
				else if(EventPara.eventTyp ==EVENT_MODULE)
				{			

				}
				else if(EventPara.eventTyp ==EVENT_TIMER)
				{
				    if( 1 == appsys.flag_no_carrier)
					{
					    appsys.flag_no_carrier =0;
						if(0 == flag_waiting_call)
                        {
                             if( (byte_blink_waiting&0x80) >0) //通话中新来电，未接听之前就挂机了。
                             {
                                  unsigned short pos_clip =0xFFFF;
							 
                                  byte_blink_waiting =0;
                                  call_cnt = get_current_calls(call_states, 3);
								  if( call_cnt >0)
								  {
                                      if(0xFFFF != pos_clip)
                                      {
                                          // send ata to accept
										  call_cnt = get_current_calls(call_states, 3);
									  }
								  }
								  else
								  {
						               appsys.flag_talk_stop_type= TALK_STOP_BY_CALLEE;
						               break;
								  }
								  
	                              if(1== appsys.flag_genie_trace)DebugPrintf("clear the new incom call blinking ------- \r\n",NULL);
								  appsys.byte_need_redraw |= UPDATE_CALL_KEY_PROMPT | UPDATE_CALL_NUM;
								  
							 }
						     else
				             {
				                 appsys.flag_talk_stop_type= TALK_STOP_BY_CALLEE;
				                 break;
				             }
						}
						else
						{
							flag_waiting_call =0;
				            call_cnt = get_current_calls(call_states, 3);

				            if ( call_cnt > 0 )
				            {		            
					            if(1== appsys.flag_genie_trace)DebugPrintf("back to the holded call ++++++++------- id=%d\r\n",0);

							    if ( 0xff == get_call_index(call_states, call_cnt, ACTIVE_CALL))
					            {
					                mu_shift_waiting_call(); 
									call_cnt = get_current_calls(call_states, 3);
					            }
							
				                appsys.byte_need_redraw |= UPDATE_CALL_KEY_PROMPT | UPDATE_CALL_NUM;
				            }
				            else
				            {
				                appsys.flag_talk_stop_type= TALK_STOP_BY_CALLEE;
				                break;
				            }
						}
					}
				
					if(	appsys.back_rating.flag_end_dial>0)
					{
					    appsys.back_rating.flag_end_dial=0;
                        goto QUIT_DIAL;
					}
					else if(appsys.back_rating.flag_money_minder>0)
					{
                        appsys.back_rating.flag_money_minder=0;
						mu_generate_beep();
					}

					if( appsys.flag_half_second> 0)
					{
					    appsys.flag_half_second =0;
					    appsys.byte_need_redraw |= UPDATE_CALL_TIME;
						appsys.byte_need_redraw |= UPDATE_CALL_CHARGE_INFO;


						if(1 == appsys.flag_showing_lowbat)
						{
						     flag_blinking=(1 == flag_blinking)?0:1;
							 if(1 == flag_blinking)
							 {
                                 app_show_battery(0);
							 }
							 else
							 {
							     lcd_goto_xy(SCREEN_WIDTH-22 ,0);
								 lcd_clear_line_ex(10, SCREEN_WIDTH-1);
							 }
						}



						if( (byte_blink_waiting&0x80) >0)
						{
		                    lcd_goto_x_line(2, 2);
						    lcd_clear_line(SCREEN_WIDTH-2);
							lcd_goto_x_line(2, 2);
                            if(0x81 == byte_blink_waiting)
                            {
                                int inc_num=get_call_index(call_states, call_cnt, WAITING_CALL);
                                byte_blink_waiting =0x80;
								if(0xFF != inc_num)
			                    {
								 	lcd_put_string(call_states[inc_num].num);
			                    }
							}
							else
							{
                                 byte_blink_waiting =0x81;
							}
						}
					}

                    if(get_duration_tick(appsys.dword_callwait_clcc_tick) >2000  && (2 == call_cnt || 1 == flag_been_callwaited))
					{
					    call_cnt = get_current_calls(call_states, 3);
                        if(1 == call_cnt)
                        {
                            flag_waiting_call =0;
						    if ( 0xff == get_call_index(call_states, call_cnt, ACTIVE_CALL))
				            {
				                mu_shift_waiting_call(); 
								call_cnt = get_current_calls(call_states, 3);
				            }
							appsys.byte_need_redraw |= UPDATE_CALL_KEY_PROMPT | UPDATE_CALL_NUM;
                        }
						else if(0 == call_cnt)
						{
                            appsys.flag_talk_stop_type= TALK_STOP_BY_CALLEE;
							break;
						}
					}
					
					if( TALK_STOP_BY_LOWBAT == appsys.flag_talk_stop_type)
					{
                        goto QUIT_DIAL;
					}
				}
		    }
		}
    }
    QUIT_DIAL:
    mu_hook_on();

	//save call total time
	{
        unsigned long tmp_long= pcall_record->len_hour;
	    tmp_long*=60;
	    tmp_long+=pcall_record->len_minute;
	    tmp_long*=60;
	    tmp_long+=pcall_record->len_second;
		
		if(1 == appsys.flag_call_comming)
		{
            sysprop->dword_call_in_tm_total+=tmp_long;
		}
		else
		{
            sysprop->dword_call_out_tm_total+=tmp_long;
		}
		app_save_prop();
	}

	appsys.flag_talk_rating=0;
	appsys.flag_holding_call=0;

}


void app_back_ground_rating()
{

    unsigned long temp_money;
	unsigned long temp_duration;	
	T_CALL_RECORD * p_call = appsys.back_rating.pcall_record;

    if( NULL !=p_call)
    {
	    refresh_call_record(p_call);
	}
}



void app_stop_call()
{
   if(1== appsys.flag_playing_tone)
   {
       mu_stop_dialtone();
   }

   if(1 == appsys.flag_muted)
   {
       mu_cancel_mute_mic();
   }
   mu_hook_on();
}

unsigned long get_talked_seconds(T_CALL_RECORD * pcall_record)
{
     unsigned long ret_long=0;
	 ret_long=pcall_record->len_hour;
	 ret_long *=60;
	 ret_long=pcall_record->len_minute;
	 ret_long *=60;
	 ret_long +=pcall_record->len_second;
     return ret_long;	 
}












void app_dial_out(char * init_str)
{
   unsigned char nret=0;
   unsigned char flag_can_play_tone =1;
  
   char str_dialout[BOOK_ENTRY_NUM_FIELD_SIZE+5];

   unsigned char dialing_ret=DIALING_RET_NO_SENSE;

   appsys.byte_need_exit=0;
   appsys.dword_last_input_tick=app_get_tick_count();
   appsys.flag_voice_doing=1;
   

   if( 0 == sysprop->flag_key_tone )
   {
       app_stop_music();
   }

   memset(str_dialout,0,sizeof(str_dialout));
   appsys.flag_talk_stop_type= TALK_STOP_BY_CALLER;
   appsys.flag_no_carrier =0;
   appsys.flag_dialout_succeed =0;
   appsys.byte_input_key_when_delay =0;


   if(0 == appsys.flag_pre_dial)
   {
	   if(1 == appsys.flag_hand_free && 0 == appsys.flag_hdfree_led_on){set_handfree_led(1); if(1 == appsys.flag_genie_trace)DebugPrintf("(03)set_handfree_led=1 \r\n",NULL);;}
   }

   if( ((0 ==init_str) || (0 == strlen(init_str))) )
   {
       flag_can_play_tone =1;
   }
   else
   {
       strcpy(str_dialout, init_str);
	   flag_can_play_tone =0;
   }

   if(1 == flag_can_play_tone)
   {
       mu_set_spk_gain(sysprop->byte_talk_volume);
	   mu_play_dialtone();
   }

   
   appsys.flag_dialing=1;
   appsys.flag_redial=0;

   if(0 == appsys.flag_doing_auto_redial)
   {
       if(1 == sysprop->flag_auto_redial)
       {
           appsys.byte_redial_cnt=sysprop->auto_redial.redial_cnt;
	   }
   }

   if(0 == appsys.flag_fast_dial)
   {
      char * pcap=NULL;

	  if(appsys.flag_nouim_emergency_call==1)
	  {
		   pcap = get_multi_string((char * *)text_emergency_number);
	  }
	  else
	  {
		   pcap = get_multi_string((char * *)text_input_number);
	  }
	  dialing_ret=wnd_input_dialnumbers(str_dialout,  pcap, BOOK_ENTRY_NUM_FIELD_SIZE+4);

	  if(DIALING_RET_DIALOUT != dialing_ret)
	  {
         appsys.byte_redial_cnt =0;
		 memset(sysprop->auto_redial.num,0,sizeof(sysprop->auto_redial.num));
	  }
   }
   else
   {
       dialing_ret =DIALING_RET_DIALOUT;
   }

   switch(dialing_ret)
   {
       case DIALING_RET_DIALOUT:
       {
		   unsigned char flag_can_call_out=1;
		   if(0 ==  is_emergency_number( str_dialout))
		   {
		   	   if( 0 == sysprop->flag_outcall_allow)
		   	   {
		   	        flag_can_call_out=0;
	                msg_warn(get_multi_string((char * *) text_call_out_is_barring));
				    appsys.byte_redial_cnt =0;
					memset(sysprop->auto_redial.num,0,sizeof(sysprop->auto_redial.num));
					delay_ms(2000);
			   }
			   else if( ((0 == sysprop->flag_internationalcall_allow) && is_international_numbers(str_dialout)) )
               {
                    flag_can_call_out=0;
	                msg_warn(get_multi_string((char * *)text_international_all_is_barring));
		            appsys.byte_redial_cnt =0;
					memset(sysprop->auto_redial.num,0,sizeof(sysprop->auto_redial.num));
					delay_ms(2000);
			   }

			   if(0 ==pre_process_numbers(str_dialout))
			   {
			        appsys.byte_redial_cnt =0;
                    flag_can_call_out=0;
				    msg_warn(get_multi_string((char * *)text_not_available));
			        delay_ms(1500);
			   }			   
		   }

		   if(1 == flag_can_call_out)
		   {		   
		       T_CALL_RECORD call_record;
			   T_BOOK_ENTRY  book_entry;
			   memset(&call_record,0 ,sizeof(T_CALL_RECORD));					   
			   memset(&book_entry, 0, sizeof(T_BOOK_ENTRY));
			 
			   strncpy(book_entry.num,  str_dialout, BOOK_ENTRY_NUM_FIELD_SIZE);

			   call_record.year=appsys.struct_date.year;
			   call_record.month=appsys.struct_date.mon;
			   call_record.day=appsys.struct_date.day;
			   call_record.hour=appsys.struct_time.hour;
			   call_record.minute=appsys.struct_time.min;
			   call_record.second=appsys.struct_time.sec;
			   call_record.money_cost=0;
			   call_record.not_answer=1;


			   strncpy(call_record.num, str_dialout, BOOK_ENTRY_NUM_FIELD_SIZE);
			   
               if(1 == appsys.flag_hand_free  && 0 == appsys.flag_hdfree_led_on){set_handfree_led(1);	if(1 == appsys.flag_genie_trace)DebugPrintf("(04)set_handfree_led=1 \r\n",NULL);}
			   
			   if(1 ==(nret = dialout_numbers(&book_entry)))
			   {  
			       appsys.flag_dialout_succeed =1;
			       appsys.byte_redial_cnt=0;
				   
				   call_record.not_answer=0;
				   call_record.year=appsys.struct_date.year;
				   call_record.month=appsys.struct_date.mon;
				   call_record.day=appsys.struct_date.day;
				   call_record.hour=appsys.struct_time.hour;
				   call_record.minute=appsys.struct_time.min;
				   call_record.second=appsys.struct_time.sec;

                   memset(&(appsys.back_rating),0,sizeof(T_BACKGROUND_RATING));
				   appsys.back_rating.pcall_record=&call_record;
				   appsys.back_rating.flag_end_dial=0;
				   appsys.back_rating.flag_start_rating=0;
				   appsys.back_rating.flag_last_duration=0;
				   appsys.back_rating.flag_money_minder=0;
				   appsys.back_rating.dword_last_minter_tick=0;		
				   
				   appsys.flag_dialing=0;
				   
				   appsys.flag_talk_rating=1;
				   talk_phone_call(&call_record, book_entry.name);
				   appsys.flag_talk_rating=0;

			   }
			   else
			   {
                   if( (1 == sysprop->flag_auto_redial) && (0xFF != nret ))
                   {
                       app_stop_call();
					   strncpy(sysprop->auto_redial.num, book_entry.num,sizeof(sysprop->auto_redial.num) -1);
					   sysprop->auto_redial.redial_tick= app_get_tick_count();
                      
				   }
				   else if(0xFF == nret)//stopped by user
				   {
				       appsys.flag_dialout_succeed=1;
                       appsys.byte_redial_cnt=0;
				   }
			   }
	   
        
			   if(strlen(trim(call_record.num)) >0)
			   {
			       app_record_append(TABLEINFO_CALL_OUT, (unsigned char *)&call_record, REC_PREFIX_APPEND);
			   }
		   }
	   }
	   break;
		
       case DIALING_RET_GET_MAINMENU:
       {
		   run_main_menu();
	   }
	   break;

	   case DIALING_RET_SUPERSETTING:
       {
           app_run_super_settings();
	   }
	   break;

	   case DIALING_RET_GET_VERISON:
       {
          	app_view_version();
	   }
	   break;

	   case DIALING_RET_GET_IMEI:
       {
           show_device_serial_number();
	   }
	   break;
	   
	   case DIALING_RET_EN_DEBUG:
       {

	   }
	   break;	

	   case DIALING_RET_SPECIAL_TEST:
       {
	   }
	   break;


	   
       case DIALING_RET_PREFIX_RW:
       {

	   }
	   break;


	   
	   case DIALING_RET_PROJECT_TEST:
	   {
	   	   appsys.flag_hardware_test =1;
           app_hardware_test();
		   appsys.flag_hardware_test =0;
	   }
	   break;	  

       case DIALING_RET_LOCK_OPER:
       {

	   }
	   break;
	   
	   case DIALING_RET_LOCK_BASE:
	   {
	   }
	   break;

	   case DIALING_RET_SIM_ENCYPT:
	   {
	   }
	   break;	 

	   case DIALING_RET_TDKEY_QUERY:
	   {
	   }
	   break;	

	   case DIALING_RET_OPEN_USB:
	   {

	   }
	   break;



	   case DIALING_RET_NORMAL_WORK:
	   {
   
	   }
	   break;

       case DIALING_RET_LOCK_PHONE:
       {   
		   app_save_prop();
		   delay_ms(1000);

	   }
	   break;


	   case DIALING_RET_SET_IP_PARA:
	   {

	   }
	   break;

	   case DIALING_RET_SET_LIFE_SRV:
	   	   app_set_fast_dial(3);  // 3 在排列上是"生活服务号码"
	   break;

	   case DIALING_RET_GET_CSQ_VAL:
	   {
           show_device_csq_level();
	   }
	   break;

	   case DIALING_RET_GET_POWER_INFO:
	   {
           show_device_power_info();
	   }
	   break;  


	   case DIALING_RET_SET_DOOR_ALMTP:
       {

	   }
	   break;
	   
	   case DIALING_RET_SET_P2P_SNSR_ERR:
	   {

	   }
	   break;
	   
	   case DIALING_RET_SET_P2P_LOW_BAT:
	   {
   
	   }
	   break;
	   
	   case DIALING_RET_NO_SENSE:
	   default:
	   break;
   }
   app_stop_call();

   mu_set_voice_path(VOICE_PATH_HANDFREE);

   appsys.flag_dialing=0;
   appsys.flag_hand_free =0;
   appsys.flag_no_carrier =0;
   appsys.byte_need_redraw=0xFF;


   if(1 == appsys.flag_hdfree_led_on)
   {
       if(1 == appsys.flag_genie_trace)DebugPrintf("(05)set_handfree_led=0 \r\n",NULL);	
       set_handfree_led(0);
   }
   appsys.flag_voice_doing=0;
   appsys.byte_need_exit=0xFF;


}





