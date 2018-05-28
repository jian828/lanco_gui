
#include "inc/app_def.h"


void wnd_record_delete(RECORDWND * p_record_wnd)
{
     if(p_record_wnd->line_cnt  >0)
     {
	     app_record_delete(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1));

         p_record_wnd->line_cnt--;
		 
		 if(p_record_wnd->line_cnt  >0)
		 {
	         if(0 == p_record_wnd->cur_line)
	         {
	             if(p_record_wnd->line_start>0)
	             {
	                 p_record_wnd->line_start-=p_record_wnd->body_lines;
				     p_record_wnd->cur_line=p_record_wnd->body_lines-1;
			     }
		     }
		     else
		     {
	             p_record_wnd->cur_line--;
		     }
		 }
		 else
		 {
	         p_record_wnd->line_start=0;
			 p_record_wnd->cur_line=0;
		 }
     }
}


void sms_box_option(RECORDWND * p_record_wnd)
{
    if ( TABLEINFO_SMS_INBOX== p_record_wnd->table_idx)
    {
        switch ( app_menu_select((char ***)mem_list_in_sms_opt, 6, NULL) )
        {
            case 1:
			    app_show_sms_txt(p_record_wnd);
			    break;

            case 2:
                wnd_record_delete(p_record_wnd);			
                break;
            case 3:
                {
					T_SMS_RECORD sms_record;
				    if(1 == app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1),(unsigned char *)&sms_record))
				    {
                         show_sms_info(&sms_record, p_record_wnd->table_idx);
					}
            	}
                break;
            case 4:
                {
				    T_SMS_RECORD sms_record;
				    if(1 == app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *)&sms_record))
				    {
				        if(strlen(sms_record.str_num) >0 )
				        {
					        appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
						    if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
						    {
						        app_dial_out(sms_record.str_num);
						    }
				        }
				    }
            	}
                break;
            case 5:
				app_show_record_capacity(p_record_wnd->table_idx);
                break;

			case 6:
                app_show_sms_from_simcard();
				break;

            default:
                break;
        }
    }
    else if( TABLEINFO_SMS_OUTBOX== p_record_wnd->table_idx || TABLEINFO_SMS_DRAFT== p_record_wnd->table_idx)
    {
        switch ( app_menu_select((char ***)mem_list_out_sms_opt, 4, NULL) )
        {
            case 1:
			    app_show_sms_txt(p_record_wnd);
			    break;
		
            case 2:
                wnd_record_delete(p_record_wnd);
                break;

            case 3:
				{
					T_SMS_RECORD sms_record;
				    if(1 == app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1),(unsigned char *)&sms_record))
				    {
                         show_sms_info(&sms_record, p_record_wnd->table_idx);
					}
            	}
                break;
            case 4:
				app_show_record_capacity(p_record_wnd->table_idx);
                break;
				

            default:
                break;
        }
    }



}







void search_book_option(RECORDWND * p_record_wnd)
{
    switch ( app_menu_select((char ***)mem_search_book_opt, 5, NULL) )
    {
        case 1:
		    {
			    unsigned short rec_pos=modify_book_entry(p_record_wnd->selected_pos +1);
                if(rec_pos >0)
                {
                   p_record_wnd->line_start=rec_pos-1;
				   p_record_wnd->cur_line=0;
				}

		    }

            break;

        case 2:
            {
                unsigned short rec_pos=new_book_entry(NULL);

				if(rec_pos>0)
				{
                   p_record_wnd->line_start=rec_pos-1;
				   p_record_wnd->cur_line=0;
				}
        	}
            break;

        case 3:
			if(delete_book_entry(p_record_wnd->selected_pos +1))
			{
               if(0 == p_record_wnd->cur_line)
               {
                   if(p_record_wnd->line_start>0)
                   {
                       p_record_wnd->line_start-=p_record_wnd->body_lines;
					   p_record_wnd->cur_line=p_record_wnd->body_lines-1;
				   }
			   }
			   else
			   {
                   p_record_wnd->cur_line--;
			   }
			}
            break;

        case 4:
            {
		        T_BOOK_ENTRY book_entry;
				memset(&book_entry,0,sizeof(T_BOOK_ENTRY));

			    if(1 ==app_record_read(TABLEINFO_PHONEBOOK, (p_record_wnd->selected_pos+1), (unsigned char *)&book_entry))
			    {
			         appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
				     if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
				     {
                          app_dial_out(book_entry.num);
				     }
				}
            }
            break;
		case 5:
			{
			    app_show_record_capacity(p_record_wnd->table_idx);
			}
            break;
        default:
            break;
    }
}



