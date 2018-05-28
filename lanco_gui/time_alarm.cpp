
#include "inc/app_def.h"


const unsigned char day_of_month[] = {29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned char YearIsRunNian( int    year )
{
    return (0==year%4&&0!=year%100) || (0==year%400);
}
unsigned short  GetGongYearDays( int year )
{
    int  days=YearIsRunNian(year)?366:365;
    return days;
}
unsigned char GongmonIsLarge( int mon )
{
    unsigned char ret=FALSE;
    if ( mon<8 )
    {
        if ( mon%2 )
        {
            ret=TRUE;
        }
    }
    else
    {
        if ( !(mon%2) )
        {
            ret=TRUE;
        }
    }
    return ret;
}
unsigned char GetGongmonDays( int year,char mon )
{
    unsigned char  ret=30;
    if ( mon==2 )
    {
        ret=YearIsRunNian(year)?29:28;
    }
    else
    {
        ret=GongmonIsLarge(mon)?31:30;
    }
    return ret;
}

unsigned char get_week_from_date(DATE * pdate)
{
    int left=0,i;
    for ( i=YEAR_MIN_VALUE; i<pdate->year; i++ )//1950-01-01 is saturday=6
    {
        left+=YearIsRunNian(i)?2:1;
        left%=7;
    }
    for ( i=1; i<pdate->mon; i++ )
    {
        left+=GetGongmonDays(pdate->year,i);
        left%=7;
    }
    left+=(pdate->day+6);
    left%=7;
    return (char)left;

}


unsigned char GetWeekInfo()
{
	return get_week_from_date(&appsys.struct_date);
}




unsigned char time24_to_time12(TIME * time24, TIME * time12)
{
     unsigned char nret=0;
     if(time24->hour>=12)
     {
          if(time24->hour>=13)
          {
              time12->hour=time24->hour -12;
		  }
		  else
		  {
              time12->hour=12;
		  }
		  nret = 1;
	 }
	 else
	 {
          if(time24->hour<1)
          {
              time12->hour=12;
		  }
		  else
		  {
              time12->hour=time24->hour;
		  }
          nret=0;
	 }

	 time12->min=time24->min;
	 time12->sec=time24->sec;
	 return nret;	 
}

void time12_to_time24(TIME * time12, TIME * time24, unsigned char flag_am_pm)
{
     if(0 == flag_am_pm)
     {
         if(12 == time12->hour)
         {
             time24->hour=0;
		 }
		 else
		 {
             time24->hour=time12->hour;
		 }
	 }
	 else
	 {
         if(12 == time12->hour)
         {
             time24->hour=12;
		 }
		 else
		 {
             time24->hour=time12->hour+12;
		 }
	 }
	 time24->min=time12->min;
	 time24->sec=time12->sec;	 
}



unsigned char app_set_time( TIME * pret_time)
{
    FlSignalBuffer EventPara;
		
    TIME time24,time12;
	TIME * pcur_time;
	char temp_str[4];
    unsigned char i, item_index=0;
	unsigned char temp, flag_am_pm=0;
	unsigned char max_items=0;


    appsys.byte_need_redraw=0xFF;
    memcpy(&time24, pret_time, sizeof(TIME));

    while ( 1 )
    {
        if ( appsys.byte_need_redraw>0)
        {
            if(0xFF == appsys.byte_need_redraw)
            {
			    lcd_clear_screen();
			    show_caption(get_multi_string((char * *)text_enter_time));
			    lcd_goto_xy(0, 12);lcd_draw_horizontal(SCREEN_WIDTH-1);	
				
			    show_soft_key_prompt(get_multi_string((char * *) text_ok), get_multi_string((char * *)text_back));
			}
        
            appsys.byte_need_redraw=0;

			flag_am_pm= time24_to_time12(&time24, &time12);

 			if(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)
			{

			   lcd_put_rect(25, 20, 95, 40);
			   lcd_goto_x_line(32, 2);
			   lcd_clear_line(94);
               lcd_goto_x_line(32, 2);
			   pcur_time=&time12;

			}
			else
			{

			   lcd_put_rect(35, 20, 90, 40);
			   lcd_goto_x_line(40, 2);
			   lcd_clear_line(84);
               lcd_goto_x_line(40, 2);	
			   pcur_time=&time24;
			}

            appsys.byte_font_type= FONT_T_X_10;


            if ( 0 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->hour/10)));
            appsys.flag_color_reverse=0;
			
            if ( 1 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->hour%10)));
            appsys.flag_color_reverse=0;

            lcd_add_x(lcd_put_char(':'));

            if ( 2 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->min/10)));
            appsys.flag_color_reverse=0;
			
            if ( 3 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->min%10)));
            appsys.flag_color_reverse=0;

            lcd_add_x(lcd_put_char(':'));

            if ( 4 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->sec/10)));
            appsys.flag_color_reverse=0;
			
            if ( 5 == item_index )
            {
                appsys.flag_color_reverse=1;
            }
            lcd_add_x(lcd_put_char('0'+(pcur_time->sec%10)));
            appsys.flag_color_reverse=0;
            lcd_add_x(lcd_put_char(' '));
			



            if(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)
            {
				if( 6 == item_index)
				{
	                appsys.flag_color_reverse=1;
				}
				lcd_put_string((0== flag_am_pm)?"am":"pm" );
				appsys.flag_color_reverse=0;
			}
        }
		else
		{
            if(1 == app_get_message( &EventPara) )
			{
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    appsys.byte_need_redraw=1;
					
					    if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)|| (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
						   return 1;
					    }
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
                            memcpy(pret_time, &time24, sizeof(TIME));
							return 1;
						}
						else if( (TFKEY_UP ==EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val))
						{
                            if ( item_index>0 )
			                {
			                    --item_index;
			                }
						}
						else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
						    temp=(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)?6:5;
                            if ( item_index<temp )
			                {
			                    ++item_index;
			                }
