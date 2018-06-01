
#include "inc/app_def.h"


unsigned char app_graph_meme( unsigned char * icons,char *** menus,unsigned char icon_nums,unsigned char cursor )
{
#define UPDATE_MEME_CAPTION 0x01
#define UPDATE_MEME_ICONS   0x02
#define UPDATE_MEME_SOFTKEY 0x04
   

    FlSignalBuffer EventPara;
    unsigned char i, nret;
	unsigned char text_width=0;
	unsigned char old_cursor=cursor;
	
	i=nret =0;

    appsys.byte_need_redraw=0xFF;
    if ( cursor > icon_nums )
    {
        cursor = 1;
    }
	
 	while(0 == appsys.byte_need_exit)
    {
        if ( appsys.byte_need_redraw >0)
        {
            if(0xFF == appsys.byte_need_redraw)
            {
                lcd_clear_screen();
			}


#if 1
		    if( (appsys.byte_need_redraw & UPDATE_MEME_CAPTION)>0 )
		    {
	            appsys.byte_font_type= FONT_T_X_10;
	            lcd_goto_xy(0,0);
				lcd_clear_line(SCREEN_WIDTH-1);
				
	            text_width =  get_str_dots_width(":");
	            text_width += get_str_dots_width( get_multi_string_ex(menus, 0));
	            text_width += get_str_dots_width( get_multi_string_ex(menus, cursor));

	            lcd_goto_xy(0, 12);

				lcd_draw_horizontal(SCREEN_WIDTH-1);

			    lcd_goto_xy( (text_width < SCREEN_WIDTH)? ((SCREEN_WIDTH - text_width) / 2): 0 , 0);
	            lcd_put_string(get_multi_string_ex(menus, 0));
				lcd_put_string(":");
				lcd_put_string(get_multi_string_ex(menus, cursor));	
		    }

		    if( (appsys.byte_need_redraw & UPDATE_MEME_ICONS)>0 )
		    {
				lcd_goto_xy(3,17);
	            for ( i=0; i<icon_nums; ++i )
	            {
	                if ( 4 == i )
	                {
						lcd_goto_xy(3,35);
	                }
	                show_bitmap(icons[i]);
					lcd_add_x(32);
	            }

	            // draw a frame at the icon that cursor points to
                if(old_cursor != cursor)
                {
	                i = old_cursor-1;
					lcd_put_rect_ex((i&0x03)*32, (i>>2)?33:14, (i&0x03)*32+31, (i>>2)?50:33,0);
				}
				
                
			    i = cursor - 1;
				lcd_put_rect((i&0x03)*32, (i>>2)?33:14, (i&0x03)*32+31, (i>>2)?50:33);
                old_cursor=cursor;
			}
			
		    if( (appsys.byte_need_redraw & UPDATE_MEME_SOFTKEY)>0 )
		    {
				show_soft_key_prompt( get_multi_string((char * *)(text_ok)),get_multi_string((char * *)text_back));
		    }
#endif
			
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
					     char input_code = get_key_value(EventPara.sig_p.key_evt.key_val);
						 if(input_code !=0)
						 {
						     input_code-=0x30;
                             if(input_code>0  && input_code <=icon_nums)
                             {
                                nret= input_code;
							    break;
							 }
						 }

						 if( (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
					     {
                              nret=0;
							  break;
						 }
						 else if(TFKEY_SOFT_LEFT== EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						 {
                              nret=cursor;
							  break;
						 }
					 	 else if( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
						 {
			                  if ( cursor > 1 )
			                  {
			                       --cursor;
			                  }
							  else
							  {
                                  cursor = icon_nums;
							  }

							  
							  appsys.byte_need_redraw |= (UPDATE_MEME_ICONS|UPDATE_MEME_CAPTION);								   

						 }
				  		 else if( TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val)
						 {
			                 if ( cursor < icon_nums)
			                 {
			                      ++cursor;
			                 }	
							 else
							 {
                                  cursor=1;
							 }
						     appsys.byte_need_redraw |= (UPDATE_MEME_ICONS|UPDATE_MEME_CAPTION);								  
						 }
                         else if(TFKEY_UP== EventPara.sig_p.key_evt.key_val)
						 {							 
                             switch(cursor)
                             {
                                case 1:
									cursor =8;
								break;
								case 2:
									cursor =5;
								break;
								case 3:
									cursor =6;
								break;
								case 4:
									cursor =7;
								break;
								case 5:
									cursor =1;
								break;
								case 6:
									cursor =2;
								break;
								case 7:
									cursor =3;
								break;
								case 8:
									cursor =4;
								break;

								default:
									break;
								
							 }						 					 

							 appsys.byte_need_redraw |= (UPDATE_MEME_ICONS|UPDATE_MEME_CAPTION);	
						 }
				  		 else if(TFKEY_DOWN== EventPara.sig_p.key_evt.key_val)
						 {
                             switch(cursor)
                             {
                                case 1:
									cursor =5;
								break;
								case 2:
									cursor =6;
								break;
								case 3:
									cursor =7;
								break;
								case 4:
									cursor =8;
								break;
								case 5:
									cursor =2;
								break;
								case 6:
									cursor =3;
								break;
							 	case 7:
									cursor =4;
								break;
							  	case 8:
									cursor =1;
								break;

								default:
									break;
								
							 }
							 
						     appsys.byte_need_redraw |= (UPDATE_MEME_ICONS|UPDATE_MEME_CAPTION);								  
						 }
					}
		        }
				else if(EVENT_TIMER== EventPara.eventTyp )
				{
			        if(appsys.byte_need_exit > 0)
	                {
	                    nret=0;
						break;
					}
				}	
			}
		}
    }
	
    if(1 == appsys.flag_genie_trace)DebugPrintf("appsys.byte_need_exit=%.2X\r\n", appsys.byte_need_exit);
	
	return nret;
}


unsigned char choose_menu_option(char *** temp_menus, unsigned char max_size, unsigned char * def_pos)
{
    return choose_menu_option_ex(temp_menus, max_size, def_pos, 0,0);
}


unsigned char choose_menu_option_ex(char *** temp_menus, unsigned char max_size, unsigned char * def_pos, unsigned char flag_read_only, unsigned char ex_style)
{
    unsigned char nret=0;
	char * menu_mask = (char *)app_alloc_memory(max_size);
	memset(menu_mask,0,max_size);
	menu_mask[*def_pos]=1;


	if(  (ex_style | MENU_STYLE_MUSIC_VOICE )  >0  || (ex_style | MENU_STYLE_ALARM_VOICE )  >0 || (ex_style | MENU_STYLE_SMSRING_VOICE )  >0    )
	{
         appsys.flag_selecting_ring=1;
	}

    if (1 == app_menu_option(temp_menus, (unsigned char *)menu_mask, max_size,*def_pos, (flag_read_only>0)? MENU_OPTION_READONLY :0 , ex_style))
    {   
        unsigned char tmp_pos=0;
		for(tmp_pos=0;tmp_pos<max_size;tmp_pos++)
		{
            if (1 == menu_mask[tmp_pos])
            {
                 *def_pos= tmp_pos;
                 nret =1;
				 break;
			}
		}
	}  
	app_free_memory(menu_mask);menu_mask=NULL;
	if(0 == nret)
	{
        appsys.byte_need_redraw=0xFF;
	}

    if(appsys.flag_selecting_ring >0)
    {
        app_stop_music();
	    appsys.flag_selecting_ring=0;
	}

    return nret;
}




unsigned char app_menu_select( char *** menus,  unsigned char max_len, unsigned short * p_pos_info)
{
    MENUWND  m_menu_wnd;
	unsigned char nret=0;
    unsigned char * menu_mask =(unsigned char *)app_alloc_memory(max_len);
	
	memset(&m_menu_wnd,0,sizeof(MENUWND));
	memset(menu_mask,0, max_len);

	m_menu_wnd.line_cnt=max_len;
	if(p_pos_info!=0)
	{   
		m_menu_wnd.line_start=   (((*p_pos_info)&0xFF00)>>8 ) ;
		m_menu_wnd.cur_line=     (((*p_pos_info)&0x00FF)>>0 ) ;
	}
	else
	{
		m_menu_wnd.line_start=0;
		m_menu_wnd.cur_line=0;
	}
	m_menu_wnd.selected_pos=m_menu_wnd.line_start+m_menu_wnd.cur_line;



	
	m_menu_wnd.pcaption=get_multi_string_ex(menus,0);;
	appsys.byte_need_redraw=0xFF;

	m_menu_wnd.style =MENU_STYLE_SHOW_FRAME| MENU_STYLE_VERTICAL_BAR|MENU_STYLE_SHOW_SEQ;
	m_menu_wnd.soft_key_type=SOFT_KEY_TYPE_BACK_NULL_OK;
	
	m_menu_wnd.frame_left =0;
	m_menu_wnd.frame_top =12;
	m_menu_wnd.frame_right =SCREEN_WIDTH-1;
	m_menu_wnd.frame_bottom =51;

	m_menu_wnd.menu_buf= (char ***)&menus[1];
	m_menu_wnd.menu_mask=menu_mask;
	m_menu_wnd.menu_font=FONT_T_X_10;

	wnd_menu_set_select(&m_menu_wnd);

   
    if(1 ==wnd_menu_show(&m_menu_wnd))
    {
        unsigned char i=0;
        for(i=0;i<max_len;i++)
        {
            if(m_menu_wnd.menu_mask[i]>0)
            {
                nret=i+1;
			}
		}
		if(p_pos_info !=0)
		{
		    (*p_pos_info) =m_menu_wnd.line_start;
	        (*p_pos_info) <<=8;
		    (*p_pos_info) +=m_menu_wnd.cur_line;
		}
	}
    app_free_memory(menu_mask);menu_mask=NULL;
	if(0 == nret)
	{
        appsys.byte_need_redraw=0xFF;
	}
	return nret;
}

unsigned char app_menu_option( char *** menus, unsigned char * menu_mask, unsigned char max_len,unsigned short  def_pos,unsigned char option_mask, unsigned char ex_style)
{
    
    MENUWND  m_menu_wnd;
	unsigned char nret=0;
	memset(&m_menu_wnd,0,sizeof(MENUWND));
	
	m_menu_wnd.line_cnt=max_len;

	m_menu_wnd.line_start=0;
	m_menu_wnd.cur_line=0;
	m_menu_wnd.selected_pos=def_pos;

	
	m_menu_wnd.pcaption=get_multi_string_ex(menus,0);

	
	appsys.byte_need_redraw=0xFF;

	m_menu_wnd.style =ex_style|MENU_STYLE_SHOW_FRAME| MENU_STYLE_VERTICAL_BAR| ((option_mask&MENU_OPTION_MULTISELECT)>0? MENU_STYLE_MULTI_OPTION : MENU_STYLE_SINGLE_OPTION);
    m_menu_wnd.soft_key_type=(option_mask&MENU_OPTION_READONLY)?SOFT_KEY_TYPE_BACK_NULL_NULL: SOFT_KEY_TYPE_BACK_SELECT_OK;
	
	m_menu_wnd.frame_left =0;
	m_menu_wnd.frame_top =12;
	m_menu_wnd.frame_right =SCREEN_WIDTH-1;
	m_menu_wnd.frame_bottom =51;

	m_menu_wnd.menu_buf= (char ***)&menus[1];
	m_menu_wnd.menu_mask=menu_mask;

	m_menu_wnd.menu_font=FONT_T_X_10;

    nret = wnd_menu_show(&m_menu_wnd);

	if((m_menu_wnd.style & MENU_STYLE_MUSIC_VOICE) >0  ||( m_menu_wnd.style & MENU_STYLE_ALARM_VOICE ) >0 )
	{
		app_stop_music();
	}
	return nret;
}




unsigned short show_bitmap(unsigned char bmp_id)
{
   unsigned short bmp_addr=0;
   unsigned short word_ret=0;
   unsigned char w,h;
   unsigned char * pdata=0;
   w=h=0;
  
   if(bmp_id<MAX_BMP_COUNT)
   {
       bmp_addr = BITMAP_ARRAY[bmp_id];
	   pdata=(unsigned char *)(&BITMAP_BIN_TXT[bmp_addr]);
	   w=pdata[0];
	   h=pdata[1];
	   if(w<=SCREEN_WIDTH  && h<=SCREEN_HEIGHT)
	   {
	     
	       lcd_display_data(w, h, &pdata[2]);
		   word_ret=w;
		   word_ret<<=8;
		   word_ret+=h;
	   }
	   return word_ret;
   }
   return 0;
}


void wnd_menu_set_select(MENUWND * p_menu_wnd)
{
    unsigned char i;
    if((p_menu_wnd->style & MENU_STYLE_MULTI_OPTION) > 0)
    { 
        if( (p_menu_wnd->menu_mask[p_menu_wnd->selected_pos] & MENU_MASK_SELECTED) >0 )
        {
            p_menu_wnd->menu_mask[p_menu_wnd->selected_pos] &= (~MENU_MASK_SELECTED);
		}
		else
		{						   
            p_menu_wnd->menu_mask[p_menu_wnd->selected_pos] |=MENU_MASK_SELECTED;
		}
	}
	else
	{
        for(i=0;i<p_menu_wnd->line_cnt;i++)
        {
            p_menu_wnd->menu_mask[i] &=(~MENU_MASK_SELECTED);
		}
		p_menu_wnd->menu_mask[p_menu_wnd->selected_pos] |=MENU_MASK_SELECTED;

		if(1 ==appsys.flag_genie_trace)DebugPrintf(" menu_pos=%d <st_line=%d  cur_pos=%d> , menu_item: [%s]  is selected! \r\n" ,p_menu_wnd->selected_pos,p_menu_wnd->line_start,p_menu_wnd->cur_line, get_multi_string_ex((char * * *)p_menu_wnd->menu_buf, p_menu_wnd->selected_pos));
	}
}

void wnd_menu_redraw(MENUWND * p_menu_wnd)
{
	unsigned char x,y,h,i,w,box_width,tmp,m_line_cnt,length,selected,delta2left;
	char * pstring=0;
	unsigned char * pdata=0;
	x=y=h=i=w=box_width=tmp=m_line_cnt=length=selected=delta2left=0;

    appsys.byte_font_type=p_menu_wnd->menu_font;

	h= get_cursor_height();

    box_width = p_menu_wnd->body_right -p_menu_wnd->body_left;

    for(i=p_menu_wnd->line_start ;i<p_menu_wnd->line_cnt;i++)
    {
        if(m_line_cnt<p_menu_wnd->body_lines)
        {
		    pstring = get_multi_string_ex(p_menu_wnd->menu_buf, i);

		    if(1 ==appsys.flag_genie_trace)DebugPrintf("show menu[%d]-->%s\r\n",i, pstring);
			if(pstring != 0)
			{

				y=p_menu_wnd->body_top+(h+1)*m_line_cnt;
				m_line_cnt++;
				
			    if(  (p_menu_wnd->style & MENU_STYLE_SHOW_SEQ) >0 )
			    {
					char lbuf[4];
			        memset(lbuf,4,0);
					sprintf(lbuf,"%d.",i+1 );
					lcd_goto_xy(p_menu_wnd->body_left, y);
					lcd_put_string(lbuf);
					delta2left=10;
				}
				else if( (p_menu_wnd->style & MENU_STYLE_SINGLE_OPTION) >0 )
				{

                    lcd_goto_xy(p_menu_wnd->body_left+1, y+ (h-8)/2);
					if( (p_menu_wnd->menu_mask[i] & MENU_MASK_SELECTED) >0 )
                    {
                        show_bitmap(BMP_SELECTED_BIN);
					}
					else
					{
                        show_bitmap(BMP_UNSELECTED_BIN);
					}
					delta2left=10;
				}
				else if(  (p_menu_wnd->style & MENU_STYLE_MULTI_OPTION) >0 )
				{
                    lcd_goto_xy(p_menu_wnd->body_left+1, y+ (h-8)/2);				
                    if( (p_menu_wnd->menu_mask[i] & MENU_MASK_SELECTED) >0 )
                    {
                        show_bitmap(BMP_CHECKED_BIN);
					}
					else
					{
					    show_bitmap(BMP_UNCHECKED_BIN);
					}
					delta2left=10;
				}
				else
				{
				    delta2left=0;
				}

				x=p_menu_wnd->body_left+delta2left;

				p_menu_wnd->context_width=box_width-delta2left-1;
					
				length=strlen(pstring);

                //get if this line is high light;
                w   = p_menu_wnd->line_start;
				tmp = i;

				selected =( (tmp-w) == p_menu_wnd->cur_line)? 1:0;

				

			    appsys.flag_color_reverse= selected;


			    lcd_goto_xy(x-1, y);
                lcd_clear_line_ex((h+1), p_menu_wnd->body_right-1);
				
				
			    x = lcd_show_to_colum(pstring, x, y, p_menu_wnd->body_right-1);
			    appsys.flag_color_reverse=0;


				if(selected>0)
				{
                    p_menu_wnd->selected_pos=i;
					p_menu_wnd->selected_left=p_menu_wnd->body_left+delta2left;
					p_menu_wnd->selected_top=p_menu_wnd->body_top+(h+1)*(m_line_cnt-1);
					p_menu_wnd->selected_right=p_menu_wnd->body_right-1;
					p_menu_wnd->selected_bottom=p_menu_wnd->selected_top+h;
					p_menu_wnd->roll_pos=0;


					if((p_menu_wnd->style & MENU_STYLE_MUSIC_VOICE) >0)
					{
						if( appsys.byte_last_music_id !=  music_array[p_menu_wnd->selected_pos]   )
						{
						    app_play_income_ring(p_menu_wnd->selected_pos, 0);
						}
					}
					else if((p_menu_wnd->style & MENU_STYLE_ALARM_VOICE) >0)
					{
						if( appsys.byte_last_music_id !=alarm_array [p_menu_wnd->selected_pos ]  )
						{
						     app_play_alarm_ring(p_menu_wnd->selected_pos, 0);
						}
					}
					else if(  (p_menu_wnd->style & MENU_STYLE_SMSRING_VOICE) >0)
					{
					    if( appsys.byte_last_music_id !=smsring_array [p_menu_wnd->selected_pos ]  )
					    {
				             app_play_sms_ring(p_menu_wnd->selected_pos);
					    }
					}
				}
    		}
		}
    }
	//clear remained blank space
	while(i-p_menu_wnd->line_start<p_menu_wnd->body_lines)
	{
	    y=p_menu_wnd->body_top+(i-p_menu_wnd->line_start)*(h+1);
        lcd_goto_xy( p_menu_wnd->body_left ,y );
		lcd_clear_line(p_menu_wnd->body_right);
		i++;	
	}

    if( (p_menu_wnd->style & MENU_STYLE_VERTICAL_BAR)>0 )
    {
		VERTICALBAR bar;
		bar.left  =p_menu_wnd->body_right+1;
		bar.top   =p_menu_wnd->frame_top;
		bar.right =p_menu_wnd->frame_right;
		bar.bottom=p_menu_wnd->frame_bottom;
		bar.line_cnt =p_menu_wnd->line_cnt;
		bar.body_lines=p_menu_wnd->body_lines;

		bar.cur_line=p_menu_wnd->line_start+p_menu_wnd->cur_line;
	    show_vertical_bar(&bar);
    }



    lcd_update_screen_area(p_menu_wnd->body_left,p_menu_wnd->body_top,p_menu_wnd->frame_right,p_menu_wnd->body_bottom);
	
}

void wnd_menu_move(MENUWND * p_menu_wnd,unsigned char key_code)
{
    unsigned char i,x,w;
	i=x=w=0;
	
    switch(key_code)
	{
	case TFKEY_UP:
	case TFKEY_LEFT:
        if(p_menu_wnd->cur_line ==0 )
		{
			 if(p_menu_wnd->line_start>0)
			 {
				  p_menu_wnd->line_start--;
			 }
			 else
			 {
			     if(p_menu_wnd->line_cnt>p_menu_wnd->body_lines)
			     {
                      p_menu_wnd->cur_line=p_menu_wnd->body_lines-1;
					  p_menu_wnd->line_start=p_menu_wnd->line_cnt-p_menu_wnd->body_lines;
				 }
				 else
				 {
                      p_menu_wnd->cur_line=p_menu_wnd->line_cnt-1;
					  p_menu_wnd->line_start=0;
				 }
			 }
		}
		else
		{ 
		    p_menu_wnd->cur_line--;
		}
	 	wnd_menu_redraw(p_menu_wnd);
		break;
	case TFKEY_DOWN:
	case TFKEY_RIGHT:
        if( (p_menu_wnd->cur_line ==p_menu_wnd->body_lines-1) && ( p_menu_wnd->selected_pos)<p_menu_wnd->line_cnt)
		{
			 if(p_menu_wnd->line_start+p_menu_wnd->body_lines< p_menu_wnd->line_cnt)
			 {
				  p_menu_wnd->line_start++; 
			 }
			 else
			 {
                  p_menu_wnd->cur_line=0;
				  p_menu_wnd->line_start=0;
			 }
			 wnd_menu_redraw(p_menu_wnd);
		}
		else 
		{ 
		    x=p_menu_wnd->line_start;

			if( x + p_menu_wnd->cur_line < p_menu_wnd->line_cnt-1)
			{
			    p_menu_wnd->cur_line++;
			}
			else
			{
                p_menu_wnd->cur_line=0;
			    p_menu_wnd->line_start=0;
			}
			wnd_menu_redraw(p_menu_wnd);
		}			
		break;
	default:
		break;
    }
}

void wnd_menu_init(MENUWND * p_menu_wnd)
{
    unsigned char h;

    lcd_clear_screen();
    appsys.byte_font_type=p_menu_wnd->menu_font;
	h=get_cursor_height();
	if(p_menu_wnd->style & MENU_STYLE_SHOW_FRAME)
	{
		lcd_put_rect(p_menu_wnd->frame_left,p_menu_wnd->frame_top  ,p_menu_wnd->frame_right ,p_menu_wnd->frame_bottom ); 
		
		p_menu_wnd->body_left=p_menu_wnd->frame_left+2;
		p_menu_wnd->body_top=p_menu_wnd->frame_top+2;
		if( (p_menu_wnd->style & MENU_STYLE_VERTICAL_BAR)>0)
		{
		   p_menu_wnd->body_right=p_menu_wnd->frame_right-5;
		}
		else
		{
           p_menu_wnd->body_right=p_menu_wnd->frame_right-1;
		}
		p_menu_wnd->body_bottom=p_menu_wnd->frame_bottom-1;

	}
	else
	{
		p_menu_wnd->body_left=p_menu_wnd->frame_left;
		p_menu_wnd->body_top=p_menu_wnd->frame_top;
		p_menu_wnd->body_right=p_menu_wnd->frame_right;
		p_menu_wnd->body_bottom=p_menu_wnd->frame_bottom;
	}
    p_menu_wnd->body_lines=  (p_menu_wnd->body_bottom-p_menu_wnd->body_top+1)/(h+1);

    if( (0== p_menu_wnd->line_start) && ( 0 == p_menu_wnd->cur_line) && ( p_menu_wnd->selected_pos>0))
	{
        //locate to default option
        if(p_menu_wnd->selected_pos< p_menu_wnd->body_lines)
        {
            p_menu_wnd->line_start=0;
			p_menu_wnd->cur_line=p_menu_wnd->selected_pos;
		}
		else
		{
		   p_menu_wnd->line_start=(p_menu_wnd->selected_pos+1)-p_menu_wnd->body_lines;
           p_menu_wnd->cur_line=p_menu_wnd->body_lines-1;
		}
	}

#if 1
   
	if( SOFT_KEY_TYPE_BACK_SELECT_OK == p_menu_wnd->soft_key_type )
	{
        show_soft_key_prompt(get_multi_string((char * *)text_select), get_multi_string((char * *)text_back));
	}
	else if( SOFT_KEY_TYPE_BACK_NULL_OK == p_menu_wnd->soft_key_type )
	{
	    show_soft_key_prompt(get_multi_string((char * *)text_ok), get_multi_string((char * *)text_back));
	}
	else if( SOFT_KEY_TYPE_BACK_NULL_NULL == p_menu_wnd->soft_key_type )
	{
	    show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_back));
	}

	show_caption(p_menu_wnd->pcaption);
	wnd_menu_redraw(p_menu_wnd);