void view_call_log( RECORDWND* p_record_wnd )
{
    FlSignalBuffer EventPara;
    T_CALL_RECORD call_record;
    T_BOOK_ENTRY  book_entry;
    unsigned char  i;
    unsigned short record_count;
	unsigned short rec_pos=p_record_wnd->selected_pos+1;
    char tmp_buf[20];

    appsys.byte_need_redraw=0xFF;
    record_count= app_get_record_used(p_record_wnd->table_idx);

    while ( 1 )
    {
        if ( appsys.byte_need_redraw>0 )
        {
            appsys.byte_need_redraw = 0;
		    if(0 == app_record_read(p_record_wnd->table_idx,rec_pos, (unsigned char *)&call_record))
            {
                return;
			}

            if(1 == call_record.not_answer  &&  TABLEINFO_MISSED_CALL == p_record_wnd->table_idx)
            {
                call_record.not_answer=0;
				app_record_edit(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *)&call_record);
			}

			
            lcd_clear_screen();

            if( find_book_entry_by_numbers(call_record.num, &book_entry))
            {
               show_caption(book_entry.name);
			}
			else
			{
               show_caption(get_multi_string((char * *)text_no_name));
			}
            

            appsys.byte_font_type= FONT_T_X_10;

		    lcd_goto_x_line(0, 1);
			lcd_put_string(get_multi_string((char * *)text_number));
			lcd_put_string(":");
            lcd_put_string(call_record.num);


            if(1)
            {
               lcd_goto_x_line(0, 2);
			   memset(tmp_buf,0,sizeof(tmp_buf));
			   sprintf(tmp_buf, "%.2d:%.2d  %.4d-%.2d-%.2d",call_record.hour,call_record.minute,call_record.year,call_record.month,call_record.day);
			   lcd_put_string(get_multi_string((char * * )text_time));
			   lcd_put_string(tmp_buf);
			}
			
			if(TABLEINFO_MISSED_CALL != p_record_wnd->table_idx)
			{
			   unsigned short bmp_size=0;
			   lcd_goto_x_line(0, 3);
			   memset(tmp_buf,0,sizeof(tmp_buf));
			   sprintf(tmp_buf, "%.2d:%.2d:%.2d",call_record.len_hour,call_record.len_minute,call_record.len_second);
			   lcd_put_string(get_multi_string((char * *)text_duration));
			   lcd_put_string(":");
			   lcd_put_string(tmp_buf);

               lcd_goto_x_line(0, 4);
			   memset(tmp_buf,0,sizeof(tmp_buf));
			   money_digit_to_asc_10(call_record.money_cost, tmp_buf);


			   lcd_put_string("Units");
			   
			   lcd_put_string(":");
			   lcd_put_string(tmp_buf);
			}

		    show_soft_key_prompt(  get_multi_string((char * *) text_option) ,get_multi_string((char * *)text_back));
	
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
						else if( TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
                            switch(  app_menu_select ((char ***)mem_view_call_log_opt, 3, NULL))
                            {
                                case 1:
									wnd_record_delete(p_record_wnd);
						
									record_count=app_get_record_used(p_record_wnd->table_idx);
									if( 0 == record_count )
									{
                                        return;
									}
									else
									{
                                        if(rec_pos> record_count)
                                        {
                                            rec_pos=record_count;
										}
									}
									break;
								case 2:
                                    {
										unsigned char flag_find =0;
										T_BOOK_ENTRY dest_entry;
										memset(&dest_entry,0,sizeof(dest_entry));
									    if ( find_book_entry_by_numbers(call_record.num, &dest_entry) )
									    {
                                            if(strlen( trim( dest_entry.name)) >0)
                                           	{
                                                 msgok("该号码已经存在!");
												 delay_ms(1000);
												 flag_find =1;
												 break;
											}
										}
										
										if(0 == flag_find)
										{
                                            new_book_entry(call_record.num);
										}
								    }
									break;
									break;
								case 3:
									app_show_record_capacity(p_record_wnd->table_idx);
								default:
									break;

							}
							appsys.byte_need_redraw=0xFF;
						}
						else if(TFKEY_REDIAL==EventPara.sig_p.key_evt.key_val || TFKEY_SEND==EventPara.sig_p.key_evt.key_val)
						{
                            if(rec_pos<= record_count)
                            {
                                if(1 == app_record_read(p_record_wnd->table_idx, rec_pos, (unsigned char *)&call_record))
                                {
                                    appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
								    if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
								    {
								         appsys.flag_fast_dial=1;
									     app_dial_out(call_record.num);
										 appsys.flag_fast_dial=0;
								    }
									appsys.byte_need_redraw=0xFF;
								}
							}
						}
				    	else if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val) || (TFKEY_LEFT==EventPara.sig_p.key_evt.key_val))
				    	{
                            if(rec_pos>1)
                            {
                                rec_pos--;
								app_record_read(p_record_wnd->table_idx, rec_pos, (unsigned char *)&call_record);
								appsys.byte_need_redraw=0xFF;
							}
						}
					 	else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || (TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val))
					 	{
                            if(rec_pos< record_count)
                            {
                                rec_pos++;
								app_record_read(p_record_wnd->table_idx, rec_pos, (unsigned char *)&call_record);
								appsys.byte_need_redraw=0xFF;
							}
						}
					}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp)
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