if(1)//C2800 NO HAVE up and down
{
                            if ( 6 == item_index )
                            {
                                 flag_am_pm = (0 == flag_am_pm)? 1:0;
                                 time12_to_time24(&time12, &time24, flag_am_pm);
						    }
}							
						}						
						else
						{
                            i=get_key_value(EventPara.sig_p.key_evt.key_val);
                            if(1 == is_digit(i))
                            {
                                if(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)
                                {
				                    if ( 0 == item_index )
					                {
					                    temp = (i-'0')*10+(pcur_time->hour%10);
					                    if ( 0<temp && temp<=12 )
					                    {
                                            pcur_time->hour = temp;
											++item_index;
											time12_to_time24(&time12, &time24, flag_am_pm);
					                    }
										else
										{
					                        pcur_time->hour = 1;
											time12_to_time24(&time12, &time24, flag_am_pm);
					                        ++item_index;
										}
										continue;
					                }
					                else if ( 1 == item_index )
					                {
					                    temp = (pcur_time->hour/10)*10 + (i-'0');
					                    if ( 0<temp && temp<=12 )
					                    {
                                            pcur_time->hour = temp;
					                        ++item_index;
											time12_to_time24(&time12, &time24, flag_am_pm);
					                    }
										continue;
					                }

									
                                }
								else
								{
				                    if ( 0 == item_index )
					                {
					                    temp = (i-'0')*10+(pcur_time->hour%10);
					                    if ( 0<=temp && temp<=23 )
					                    {
					                        pcur_time->hour = temp;
					                        ++item_index;
					                    }
										else
										{
                                            if( (i-'0')*10 == 20)
                                            {
						                        pcur_time->hour = 20;
						                        ++item_index;
											}
										}
										continue;
					                }
					                else if ( 1 == item_index )
					                {
					                    temp = (pcur_time->hour/10)*10 + (i-'0');
					                    if ( 0<=temp && temp<=23 )
					                    {
					                         pcur_time->hour = temp;
					                        ++item_index;
					                    }
										continue;
					                }
                                }



								if ( 2 == item_index )
				                {
				                    temp = (i-'0')*10+(pcur_time->min%10);
				                    if ( 0<=temp && temp<=59 )
				                    {
				                        time24.min =time12.min =temp;
				                        ++item_index;
				                    }
				                }
				                else if ( 3 == item_index )
				                {
				                    temp = (pcur_time->min/10)*10 + (i-'0');
				                    if ( 0<=temp && temp<=59 )
				                    {
				                        time24.min =time12.min =temp;
				                        ++item_index;
				                    }
				                }
								else if ( 4 == item_index )
				                {
				                    temp = (i-'0')*10+(pcur_time->sec%10);
				                    if ( 0<=temp && temp<=59 )
				                    {
				                        time24.sec=time12.sec=temp;
				                        ++item_index;
				                    }
				                }
				                else if ( 5 == item_index )
				                {
				                    temp = (pcur_time->sec/10)*10 + (i-'0');
				                    if ( 0<=temp && temp<=59 )
				                    {
				                        time24.sec=time12.sec=temp;

										if(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)
										{
				                           ++item_index;
										}
										else
										{
                                           item_index=0;
										}
				                    }
				                }
								

							}
							else
							{
                                if( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) ||  (TFKEY_LEFT== EventPara.sig_p.key_evt.key_val) || (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
                                {
                                    if ( 6 == item_index )
                                    {
                                         flag_am_pm = (0 == flag_am_pm)? 1:0;
                                         time12_to_time24(&time12, &time24, flag_am_pm);
								    }
								}
							}
						}
					}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    return 1;
					}
				}	
            }
		}
    }
    return 1;
}