#endif

}

void wnd_roll_selected_menu(MENUWND * p_menu_wnd)
{
   static unsigned char direction=1;
   unsigned char length   =0;
   char * pstring =0;
   unsigned char * pdata=0;
   unsigned char i,x,w;
   if(p_menu_wnd->line_cnt>0)
   {
       pstring  = get_multi_string_ex(p_menu_wnd->menu_buf, p_menu_wnd->selected_pos);
       if(pstring !=0 )
       {
            length = strlen(pstring);
            w = get_str_dots_width_ex(pstring, length);
			if(w> p_menu_wnd->context_width)
			{
			    x=p_menu_wnd->selected_left;

				appsys.flag_color_reverse=1;
                
				for(i=p_menu_wnd->roll_pos;i<length ;i++)
                {
                    pdata=(unsigned char *)get_char_font_data(pstring[i]);
					if(pdata !=0)
					{
					    if(x+pdata[0] <p_menu_wnd->selected_right )
					    {
							lcd_goto_xy(x,p_menu_wnd->selected_top);
                            lcd_put_char( pstring[i]);
							x+=pdata[0];
						}
						else
						{
						    break;
						}
					}
				}
				lcd_goto_xy(x, p_menu_wnd->selected_top);
				lcd_clear_line(p_menu_wnd->selected_right);

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
	                        p_menu_wnd->roll_pos++;
						}
	                    else
	                    {
	                        direction |=0x80;
							direction |=0x04;
						}
					}
					else
					{
					    if(p_menu_wnd->roll_pos>0)
					    {
	                        p_menu_wnd->roll_pos--;
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




unsigned char wnd_menu_show( MENUWND * p_menu_wnd)
{
    FlSignalBuffer EventPara;
	char input_code=0;
	unsigned char update_select_cnt=0;
		
	appsys.byte_need_redraw=0xFF;

    while(0 == appsys.byte_need_exit)
	{
	    if(appsys.byte_need_redraw>0)
		{
		   appsys.byte_need_redraw=0;
           wnd_menu_init( p_menu_wnd);
	    }
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{
				
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    char input_code = get_key_value(EventPara.sig_p.key_evt.key_val);
						if(input_code !=0)
						{
                             if( SOFT_KEY_TYPE_BACK_SELECT_OK == p_menu_wnd->soft_key_type  ||  SOFT_KEY_TYPE_BACK_NULL_NULL== p_menu_wnd->soft_key_type )
                             {
                                  continue;
							 }
							 else
							 {
							      input_code-=0x30;
                                  if(input_code>0  && input_code <=9)
                                  {
                                      if( input_code <=  p_menu_wnd->line_cnt)
                                      {
                                           p_menu_wnd->selected_pos =input_code -1;
										   wnd_menu_set_select(p_menu_wnd);
										   return 1;
									  }
								  }
							 }
						}
					    
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val) || (TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
                            return 0;
						}
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if( SOFT_KEY_TYPE_BACK_NULL_OK == p_menu_wnd->soft_key_type )
						    {
                                wnd_menu_set_select(p_menu_wnd);
								return 1;
							}
							else if(SOFT_KEY_TYPE_BACK_SELECT_OK== p_menu_wnd->soft_key_type)
							{
							    wnd_menu_set_select(p_menu_wnd);
                                return 1;
							}
						}								
						else if(
								  (TFKEY_UP==EventPara.sig_p.key_evt.key_val)
								||(TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
								||(TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)
								||(TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val)
							    )
						{
						
                            wnd_menu_move(p_menu_wnd,EventPara.sig_p.key_evt.key_val);
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
                       // wnd_roll_selected_menu(p_menu_wnd);
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




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////





void app_run_books( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_book,5, &pos_info) )
        {
            case 1 :
			    app_records_show(TABLEINFO_PHONEBOOK, get_multi_string((char * *)text_phonebook));
                break;
            case 2 :
                new_book_entry(NULL);
                break; 
			
				
            case 3:
                app_show_record_capacity(TABLEINFO_PHONEBOOK);
                break;

			case 4:
				if(1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_do_you_want_to_clear_phone_book)))
				{
                    reset_table_index(TABLEINFO_PHONEBOOK);
				}
				break;
				
            case 5:
				app_show_book_from_simcard();
				break;
				
			default:
				return;
				break;
        }
    }
}















void app_run_recent_calls( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_calls,5 ,&pos_info))
        {
            case 1:
                app_records_show(TABLEINFO_CALL_OUT, get_multi_string((char * *)text_dialed_calls) );
                break;

            case 2:
                app_records_show(TABLEINFO_CALL_IN, get_multi_string((char * *)text_received_calls));
                break;

            case 3:
                app_records_show(TABLEINFO_MISSED_CALL, get_multi_string((char * *)text_missed_calls));
                break;

            case 4:
			    app_view_call_meter_total();
                break;

            case 5:
				{
	                switch ( app_menu_select((char ***)mem_del_all_call, 4, 0) )
	                {
	                    case 1:
							if(1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_incoming_calls)))
							{
                                 reset_table_index(TABLEINFO_CALL_IN);
							}
	                        break;

	                    case 2:
	                        if (1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_dialed_calls)) )
	                        {
								reset_table_index(TABLEINFO_CALL_OUT);
	                        }
	                        break;

	                    case 3:
	                        if (1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_missed_calls)) )
	                        {
	                            reset_table_index(TABLEINFO_MISSED_CALL);
								
	                        }
	                        break;

	                    case 4:
	                        if (1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_call_logs)) )
	                        {
	                            msg(get_multi_string((char * *)text_please_wait));
	                            reset_table_index(TABLEINFO_CALL_IN);
								reset_table_index(TABLEINFO_CALL_OUT);
								reset_table_index(TABLEINFO_MISSED_CALL);
								appsys.flag_having_missed_call=0;
								msgok(get_multi_string((char * *)text_succeed));
								delay_ms(1000);
	                        }
	                        break;

	                    default:
	                        break;
	                }
	            }
			    break;
			case 0:
			default:
				return;
				break;
        }
    }
}