void app_records_show( unsigned char table_idx, char * pcap)
{
    RECORDWND  m_record_wnd;
	unsigned char nret=0;
    unsigned short pos_info=0;
	
	memset(&appsys.fst_dial,0, sizeof(appsys.fst_dial));


	if(TABLEINFO_CALL_IN == table_idx
    ||TABLEINFO_CALL_OUT== table_idx
    ||TABLEINFO_MISSED_CALL== table_idx
	||TABLEINFO_SMS_INBOX== table_idx
	||TABLEINFO_SMS_OUTBOX== table_idx
    ||TABLEINFO_SMS_DRAFT== table_idx
	)
	{
	    if(1 ==appsys.flag_genie_trace)DebugPrintf("Load book infomation in app_records_show ++++\r\n",NULL);
       app_load_table_info(TABLEINFO_PHONEBOOK, &appsys.book_info);
	}
	
	memset(&m_record_wnd,0,sizeof(RECORDWND));
    m_record_wnd.table_idx=table_idx;
    m_record_wnd.pindex = (unsigned char *)app_alloc_memory(FLASH_CELL_LENGTH_1K);
	m_record_wnd.pcaption=pcap;

	m_record_wnd.style =RECORD_STYLE_SHOW_FRAME| RECORD_STYLE_VERTICAL_BAR;

	m_record_wnd.frame_left =0;
	m_record_wnd.frame_top =12;
	m_record_wnd.frame_right =SCREEN_WIDTH-1;
	m_record_wnd.frame_bottom =51;
	m_record_wnd.record_font=FONT_T_X_10;
	
	while(0 == appsys.byte_need_exit)
	{

		if(pos_info!=0)
		{   
			m_record_wnd.line_start=   ((pos_info&0xFFF0)>>4 ) ;
			m_record_wnd.cur_line=     ((pos_info&0x000F)>>0 ) ;
		}
		else
		{
			m_record_wnd.line_start=0;
			m_record_wnd.cur_line=0;
		}
		m_record_wnd.selected_pos=m_record_wnd.line_start+m_record_wnd.cur_line;



		

		appsys.byte_need_redraw=0xFF;
		appsys.flag_record_showing=1;

	    if(1 ==wnd_record_show(&m_record_wnd))
	    {
		    pos_info =m_record_wnd.line_start;
	        pos_info <<=4;
		    pos_info +=m_record_wnd.cur_line;
		}
		else
		{
		    appsys.byte_need_redraw=0xFF;
            break;
		}
		
	}
    if(1 ==appsys.flag_genie_trace)DebugPrintf("Free book infomation in app_records_show ----\r\n",NULL);
	app_free_table_info(&appsys.book_info);
	appsys.flag_record_showing=0;
	app_free_memory(m_record_wnd.pindex);m_record_wnd.pindex=NULL;
    memset(&appsys.fst_dial,0,sizeof(appsys.fst_dial));
	app_flash_might_save("APP_RECORDS_SHOW");
}

unsigned char run_records_list_softkey_action(RECORDWND* p_record_wnd, unsigned char soft_key)
{
    unsigned char nret=0;
    if(TABLEINFO_PHONEBOOK== p_record_wnd->table_idx)
	{
	    if(TFKEY_SOFT_RIGHT== soft_key)
	    {
             return 0;
		}
		else if((TFKEY_SOFT_LEFT== soft_key || (TFKEY_CONFIRM ==soft_key)) && p_record_wnd->line_cnt>0 )
		{
            search_book_option(p_record_wnd);
		}
		else if((TFKEY_REDIAL==soft_key   ||  TFKEY_SEND ==soft_key) && (p_record_wnd->line_cnt>0))
		{
	        T_BOOK_ENTRY book_entry;
			memset(&book_entry,0,sizeof(T_BOOK_ENTRY));

		    if(1 ==app_record_read(TABLEINFO_PHONEBOOK, (p_record_wnd->selected_pos+1), (unsigned char *)&book_entry))
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
    else if( TABLEINFO_CALL_IN == p_record_wnd->table_idx
      ||TABLEINFO_CALL_OUT     == p_record_wnd->table_idx
      ||TABLEINFO_MISSED_CALL  == p_record_wnd->table_idx
	)
    {
        if(  TFKEY_SOFT_RIGHT== soft_key )
        {
             return 0;
		}
        else if(  TFKEY_SOFT_LEFT== soft_key|| (TFKEY_CONFIRM ==soft_key) )
		{
            view_call_log(p_record_wnd);
		}
		else if(  (TFKEY_REDIAL==soft_key || TFKEY_SEND== soft_key) && p_record_wnd->line_cnt>0 )
		{
            T_CALL_RECORD call_record;
			memset(&call_record, 0, sizeof(T_CALL_RECORD));
			if( 1== app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *)&call_record))
			{
                appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
			    if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
			    {
			         appsys.flag_fast_dial=1;
				     app_dial_out(call_record.num);
					 appsys.flag_fast_dial=0;
			    }
			}
		}
	}
	else if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx
          ||TABLEINFO_SMS_OUTBOX == p_record_wnd->table_idx
	)
	{
	    if(TFKEY_SOFT_RIGHT== soft_key)
	    {
            return 0;
		}
		else if( (TFKEY_SOFT_LEFT== soft_key || TFKEY_CONFIRM ==soft_key)&& p_record_wnd->line_cnt>0 )
		{
		    sms_box_option(p_record_wnd);
		}
		else if(  (TFKEY_REDIAL==soft_key || TFKEY_SEND == soft_key) && p_record_wnd->line_cnt>0 )
		{
            T_SMS_RECORD sms_record;
			memset(&sms_record, 0, sizeof(T_SMS_RECORD));
			if( 1== app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *)&sms_record))
			{
                appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
			    if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
			    {
			         appsys.flag_fast_dial=1;
				     app_dial_out(sms_record.str_num);
					 appsys.flag_fast_dial=0;
			    }
			}
		}
	}
    else if(TABLEINFO_SMS_DRAFT== p_record_wnd->table_idx)
	{
		if(TFKEY_SOFT_RIGHT== soft_key)
	    {
            return 0;
		}
		else if((TFKEY_SOFT_LEFT== soft_key || (TFKEY_CONFIRM ==soft_key))&& p_record_wnd->line_cnt>0 )
		{
		    sms_box_option(p_record_wnd);
		}
	}



	return 1;
}