void get_alarm_time_str(TIME * p_time, char * ret_str)
{
   get_time_str_ex(p_time,ret_str,1,1);
}

void get_normal_time_str(TIME * p_time, char * ret_str)
{
   get_time_str_ex(p_time,ret_str,1,0);
}

void get_time_str_ex(TIME * p_time, char * ret_str,unsigned char bfixed_len, unsigned char is_alarm)
{
   if(CLOCK_FORMAT_12 ==sysprop->flag_clock_format)
   {
       unsigned char flag_am_pm=0;
	   TIME time12;
	   memset(&time12,0,sizeof(TIME));
	   flag_am_pm = time24_to_time12(p_time, &time12);

       if(1 ==bfixed_len)
       {
          if(1 == is_alarm)
          {
	          sprintf(ret_str,"%.2d:%.2d%s", time12.hour,time12.min, (0 ==flag_am_pm)?"a":"p");
		  }
		  else
		  {
	          sprintf(ret_str,"%.2d:%.2d:%.2d%s", time12.hour,time12.min,time12.sec,(0 ==flag_am_pm)?"am":"pm");
		  }
       }
	   else
	   {
          if(1 == is_alarm)
          {	   
	          sprintf(ret_str,"%d:%d%s", time12.hour,time12.min, (0 ==flag_am_pm)?"a":"p");
		  }
		  else
		  {
	          sprintf(ret_str,"%d:%d:%d%s", time12.hour,time12.min,time12.sec,(0 ==flag_am_pm)?"am":"pm");
		  }
	   }
   	}
   else
   {
       if(1 ==bfixed_len)
       {
           if(1 == is_alarm)
           {
                sprintf(ret_str,"%.2d:%.2d", p_time->hour,p_time->min);
		   }
		   else
		   {
                sprintf(ret_str,"%.2d:%.2d:%.2d", p_time->hour,p_time->min,p_time->sec);
		   }
       }
	   else
	   {
           if(1 == is_alarm)
           {	   
               sprintf(ret_str,"%d:%d", p_time->hour,p_time->min);
           }
		   else
		   {
               sprintf(ret_str,"%d:%d:%d", p_time->hour,p_time->min,p_time->sec);
		   }
	   }
   }
}

void get_date_str(DATE * p_date, char * ret_str)
{
    if(LANGUAGE_ENGLISH== sysprop->byte_language_type || LANGUAGE_SPANISH== sysprop->byte_language_type)
    {
        sprintf(ret_str, "%.2d/%.2d/%.4d",  p_date->mon, p_date->day, p_date->year);
	}
	else
	{
        sprintf(ret_str, "%.4d-%.2d-%.2d",  p_date->year,p_date->mon, p_date->day);
	}
}