void app_run_messages( void )
{
    unsigned short pos_info=0;

    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_messages,6, &pos_info) )
        {
            case 1 :
				{
	                T_SMS_RECORD sms_record;
					memset(&sms_record,0, sizeof(T_SMS_RECORD));
					edit_sms_message(&sms_record);
            	}
                break;      
            case 2 :
                app_records_show(TABLEINFO_SMS_INBOX, get_multi_string((char * *)text_inbox_list));
                break;
            case 3 :
                app_records_show(TABLEINFO_SMS_OUTBOX, get_multi_string((char * *)text_outbox_list));
                break;
            case 4 :
                app_records_show(TABLEINFO_SMS_DRAFT, get_multi_string((char * *)text_draft_list));
                break;
				
			case 5:
                app_view_sms_total();
				break;
            case 6:
				 {
	                switch ( app_menu_select((char ***)mem_del_all_sms, 3,0) )
	                {
	                    case 1:
	                        if (1 == choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_incoming_sms)) )
	                        {
	                            reset_table_index(TABLEINFO_SMS_INBOX);

								//if need clear simcard?
								appsys.flag_having_unread_sms=0;
								appsys.byte_simcard_sms_cnt=0;
								
	                        }
	                        break;

	                    case 2:
	                        if ( choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_sent_sms)) )
	                        {
	                           reset_table_index(TABLEINFO_SMS_OUTBOX);
	                        }
	                        break;

	                    case 3:
							 {
							 	 char tmp_buf[16];
		                         if ( choose_yes_no(get_multi_string((char * *)text_confirm),get_multi_string((char * *)text_delete_all_sms)) )
		                         { 
		                             unsigned char cst=0;
									 char * pstr=NULL;
									 msg(get_multi_string((char * *)text_clear_simcard_sms));


									 reset_table_index(TABLEINFO_SMS_INBOX);
									 reset_table_index(TABLEINFO_SMS_OUTBOX);	
								 	 reset_table_index(TABLEINFO_SMS_DRAFT);	
									 appsys.flag_having_unread_sms=0;
								     appsys.byte_simcard_sms_cnt=0;
									 
    								 msgok(get_multi_string((char * *)text_delete_all_succeed));
									 delay_ms(1000);

		                         }
	                    	 }
	                         break;

	                    default:
	                        break;
	                 }
            	}
                break;
			case 0:
			default:
				return;
                break;
        }
    }
}