void set_records_list_soft_key(RECORDWND* p_record_wnd)
{
    unsigned char nret=0;


    if( (p_record_wnd ->style & RECORD_STYLE_RET_SELECT)>0 )
    {
         show_soft_key_prompt(get_multi_string((char * *)text_select), get_multi_string((char * *)text_back));
	}
	else
	{
	    if(TABLEINFO_PHONEBOOK== p_record_wnd->table_idx)
		{
		    if(p_record_wnd->line_cnt >0)
		    {			
	            show_soft_key_prompt(get_multi_string((char * *)text_option), get_multi_string((char * *)text_back));
		    }
			else
			{
	            show_soft_key_prompt(get_multi_string((char * *)text_new), get_multi_string((char * *)text_back));
			}
		}
	    else if( TABLEINFO_CALL_IN == p_record_wnd->table_idx
	      ||TABLEINFO_CALL_OUT     == p_record_wnd->table_idx
	      ||TABLEINFO_MISSED_CALL  == p_record_wnd->table_idx
		)
	    {
		    if(p_record_wnd->line_cnt >0)
		    {	    
	            show_soft_key_prompt(get_multi_string((char * *)text_view), get_multi_string((char * *)text_back));
		    }
			else
			{
	            show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_back));
			}
		}
		else if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx
	          ||TABLEINFO_SMS_OUTBOX == p_record_wnd->table_idx
	          ||TABLEINFO_SMS_DRAFT  == p_record_wnd->table_idx
		)
		{
		    if(p_record_wnd->line_cnt >0)
		    {			
	            show_soft_key_prompt(get_multi_string((char * *)text_option), get_multi_string((char * *)text_back));
		    }
			else
			{
			    if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx)
			    {
                    show_soft_key_prompt(get_multi_string((char * *)text_simcard_sms), get_multi_string((char * *)text_back));
				}
				else
				{
	                show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_back));
				}
			}
		}


	}
}

void wnd_load_table_index(RECORDWND* p_record_wnd)
{
    if(1 ==appsys.flag_genie_trace)DebugPrintf("load index =%d+++++++++\r\n",p_record_wnd->table_idx);
    flash_cell_read( (p_record_wnd->table_idx  + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start), g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, p_record_wnd->pindex);
    p_record_wnd->line_cnt=wnd_get_record_used(p_record_wnd);
}

unsigned short wnd_get_record_used(RECORDWND* p_record_wnd)
{
    unsigned long tmp_long = wnd_get_record_used_and_index(p_record_wnd,0);
    return (unsigned short)(tmp_long>>16);
}

unsigned long wnd_get_record_used_and_index(RECORDWND* p_record_wnd, unsigned short rec_no)
{
     unsigned long ret_idx=0;
     unsigned short * p_idx  = (unsigned short *)p_record_wnd->pindex;

	 if(NULL != p_idx)
	 {
		 ret_idx= p_idx[0];
		 ret_idx <<=16;
		 ret_idx +=p_idx[rec_no];
	 }
	 return ret_idx;
}

unsigned char wnd_record_read(RECORDWND* p_record_wnd, unsigned short  rec_no, unsigned char * pbuf )
{
    if(NULL != p_record_wnd->pindex)
    {
		 unsigned short rec_used=0;
		 unsigned short rec_idx=0;

		 unsigned long tmp_long =wnd_get_record_used_and_index(p_record_wnd, rec_no);

		 rec_used= (tmp_long>>16)&0xFFFF;
		 rec_idx = (tmp_long>>0 )&0xFFFF;   
		 
		 if(rec_used> g_table_info[p_record_wnd->table_idx].tb_info.rec_max_cnt  ||  rec_no> rec_used ||  rec_idx ==0)
		 {
	         if(1 ==appsys.flag_genie_trace)DebugPrintf("error2 parameteres rec_no=%d  used=%d  idx=%d  cell_cnt=%d   in <app_record_read>\r\n", rec_no,rec_used, rec_idx,g_table_info[p_record_wnd->table_idx].tb_info.rec_max_cnt);
			 return 0;
		 }
		 if(app_flash_read((p_record_wnd->table_idx+DBINFO_TABLE_START), (rec_idx-1),  pbuf))
		 {
		     return 1;
		 }
		 return 0;
	}
	else
	{
         return app_record_read(p_record_wnd->table_idx, rec_no, pbuf);
	}
}