void app_set_alarm_clock( void )
{
   FlSignalBuffer EventPara;
   T_ALARM_CLOCK alarm_clock;
   unsigned char cursor=1;



   appsys.byte_need_redraw=0xFF;   

   memcpy(&alarm_clock, &(sysprop->struct_alarm),sizeof(T_ALARM_CLOCK));
   while(1)
   {
       if(appsys.byte_need_redraw>0)
       {
			if(0xFF == appsys.byte_need_redraw)
			{
                lcd_clear_screen();
			    show_caption( get_multi_string((char * *)text_alarm_time_setting));
                lcd_goto_xy(0, 12);	lcd_draw_horizontal(SCREEN_WIDTH-1);
				show_soft_key_prompt( get_multi_string((char * *)text_edit), get_multi_string((char * *)text_finish));

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
            lcd_put_string(get_multi_string((char * *) text_time));
			lcd_put_string(":");
			
      
			if(1)
			{
               char tmp_buf[16];
               memset(tmp_buf,0,16);
			   get_alarm_time_str(&(alarm_clock.alarm_time), tmp_buf);
			   lcd_goto_x_line(38, 1);
			   lcd_put_string(tmp_buf);
			}
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

            if ( 2 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 2);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 2);	
			
            lcd_put_string("2.");
            lcd_put_string(get_multi_string((char * *) text_music));
            lcd_put_string(":");
			
			if(1)
			{
			   char tmp_buf[16];
			   memset(tmp_buf,0,16);

               sprintf(tmp_buf, "AUDIO_%d", (alarm_clock.alarm_ring +1) );
			  
			   lcd_goto_x_line(38, 2);
               lcd_put_string(tmp_buf);
    		}
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;


            if ( 3 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 3);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 3);	
			
            lcd_put_string("3.");
            lcd_put_string(get_multi_string((char * *) text_mode));
            lcd_put_string(":");
			
			lcd_goto_x_line(38, 3);

            lcd_put_string(get_multi_string_ex((char * * *)alarm_clock_set, (alarm_clock.alarm_on+1) ));

			
			lcd_clear_line(SCREEN_WIDTH-1);
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
								   if(0 != memcmp(&alarm_clock, &(sysprop->struct_alarm), sizeof(T_ALARM_CLOCK)))
								   {
								        appsys.byte_alarm_clock_hour=appsys.byte_alarm_clock_min=0xFF;
										appsys.flag_need_clock_alarm=0;
										appsys.dword_last_clock_alarm_tick =0;
		                                memcpy(&(sysprop->struct_alarm),&alarm_clock,sizeof(T_ALARM_CLOCK));
										app_save_prop();		
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
		                            if(1 == cursor)
		                            {
		                                alarm_clock.alarm_time.sec=0;
										app_set_time(&(alarm_clock.alarm_time));
										appsys.byte_need_redraw=0xFF;
									}
									else if(2 == cursor)
									{
					                    unsigned char music_id= alarm_clock.alarm_ring;
										
					                    if(1 == app_alarm_show(&music_id))
					                    {
					                        if(music_id != alarm_clock.alarm_ring)
					                        {
					                            alarm_clock.alarm_ring=music_id;
										    }
											appsys.byte_need_redraw=0xFF;
									    }
				           		    }
									else if(3 == cursor)
									{
									     unsigned char def_pos= alarm_clock.alarm_on;
									   

										 if (1 == choose_menu_option((char *** )alarm_clock_set, 3, &def_pos))
										 {
									           alarm_clock.alarm_on = def_pos;		   
										 }
										 appsys.byte_need_redraw=0xFF;
									}
							  }
							  break;
							  default:
							  break;
						}
						 
                        if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val))
						{
                            if(cursor>1)
                            {
                               cursor--;
							}
							else 
							{
                               cursor =3;
							}
						    appsys.byte_need_redraw |=REDRAW_PATITIAL;
						}
						else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
                            if(cursor<3)
                            {
                                cursor++;
							}
							else
							{
                                cursor=1;
							}
						    appsys.byte_need_redraw|=REDRAW_PATITIAL;
						}						
					}
		        }
				else if(EVENT_TIMER  == EventPara.eventTyp)
				{
	                if(appsys.byte_need_exit > 0)
	                {
                        break;
					}
				}
            }
	   }
   }
   appsys.byte_need_redraw=0xFF;
}