void app_run_security_settings( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_security_set,4,&pos_info) )
        {     
		    case 1:
			{
                appsys.byte_need_exit=0xFF;
                appsys.flag_keypad_locked =1;
				return ;
		    }
			break;
			
            case 2:
                {
					unsigned char def_pos= (0 ==sysprop->flag_auto_lock_keypad)?1:0;
                    if(1 == choose_menu_option( (char ***)active_or_cancel, 2, &def_pos))
                    {
                         if(0 == def_pos )
                         {
	                         char tmp_buf[4];
							 memset(tmp_buf,0,4);
							 sprintf(tmp_buf,"%d",  (0 == sysprop->byte_auto_lock_keypad_timeout)?1:sysprop->byte_auto_lock_keypad_timeout);
							 
							 if(1 == wnd_input_text(tmp_buf, (get_combined_string(get_multi_string((char * *)text_edit), "(>=1分钟) ", get_multi_string((char * *)text_null))), 3, IN_FIXED, EDIT_STYLE_DIGITALONLY))
							 {
	                             unsigned char temp_val= atoi(tmp_buf);
								 if( temp_val != sysprop->byte_auto_lock_keypad_timeout)
								 {
	                                 sysprop->byte_auto_lock_keypad_timeout = temp_val;
								 }
							 }
							 sysprop->flag_auto_lock_keypad =1;
                         }
						 else
						 {
                             sysprop->flag_auto_lock_keypad =0;
						 }
					     app_save_prop();
					}
			    }
                break;

            case 3:
                {
                     app_pin_setting();
			    }               
                break;
            case 4:
                {
                    if(1 ==mu_is_pin_active())
                    {
                        app_user_change_simpin();
                    }
					else
					{
                        msg_warn(get_multi_string((char * *)text_pin_is_not_protected_cannot_change_simpin));
						
						delay_ms(1500);
					}
			    }
                break;		
           
			default:
				return;
				break;

        }
    }

}