unsigned char get_record_string(RECORDWND* p_record_wnd, char *rec_buf, unsigned short rec_pos)
{
    // must put the first byte as flag '0'~'9'
    unsigned char nret=0;
    unsigned char i,flag_find;
	unsigned short st_pos, et_pos,pre_cnt;
    i=flag_find=pre_cnt=0;
	st_pos=et_pos=0;
	
	if(1 == appsys.flag_prerec_changed)
	{
	    for(i=0;i<4;i++)
		{
		    p_record_wnd->pre_rec[i].pos =0	 ;
		}
	}
	
	for(i=0;i<4;i++)
	{
        if(p_record_wnd->pre_rec[i].pos == rec_pos)
        {  
            sprintf(rec_buf,"%s", p_record_wnd->pre_rec[i].rec_string);
			flag_find=1;
		}
		if(0 !=  p_record_wnd->pre_rec[i].pos)
		{
		   if(0 == st_pos)
		   {
               st_pos=p_record_wnd->pre_rec[i].pos;
		   }
           et_pos=p_record_wnd->pre_rec[i].pos;

		   pre_cnt++;
		}
	}

	if(1 == flag_find)
	{
		return 1;
	}

    if(TABLEINFO_PHONEBOOK== p_record_wnd->table_idx)
	{
        T_BOOK_ENTRY book_entry;
		memset(&book_entry,0,sizeof(T_BOOK_ENTRY));

	    if(1 ==wnd_record_read(p_record_wnd, rec_pos, (unsigned char *)&book_entry))
	    {
             sprintf(rec_buf, "0%.3d %s",rec_pos, book_entry.name);
			 nret=1;
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
             T_BOOK_ENTRY temp_entry;
			 memset(&temp_entry,0, sizeof(T_BOOK_ENTRY));
			 
			 if(1 == find_book_entry_by_numbers(call_record.num, &temp_entry))
			 {
                 sprintf(rec_buf, "%c%.2d-%.2d %s",(0 ==call_record.not_answer)?'0':'1', call_record.month,call_record.day,temp_entry.name);
			 }
			 else
			 {
                 sprintf(rec_buf, "%c%.2d-%.2d %s",(0 ==call_record.not_answer)?'0':'1', call_record.month,call_record.day,call_record.num);
			 }

			 nret=1;
		}
	}
	else if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx
          ||TABLEINFO_SMS_OUTBOX == p_record_wnd->table_idx
          ||TABLEINFO_SMS_DRAFT  == p_record_wnd->table_idx
	)
	{
        T_SMS_RECORD sms_record;
		T_BOOK_ENTRY temp_entry;
		
		memset(&temp_entry,0, sizeof(T_BOOK_ENTRY));		
		memset(&sms_record,0,sizeof(T_SMS_RECORD));

	    if(1 ==wnd_record_read(p_record_wnd, rec_pos, (unsigned char *)&sms_record))
	    {
		     if(1 == find_book_entry_by_numbers(sms_record.str_num, &temp_entry))
			 {
		         if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx)
		         {
	                 sprintf(rec_buf, "%c %.2d-%.2d %s", (1==sms_record.sms_flag.flag_read)?'1':'0',sms_record.date.mon,sms_record.date.day,temp_entry.name);
		         }
				 else
				 {
	                 sprintf(rec_buf, "%c %.2d-%.2d %s", (1==sms_record.sms_flag.flag_sent)?'1':'0',sms_record.date.mon,sms_record.date.day,temp_entry.name);
				 }
		     }
			 else
			 {
		         if(TABLEINFO_SMS_INBOX  == p_record_wnd->table_idx)
		         {
	                 sprintf(rec_buf, "%c %.2d-%.2d %s", (1==sms_record.sms_flag.flag_read)?'1':'0',sms_record.date.mon,sms_record.date.day,sms_record.str_num);
		         }
				 else
				 {
	                 sprintf(rec_buf, "%c %02d-%02d %s", (1==sms_record.sms_flag.flag_sent)?'1':'0',sms_record.date.mon,sms_record.date.day, strlen( sms_record.str_num)>0?sms_record.str_num:"<空号码>");
				 }
			 }

			 nret=1;
		}
	}





	if(st_pos>0 &&  rec_pos>st_pos && rec_pos <et_pos)
	{
        if(rec_pos-st_pos<=3)
        {
			 p_record_wnd->pre_rec[rec_pos-st_pos].pos= rec_pos;
		     sprintf(p_record_wnd->pre_rec[rec_pos-st_pos].rec_string,"%s", rec_buf);
		}
	}
	else
	{

	    if(0 ==pre_cnt || rec_pos < st_pos)
	    {
	        if(pre_cnt>0)
	        {
	             for(i=3;i>0;i--)
	             {
	                 memcpy(&(p_record_wnd->pre_rec[i]), &(p_record_wnd->pre_rec[i-1]),sizeof(T_PRE_RECORD));
				 }
			}

	        p_record_wnd->pre_rec[0].pos= rec_pos;
			sprintf(p_record_wnd->pre_rec[0].rec_string,"%s", rec_buf);
		}
		else if(rec_pos > st_pos)
		{
	         if(pre_cnt==4)
	         {
	             for(i=1;i<4;i++)
	             {
	                 memcpy(&(p_record_wnd->pre_rec[i-1]), &(p_record_wnd->pre_rec[i]),sizeof(T_PRE_RECORD));
				 }
				 p_record_wnd->pre_rec[3].pos= rec_pos;
			     sprintf(p_record_wnd->pre_rec[3].rec_string,"%s", rec_buf);
			 }
			 else if(pre_cnt<4)
			 {
				 p_record_wnd->pre_rec[pre_cnt].pos= rec_pos;
			     sprintf(p_record_wnd->pre_rec[pre_cnt].rec_string,"%s", rec_buf);
			 }
		}
	}
    return nret;
}


