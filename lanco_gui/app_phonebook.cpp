
#include "inc/app_def.h"


char * get_cmp_numbers(char * pnumber)
{
	 if( ('+' == pnumber[0])
       ||(('0' == pnumber[0] && '0' != pnumber[1] ))
	 )
	 {
           return (char *)&pnumber[1];
	 }						 
     return  (char *)pnumber;
}

unsigned char entry_numbers_compare(char * pnumber1,char * pnumber2 )
{
    char *pt_1, *pt_2;
    char * num_1 = get_cmp_numbers(pnumber1); 
	char * num_2 = get_cmp_numbers(pnumber2);

    //if(1 == appsys.flag_genie_trace)DebugPrintf("orc num1=%s num2=%s, new num1=%s  num2=%s\r\n",pnumber1,pnumber2, num_1,num_2);


    if ( (0==*num_1) || (0==*num_2) )
    {
        return 0;
    }

    pt_1 = num_1 + strlen(num_1)-1;
    pt_2 = num_2 + strlen(num_2)-1;

    while ( pt_1>=num_1 && pt_2>=num_2 )
    {
        if ( *pt_1 != *pt_2 )
        {
            return 0;
        }

        --pt_1;
        --pt_2;
    }
    return 1;
}

char asc_2_lower( int ch )
{
    if ( ch >= 'A' && ch <= 'Z' )
    {
        ch += ('a' - 'A');
    }
    return ch;
}
char asc_2_upper( int ch )
{
    if ( ch >= 'a' && ch <= 'z' )
    {
        ch -= ('a' - 'A');
    }
    return ch;
}

void str_replace( char * pchar, char old_c, char new_c )
{
    unsigned char tmp_len= strlen(pchar);
	unsigned char i=0;
	for(i=0;i< tmp_len;i++)
	{
	    if(old_c == pchar[i])
	    {
            pchar[i]= new_c;
		}
	}
}


unsigned char entry_name_compare( char * name1,char * name2 )
{
    char *p_1, *p_2;
    unsigned int i=0;
    char c_1, c_2;


    p_1 = name1;
    p_2 = name2;

    while ( i < BOOK_ENTRY_NAME_FIELD_SIZE )
    {
        c_1 = asc_2_lower(*p_1++);
        c_2 = asc_2_lower(*p_2++);

        if ( c_1 < c_2 )
        {
            return 0xFF;
        }
        else if ( c_1 > c_2 )
        {
            return 1;
        }
        else
        {
            ;
		}

        ++i;
    }

    return 0;
}

unsigned char find_book_entry_by_numbers( char *numbers,T_BOOK_ENTRY * pdest_entry )
{
    T_BOOK_ENTRY * ptmp_entry;
	unsigned char nret=0;
    unsigned short entry_count;
	char tmp_number[BOOK_ENTRY_NUM_FIELD_SIZE+1];

	
	f_memset(tmp_number,0,sizeof(tmp_number));

	if(1 ==appsys.flag_genie_trace)DebugPrintf("Load book infomation in find_book_entry_by_numbers ++++\r\n",NULL);
	app_load_table_info(TABLEINFO_PHONEBOOK, &appsys.book_info);


    memset(pdest_entry, 0, sizeof(T_BOOK_ENTRY));
    entry_count = appsys.book_info.rec_cnt;

    while ( entry_count )
    {
        ptmp_entry = (T_BOOK_ENTRY *)(&appsys.book_info.precords[sizeof(T_BOOK_ENTRY) * (entry_count-1) ]);

		if( (strlen(ptmp_entry->num) == strlen(numbers)))
		{
		    if(0 ==memcmp(numbers, ptmp_entry->num, strlen(ptmp_entry->num)))
		    {
			    memcpy(pdest_entry, ptmp_entry, sizeof(T_BOOK_ENTRY));
	            nret = 1;
				break;
		    }
		}
		else 
		{
		    if(0 ==memcmp(numbers, ptmp_entry->num, strlen(numbers)))
		    {
		        if(f_strlen(tmp_number) < f_strlen(numbers))
			    {
			        f_strcpy(tmp_number,numbers);
				    memcpy(pdest_entry, ptmp_entry, sizeof(T_BOOK_ENTRY));
		            nret = 1;
		        }
		    }

		    if(strlen(ptmp_entry->num) >=4)
		    {
		        if (1 == entry_numbers_compare(numbers, ptmp_entry->num) )
		        {
			        if(f_strlen(tmp_number) < f_strlen(ptmp_entry->num))
			        {
			            f_strcpy(tmp_number,ptmp_entry->num);
		                memcpy(pdest_entry, ptmp_entry, sizeof(T_BOOK_ENTRY));
						if(1 == appsys.flag_genie_trace)DebugPrintf("Afind the number=%s name=%s\r\n",pdest_entry->num, pdest_entry->name);
						nret = 1;
			        }
		        }
		    }
		}
        --entry_count;
    }

    if ( strlen (pdest_entry->name) >0 ) // found,  but not equal completely
    {
        if(1 == appsys.flag_genie_trace)DebugPrintf("Bfind the number=%s name=%s\r\n",pdest_entry->num, pdest_entry->name);
        nret = 1;
    }
	
    return nret;
}