void app_set_date( DATE * p_date )
{
   FlSignalBuffer EventPara;
   DATE  temp_date;
   unsigned char cursor=1;
   char tmp_buf[16];
   appsys.byte_need_redraw=0xFF;

   memcpy(&temp_date, p_date,sizeof(DATE));
   while(1)
   {
       if( appsys.byte_need_redraw>0)
       {
			if(0xFF == appsys.byte_need_redraw )
			{
                lcd_clear_screen();
			    show_caption( get_multi_string((char * *)text_date));
                lcd_goto_xy(0, 12);	lcd_draw_horizontal(SCREEN_WIDTH-1);
				show_soft_key_prompt( get_multi_string((char * *)text_edit) , get_multi_string((char * *)text_back));

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
            lcd_put_string(get_multi_string((char * *)text_year));
			lcd_put_string(": ");
           
			
			memset(tmp_buf,0,16);
			sprintf(tmp_buf,"%.4d", temp_date.year);
			lcd_goto_x_line(43, 1);
			lcd_put_string(tmp_buf);

			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

			
            if ( 2 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 2);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 2);	
			
			lcd_put_string("2.");
            lcd_put_string(get_multi_string((char * *)text_month));
			lcd_put_string(": ");
			
			memset(tmp_buf,0,16);
			sprintf(tmp_buf,"%.2d", temp_date.mon);
			lcd_goto_x_line(43, 2);
			lcd_put_string(tmp_buf);
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

            if ( 3 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 3);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 3);	
			
			lcd_put_string("3.");
            lcd_put_string(get_multi_string((char * *)text_day));
			lcd_put_string(": ");

			
			memset(tmp_buf,0,16);
			sprintf(tmp_buf,"%.2d", temp_date.day);
			lcd_goto_x_line(43, 3);
			lcd_put_string(tmp_buf);
			
			lcd_clear_line(SCREEN_WIDTH-1);
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
			                       memcpy(p_date,&temp_date,sizeof(DATE));
								   return ;
							  }
							  break;
							  case TFKEY_EXIT:
							  {
                                   return;
							  }
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
								    memset(tmp_buf,0,16);
		                            if(1== cursor)
		                            {
		                                 sprintf(tmp_buf,"%.4d", temp_date.year);
		__INPUT_YEAR:					 
		                                 if (1 == wnd_input_text(tmp_buf, get_multi_string((char * *)text_year),  4, IN_FIXED, EDIT_STYLE_DIGITALONLY))	
		                                 {
		                                     unsigned short yyyy= atoi(tmp_buf);
											 if(yyyy>=2000 && yyyy<=2200)
											 {
		                                        temp_date.year= atoi(tmp_buf);
											 }
											 else
											 {
		                                         msg_warn(get_multi_string((char * *)text_error));
												 delay_ms(1000);
												 goto __INPUT_YEAR;
											 }
										 }
										 appsys.byte_need_redraw=0xFF;
								    }
									else if(2 == cursor)
									{
		                                 sprintf(tmp_buf,"%.2d", temp_date.mon);
		__INPUT_MONTH: 
		                                 if (1 == wnd_input_text(tmp_buf, get_multi_string((char * *)text_month),  2, IN_FIXED, EDIT_STYLE_DIGITALONLY))	
		                                 {
		                                     unsigned char mm=atoi(tmp_buf);
											 if(mm>=1 && mm<=12)
											 {
											     if( temp_date.day <= GetGongmonDays(temp_date.year, mm) )
											     {
		                                             temp_date.mon= mm;
											     }
												 else
												 {
			                                         msg_warn(get_multi_string((char * *)text_error));
													 delay_ms(1000);

		                                             temp_date.mon= mm; 
													 memset(tmp_buf,0,16);
													 sprintf(tmp_buf,"%.2d", GetGongmonDays(temp_date.year, mm));
													 goto __INPUT_DAY;

												 }
											 }
											 else
											 {
		                                         msg_warn(get_multi_string((char * *)text_error));
												 delay_ms(1000);
												 goto __INPUT_MONTH;
											 }
										 }
										 appsys.byte_need_redraw=0xFF;
									}
									else if(3 == cursor)
									{
									     sprintf(tmp_buf,"%.2d", temp_date.day);
		__INPUT_DAY: 							
		                                 if (1 == wnd_input_text(tmp_buf, get_multi_string((char * *)text_day),  2, IN_FIXED, EDIT_STYLE_DIGITALONLY))	
		                                 {
		                                      unsigned char dd= atoi(tmp_buf);
											  unsigned char max_month_days= GetGongmonDays(temp_date.year, temp_date.mon);
											  if( dd>=1  && dd<= max_month_days)
											  {
		                                           temp_date.day= dd;
											  }
											  else
											  {
		                                         msg_warn(get_multi_string((char * *)text_error));
												 delay_ms(2000);
												 goto __INPUT_DAY;
											  }
										 }
									  	 appsys.byte_need_redraw=0xFF;
									}
		    				  }
							  break;
							  default:
							  break;
						}

						if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val))
						{
                            if(cursor>1)
                            {
                               cursor--;
							}
							else 
							{
							   cursor =3;
							}
					        appsys.byte_need_redraw|=REDRAW_PATITIAL;

						}
						else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
                            if(cursor<3)
                            {
                                cursor++;
							}
							else
							{
							    cursor =1;
							}
						 	appsys.byte_need_redraw|=REDRAW_PATITIAL;

						}						
					}
		        }
				else if(EVENT_TIMER == EventPara.eventTyp )
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