void wnd_roll_selected_record(RECORDWND* p_record_wnd)
{
   static unsigned char direction=1;
   unsigned char length   =0;
   unsigned short * pstring =NULL;
   unsigned char * pdata=0;
   unsigned char i,x,w;
   unsigned char rec_state='0';

   
   if(p_record_wnd->line_cnt>0)
   {
	  char * rec_fast_buf= p_record_wnd->fast_buf;
	  if(DCS_MODE_DEFA == get_send_dcs(rec_fast_buf) 	  )
	  {

	        rec_state= rec_fast_buf[0];
            
			pstring = a2w(NULL,&rec_fast_buf[1]);

            length = wstrlen(pstring);

            w = get_wstr_dots_width_ex(pstring, length);
			
			if(w> p_record_wnd->context_width)
			{
			    x=p_record_wnd->selected_left;

				appsys.flag_color_reverse=1;
				
				lcd_goto_xy(x, p_record_wnd->selected_top);
				lcd_clear_line(p_record_wnd->selected_right);
			
				for(i=p_record_wnd->roll_pos;i<length ;i++)
                {
                    pdata=(unsigned char *)get_char_font_data(pstring[i]);
					if(pdata !=0)
					{
					    if(x+pdata[0] <p_record_wnd->selected_right )
					    {
							lcd_goto_xy(x,p_record_wnd->selected_top);
                            lcd_put_wchar( pstring[i]);
							x+=pdata[0];
						}
						else
						{
						    break;
						}
					}
				}
				
				//lcd_goto_xy(x, p_record_wnd->selected_top);
				//lcd_clear_line(p_record_wnd->selected_right);

				appsys.flag_color_reverse=0;

				if( (direction & 0x7F)>0 )
				{
                      direction--;
				}
				else
				{
					if( (direction & 0x80) ==0 )
					{
					    if(i< length)
					    {
	                        p_record_wnd->roll_pos++;
						}
	                    else
	                    {
	                        direction |=0x80;
							direction |=0x04;
						}
					}
					else
					{
					    if(p_record_wnd->roll_pos>0)
					    {
	                        p_record_wnd->roll_pos--;
					    }
						else
						{
	                        direction &= ~(0x80);
							direction |=0x04;
						}
					}
				}
			}
	   }
   } 
}