unsigned char find_book_entry_by_speed_location( char * str_location,T_BOOK_ENTRY * pdest_entry )
{
    T_BOOK_ENTRY book_entry;
    unsigned char entry_count;

    entry_count= (unsigned char )app_get_record_used(TABLEINFO_PHONEBOOK);
	while(entry_count)
	{
        memset(&book_entry, 0, sizeof(T_BOOK_ENTRY));
        if(1 == app_record_read(TABLEINFO_PHONEBOOK, entry_count, (unsigned char *) (&book_entry)))
        {
            if(0 == strcmp(str_location,book_entry.location))
            {
                memcpy(pdest_entry, &book_entry, sizeof(T_BOOK_ENTRY));
                return entry_count;
			}
			--entry_count;
		}
		else
		{
            break;
		}
	}
    return 0;
}


unsigned char edit_book_entry( T_BOOK_ENTRY * pbook_entry )
{
    FlSignalBuffer EventPara;
	unsigned char cursor=1, ret=0;
    T_BOOK_ENTRY bkp_entry;


	memcpy(&bkp_entry,pbook_entry,sizeof(bkp_entry));

	appsys.byte_need_redraw=0xFF;

    while ( 1 )
    {
        if ( appsys.byte_need_redraw>0 )
        {
            if(0xFF == appsys.byte_need_redraw)
            {
	            lcd_clear_screen();
				show_caption( get_multi_string((char * *)text_entry_editor));
				lcd_goto_xy(0, 12);	lcd_draw_horizontal(SCREEN_WIDTH-1);
            }

            // Display book entries content
            appsys.byte_font_type= FONT_T_X_10;
			
            if ( 1 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 1);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 1);	
			
            lcd_put_string("1.");
            lcd_put_string(get_multi_string((char * *)text_name));
            lcd_put_string(":");

			lcd_put_string(pbook_entry->name);
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

			
            if ( 2 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 2);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 2);	
			
            lcd_put_string("2.");
            lcd_put_string(get_multi_string((char * *)text_number));
            lcd_put_string(":");

            lcd_put_string(pbook_entry->num);
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

            if ( 3 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 3);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 3);	
            lcd_put_string("3.");
            lcd_put_string(get_multi_string((char * *)text_fastkey));
            lcd_put_string(":");

            lcd_put_string(pbook_entry->location);
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;


			ret=strlen(pbook_entry->name)>0?1:0;

			show_soft_key_prompt( get_multi_string((char * *)text_edit),   get_multi_string((char * *)text_back));

            appsys.byte_need_redraw=0;
        }
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp)
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{

                         switch(EventPara.sig_p.key_evt.key_val)
                         {
                              case TFKEY_SOFT_RIGHT:
							  {
							  	   if(1 == ret &&  0 != memcmp(&bkp_entry,pbook_entry,sizeof(bkp_entry) ))
							  	   {
                                       if(1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_ask_save_or_not)) )
                                       {
                                           return 1;
									   }
									   else
									   {
                                           return 0;
									   }
							  	   }
                                   return 0;
							  }
							  break;
							  case TFKEY_EXIT:
                                   return 0;
							  break;
							  case TFKEY_1:
							  case TFKEY_2:
							  case TFKEY_3:
							  {
								  if(TFKEY_1 == EventPara.sig_p.key_evt.key_val)
								  {
	                                  cursor =1;
								  }
								  else if(TFKEY_2 == EventPara.sig_p.key_evt.key_val)
								  {
	                                  cursor =2;
								  }
								  else if(TFKEY_3 == EventPara.sig_p.key_evt.key_val)
								  {
	                                  cursor =3;
								  }
								  else
								  {
	                                  break;
								  }
							  }
							  case TFKEY_SOFT_LEFT:
		                      {
							       appsys.byte_need_redraw=0xFF;
		                           if(1  == cursor)
		                           {
		                           	   char tmp_buf[BOOK_ENTRY_NAME_FIELD_SIZE+1];
								       memset(tmp_buf,0, BOOK_ENTRY_NAME_FIELD_SIZE+1);
									   strcpy(tmp_buf, pbook_entry->name);
		                               if(1 ==wnd_input_text(tmp_buf, (get_combined_string(get_multi_string((char * *)text_edit), " ", get_multi_string((char * *)text_name))), BOOK_ENTRY_NAME_FIELD_SIZE,(LANGUAGE_CHINESE == sysprop->byte_language_type)?IN_PY5D:IN_ABC,EDIT_STYLE_NOEXSTYLE))
		                               {
		                                   strncpy(pbook_entry->name,tmp_buf, BOOK_ENTRY_NAME_FIELD_SIZE);
									   }
								   }
								   else if(2 == cursor)
								   {
								       char tmp_buf[BOOK_ENTRY_NUM_FIELD_SIZE+1];
								       memset(tmp_buf,0, BOOK_ENTRY_NUM_FIELD_SIZE+1);
									   strcpy(tmp_buf, pbook_entry->num);
		                               if(1 ==wnd_input_text(tmp_buf, (get_combined_string(get_multi_string((char * *)text_edit), " ", get_multi_string((char * *)text_no))), BOOK_ENTRY_NUM_FIELD_SIZE,IN_FIXED,EDIT_STYLE_DIGITALONLY))
		                               {
		                                   strncpy(pbook_entry->num,tmp_buf, BOOK_ENTRY_NUM_FIELD_SIZE);
									   }
								   }
								   else if(3 == cursor)
								   {
								       char tmp_buf[1+1];
								       memset(tmp_buf,0, 1+1);
									   tmp_buf[0]=pbook_entry->location[0];
__RE_INPUT:
									   
								       wnd_input_text(tmp_buf, get_combined_string(get_multi_string((char * *)text_fastkey),"1~9",NULL), 1,IN_FIXED,EDIT_STYLE_DIGITALONLY);
								       {
								           if( (tmp_buf[0]>='1' && tmp_buf[0] <='9') )
								           {
									           T_BOOK_ENTRY temp_entry;
			                                   unsigned char rec_pos=0;
											   memset(&temp_entry, 0,sizeof(T_BOOK_ENTRY));
											   rec_pos= find_book_entry_by_speed_location( tmp_buf, &temp_entry);
											   if(rec_pos>0)
											   {
			                                        if(1 ==choose_yes_no( get_multi_string((char * *)text_confirm) , get_multi_string((char * *) text_overwrite_original_assignment)))
			                                        {
			                                            strncpy(pbook_entry->location, tmp_buf,2);
														memset(temp_entry.location,0,2);
														app_record_edit(TABLEINFO_PHONEBOOK, rec_pos, (unsigned char *)(&temp_entry));
													}
											   }
											   else
											   {
			                                        strncpy(pbook_entry->location, tmp_buf,2);
											   } 
								           }
										   else if( 0 == strlen(tmp_buf))
										   {
                                               memset(pbook_entry->location,0,sizeof(pbook_entry->location));
										   }
										   else
										   {
                                                tmp_buf[0]=pbook_entry->location[0];
												tmp_buf[1]=0;

											 	msg_warn("(1 ~ 9)");
												delay_ms(1000);
												goto __RE_INPUT;
										   }
									   }
								   }
							  }
							  break;
							  default:
							  break;
						}
					    
                        if( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val))
						{
                           if(cursor>1)
						   {
						       cursor--;	
                           }
						   else
						   {
                               cursor=3;
						   }
						   appsys.byte_need_redraw=1;
						}
						else if( (TFKEY_DOWN== EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
                           if(cursor<3)
						   {
						       cursor++;	
                           }
						   else
						   {
                               cursor=1;
						   }
						   appsys.byte_need_redraw=1;
						}						
					}
		        }
				else if(EVENT_TIMER ==EventPara.eventTyp)
				{
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


unsigned short new_book_entry( char * numbers )
{
    T_BOOK_ENTRY book_entry;

    memset(&book_entry, 0, sizeof(T_BOOK_ENTRY));

	if(app_get_record_used(TABLEINFO_PHONEBOOK) < g_table_info[TABLEINFO_PHONEBOOK].tb_info.rec_max_cnt)
	{
		if(numbers != 0)
		{
		    strcpy(book_entry.num,  numbers);
		}

	    if ( edit_book_entry(&book_entry) )
	    {
	        return save_book_entry(&book_entry);
	    }
	}
	else
	{
        msg_warn(get_multi_string((char * *)text_phonebook_is_full));
		delay_ms(1000);
	}
	return 0;
}

unsigned short find_book_entry_insert_location( T_BOOK_ENTRY *pbook_entry )
{
    T_BOOK_ENTRY exist_entry;
    unsigned short i=1;
	unsigned short entry_count, result;

    entry_count= app_get_record_used(TABLEINFO_PHONEBOOK);
    for ( i=1; i<=entry_count; ++i )
	{
        memset(&exist_entry, 0, sizeof(T_BOOK_ENTRY));
        if(1 == app_record_read(TABLEINFO_PHONEBOOK, i, (unsigned char *) (&exist_entry)))
        {
	        result = entry_name_compare(pbook_entry->name, exist_entry.name);
	        if ( 1!=result && 0!=result )
	        {
	            break;
	        }        
		}
		else 
		{
            break;
		}
	}
    return i;
}

unsigned short save_book_entry( T_BOOK_ENTRY *pbook_entry )
{
    unsigned short rec_pos= find_book_entry_insert_location(pbook_entry);
	if(1 == app_record_insert(TABLEINFO_PHONEBOOK, rec_pos, (unsigned char *) pbook_entry))
	{
        return rec_pos;
	}
	return 0;
}
unsigned char delete_book_entry( unsigned short rec_no )
{
    return app_record_delete(TABLEINFO_PHONEBOOK, rec_no);
}

unsigned short modify_book_entry( unsigned short rec_no )
{
    T_BOOK_ENTRY book_entry, entry_backup;

    memset(&book_entry, 0, sizeof(T_BOOK_ENTRY));
	if( 1 == app_record_read(TABLEINFO_PHONEBOOK,rec_no, (unsigned char *)&book_entry))
	{
	    memcpy(&entry_backup, &book_entry, sizeof(T_BOOK_ENTRY));
		
        if(1 == edit_book_entry(&book_entry))
        {
            if(0 != memcmp(&book_entry, &entry_backup, sizeof(T_BOOK_ENTRY)))
            {
	            if(1 == delete_book_entry(rec_no))
	            {
	                return save_book_entry(&book_entry);
			    }
			}
			else
			{
                return 1;
			}
		}
	}
	return 0;
}



int get_simcard_book_count()
{

	return 0;
}

void app_convert_ucs2_str_to_pshort(char * pucs2_str, unsigned short * pshort)
{
    int length = strlen(pucs2_str);
	int i=0;

	for(i=0;i<length /4;i++)
	{
	    pshort[i]  = get_hex(pucs2_str[i*4 +0]); pshort[i]<<=4;
		pshort[i] += get_hex(pucs2_str[i*4 +1]); pshort[i]<<=4;
		pshort[i] += get_hex(pucs2_str[i*4 +2]); pshort[i]<<=4;
		pshort[i] += get_hex(pucs2_str[i*4 +3]); 
	} 
}

void app_convert_pshort_str_to_ucs2(unsigned short * pshort, char * pucs2_str)
{
    int length = wstrlen(pshort);
	int i=0;

	for(i=0;i<length;i++)
	{
        sprintf(&pucs2_str[strlen(pucs2_str)], "%.4X", pshort[i]);
	} 
}



unsigned char get_simcard_book_entry(unsigned char book_idx, T_BOOK_ENTRY * pbook_entry)
{

	return 0;
}