void app_run_all_settings( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_settings,6,&pos_info) )
        {
            case 1:
                app_run_display_settings();
                break;
            case 2:
                app_set_date_time();
                break;
            case 3:
                app_run_voice_settings();
                break;
            case 4:
                app_run_call_ctrl_settings();
                break;
			case 5:
                app_view_version();
                break;
			case 6:
				app_run_factory_settings();
				break;
				
			case 0:
			default:
				return;
				break;
        }
    }
}

















void app_run_display_settings( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_display_set,2,&pos_info) )
        {
            case 1:
				{
					  char * temp_menus[5][4];
					  char ** pmenus[5];
					  unsigned char i=0;
					  unsigned char def_pos=0;
					  temp_menus[0][0]=(char *)text_language[0];
					  temp_menus[0][1]=(char *)text_language[1];
					  temp_menus[0][2]=(char *)text_language[2];
					  temp_menus[0][3]=(char *)text_language[3];
					  
					  if(LANGUAGE_ENGLISH== sysprop->byte_language_type)
					  {
					      temp_menus[1][0]=(char *)text_english[0];    //<--
						  temp_menus[1][1]=(char *)text_english[0];
					      temp_menus[1][2]=(char *)text_english[0];
						  temp_menus[1][3]=(char *)text_english[0];

					      temp_menus[2][0]=(char *)text_spanish[0];    //<--
						  temp_menus[2][1]=(char *)text_spanish[0];
					      temp_menus[2][2]=(char *)text_spanish[0];
						  temp_menus[2][3]=(char *)text_spanish[0];

						  temp_menus[3][0]=(char *)text_portuguese[0]; //<--
						  temp_menus[3][1]=(char *)text_portuguese[0];
						  temp_menus[3][2]=(char *)text_portuguese[0];
						  temp_menus[3][3]=(char *)text_portuguese[0];

						  temp_menus[4][0]=(char *)text_chinese[3];    //<--
						  temp_menus[4][1]=(char *)text_chinese[3];
						  temp_menus[4][2]=(char *)text_chinese[3];
						  temp_menus[4][3]=(char *)text_chinese[3];
					  }
					  else  if(LANGUAGE_SPANISH== sysprop->byte_language_type)
				      {
					      temp_menus[1][0]=(char *)text_english[1];    
						  temp_menus[1][1]=(char *)text_english[1];//<--
					      temp_menus[1][2]=(char *)text_english[1];
						  temp_menus[1][3]=(char *)text_english[1];

					      temp_menus[2][0]=(char *)text_spanish[1];   
						  temp_menus[2][1]=(char *)text_spanish[1]; //<--
					      temp_menus[2][2]=(char *)text_spanish[1];
						  temp_menus[2][3]=(char *)text_spanish[1];

						  temp_menus[3][0]=(char *)text_portuguese[1]; 
						  temp_menus[3][1]=(char *)text_portuguese[1];//<--
						  temp_menus[3][2]=(char *)text_portuguese[1];
						  temp_menus[3][3]=(char *)text_portuguese[1];

						  temp_menus[4][0]=(char *)text_chinese[1];    
						  temp_menus[4][1]=(char *)text_chinese[1];//<--
						  temp_menus[4][2]=(char *)text_chinese[1];
						  temp_menus[4][3]=(char *)text_chinese[1];
					  }
					  else if(LANGUAGE_PORTUGE== sysprop->byte_language_type)
					  {
					      temp_menus[1][0]=(char *)text_english[2];   
						  temp_menus[1][1]=(char *)text_english[2];
					      temp_menus[1][2]=(char *)text_english[2]; //<--
						  temp_menus[1][3]=(char *)text_english[2];

					      temp_menus[2][0]=(char *)text_spanish[2];   
						  temp_menus[2][1]=(char *)text_spanish[2];
					      temp_menus[2][2]=(char *)text_spanish[2]; //<--
						  temp_menus[2][3]=(char *)text_spanish[2];

						  temp_menus[3][0]=(char *)text_portuguese[2];
						  temp_menus[3][1]=(char *)text_portuguese[2];
						  temp_menus[3][2]=(char *)text_portuguese[2]; //<--
						  temp_menus[3][3]=(char *)text_portuguese[2];

						  temp_menus[4][0]=(char *)text_chinese[2];  
						  temp_menus[4][1]=(char *)text_chinese[2];
						  temp_menus[4][2]=(char *)text_chinese[2];  //<--
						  temp_menus[4][3]=(char *)text_chinese[2];

					  }
					  else//chinese
					  {
					      temp_menus[1][0]=(char *)text_english[0];   
						  temp_menus[1][1]=(char *)text_english[0];
					      temp_menus[1][2]=(char *)text_english[0];
						  temp_menus[1][3]=(char *)text_english[0]; //<--

					      temp_menus[2][0]=(char *)text_spanish[0];    
						  temp_menus[2][1]=(char *)text_spanish[0];
					      temp_menus[2][2]=(char *)text_spanish[0];
						  temp_menus[2][3]=(char *)text_spanish[0];//<--

						  temp_menus[3][0]=(char *)text_portuguese[0]; 
						  temp_menus[3][1]=(char *)text_portuguese[0];
						  temp_menus[3][2]=(char *)text_portuguese[0];
						  temp_menus[3][3]=(char *)text_portuguese[0];//<--

						  temp_menus[4][0]=(char *)text_chinese[3];    
						  temp_menus[4][1]=(char *)text_chinese[3];
						  temp_menus[4][2]=(char *)text_chinese[3];
						  temp_menus[4][3]=(char *)text_chinese[3];//<--
					  }

					  
					  for(i=0;i<5;i++)
					  {
                          pmenus[i]=(char **)temp_menus[i];
					  }
					  
					  
					  def_pos=sysprop->byte_language_type;
	                  if (1 == choose_menu_option((char ***)pmenus, 4, &def_pos))
	                  {
	                      if(def_pos  != sysprop->byte_language_type)
	                      {
	                           sysprop->byte_language_type  = def_pos;
							   app_save_prop();
						  }
					  }
			    }
			    break;
			  
            case 2:
               	{
					 unsigned char def_pos=sysprop->byte_sleep_timeout;
					 if (1 == choose_menu_option((char ***)mem_sleep_delay, 10, &def_pos))
					 {
                         if(def_pos !=sysprop->byte_sleep_timeout)
                         {
                             sysprop->byte_sleep_timeout=def_pos;
							 app_save_prop();
						 }

						 if(def_pos >=0 && def_pos <=9)
						 {
                             char tmp_cmd[128];
							 memset(tmp_cmd ,0,sizeof(tmp_cmd));
							 send_android_command(andr_build_44G_set_sleep(tmp_cmd, (0x30 + def_pos) ));
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

void app_run_call_ctrl_settings( void )
{
    unsigned short pos_info=0;
    while ( 1 )
    {
        switch ( app_menu_select((char *** )mem_call_ctrl_settings,5,&pos_info)  )
        {
            case 1:
              	 {
					  unsigned char def_pos=(sysprop->flag_auto_redial==1)?0:1;
					  
	                  if (1 == choose_menu_option((char *** )active_or_cancel, 2, &def_pos))
	                  {
	                      if(def_pos  != ((sysprop->flag_auto_redial ==1)?0:1))
	                      {
	                           sysprop->flag_auto_redial  = (1 ==def_pos)?0:1;
							   
							   app_save_prop();

						  }
						  
						  if(1 ==  sysprop->flag_auto_redial)
						  {
	                           setup_auto_redial();
						  }
					  }
			     }
                 break;

            case 2:
                run_set_call_wait();
                break;

            case 3:
                run_set_call_divert();
                break;

            case 4:
                run_call_barring_set();
                break;

			case 5:
			   {
					unsigned char def_pos= sysprop->byte_call_delay_s;
				    if (1 == choose_menu_option((char *** )mem_call_delay, 5, &def_pos))
				    {
					      if(def_pos  != sysprop->byte_call_delay_s )
					      {
					           sysprop->byte_call_delay_s = def_pos;
							   app_save_prop();
							   msgok(get_multi_string((char * *)text_changed_successful));
							   delay_ms(1500);			   
						  }
					 }
            	}

            default:
                return;
                break;
        }
    }
}
void app_select_ring_music()
{
   unsigned char music_id= sysprop->byte_income_ring;


   DebugPrintf("sysprop->byte_income_ring =%d\r\n",sysprop->byte_income_ring);
   
   if(1 == app_music_show(&music_id))
   {
       if(music_id != sysprop->byte_income_ring)
       {
           char tmp_cmd[128];
		   memset(tmp_cmd,0,sizeof(tmp_cmd));
		   
           sysprop->byte_income_ring=music_id;

		   send_android_command(andr_build_43D1_set_ring_music(tmp_cmd,  music_array[sysprop->byte_income_ring]));

		   app_save_prop();
	   }
   }
   mu_set_spk_gain(sysprop->byte_talk_volume);
}

void app_run_voice_settings( void )
{
    unsigned short pos_info=0; 
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_voice_set,7,&pos_info) )
        {
            case 1:
                // ring select
                {
                      app_select_ring_music();
				}
                break;


            case 2:
                // alarm select
                 {
                   unsigned char music_id= sysprop->struct_alarm.alarm_ring;
                   if(1 == app_alarm_show(&music_id))
                   {
                       if(music_id != sysprop->struct_alarm.alarm_ring)
                       {
                           sysprop->struct_alarm.alarm_ring=music_id;
						   app_save_prop();
					   }
				   }
				}               
                break;

            case 3:
                // smsring select
                 {
                   unsigned char music_id= sysprop->byte_sms_ring;
                   if(1 == app_smsring_show(&music_id))
                   {
                       if(music_id != sysprop->byte_sms_ring)
                       {
                       
                       	   char tmp_cmd[128];
		                   memset(tmp_cmd,0,sizeof(tmp_cmd));
                           sysprop->byte_sms_ring=music_id;

		                   send_android_command(andr_build_43D2_set_sms_music(tmp_cmd,  smsring_array[sysprop->byte_sms_ring]));
						   app_save_prop();
					   }
				   }
				}                  
                break;




				

		    case 4:
                {
 					appsys.flag_key_tone=0;
	                app_set_ring_volume();
					appsys.flag_key_tone=sysprop->flag_key_tone;
			    }
                break; 	

            case 5:
                {
					appsys.flag_key_tone=0;
	                app_set_sms_volume();
					appsys.flag_key_tone=sysprop->flag_key_tone;
			    }
                break;    

		    case 6:
                {
	                app_set_speech_volume();
			    }
                break; 	




				

            case 7:
              	 {
					  unsigned char def_pos=(sysprop->flag_key_tone==1)?0:1  ;
	                  if (1 == choose_menu_option((char ***)active_or_cancel, 2, &def_pos))
	                  {
	                      if(def_pos  != ((sysprop->flag_key_tone ==1)?0:1))
	                      {
	                           appsys.flag_key_tone =sysprop->flag_key_tone  = (1 ==def_pos)?0:1;

							   app_save_prop();
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



void app_keyboard_test()
{
	FlSignalBuffer EventPara;
	char * key_str=NULL;
	char tmp_buf[32];
	unsigned char byte_last_key=0;

    appsys.byte_need_redraw=0xFF;
	appsys.flag_key_tone =1;
	appsys.flag_hand_free=1;
	
	mu_set_voice_path(VOICE_PATH_HANDFREE);

	appsys.flag_sending_sms =appsys.flag_call_comming =appsys.flag_dialing =0;

    while ( 1 )
    {
		if(appsys.byte_need_redraw >0)
		{
             if(0xFF == appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
                 show_caption(get_multi_string((char * *) text_key_test));
				 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);
				 show_soft_key_prompt( get_multi_string((char * *) text_null), get_multi_string((char * *) text_back));
			 }
			 lcd_goto_x_line(3, 2);
			 lcd_clear_line(SCREEN_WIDTH-3);

			 memset(tmp_buf,0,sizeof(tmp_buf));
	
             if(byte_last_key >0)
             {
			     sprintf(tmp_buf,"[0x%.2X]%s",byte_last_key,key_str);
             }
		
			 
			 lcd_goto_x_line( (SCREEN_WIDTH-get_str_dots_width(tmp_buf))/2, 2);
			 lcd_put_string(tmp_buf);

			 appsys.byte_need_redraw =0;
		}
		else
		{
		     key_str=NULL;
		     if(1 == app_get_message( &EventPara) )
			 {
		        if(EVENT_KEY == EventPara.eventTyp )
				{
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    appsys.byte_need_redraw=1;

						byte_last_key = EventPara.sig_p.key_evt.key_val;
						
					    switch(EventPara.sig_p.key_evt.key_val)
					    {

						    case TFKEY_SOFT_LEFT:
							key_str="左导航";
							break;

							case TFKEY_SOFT_RIGHT:
							key_str="右导航";						
							msg("刚才您按下了 '右导航' ，退出测试.");
							delay_ms(1500);
							return;
							break;

							case TFKEY_ALARM:
                            key_str="闹钟";
							break;
							
                            case TFKEY_MUSIC:
                            key_str="铃声";
							break;

							case TFKEY_UP:
							key_str="方向键(上)";
							break;

							case TFKEY_LEFT:
							key_str ="方向键(左)";
							break;
								
							case TFKEY_DOWN:
							key_str="方向键(下)";
							break;

						  	case TFKEY_RIGHT:
							key_str="方向键(右)";
							break;

							
//						
							
								
							case TFKEY_1:
							key_str="1";
							break;
							
								
							case TFKEY_2:
							key_str="2";
							break;
							
								
							case TFKEY_3:
							key_str="3";
							break;
							
								
							case TFKEY_4:
							key_str="4";
							break;
							
								
							case TFKEY_5:
							key_str="5";
							break;
							
								
							case TFKEY_6:
							key_str="6";
							break;
							
								
							case TFKEY_7:
							key_str="7";
							break;
							
								
							case TFKEY_8:
							key_str="8";
							break;
							
								
							case TFKEY_9:
							key_str="9";
							break;
							
								
							case TFKEY_STAR:
							key_str="*";
							break;
							
								
							case TFKEY_0:
							key_str="0";
							break;
							
								
							case TFKEY_SHARP:
							key_str="#";
							break;



							case TFKEY_SEND:
							key_str="发送";
							break;	

						 	case TFKEY_POWER:
							key_str="开关机";
							break;	
         
							
							case TFKEY_MUTE:
							key_str="喇叭/MIC静音";
							break;	
							
							case TFKEY_REDIAL:
							key_str="重拨";
							break;		

						
							case TFKEY_HANDFREE:
							key_str="免提";
							break;

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

void app_echo_test()
{
    FlSignalBuffer EventPara;
    char * pstr_show=NULL;
	appsys.byte_need_redraw=0xFF;
	appsys.flag_key_tone =0;


	
	appsys.flag_voice_path=0xFF;
	appsys.flag_hand_free =1;
    mu_set_voice_path(VOICE_PATH_HANDFREE);





    while(1)
    {
        if(appsys.byte_need_redraw >0)
        {
             if(0xFF ==  appsys.byte_need_redraw)
             {
                 lcd_clear_screen();
				 show_caption(get_multi_string((char * *)text_echo_test));
                 lcd_put_rect(1, 12, SCREEN_WIDTH-1, 51);

				 lcd_goto_x_line(3 ,1);
				 
                 show_soft_key_prompt(get_multi_string((char * *) text_null) , get_multi_string((char * *) text_back));
             }
			 pstr_show =  (HOOK_STATE_OFFHOOK == appsys.flag_hook_state)? "摘机测试手柄":"挂机测试免提";

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
				             mu_set_voice_path(VOICE_PATH_HANDFREE);
							 mu_set_spk_gain(sysprop->byte_talk_volume);
							 
							 appsys.flag_key_tone = sysprop->flag_key_tone;
							 
							 return;
						}      
				 	}
		        }
				else if( EVENT_HANDSET == EventPara.eventTyp)
				{
                    if(HOOK_STATE_OFFHOOK == appsys.flag_hook_state)
                    {
                        mu_set_voice_path(VOICE_PATH_HANDSET);
					}
					else
					{
					    mu_set_voice_path(VOICE_PATH_HANDFREE);
					}


					
					appsys.byte_need_redraw=1;
				}
			}
		}		
	}
}
   



void app_hardware_test( void )
{
    unsigned short pos_info=0; 

    unsigned char bk_language = sysprop->byte_language_type;
	sysprop->byte_language_type =LANGUAGE_CHINESE;
    while ( 1 )
    {
        switch ( app_menu_select((char ***)mem_test,8,&pos_info) )
        {    
            case 1:
		        app_keyboard_test();
                break;              
            case 2:
                app_handset_test();
                break;                   
            case 3:
                app_handfree_test();
                break;                   
            case 4:
                app_lcd_test();
                break;
            case 5:
                app_battery_test();
                break;
			case 6:
			    show_device_power_info();
				break;
		  	case 7:
			    show_device_csq_level();
				break;		
		 	case 8:
			    app_echo_test();
				break;	
			default:
			    sysprop->byte_language_type =bk_language;
				return ;
				break;
        }
    } 

}