void wnd_record_move(RECORDWND * p_record_wnd,unsigned char key_code)
{
    unsigned char i,x,w;
	i=x=w=0;
	
    switch(key_code)
	{
	case TFKEY_UP:
	case TFKEY_LEFT:
        if(p_record_wnd->cur_line ==0 )
		{
			 if(p_record_wnd->line_start>0)
			 {
				  p_record_wnd->line_start--;
			 }
			 else
			 {
			     if(p_record_wnd->line_cnt>p_record_wnd->body_lines)
			     {
                      p_record_wnd->cur_line=p_record_wnd->body_lines-1;
					  p_record_wnd->line_start=p_record_wnd->line_cnt-p_record_wnd->body_lines;
				 }
				 else
				 {
                      p_record_wnd->cur_line=p_record_wnd->line_cnt-1;
					  p_record_wnd->line_start=0;
				 }
			 }
		}
		else
		{ 
		    p_record_wnd->cur_line--;
		}
	 	wnd_record_redraw(p_record_wnd);
		break;
	case TFKEY_DOWN:
	case TFKEY_RIGHT:
        if( (p_record_wnd->cur_line ==p_record_wnd->body_lines-1) && ( p_record_wnd->selected_pos)<p_record_wnd->line_cnt)
		{
			 if(p_record_wnd->line_start+p_record_wnd->body_lines< p_record_wnd->line_cnt)
			 {
				  p_record_wnd->line_start++; 
			 }
			 else
			 {
                  p_record_wnd->cur_line=0;
				  p_record_wnd->line_start=0;
			 }
			 wnd_record_redraw(p_record_wnd);
		}
		else 
		{ 
		    x=p_record_wnd->line_start;

			if( x + p_record_wnd->cur_line < p_record_wnd->line_cnt-1)
			{
			    p_record_wnd->cur_line++;
			}
			else
			{
                p_record_wnd->cur_line=0;
			    p_record_wnd->line_start=0;
			}
			wnd_record_redraw(p_record_wnd);
		}			
		break;
	default:
		break;
    }
}
//********************************************************************
// record list is not same to menu list , it maybe more than 256 lines
//********************************************************************
void wnd_record_redraw(RECORDWND * p_record_wnd)
{
	unsigned char x,y,h,box_width,m_line_cnt,length,selected,delta2left;
	unsigned short w,tmp,i;
	char * pstring=0;
	unsigned char * pdata=0;
	unsigned char rec_state=0;
	x=y=h=i=box_width=m_line_cnt=length=selected=delta2left=0;
	w=tmp=i=0;

    appsys.byte_font_type=p_record_wnd->record_font;

	h= get_cursor_height();
    box_width = p_record_wnd->body_right -p_record_wnd->body_left;

    for(i=p_record_wnd->line_start ;i<p_record_wnd->line_cnt;i++)
    {
        if(m_line_cnt<p_record_wnd->body_lines)
        {
            char rec_fast_buf[48];
			memset(rec_fast_buf,0,sizeof(rec_fast_buf));
			
			if(1 == get_record_string(p_record_wnd,  rec_fast_buf, (i+1) ))
			{
		        rec_state= rec_fast_buf[0];
	            pstring = &rec_fast_buf[1];				

			    //if(1 ==appsys.flag_genie_trace)DebugPrintf("show record[%d]-->%s\r\n",i, pstring);

				y=p_record_wnd->body_top+(h+1)*m_line_cnt;
				m_line_cnt++;
				

                //if has some picture ahead,then this value >0

				delta2left=0;
				if(TABLEINFO_SMS_INBOX ==p_record_wnd->table_idx )
				{
				    delta2left =11;
                    lcd_goto_xy(p_record_wnd->body_left, y);
					show_bitmap(('0' == rec_state)?BMP_SMSUNREADED_BIN:BMP_SMSREADED_BIN);
				}
				else if(TABLEINFO_SMS_INBOX ==p_record_wnd->table_idx)
				{
				    delta2left =11;
                    lcd_goto_xy(p_record_wnd->body_left, y);
					show_bitmap(('0' == rec_state)?BMP_SMSUNREADED_BIN:BMP_SMSREADED_BIN);
				}
				else if(TABLEINFO_MISSED_CALL==p_record_wnd->table_idx)
				{
				    delta2left =13;
                    lcd_goto_xy(p_record_wnd->body_left, y);
					show_bitmap(('0' == rec_state)?BMP_CALLIN_BIN:BMP_MISSEDCALL_BIN);
				}
				else if(TABLEINFO_CALL_IN==p_record_wnd->table_idx)
				{
				    delta2left =13;
                    lcd_goto_xy(p_record_wnd->body_left, y);
					show_bitmap(('0' == rec_state)?BMP_CALLIN_BIN:BMP_CALLFAILED_BIN);
				}	
				else if(TABLEINFO_CALL_OUT==p_record_wnd->table_idx)
				{
				    delta2left =13;
                    lcd_goto_xy(p_record_wnd->body_left, y);
					show_bitmap(('0' == rec_state)?BMP_CALLOUT_BIN:BMP_CALLFAILED_BIN);
				}					
			

				x=p_record_wnd->body_left+delta2left;

				p_record_wnd->context_width=box_width-delta2left-1;
					
				length=strlen(pstring);

                //get if this line is high light;
                w   = p_record_wnd->line_start;
				tmp = i;

				selected =( (tmp-w) == p_record_wnd->cur_line)? 1:0;

                appsys.flag_color_reverse= selected;
				if(1 == selected)
				{
	                appsys.fst_dial.p_record_wnd= p_record_wnd;
					appsys.fst_dial.rec_pos = (i+1);
				}

				lcd_goto_xy(x, y);
				lcd_clear_line_ex((h+1), p_record_wnd->body_right-1);

				lcd_show_to_colum( pstring, x, y, p_record_wnd->body_right-1);

			   /*
				for(tmp=0;tmp<length;tmp++)
				{
                    pdata=get_char_font_data(pstring[tmp]);
					if(pdata !=0)
					{
					     w=pdata[0];
                         if(x+w< p_record_wnd->body_right)
                         {
                             lcd_goto_xy(x, y);
							 lcd_put_char(pstring[tmp]);
							 x+=w;
						 }
						 else 
						 {
                             break;
						 }
					}
				}
				*/

				appsys.flag_color_reverse=0;


				if(selected>0)
				{
                    p_record_wnd->selected_pos=i;
					p_record_wnd->selected_left=p_record_wnd->body_left+delta2left;
					p_record_wnd->selected_top=p_record_wnd->body_top+(h+1)*(m_line_cnt-1);
					p_record_wnd->selected_right=p_record_wnd->body_right-1;
					p_record_wnd->selected_bottom=p_record_wnd->selected_top+h;
					p_record_wnd->roll_pos=0;
					strncpy(p_record_wnd->fast_buf, rec_fast_buf,sizeof(p_record_wnd->fast_buf)-1);
				}
	    	}
			
		}
    }
	//clear remained blank space
	while(i-p_record_wnd->line_start<p_record_wnd->body_lines)
	{
	    y=p_record_wnd->body_top+(i-p_record_wnd->line_start)*(h+1);
        lcd_goto_xy( p_record_wnd->body_left ,y );
		lcd_clear_line(p_record_wnd->body_right);
		i++;	
	}
    if( (p_record_wnd->style & RECORD_STYLE_VERTICAL_BAR)>0 )
    {
		VERTICALBAR bar;
		bar.left  =p_record_wnd->body_right+1;
		bar.top   =p_record_wnd->frame_top;
		bar.right =p_record_wnd->frame_right;
		bar.bottom=p_record_wnd->frame_bottom;
		bar.line_cnt =p_record_wnd->line_cnt;
		bar.body_lines=p_record_wnd->body_lines;

		bar.cur_line=p_record_wnd->line_start+p_record_wnd->cur_line;
	    show_vertical_bar(&bar);
    }

    lcd_update_screen_area(p_record_wnd->body_left,p_record_wnd->body_top,p_record_wnd->frame_right,p_record_wnd->body_bottom);
}