void set_date_time()
{
   FlSignalBuffer EventPara;
   DATE temp_date,bkp_date;
   TIME temp_time,bkp_time;
   unsigned char cursor=1;
   memcpy(&temp_date, &(appsys.struct_date),sizeof(DATE));
   memcpy(&temp_time, &(appsys.struct_time),sizeof(TIME));

   memcpy(&bkp_date, &temp_date,sizeof(DATE));
   memcpy(&bkp_time, &temp_time,sizeof(TIME));

   appsys.byte_need_redraw=0xFF;
   while(1)
   {
       if( appsys.byte_need_redraw>0)
       {
            if(0xFF == appsys.byte_need_redraw)
            {
		        lcd_clear_screen();
			    show_caption(get_multi_string((char * *)text_date_and_time));
		        lcd_goto_xy(0, 12);lcd_draw_horizontal(SCREEN_WIDTH-1);
				show_soft_key_prompt(get_multi_string((char * *)text_edit), get_multi_string((char * *)text_back));
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
            lcd_put_string(get_multi_string((char * *)text_date));

			if(1)
			{
               char tmp_buf[16];
               memset(tmp_buf,0,16);

			   get_date_str(&temp_date, tmp_buf);		
			   lcd_goto_x_line(33, 1);
			   lcd_put_string(tmp_buf);
			}
			lcd_clear_line(SCREEN_WIDTH-1);
            appsys.flag_color_reverse=0;

			

            if ( 2 == cursor )
            {
                appsys.flag_color_reverse=1;
            }
		    lcd_goto_x_line(0, 2);lcd_clear_line_ex(get_cursor_height()+1,SCREEN_WIDTH-1);
			lcd_goto_x_line(0, 2);	
			
			lcd_put_string("2.");
            lcd_put_string(get_multi_string((char * *)text_time));
			lcd_put_string(": ");
			
			if(1)
			{
               char tmp_buf[16];
               memset(tmp_buf,0,16);
			   get_normal_time_str(&temp_time, tmp_buf);
			   lcd_goto_x_line(33, 2);
			   lcd_put_string(tmp_buf);
			}

			lcd_clear_line(SCREEN_WIDTH-1);
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
                                   if( (0 != memcmp(&bkp_date, &temp_date,sizeof(DATE)) ||  0 != memcmp(&bkp_time, &temp_time,sizeof(TIME) )))
                                   {
								  	   if (1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_ask_save_or_not)) )
								  	   {
			                                mu_set_date_time(&temp_date, &temp_time);
								  	   }
                                   }
								   return ;   
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
		                                app_set_date(&temp_date);
									}
									else if(2 == cursor)
									{
		                                app_set_time(&temp_time);
									}
									appsys.byte_need_redraw=0xFF;
							  }
							  break;
							  default:
							  break;
						}
					    
                        if( (TFKEY_UP==EventPara.sig_p.key_evt.key_val) || ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val))
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
						else if( (TFKEY_DOWN==EventPara.sig_p.key_evt.key_val) || ( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val))
						{
                            if(cursor<2)
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



void app_set_date_time()
{
    unsigned short pos_info=0;

    while ( 1 )
    {
        switch (app_menu_select((char ***)mem_time_and_date, 2, &pos_info) )
        {
            case 1:
                set_date_time();
                break;

            case 2:
                {
					unsigned char def_pos= sysprop->flag_clock_format;
					if(1 == choose_menu_option((char ***)mem_time_format, 2, &def_pos))
					{
                        if( def_pos != sysprop->flag_clock_format)
                        {
                            sysprop->flag_clock_format = def_pos;
							app_save_prop();
							msgok(get_multi_string((char * *)text_changed_successful));
							delay_ms(1000);
						}
					}
			    }
                break;
            default:
                return;
                break;
        }
    }
}