void wnd_show_record_caption(RECORDWND * p_record_wnd)
{
    char tmp_cap[32];
	signed char tb_idx  = p_record_wnd->table_idx;
	memset(tmp_cap,0,sizeof(tmp_cap));
	sprintf(tmp_cap,"%s(%d/%d)", p_record_wnd->pcaption,  app_get_record_used( tb_idx),   g_table_info[tb_idx].tb_info.rec_max_cnt);
	show_caption(tmp_cap);
}


void wnd_record_init(RECORDWND * p_record_wnd)
{
    unsigned char h;
    lcd_clear_screen();
    appsys.byte_font_type=p_record_wnd->record_font;
	h=get_cursor_height();
	if(p_record_wnd->style & RECORD_STYLE_SHOW_FRAME)
	{
		lcd_put_rect(p_record_wnd->frame_left,p_record_wnd->frame_top  ,p_record_wnd->frame_right ,p_record_wnd->frame_bottom ); 
		
		p_record_wnd->body_left=p_record_wnd->frame_left+2;
		p_record_wnd->body_top=p_record_wnd->frame_top+2;
		if( (p_record_wnd->style & RECORD_STYLE_VERTICAL_BAR)>0)
		{
		   p_record_wnd->body_right=p_record_wnd->frame_right-5;
		}
		else
		{
           p_record_wnd->body_right=p_record_wnd->frame_right-1;
		}
		p_record_wnd->body_bottom=p_record_wnd->frame_bottom-1;

	}
	else
	{
		p_record_wnd->body_left=p_record_wnd->frame_left;
		p_record_wnd->body_top=p_record_wnd->frame_top;
		p_record_wnd->body_right=p_record_wnd->frame_right;
		p_record_wnd->body_bottom=p_record_wnd->frame_bottom;
	}

    p_record_wnd->body_lines=  (p_record_wnd->body_bottom-p_record_wnd->body_top+1)/(h+1);

    set_records_list_soft_key(p_record_wnd);

    wnd_show_record_caption(p_record_wnd);
	
	wnd_record_redraw(p_record_wnd);

}


unsigned char wnd_record_show( RECORDWND * p_record_wnd)
{
    FlSignalBuffer EventPara;
	char input_code=0;
	unsigned char update_select_cnt=0;

	appsys.byte_need_redraw=0xFF;


    while(1)
	{
	    if(appsys.byte_need_redraw>0)
		{

           if(0xFF == appsys.byte_need_redraw)
           {
               wnd_load_table_index(p_record_wnd);
		   }
		
		   appsys.byte_need_redraw=0;

           wnd_record_init( p_record_wnd);

           if(appsys.byte_fast_key_times>0)appsys.byte_fast_key_times--;
		}
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    
					    if(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val)
						{
                            return 0;
						}
					  	else if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)  
							||  (TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val)  
							|| TFKEY_REDIAL==EventPara.sig_p.key_evt.key_val 
							|| TFKEY_SEND==EventPara.sig_p.key_evt.key_val
							|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val)
							)
						{
						    if( (p_record_wnd ->style & RECORD_STYLE_RET_SELECT)>0 )
						    {
                                  if(TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)
                                  {
                                       return 1;// select current record
								  }
							}
							else if(p_record_wnd->line_cnt>0)
							{
                                 return run_records_list_softkey_action( p_record_wnd, EventPara.sig_p.key_evt.key_val);
							}
							else
							{
							    if(TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)
								{
	                                 return 0;
								}
								else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val|| (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
								{
								    if(TABLEINFO_PHONEBOOK== p_record_wnd->table_idx)
								    {
						                unsigned short rec_pos=new_book_entry(NULL);

										if(rec_pos>0)
										{
						                   p_record_wnd->line_start=rec_pos-1;
										   p_record_wnd->cur_line=0;
										}
										appsys.byte_need_redraw=0xFF;
									}
									else if(TABLEINFO_SMS_INBOX== p_record_wnd->table_idx)
									{
                                        app_show_sms_from_simcard();
									}
								}
							}
						}								
						else if(
								(TFKEY_UP==EventPara.sig_p.key_evt.key_val)
								||(TFKEY_LEFT==EventPara.sig_p.key_evt.key_val)
								||(TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)
								||(TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val)
							    )
						{
                            wnd_record_move(p_record_wnd,EventPara.sig_p.key_evt.key_val);
						}
						else
						{
						    //process other keys
						}
					}
					
				}
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
				    if(++update_select_cnt>2)
				    {   
				        update_select_cnt=0;
                        wnd_roll_selected_record(p_record_wnd);
				    }
				    if(appsys.byte_need_exit > 0)
	                {
                        return 0;
					}
				}
			}
		}
	}
	return 0;
}



