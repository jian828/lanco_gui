
#include "inc/app_def.h"


extern T_PYINPUTCODE  gPYCode;

char * get_combined_string(char * str1, char * str2 , char * str3)
{
    unsigned char length=((NULL == str1)?0:strlen(str1))+((NULL == str2)?0:strlen(str2))+((NULL == str3)?0:strlen(str3));
    if(appsys.combined_buf !=NULL)
   	{
        app_free_memory(appsys.combined_buf);appsys.combined_buf=NULL;
	}
	appsys.combined_buf  = (char *)app_alloc_memory(length+1);
	memset(appsys.combined_buf,0, length+1);
    if(NULL != str1)strcat(appsys.combined_buf,str1);
	if(NULL != str2)strcat(appsys.combined_buf,str2);
	if(NULL != str3)strcat(appsys.combined_buf,str3);	
	return appsys.combined_buf;
}

void msg(char * disp_txt)
{
    wnd_show_notice(get_multi_string((char * *)text_prompt), disp_txt, NOTICE_TYPE_INFO, IGNORE_SOFT_KEY_INPUT);
}

void msg_area_ex(char * disp_txt, unsigned char line)
{
    unsigned char str_wdith= get_str_dots_width(disp_txt);

	lcd_goto_x_line(2, line);
	lcd_clear_line(SCREEN_WIDTH-2);
	lcd_goto_x_line((SCREEN_WIDTH-str_wdith)/2, line);
	lcd_put_string(disp_txt);	
}
void msg_area(char * disp_txt)
{
    msg_area_ex(disp_txt,5);
}

void msgok(char * disp_txt)
{
    wnd_show_notice(get_multi_string((char * *)text_succeed), disp_txt, NOTICE_TYPE_SUCCEED, IGNORE_SOFT_KEY_INPUT);
}

void msg_warn(char * disp_txt)
{
    wnd_show_notice(get_multi_string((char * *)text_warning), disp_txt, NOTICE_TYPE_ALARM, IGNORE_SOFT_KEY_INPUT);
}

unsigned char choose_yes_no(char * strcap, char * disp_txt)
{
    return wnd_show_notice(strcap, disp_txt, NOTICE_TYPE_QUESTION, NEED_SOFT_KEY_INPUT);
}


unsigned char wnd_show_notice(char * pcap, char * buf,unsigned char notice_style,unsigned char need_soft_key)
{
	EDITWND m_edit_wnd;
	unsigned char nret=0;
	memset(&m_edit_wnd,0,sizeof(EDITWND));
    m_edit_wnd.notice_style=notice_style;


	m_edit_wnd.frame_left = (notice_style >0)?32:2;
	m_edit_wnd.frame_top =20;
	m_edit_wnd.frame_right =SCREEN_WIDTH-2;
	m_edit_wnd.frame_bottom =45;
	m_edit_wnd.data_font=FONT_T_X_10;


    if(IGNORE_SOFT_KEY_INPUT == need_soft_key)
    {
        m_edit_wnd.soft_key_type= SOFT_KEY_TYPE_NULL_NULL_NULL;
	}
	else
    {
	    if(NOTICE_TYPE_QUESTION== notice_style)
	    {
		    m_edit_wnd.soft_key_type=SOFT_KEY_TYPE_NO_NULL_YES;
	    }
		else
		{
		    m_edit_wnd.soft_key_type= SOFT_KEY_TYPE_NULL_NULL_OK;
		}
    }
	
    m_edit_wnd.style= EDIT_STYLE_ALIGE_CENTER|EDIT_STYLE_SPLIT_WORDS|EDIT_STYLE_READONLY|EDIT_STYLE_HIDE_CURSOR;

	a2w( m_edit_wnd.data_buf, buf);
	m_edit_wnd.data_len=wstrlen(m_edit_wnd.data_buf);
	m_edit_wnd.max_len=m_edit_wnd.data_len;

    m_edit_wnd.pcaption=pcap;
    m_edit_wnd.data_pos=0;
 	m_edit_wnd.cursor_x_last=0;
	m_edit_wnd.cursor_y_last=0;

	m_edit_wnd.input_mode=IN_FIXED;

	if(1)// get lines info
	{
		m_edit_wnd.body_left=m_edit_wnd.frame_left;
		m_edit_wnd.body_top=m_edit_wnd.frame_top;
		m_edit_wnd.body_right=m_edit_wnd.frame_right;
		m_edit_wnd.body_bottom=m_edit_wnd.frame_bottom;

	    wnd_get_lines_info(&m_edit_wnd);

	    if(1  == (m_edit_wnd.line_cnt -1) )
	    {
			m_edit_wnd.frame_top =26;
			m_edit_wnd.frame_bottom =38;
		}
		else if(2  == (m_edit_wnd.line_cnt -1) )
		{
			m_edit_wnd.frame_top =20;
			m_edit_wnd.frame_bottom =46;
		}
		else if(3  <= (m_edit_wnd.line_cnt -1) )	
		{
			m_edit_wnd.frame_top =14;
			m_edit_wnd.frame_bottom =51;
		}
	}
 
    nret=wnd_edit_txt( &m_edit_wnd);   

	appsys.byte_need_redraw=0xFF;
	return nret;
}

unsigned char wnd_input_password(char * buf,  char * pcap,unsigned char maxlen)
{
	EDITWND m_edit_wnd;
	unsigned char x,y,nret;
	memset(&m_edit_wnd,0,sizeof(EDITWND));

    x=13;
	y=22;
	
	m_edit_wnd.frame_left =x+33;
	m_edit_wnd.frame_top =y+4;
	m_edit_wnd.frame_right =x+100;
	m_edit_wnd.frame_bottom =y+16;
	m_edit_wnd.data_font=FONT_B_X_10;

	
	m_edit_wnd.style= EDIT_STYLE_SHOW_FRAME|EDIT_STYLE_DIGITALONLY|EDIT_STYLE_PASSWD_CHAR;


	a2w(m_edit_wnd.data_buf, buf);
	m_edit_wnd.data_len=wstrlen(m_edit_wnd.data_buf);
	m_edit_wnd.max_len=maxlen;

	m_edit_wnd.max_bkp=maxlen;

    m_edit_wnd.pcaption=pcap;
    m_edit_wnd.data_pos=0;
 	m_edit_wnd.cursor_x_last=0;
	m_edit_wnd.cursor_y_last=0;
	m_edit_wnd.soft_key_type=SOFT_KEY_TYPE_DEL_CANCLE_OK;

	m_edit_wnd.input_mode=IN_FIXED;
 

	nret= wnd_edit_txt( &m_edit_wnd);
 	appsys.byte_need_redraw=0xFF;

    w2a(buf, m_edit_wnd.data_buf);
	
	return nret;

}




unsigned char wnd_input_decimal(char * buf,  char * pcap,unsigned char maxlen, unsigned short ex_style)
{
	EDITWND m_edit_wnd;
	unsigned char x,y,nret;
	memset(&m_edit_wnd,0,sizeof(EDITWND));

    x=5;
	y=19;

	lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);

	if(  (ex_style & EDIT_STYLE_IPADDRESSINPUT)>0  )
	{
		m_edit_wnd.frame_left =x+10;
		m_edit_wnd.frame_top =y+8;
		m_edit_wnd.frame_right =x+115;
		m_edit_wnd.frame_bottom =y+22;	
		
        m_edit_wnd.data_font=FONT_B_X_10;
	}
	else
	{
		m_edit_wnd.frame_left =x+15;
		m_edit_wnd.frame_top =y+3;
		m_edit_wnd.frame_right =x+113;
		m_edit_wnd.frame_bottom =y+23;	
	    m_edit_wnd.data_font=FONT_B_X_16;
	}

	
	m_edit_wnd.style= EDIT_STYLE_SHOW_FRAME|EDIT_STYLE_DIGITALONLY|EDIT_STYLE_DECIMALINPUT|ex_style;


	a2w(m_edit_wnd.data_buf, buf);
	m_edit_wnd.data_len=wstrlen(m_edit_wnd.data_buf);
	m_edit_wnd.max_len=maxlen;
	m_edit_wnd.max_bkp=maxlen;

    m_edit_wnd.pcaption=pcap;
    m_edit_wnd.data_pos=m_edit_wnd.data_len;
 	m_edit_wnd.cursor_x_last=0;
	m_edit_wnd.cursor_y_last=0;

	m_edit_wnd.input_mode=IN_FIXED;
	m_edit_wnd.soft_key_type=SOFT_KEY_TYPE_DEL_CANCLE_OK;
 

	nret= wnd_edit_txt( &m_edit_wnd);
    appsys.byte_need_redraw=0xFF;

    w2a(buf, m_edit_wnd.data_buf);
	return nret;

}


void app_show_normal_text(char * buf,  char * pcap)
{
    wnd_input_text(buf, pcap,  strlen(buf), IN_FIXED, EDIT_STYLE_READONLY); 
	appsys.byte_need_redraw=0xFF;
}



unsigned char wnd_input_text(char * buf,  char * pcap,unsigned char maxlen, unsigned char input_mode,unsigned short ex_style)
{
	EDITWND m_edit_wnd;
	unsigned char nret=0;
	memset(&m_edit_wnd,0,sizeof(EDITWND));

    m_edit_wnd.data_font=FONT_T_X_10;

	m_edit_wnd.frame_left =0;

	m_edit_wnd.frame_right =SCREEN_WIDTH-1;
	
	
 	m_edit_wnd.frame_top =12;
	m_edit_wnd.frame_bottom =51;
 	m_edit_wnd.style= EDIT_STYLE_SHOW_FRAME|EDIT_STYLE_VERTICAL_BAR| EDIT_STYLE_SPLIT_WORDS|ex_style;

	
	a2w(m_edit_wnd.data_buf, buf);
	m_edit_wnd.data_len=wstrlen(m_edit_wnd.data_buf);

	m_edit_wnd.max_bkp=maxlen;
	m_edit_wnd.max_len= (IN_PY5D == input_mode || IN_BX5D == input_mode || ( (LANGUAGE_CHINESE == sysprop->byte_language_type) && (IN_PHRA== input_mode)) )? ((m_edit_wnd.max_bkp/2)>70?70: (m_edit_wnd.max_bkp/2)): m_edit_wnd.max_bkp;
	

    m_edit_wnd.pcaption=pcap;
	m_edit_wnd.data_pos=  (m_edit_wnd.style& (EDIT_STYLE_SMS_FEATURE | EDIT_STYLE_READONLY))?0:m_edit_wnd.data_len;

	
 	m_edit_wnd.cursor_x_last=0;
	m_edit_wnd.cursor_y_last=0;
    m_edit_wnd.input_mode=input_mode;	

	if( (m_edit_wnd.style & EDIT_STYLE_READONLY) > 0)
	{
        m_edit_wnd.soft_key_type = SOFT_KEY_TYPE_NULL_NULL_OK;
	}
	else
	{
 	    m_edit_wnd.soft_key_type = SOFT_KEY_TYPE_DEL_CANCLE_OK;
	}
 
	nret = wnd_edit_txt( &m_edit_wnd);
   	appsys.byte_need_redraw=0xFF;

    w2a(buf, m_edit_wnd.data_buf);

	return nret;
}


unsigned char get_cursor_height()
{
    switch(appsys.byte_font_type)
	{
	   case FONT_B_X_16:
		   return 16;
		   break;

	   case FONT_T_X_10:
	   case FONT_B_X_10:
	   default:
		   return 11;
		   break;
	}
    return 11;
}


void wnd_put_cursor(EDITWND * p_edit_wnd)
{
    unsigned char data[8];
	unsigned char c_x,c_y;
	unsigned char height;


    appsys.byte_font_type= p_edit_wnd->data_font;
	
	lcd_goto_xy(p_edit_wnd->cursor_x,p_edit_wnd->cursor_y);
	c_x = lcd_get_x();
	c_y = lcd_get_y();
	height=get_cursor_height()%32;

    if(p_edit_wnd->cursor_x_last==0 && p_edit_wnd->cursor_y_last==0)
	{
	     p_edit_wnd->cursor_x_last=lcd_get_x();
		 p_edit_wnd->cursor_y_last=lcd_get_y();
	}
	else
	{
	     if(p_edit_wnd->cursor_x_last != c_x  ||  p_edit_wnd->cursor_y_last != c_y)
		 {
		     // must clear old one!
			 lcd_goto_xy(p_edit_wnd->cursor_x_last,p_edit_wnd->cursor_y_last);
			 memset(data, 0x00, 8);
			 lcd_display_data(1,height,data);
              
			 // restore position to current.
			 lcd_goto_xy(c_x,c_y);
			 p_edit_wnd->cursor_x_last=c_x;
			 p_edit_wnd->cursor_y_last=c_y;
		 }
		 
	     memset(data, p_edit_wnd->flag_show_cursor!=0? 0xFF:0x00, 8);
	     p_edit_wnd->flag_show_cursor = !p_edit_wnd->flag_show_cursor;

		 lcd_display_data(1,height,data);
	}
}



unsigned char * get_char_font_data(unsigned short data)
{
	unsigned char  data_idx;
	unsigned short data_pos;
	unsigned char  i=0;
    if(data<0x20  || ( data >=0x80 && data <=0xA0))
	{
	     return NULL;
	}
	else if(data >0x0100)
	{
         return get_uni_font(data);
	}
	else 
	{
        if(data >=0xA1)
        {
            data_idx=(data-0xA1)+(0x7F-0x20+1);      
		}
		else
		{
		    data_idx=data-0x20;
		}

        switch(appsys.byte_font_type)
        {
            case FONT_T_X_10:
				{
					data_pos = FONT_TX_10_ARRAY[data_idx];
					return   (unsigned char *)(&FONT_TX_10_BIN_TXT[data_pos]);
            	}
				break;
			case FONT_B_X_10:
				{
					data_pos = FONT_BX_10_ARRAY[data_idx];
					return   (unsigned char *)(&FONT_BX_10_BIN_TXT[data_pos]);
            	}				
				break;
			case FONT_B_X_16:
				{
/*
#define BMP_0X20_BIN (0) 
#define BMP_0X23_BIN (1) 
#define BMP_0X2A_BIN (2) 
#define BMP_0X2B_BIN (3) 
#define BMP_0X2E_BIN (4) 
#define BMP_0X30_BIN (5) 
#define BMP_0X31_BIN (6) 
#define BMP_0X32_BIN (7) 
#define BMP_0X33_BIN (8) 
#define BMP_0X34_BIN (9) 
#define BMP_0X35_BIN (10) 
#define BMP_0X36_BIN (11) 
#define BMP_0X37_BIN (12) 
#define BMP_0X38_BIN (13) 
#define BMP_0X39_BIN (14) 
*/

                    if( 0x23 == data_idx+0x20 )
					{
                         data_pos = FONT_BX_16_ARRAY[1];
					}
				    else if( 0x2A == data_idx+0x20 )
					{
                         data_pos = FONT_BX_16_ARRAY[2];
					}
				    else if( 0x2B == data_idx+0x20 )
					{
                         data_pos = FONT_BX_16_ARRAY[3];
					}
				    else if( 0x2E == data_idx+0x20 )
					{
                         data_pos = FONT_BX_16_ARRAY[4];
					}
					else if((data_idx+0x20 )>='0' && (data_idx+0x20)<='9'  )
					{
					    data_pos = FONT_BX_16_ARRAY[5 + (data_idx+0x20 -0x30)];
					}
					else
                    {
                         data_pos = FONT_BX_16_ARRAY[0];
					}

					return   (unsigned char *)(&FONT_BX_16_BIN_TXT[data_pos]);
            	}				
				break;
			default:
				return NULL;
 
		} 
	}
    return NULL;
}


void  wstr_insert_char(unsigned short * pdata, unsigned char max_len, unsigned char pos , unsigned short c)
{
 	int  i=0;

	for(i=max_len-2 ;i>=pos ;i--)
	{
	   pdata[i+1]=pdata[i];
	}
	pdata[pos]=c;
}

void  wstr_delete(unsigned short * pdata, unsigned char max_len, unsigned char pos )
{
 	unsigned char i=0;

	for(i=pos;i<max_len -1 ;i++)
	{
	   pdata[i]=pdata[i+1];
	}
	pdata[i]=0;
}



unsigned char wnd_wstr_insert_str(EDITWND * p_edit_wnd, char * str_add)
{
    unsigned char i=0;
    if( (p_edit_wnd->style & EDIT_STYLE_READONLY) == 0)
    {
        unsigned short * pshort = a2w(NULL, str_add);
        unsigned char add_len =wstrlen(pshort);
		if(p_edit_wnd->data_len + add_len < p_edit_wnd->max_len )
		{
            for(i=0;i<add_len;i++)
            {
			    wstr_insert_char(p_edit_wnd->data_buf, p_edit_wnd->max_len, p_edit_wnd->data_pos , pshort[i]);
			    p_edit_wnd->data_len=wstrlen(p_edit_wnd->data_buf);
				p_edit_wnd->data_pos++;
		    }
			wnd_get_lines_info( p_edit_wnd);

			appsys.byte_need_redraw|=(EDIT_REDRAW_DATA|EDIT_REDRAW_SIZE);
		    return 1;
		}
	}
	return 0;
}


unsigned char wnd_wstr_insert_char(EDITWND * p_edit_wnd, unsigned short c)
{
    unsigned char flag_insert_middle=0;
	unsigned char bak_line_cnt=p_edit_wnd->line_cnt;

	
	
    if( (p_edit_wnd->style & EDIT_STYLE_READONLY) == 0)
    {
		if(p_edit_wnd->data_len < p_edit_wnd->max_len )
		{
            flag_insert_middle =(p_edit_wnd->data_len == p_edit_wnd->data_pos)?0:1;

		    wstr_insert_char(p_edit_wnd->data_buf, p_edit_wnd->max_len, p_edit_wnd->data_pos , c);

		    p_edit_wnd->data_len=wstrlen(p_edit_wnd->data_buf);
			p_edit_wnd->data_pos++;
			wnd_get_lines_info( p_edit_wnd);

			if( ( p_edit_wnd->line_start +p_edit_wnd->body_lines) < p_edit_wnd->line_cnt-1)
			{
			    if(p_edit_wnd->data_pos >= p_edit_wnd->line_pos[ p_edit_wnd->line_start +p_edit_wnd->body_lines ] )
				{
					p_edit_wnd->line_start++; 	
				}	
			}	
#if 0
			if( (1 ==flag_insert_middle) || (p_edit_wnd->line_cnt != bak_line_cnt))
			{
                appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			}
			else
#endif
			if( (p_edit_wnd->style&EDIT_STYLE_SMS_FEATURE)>0 )
			{
                appsys.byte_need_redraw|=EDIT_REDRAW_DATA_PTL;
			    appsys.byte_need_redraw|=EDIT_REDRAW_SIZE;
			}
			else
			{
                appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			   
			}
		    return 1;
		}
		if( 0 == (p_edit_wnd->style& EDIT_STYLE_NUM_PARTICIAL))
		{
			wnd_show_notice(get_multi_string((char * *)text_prompt), get_multi_string((char * *)text_data_buffer_is_full), NOTICE_TYPE_FORBIDDEN,NEED_SOFT_KEY_INPUT);
		    appsys.byte_need_redraw=0xFF;
		}
	}
	return 0;
}
unsigned char wnd_str_insert_str(EDITWND * p_edit_wnd, char * str_add)
{
    unsigned char i=0;
    if( (p_edit_wnd->style & EDIT_STYLE_READONLY) == 0)
    {
        
		unsigned short * pshort = a2w(NULL,str_add);
		unsigned char add_len   = wstrlen(pshort);
	
		if(p_edit_wnd->data_len + add_len < p_edit_wnd->max_len )
		{
            for(i=0;i<add_len;i++)
            {
			    wstr_insert_char(p_edit_wnd->data_buf, p_edit_wnd->max_len, p_edit_wnd->data_pos , pshort[i]);
			    p_edit_wnd->data_len=wstrlen(p_edit_wnd->data_buf);
				p_edit_wnd->data_pos++;
		    }
			wnd_get_lines_info( p_edit_wnd);

			appsys.byte_need_redraw|=(EDIT_REDRAW_DATA|EDIT_REDRAW_SIZE);
		    return 1;
		}
	}
	return 0;
}


unsigned char wnd_batch_wstr_delete(EDITWND * p_edit_wnd)
{
    // if direction is 0, then it is backspace, else it is delete
    if( (p_edit_wnd->style & EDIT_STYLE_READONLY) == 0)
    {
		memset(p_edit_wnd->data_buf,0, p_edit_wnd->max_len);
       	p_edit_wnd->data_len=0;
		p_edit_wnd->data_pos=0;
		p_edit_wnd->line_start=0;

		wnd_get_lines_info( p_edit_wnd);
		appsys.byte_need_redraw|=(EDIT_REDRAW_DATA|EDIT_REDRAW_SIZE);
		return 1;
    }
    return 0;
}

unsigned char wnd_wstr_delete(EDITWND * p_edit_wnd)
{
    // if direction is 0, then it is backspace, else it is delete
    
    if( (p_edit_wnd->style & EDIT_STYLE_READONLY) == 0)
    {
		if(p_edit_wnd->data_len >0 )
		{
			wstr_delete(p_edit_wnd->data_buf, p_edit_wnd->max_len, (p_edit_wnd->data_pos >0)?(p_edit_wnd->data_pos -1) : p_edit_wnd->data_pos );
			
	       	p_edit_wnd->data_len=wstrlen(p_edit_wnd->data_buf);

			if(p_edit_wnd->data_pos>0)
			{
			    p_edit_wnd->data_pos--;
			}
			wnd_get_lines_info( p_edit_wnd);
			appsys.byte_need_redraw|=(EDIT_REDRAW_DATA|EDIT_REDRAW_SIZE);
			return 1;
		}
    }
    return 0;
}



void show_vertical_bar(VERTICALBAR * pbar)
{
   unsigned char data[24]; // could max to w*h =8*3
   unsigned char scoll_range ,scoll_y;
   unsigned short tmpval=0;
   int scoll_height;


   lcd_put_rect(pbar->left,pbar->top,pbar->right,pbar->bottom ); 
  	

   data[0]=0x20;
   data[1]=0x60;
   data[2]=0x20;
   lcd_goto_xy(pbar->left+1, pbar->top+1);
   lcd_display_data(3, 3, data);


   data[0]=0x80;
   data[1]=0xC0;
   data[2]=0x80;
  
   lcd_goto_xy(pbar->left+1, pbar->bottom-3);
   lcd_display_data(3, 3, data);

   //clear anyway
   scoll_range = pbar->bottom- pbar->top-8;
   memset(data,0x00,24);
   lcd_goto_xy( pbar->left+1,pbar->top+4);//decimal org:+5
   lcd_display_data(3,scoll_range, data);//decimal org : no have +2

   if(pbar->body_lines< pbar->line_cnt)
   {

        tmpval=pbar->body_lines;
		tmpval*=scoll_range;
		tmpval/=pbar->line_cnt;
         
		scoll_height= (unsigned char)tmpval ;
		if(scoll_height==0)
		{
            scoll_height=1;
		}
		
        tmpval=pbar->cur_line ;
		tmpval*=(scoll_range-scoll_height);
		tmpval/=(pbar->line_cnt -1);

		scoll_y=pbar->top+5+ (unsigned char)tmpval;

        memset(data,0xff,24);
		lcd_goto_xy(pbar->left+1, scoll_y);

		if(scoll_height > (pbar->bottom -4 -scoll_y) )
		{
           scoll_height = pbar->bottom -4 -scoll_y;//safety height
		}
		if(scoll_height>0)
		{
		    lcd_display_data(3,scoll_height, data);	
		}
   }  
}

void wnd_get_lines_info(EDITWND * p_edit_wnd)
{
	unsigned char x,y,h,i,w,box_width,tmp;
	int lblk_pos;
	unsigned char * pdata=0;
	unsigned char w_char_slot=0;
	x=y=h=i=w=box_width=tmp=0;
	h= get_cursor_height();

    box_width = p_edit_wnd->body_right -p_edit_wnd->body_left-1;
	p_edit_wnd->cursor_x=p_edit_wnd->body_left;
	p_edit_wnd->cursor_y=p_edit_wnd->body_top;


 
    memset(p_edit_wnd->line_pos,0,40);
	p_edit_wnd->line_cnt=0;
	lblk_pos=-1; //last blank pos
	w=0;
	
	p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=0;
    for(i=0;i<p_edit_wnd->data_len && p_edit_wnd->line_cnt<39  ;i++)
   	{
   	   w_char_slot =0;
	   
       if(p_edit_wnd->data_buf[i]=='\n')
       {
           p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=i+1 ;
		   
		   lblk_pos=-1;
		   w=0;
	   }
	   else 
       {
           if(p_edit_wnd->style & EDIT_STYLE_PASSWD_CHAR)
           {
               pdata=get_char_font_data('*');
		   }
		   else
		   {
		       pdata=get_char_font_data(p_edit_wnd->data_buf[i]);

			   if(p_edit_wnd->data_buf[i]<0xFF && p_edit_wnd->data_buf[i+1] >0xFF)
			   {
                  w+=1; // ansi +1 when next is chs
			   }
		   }

		   if(pdata != 0)
		   {
                if( (p_edit_wnd->style & EDIT_STYLE_SPLIT_WORDS) >0  )
                {		      
				   if( (w+pdata[0] +w_char_slot) >box_width)
				   {
					   if(lblk_pos==-1  || p_edit_wnd->data_buf[i] ==' ' )
					   {
						   p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=i;
						   lblk_pos=-1;
						   
						   w=pdata[0]+w_char_slot; // it is same to {w=0; i--; }				   
					   }
					   else
					   {
						   //try to move the word to next line
						   tmp=i-lblk_pos-1;
							
						   p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=i-tmp;
						   lblk_pos=-1;
						   w=0;
						   i-=(tmp+1)  ;
					   }
				   }
				   else
				   {
					   if(p_edit_wnd->data_buf[i]==' ')
					   {
						   lblk_pos=i;
					   }
					   w+=pdata[0]+w_char_slot;
				   }
                }
				else
				{
				   if(w+pdata[0]+w_char_slot>box_width)
				   {
					   p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=i;
					   w=pdata[0]+w_char_slot; // it is same to {w=0; i--; }				   
				   }
				   else 
				   {
                       w+=pdata[0]+w_char_slot;
				   }
				}
		   }
	   }
	}
 
	p_edit_wnd->line_pos[p_edit_wnd->line_cnt++]=p_edit_wnd->data_len;// last pos;

	if(p_edit_wnd->notice_style>0)
	{
         if(p_edit_wnd->line_cnt>4)
         {
             p_edit_wnd->line_cnt=4;
		 }
	}
}
void wnd_mark_cursor( EDITWND * p_edit_wnd)
{
    if((p_edit_wnd ->data_pos >0 &&   p_edit_wnd ->data_pos <= p_edit_wnd ->data_len
    &&(p_edit_wnd ->data_buf[p_edit_wnd ->data_pos -1 ] >0x0100  || ((p_edit_wnd ->data_buf[p_edit_wnd ->data_pos -1 ])<0x0100  && (p_edit_wnd ->data_buf[p_edit_wnd ->data_pos  ])>0x0100  )  ))
    || ( (0 == p_edit_wnd ->data_pos)  &&  (p_edit_wnd->style & EDIT_STYLE_VERTICAL_BAR)>0 )
	)
    {
	   p_edit_wnd->cursor_x=lcd_get_x()-1;
    }
	else
	{
       p_edit_wnd->cursor_x=lcd_get_x();
	}
	
	p_edit_wnd->cursor_y=lcd_get_y();
}

void wnd_data_redraw( EDITWND * p_edit_wnd)
{
	unsigned char x,y,h,i,w,box_width,tmp;
	unsigned char * pdata=0;
	x=y=h=i=w=box_width=tmp=0;

	h= get_cursor_height();

    box_width = p_edit_wnd->body_right -p_edit_wnd->body_left;

    if(0 == p_edit_wnd->data_len)
    {
		p_edit_wnd->cursor_x=p_edit_wnd->body_left-1;
	    p_edit_wnd->cursor_y=p_edit_wnd->body_top;
    }
	
	lcd_goto_xy(p_edit_wnd->body_left, p_edit_wnd->body_top);
	
    for(i=p_edit_wnd->line_start ;i<p_edit_wnd->line_cnt-1;i++)
    {
       if(i-p_edit_wnd->line_start<p_edit_wnd->body_lines)
       {
          y=p_edit_wnd->body_top+(i-p_edit_wnd->line_start)*(h+1);

 		  if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_MIDDLE)>0  )
          {

              lcd_goto_xy( p_edit_wnd->body_left ,y );
			  lcd_clear_line_ex((h+1), p_edit_wnd->body_right);	
			  
              w=get_wstr_dots_width_ex( &p_edit_wnd->data_buf[p_edit_wnd->line_pos[i]],p_edit_wnd->line_pos[i+1]-p_edit_wnd->line_pos[i] );
			  lcd_goto_xy( p_edit_wnd->body_left + (box_width-w)/2 ,y );
		  }
		  else if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_RIGHT)>0 )
		  {
		      lcd_goto_xy( p_edit_wnd->body_left ,y );
			  lcd_clear_line(p_edit_wnd->body_right);
			  
              w=get_wstr_dots_width_ex( &p_edit_wnd->data_buf[p_edit_wnd->line_pos[i]],p_edit_wnd->line_pos[i+1]-p_edit_wnd->line_pos[i] );
			  lcd_goto_xy( p_edit_wnd->body_right -w ,y );
			  p_edit_wnd->cursor_x=p_edit_wnd->body_right;
			  p_edit_wnd->cursor_y=y;	
		  }
		  else if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_CENTER)>0 )
		  {
              if(p_edit_wnd->line_cnt-2==0 &&  p_edit_wnd->data_len>0)
              {
                  //only one line,so show to the center
                  y= p_edit_wnd->body_top+ (p_edit_wnd->body_bottom - p_edit_wnd->body_top -h)/2;
	              lcd_goto_xy( p_edit_wnd->body_left ,y );
				  lcd_clear_line(p_edit_wnd->body_right);
				  
	              w=get_wstr_dots_width_ex( &p_edit_wnd->data_buf[p_edit_wnd->line_pos[i]],p_edit_wnd->line_pos[i+1]-p_edit_wnd->line_pos[i] );
				  lcd_goto_xy( p_edit_wnd->body_left + (box_width-w)/4 ,y );				  
			  }
			  else
			  {
			      p_edit_wnd->style &= ~EDIT_STYLE_ALIGE_CENTER;
                  lcd_goto_xy( p_edit_wnd->body_left ,y );
			  }
		  }
		  else
		  {
              lcd_goto_xy( p_edit_wnd->body_left ,y );
			  lcd_clear_line(p_edit_wnd->body_right);
		      lcd_goto_xy( p_edit_wnd->body_left ,y );
		  }


		  for(x=p_edit_wnd->line_pos[i];  x<p_edit_wnd->line_pos[i+1] ; x++ )
		  {
			  if(x  ==  p_edit_wnd ->data_pos)
			  {
                  wnd_mark_cursor(p_edit_wnd);
				  p_edit_wnd->cur_line=i-p_edit_wnd->line_start;
				  
			  }	
              if( (p_edit_wnd->style & EDIT_STYLE_PASSWD_CHAR)>0 )
              {
                  w = lcd_put_char('*');
			  }
			  else
			  {		  
                  w = lcd_put_wchar(p_edit_wnd->data_buf[x]);
				  
				  if(p_edit_wnd->data_buf[x]<0xFF && p_edit_wnd->data_buf[x+1] >0xFF)
			      {
                      w+=1; // ansi +1 when next is chs
			      }
			  }
			  

			  lcd_add_x(w);

			  //check if to the end;
			  if(   (x == p_edit_wnd ->data_len- 1)  &&  (p_edit_wnd ->data_len == p_edit_wnd ->data_pos)  )
			  {
                  if(p_edit_wnd->data_buf[x] == '\n')
				  {
					  if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_MIDDLE)>0  )
					  {
    					  p_edit_wnd->cursor_x=p_edit_wnd->body_left+(p_edit_wnd->body_right-p_edit_wnd->body_left)/2 ;
						  p_edit_wnd->cursor_y=lcd_get_y()+h;
					  }
					  else if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_RIGHT)>0  )
					  {
    					  p_edit_wnd->cursor_x=p_edit_wnd->body_right;
						  p_edit_wnd->cursor_y=lcd_get_y()+h;
					  }
					  else
					  {
						  p_edit_wnd->cursor_x=p_edit_wnd->body_left;
						  p_edit_wnd->cursor_y=lcd_get_y()+h;
					  }
					  
					  if(p_edit_wnd->cur_line<p_edit_wnd->body_lines-1)
					  {
					      p_edit_wnd->cur_line=i-p_edit_wnd->line_start+1;
					  }
				  }
				  else
				  {
                      wnd_mark_cursor(p_edit_wnd);
					  p_edit_wnd->cur_line=i-p_edit_wnd->line_start;
				  }
			  }
		  }
		  lcd_clear_line(p_edit_wnd->body_right);
	   }
	}

	if( (p_edit_wnd->style & EDIT_STYLE_ALIGE_CENTER) == 0  )
	{
		//clear remained blank space
		while(i-p_edit_wnd->line_start<p_edit_wnd->body_lines)
		{
		    y=p_edit_wnd->body_top+(i-p_edit_wnd->line_start)*(h+1);
	        lcd_goto_xy( p_edit_wnd->body_left ,y );
			lcd_clear_line(p_edit_wnd->body_right);
			i++;	
		}
	}
    if( (p_edit_wnd->style & EDIT_STYLE_HIDE_CURSOR) ==0   )
    {
        wnd_put_cursor(p_edit_wnd);
	}
	
    if(p_edit_wnd->style & EDIT_STYLE_VERTICAL_BAR)
    {
       VERTICALBAR bar;
	   bar.left  =p_edit_wnd->body_right+1;
	   bar.top   =p_edit_wnd->frame_top;
	   bar.right =p_edit_wnd->frame_right;
	   bar.bottom=p_edit_wnd->frame_bottom;
	   bar.line_cnt =p_edit_wnd->line_cnt-1;
	   bar.body_lines=p_edit_wnd->body_lines;
	   bar.cur_line=p_edit_wnd->line_start+ p_edit_wnd->cur_line;
	   
       show_vertical_bar(&bar);
    }


    lcd_update_screen_area(p_edit_wnd->body_left,p_edit_wnd->body_top,p_edit_wnd->frame_right,p_edit_wnd->body_bottom);
	
}



unsigned char get_wchar_width(unsigned short c)
{
    unsigned char * p =NULL;
	unsigned char flag_slot=0;
	unsigned char ret_width =0 ;

    flag_slot =(c >=0x0100)?1:0;

	p=get_char_font_data(c);

	if(NULL !=p )
	{
        ret_width =p[0];
	}

	return ret_width +flag_slot;
}

unsigned char get_wstr_dots_width_ex( unsigned short * pdata, unsigned char length)
{
   unsigned char cnt=0;
   unsigned char i=0;
   unsigned char * p;
   

   if(length>wstrlen(pdata))
   {
       length=wstrlen(pdata);
   }
   
   
   for(i=0;i<length;i++)
   {
       cnt+=get_wchar_width(pdata[i]);

	   if(cnt>SCREEN_WIDTH)
	   {
	      break;
	   }
   }
   
   return cnt;
}
unsigned char get_wstr_dots_width( unsigned short * pdata)
{
    return get_wstr_dots_width_ex(pdata, wstrlen(pdata));
}


unsigned char get_str_dots_width_ex( char * pdata, unsigned char length)
{
   unsigned char ret_width =0;
   char * pmem = (char *)app_alloc_memory(length+1);
   unsigned short * pshort =NULL;

   memset(pmem,0, length+1);
   strncpy(pmem, pdata,length);
   pshort = a2w(NULL,pmem);

   ret_width = get_wstr_dots_width( pshort);

   app_free_memory(pmem);pmem=NULL;
   return ret_width;
}

unsigned char get_str_dots_width( char * pdata)
{
    return get_str_dots_width_ex(pdata, strlen(pdata));
}


void show_inputmode(char * pcap ,unsigned char inputmode)
{
	appsys.byte_font_type=FONT_B_X_10;
	lcd_goto_xy(0,0);
	lcd_clear_line((SCREEN_WIDTH-get_str_dots_width(pcap))/2-1 );

    appsys.byte_font_type=FONT_T_X_10;
    lcd_goto_xy(0,0);
    if(IN_123 == inputmode)
    {
        lcd_put_string("123");
    }
    else if(IN_ABC ==inputmode)
    {
        lcd_put_string("ABC");
    }
    else if(IN_abc ==inputmode)
    {
        lcd_put_string("abc");
    }
    else if(IN_SIGN==inputmode)
    {
        lcd_put_string("abc");
    } 
	else if(IN_PY5D==inputmode)
	{
        lcd_put_string("Æ´Òô");
	}	
	else if(IN_BX5D==inputmode)
	{
        lcd_put_string("±Ê»­");
	}		
}
void show_remained_bytes(char * pcap ,unsigned char cur_bytes,unsigned char total_bytes)
{
    char tmp_buf[8];

	appsys.byte_font_type=FONT_B_X_10;
	lcd_goto_xy( (SCREEN_WIDTH+get_str_dots_width(pcap))/2  ,0);
	lcd_clear_line(SCREEN_WIDTH-1);

   
    appsys.byte_font_type=FONT_T_X_10;
    sprintf(tmp_buf,"%d/%d",cur_bytes,total_bytes);

    lcd_goto_xy(SCREEN_WIDTH- get_str_dots_width(tmp_buf)-1,0);
    lcd_put_string(tmp_buf);
}

void show_caption(char * pcap)
{
	unsigned char x,y,w;
	
	appsys.byte_font_type =FONT_T_X_10;

    x=y=w=0;

	w=get_str_dots_width(pcap);	
	x=0+(SCREEN_WIDTH-w)/2;
	lcd_goto_xy(0,0);
	lcd_clear_line(SCREEN_WIDTH-1);

    lcd_show_to_colum(pcap, x, y, SCREEN_WIDTH-1);

	appsys.byte_font_type =FONT_T_X_10;
	
}

unsigned char wnd_edit_init( EDITWND * p_edit_wnd)
{

	if(0xFF == appsys.byte_need_redraw)
	{
        lcd_clear_screen();
		
	    appsys.byte_font_type=p_edit_wnd->data_font;

	    if( (p_edit_wnd->style & EDIT_STYLE_PASSWD_CHAR) >0)
		{
	        wnd_show_password_style();
		}
		else if((p_edit_wnd->style & EDIT_STYLE_DECIMALINPUT) >0)
		{
	        wnd_show_decimal_style();
		}
		
	    lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
		if(p_edit_wnd->notice_style>0)
		{
		
			lcd_goto_xy(5,20);

			if(NOTICE_TYPE_INFO == p_edit_wnd->notice_style)
			{
		       show_bitmap(BMP_NOTICE_BIN);
			}
			else if(NOTICE_TYPE_FORBIDDEN== p_edit_wnd->notice_style)
			{
		       show_bitmap(BMP_FORBIDDEN_BIN);
			}	
			else if(NOTICE_TYPE_ALARM == p_edit_wnd->notice_style)
			{
		       show_bitmap(BMP_WARNING_BIN);
			}
			else if(NOTICE_TYPE_QUESTION== p_edit_wnd->notice_style)
			{
		       show_bitmap(BMP_ASK_BIN);
			}
			else if(NOTICE_TYPE_SUCCEED== p_edit_wnd->notice_style)
			{
		       show_bitmap(BMP_SUCCEED_BIN);
			}
			else if(NOTICE_TYPE_POWEROFF== p_edit_wnd->notice_style)
			{
               show_bitmap(BMP_POWERDOWN_BIN); 
			}			
		}
	 	
		if(p_edit_wnd->style & EDIT_STYLE_SHOW_FRAME)
		{
			lcd_put_rect(p_edit_wnd->frame_left,p_edit_wnd->frame_top  ,p_edit_wnd->frame_right ,p_edit_wnd->frame_bottom ); 
			
			p_edit_wnd->body_left=p_edit_wnd->frame_left+2;
			p_edit_wnd->body_top=p_edit_wnd->frame_top+1;
			p_edit_wnd->body_right=p_edit_wnd->frame_right-5;
			p_edit_wnd->body_bottom=p_edit_wnd->frame_bottom-1;

		}
		else
		{
			p_edit_wnd->body_left=p_edit_wnd->frame_left;
			p_edit_wnd->body_top=p_edit_wnd->frame_top;
			p_edit_wnd->body_right=p_edit_wnd->frame_right;
			p_edit_wnd->body_bottom=p_edit_wnd->frame_bottom;
		}
	  
		p_edit_wnd->body_lines=  (p_edit_wnd->body_bottom-p_edit_wnd->body_top+1)/get_cursor_height();
	}

	
	if( (appsys.byte_need_redraw & EDIT_REDRAW_CAPTION) >0)
	{
		show_caption(p_edit_wnd->pcaption);
	}
	if( (appsys.byte_need_redraw & EDIT_REDRAW_BOTTOM) >0)
	{
	    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK)
	    {
	        if(p_edit_wnd ->data_len >0)
	        {
		        show_soft_key_prompt(get_multi_string((char * *)text_ok), get_multi_string((char * *)text_del));
	        }
			else
			{
			    show_soft_key_prompt(get_multi_string((char * * )text_ok), get_multi_string((char * * )text_back));
			}
		}
		else if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NO_NULL_YES)
		{
		    show_soft_key_prompt(get_multi_string((char * *)text_yesno_yes), get_multi_string((char * *)text_yesno_no));
		}
		else if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NULL_NULL_OK)
		{
		    show_soft_key_prompt(get_multi_string((char * *)text_null), get_multi_string((char * *)text_back));
		}
	}
	if( (appsys.byte_need_redraw & EDIT_REDRAW_INPUTMODE) >0)
	{
        show_inputmode(p_edit_wnd->pcaption, p_edit_wnd->input_mode);
	}
	if((p_edit_wnd->style & EDIT_STYLE_SMS_FEATURE )>0   &&  (appsys.byte_need_redraw & EDIT_REDRAW_SIZE) >0  )
	{
        show_remained_bytes(p_edit_wnd->pcaption, p_edit_wnd->data_len, p_edit_wnd->max_len);
	}	
	else
	{
	    appsys.byte_font_type =FONT_B_X_10;
		lcd_goto_xy( (SCREEN_WIDTH+get_str_dots_width(p_edit_wnd->pcaption))/2  ,0);
		lcd_clear_line(SCREEN_WIDTH-1);
		appsys.byte_font_type= p_edit_wnd->data_font;
	}
	if( (appsys.byte_need_redraw & EDIT_REDRAW_DATA) >0)
	{
	    wnd_get_lines_info(p_edit_wnd);
		wnd_data_redraw(p_edit_wnd);
	}

    appsys.byte_need_redraw=0;

	if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NULL_NULL_NULL)
	{
        return 0;
	}
	return 1;
}



void wnd_cursor_move(EDITWND * p_edit_wnd,unsigned char key_code)
{
    if( (p_edit_wnd->style & EDIT_STYLE_DECIMALINPUT) > 0 || (p_edit_wnd->style & EDIT_STYLE_PASSWD_CHAR) > 0  || (p_edit_wnd->style & EDIT_STYLE_HIDE_CURSOR)>0)
    {
        // decimal mode or password mode cannot move cursor because it is no sense,and will make code rot
        return;       
    }
	
    switch(key_code)
    {
		case TFKEY_LEFT:
		{
		 	if(p_edit_wnd->data_pos>0)
			{
				p_edit_wnd->data_pos--;

	            if(p_edit_wnd->data_pos < p_edit_wnd->line_pos[ p_edit_wnd->line_start ] )
				{
					p_edit_wnd->line_start--; 
				}
				appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			}
		}
			break;

		case TFKEY_RIGHT:
			if(p_edit_wnd->data_pos<p_edit_wnd->data_len)
			{
				p_edit_wnd->data_pos++;


	            if( ( p_edit_wnd->line_start +p_edit_wnd->body_lines) < p_edit_wnd->line_cnt-1)
	            {
		            if(p_edit_wnd->data_pos >= p_edit_wnd->line_pos[ p_edit_wnd->line_start +p_edit_wnd->body_lines ] )
					{
						p_edit_wnd->line_start++; 
					}
	            }
				appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			}
			break;

			
		case TFKEY_UP:
	        if(p_edit_wnd->cur_line ==0 )
			{ 
				 if(p_edit_wnd->line_start>0)
				 {
					  p_edit_wnd->line_start--;
					  p_edit_wnd->data_pos=p_edit_wnd->line_pos[p_edit_wnd->line_start];

					  appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
				 }
			}
			else
			{  
			    unsigned char st_pos=0, et_pos=0 ,dt_pos=0;
				st_pos= p_edit_wnd->line_pos[(p_edit_wnd->line_start+p_edit_wnd->cur_line)-1];
				et_pos= p_edit_wnd->line_pos[(p_edit_wnd->line_start+p_edit_wnd->cur_line)]-1;

				dt_pos= p_edit_wnd->data_pos-  p_edit_wnd->line_pos[(p_edit_wnd->line_start+p_edit_wnd->cur_line)];
	            p_edit_wnd->data_pos = (st_pos+ dt_pos <et_pos)? (st_pos+ dt_pos):et_pos;
			
				appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			}

			break;

		case TFKEY_DOWN:
	        if(p_edit_wnd->cur_line == p_edit_wnd->body_lines-1 )
			{
				 if( ( p_edit_wnd->line_start +p_edit_wnd->body_lines) < p_edit_wnd->line_cnt-1)
				 {
					  p_edit_wnd->line_start++;
					  p_edit_wnd->data_pos=p_edit_wnd->line_pos[p_edit_wnd->line_start+ p_edit_wnd->body_lines-1];
					  
	    			  appsys.byte_need_redraw|=EDIT_REDRAW_DATA;  
				 }
			}
			else
			{ 
			    unsigned char st_pos=0, dt_pos=0;
				st_pos= p_edit_wnd->line_pos[(p_edit_wnd->line_start+p_edit_wnd->cur_line)+1];
				dt_pos= p_edit_wnd->data_pos- p_edit_wnd->line_pos[(p_edit_wnd->line_start+p_edit_wnd->cur_line)];
				p_edit_wnd->data_pos = (st_pos+ dt_pos < p_edit_wnd->data_len)? (st_pos+ dt_pos):p_edit_wnd->data_len;

				appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
			}
			break;
		default:
			break;
	}

}

unsigned char get_input_mode( EDITWND *  p_edit_wnd)
{
   unsigned char i=0,nret=0;
   unsigned char input_md=p_edit_wnd->input_mode;
   //const unsigned char mode_array[]={IN_123,IN_ABC,IN_abc,IN_PHRA,};


   if(LANGUAGE_CHINESE  == sysprop->byte_language_type)
   {
	   const unsigned char mode_array[]={IN_123,IN_ABC,IN_abc,IN_PY5D,IN_BX5D};

	   for(i=0;i<5;i++)
	   {
	       if(mode_array[i] == p_edit_wnd->input_mode)
	       {
			   nret=(i+1)%5;
			   break;
		   }
	   }

	   input_md = mode_array[nret];

	   InitLXIndexTbl();
	   InitPYIndexTbl();
	   InitBHIndexTbl();
	   p_edit_wnd->max_len= (IN_PY5D == mode_array[nret] || IN_BX5D == mode_array[nret] || ( (LANGUAGE_CHINESE == sysprop->byte_language_type) && (IN_PHRA== mode_array[nret]) ))? ( (p_edit_wnd->max_bkp/2) >70?70: (p_edit_wnd->max_bkp/2)) : p_edit_wnd->max_bkp;

   }
   else
   {
	   const unsigned char mode_array[]={IN_123,IN_ABC,IN_abc};

	   for(i=0;i<3;i++)
	   {
	       if(mode_array[i] == p_edit_wnd->input_mode)
	       {
			   nret=(i+1)%3;
			   break;
		   }
	   }
	   input_md = mode_array[nret];

	   p_edit_wnd->max_len = p_edit_wnd->max_bkp;
   }

   appsys.byte_need_redraw|=EDIT_REDRAW_INPUTMODE|EDIT_REDRAW_SIZE;
   
   return input_md;
}


unsigned short wnd_get_sign_option(EDITWND * p_edit_wnd)
{
    unsigned char i,x,y,cur_pos,length,w,h;

    FlSignalBuffer EventPara;
	
    char sign_chars[]= {'\n','.','*','#','!','"','$','%','&','\'','(',')','+',',','-','/',':',';','<','=','>','?','@','[','\\',']','^','_','`','{','|','}','~','\0'}; // ",.;?!+-*/=@#$\\%&(){}:<>|[]\"\\^~`'";  //all 31 char



	#define CHAR_SLOT 11

    cur_pos=0;
	appsys.byte_need_redraw=0xFF;

	while(1)
	{
        if(	appsys.byte_need_redraw>0)
        {
             if(0xFF == appsys.byte_need_redraw)
             {
                lcd_clear_screen();
				length=strlen(sign_chars);	
				h=get_cursor_height();
				show_caption(get_multi_string( (char * * )text_select));
				show_soft_key_prompt(get_multi_string( (char * * )text_select), get_multi_string( (char * * )text_back));
				lcd_put_rect(0, 12, SCREEN_WIDTH-1, 50);
			 }
             appsys.byte_need_redraw=0;
             x=4;
			 y=14;
             for(i=0;i<length;i++)
             {
                 if( i>0  && i %11 == 0)
                 {
                     x=4;
					 y+=CHAR_SLOT;
				 }
				 lcd_goto_xy(x, y);

                 appsys.flag_color_reverse= (cur_pos == i)? 1:0;
				 if(0 == i)
				 {
                     unsigned short xy= show_bitmap(BMP_RETURN_BIN);
					 w= (xy>>8)&0xFF;
				 }
				 else if(1 == i)
				 {

                     if(IN_PY5D== p_edit_wnd->input_mode)
                     {
                         w=lcd_put_wchar(0xA1A3);
					 }
					 else
					 {
                         w=lcd_put_char(sign_chars[i]);
					 }
				 }
				 else
				 {
				     w=lcd_put_char(sign_chars[i]);
				 }
				 appsys.flag_color_reverse=0;
				 //give some area for black area
				 while(w<5)
				 {
                    lcd_goto_xy(x+w, y);
					lcd_draw_vertical((cur_pos == i)?0xff:0x00,h);
                    w++;
				 }
				 x+=CHAR_SLOT;
 
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
					    if( (TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
                            return 0;
						}
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
						{
						    if( (1 ==cur_pos) && (IN_PY5D== p_edit_wnd->input_mode))
						    {
                                return 0xA1A3;
							}
							else
							{
						        return sign_chars[cur_pos];
							}
						}
						else
						{
                            unsigned char key_code =  EventPara.sig_p.key_evt.key_val;
						
							if(TFKEY_LEFT==key_code)
							{
                                if(cur_pos>0)
                                {
                                   cur_pos--;
								   appsys.byte_need_redraw=1;
								}
								else
								{
                                   cur_pos=length-1;
								}
							}
							else if(TFKEY_RIGHT==key_code)
							{
                                if(cur_pos<length-1)
                                {
                                    cur_pos++;
								    appsys.byte_need_redraw=1;
								}
								else
								{
                                    cur_pos=0;
								}
							}
						 	else if(TFKEY_UP==key_code)
							{
							    if(cur_pos-11>=0)
                                {
                                   cur_pos-=11;
								   appsys.byte_need_redraw=1;
								}
							}
							else if(TFKEY_DOWN==key_code)
							{
							    if(cur_pos+11<length-1)
                                {
                                   cur_pos+=11;
								   appsys.byte_need_redraw=1;
								}
								else
								{
                                   cur_pos=length-1;
								   appsys.byte_need_redraw=1;
								}
							}
						}
					}
		       	}
				else if(EVENT_TIMER == EventPara.eventTyp )
				{
	                if(appsys.byte_need_exit > 0)
	                {
	                    return 0;
					}				
				}
		    }
		}
	}
}

char wnd_edit_IN_123(EDITWND * p_edit_wnd,unsigned char key_code)  
{
    unsigned char i,x,cur_pos,length;
	unsigned char ret_char;
	const char  char_array[]={'0','+','\0'};
	unsigned long m_tick=app_get_tick_count();
	
    FlSignalBuffer EventPara;


	i=x=cur_pos=0;

	length=strlen(char_array);

	if( (p_edit_wnd->style & EDIT_STYLE_SMS_FEATURE) >0 )
	{
        if( TFKEY_STAR == key_code)
		{
			ret_char=wnd_get_sign_option(p_edit_wnd);
			appsys.byte_need_redraw=0xFF;
		    return ret_char;
		}
	}
	
	if(p_edit_wnd->data_len>0  ||  key_code !=TFKEY_0)
    {
        return get_key_value(key_code);
	}

	
    appsys.byte_need_redraw=1;
	if(key_code == TFKEY_0)
	{ 
	    while(1)
	    {
            if(appsys.byte_need_redraw>0)
            {
                appsys.byte_need_redraw=0;
				appsys.byte_font_type=FONT_T_X_10;
			    x=SCREEN_WIDTH-14;
				for(i=0;i<length;i++)
				{
                    lcd_goto_xy(x+i*7, 0);
				    appsys.flag_color_reverse= (cur_pos == i)? 1:0;
				    lcd_put_char(char_array[i]);
				    appsys.flag_color_reverse=0;
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
						    if(TFKEY_0==EventPara.sig_p.key_evt.key_val)
						    {
                                cur_pos=(cur_pos+1)%length;
								m_tick=app_get_tick_count();
								appsys.byte_need_redraw=1;
							}
							else
							{
                                // process as special way : add 2 char
								if(wnd_wstr_insert_char( p_edit_wnd, char_array[cur_pos]) > 0)
								{
                                    return get_key_value(EventPara.sig_p.key_evt.key_val);
								}
							}
						}
			       	}
					else if(EVENT_TIMER == EventPara.eventTyp )
					{
                        if(get_duration_tick(m_tick) >1000)
                        {
                            return char_array[cur_pos];
						}
					}
		        }
			}
		}
	}
	return 0;
}
char wnd_edit_IN_AaBbCc(EDITWND * p_edit_wnd,unsigned char key_code) 
{
    unsigned char i,x,cur_pos,length,range,keyVal,ret_char,tokens_width,keyValEx;
    char st_char='1';

	


	#if 1
	
    char * upper_char;
	char * lower_char;
	char * start_char_index;
	unsigned char  * mult_tap_range;

        //.,1!;:ABC2ÁÀÇDEF3ÉÈGHI4ÍÏJKL5MNO6ÑÓÒPQRS7TUV8ÚÜWXYZ9
		char upper_char_sp[] ="\x2E\x2C\x31\x21\x3B\x3A\x41\x42\x43\x32\xC1\xC0\xC7\x44\x45\x46\x33\xC9\xC8\x47\x48\x49\x34\xCD\xCF\x4A\x4B\x4C\x35\x4D\x4E\x4F\x36\xD1\xD3\xD2\x50\x51\x52\x53\x37\x54\x55\x56\x38\xDA\xDC\x57\x58\x59\x5A\x39";

		//.,1!;:abc2áàçdef3éèghi4íïjkl5mno6ñóòpqrs7tuv8úüwxyz9
		char lower_char_sp[] ="\x2E\x2C\x31\x21\x3B\x3A\x61\x62\x63\x32\xE1\xE0\xE7\x64\x65\x66\x33\xE9\xE8\x67\x68\x69\x34\xED\xEF\x6A\x6B\x6C\x35\x6D\x6E\x6F\x36\xF1\xF3\xF2\x70\x71\x72\x73\x37\x74\x75\x76\x38\xFA\xFC\x77\x78\x79\x7A\x39";

		char start_char_index_sp[]        = {0, 6, 13, 19, 25, 29, 36, 41, 47};
		unsigned char mult_tap_range_sp[] = {6, 7,  6,  6,  4,  7,  5,  6 , 5};


	 	char upper_char_en[] = ".,1!;:ABC2DEF3GHI4JKL5MNO6PQRS7TUV8WXYZ9";
		char lower_char_en[] = ".,1!;:abc2def3ghi4jkl5mno6pqrs7tuv8wxyz9";
		                      //0123456789012345678901234567890123456789
		
		char start_char_index_en[]        = {0, 6, 10, 14, 18, 22, 26, 31, 35};
		unsigned char mult_tap_range_en[] = {6, 4,  4,  4,  4,  4,  5,  4 , 5};

		

		if(LANGUAGE_SPANISH == sysprop->byte_language_type || LANGUAGE_PORTUGE== sysprop->byte_language_type) //spanish support
		{
		    upper_char = (char *)upper_char_sp;
			lower_char = (char *)lower_char_sp;
			start_char_index = (char *)start_char_index_sp;
			mult_tap_range = (unsigned char *)mult_tap_range_sp;
		}
		else
		{
		    upper_char = (char *)upper_char_en;
			lower_char = (char *)lower_char_en;
			start_char_index = (char *)start_char_index_en;
			mult_tap_range = (unsigned char *)mult_tap_range_en;
		}
	#else
	const char upper_char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char lower_char[] = "abcdefghijklmnopqrstuvwxyz";
	const char start_char_index[] = {0, 3, 6, 9, 12, 15, 19, 22};
	const unsigned char mult_tap_range[] = {3, 3, 3, 3, 3, 4, 3, 4};
	#endif

    FlSignalBuffer EventPara;
    unsigned long m_tick=app_get_tick_count();

    keyVal=get_key_value(key_code);


KEY_CHANGED:
	appsys.byte_font_type=FONT_T_X_10;
	
	i=x=cur_pos=length=range=keyValEx=ret_char=tokens_width=0;
	
	if('0'==keyVal)
    {
        return ' ';
	}	
	else if( '#' == keyVal)
	{
        p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
		return 0;
	}
	else if( '*' == keyVal)
	{
		ret_char=wnd_get_sign_option(p_edit_wnd);
		appsys.byte_need_redraw=0xFF;
	    return ret_char;
	}	
	else if( keyVal>='1'  && keyVal <='9')
	{
		
		if(p_edit_wnd->input_mode == IN_ABC)
		{
             ret_char = upper_char[start_char_index[keyVal - st_char]];
		}
		else
		{
             ret_char = lower_char[start_char_index[keyVal - st_char]];
		}
		
        range = mult_tap_range[keyVal - st_char];
		

        if ( p_edit_wnd->input_mode == IN_ABC )
        {
            tokens_width = get_str_dots_width_ex((char *)(upper_char+(start_char_index[keyVal-st_char])), range);
		}
        else
        {
            tokens_width = get_str_dots_width_ex((char *)(lower_char+(start_char_index[keyVal-st_char])), range);
        }

	 	appsys.byte_need_redraw|=EDIT_REDRAW_TOKEN;
		
	    while(1)
	    {
            if(appsys.byte_need_redraw>0)
            {
                appsys.byte_need_redraw=0;
				appsys.byte_font_type=FONT_T_X_10;
		
				
				x= (SCREEN_WIDTH-tokens_width)/2;	//x= SCREEN_WIDTH-tokens_width
				for(i=0;i<range;i++)
				{
				    appsys.flag_color_reverse= (cur_pos == i)? 1:0;
				
					lcd_goto_xy(x, 53); //lcd_goto_xy(x, 0);
                    if ( IN_ABC == p_edit_wnd->input_mode   )
                    {
                        x+=lcd_put_char(upper_char[start_char_index[keyVal-st_char]+i]);
                    }
                    else
                    {
                        x+=lcd_put_char(lower_char[start_char_index[keyVal-st_char]+i]);
                    }
					appsys.flag_color_reverse=0;
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
                             keyValEx=get_key_value(EventPara.sig_p.key_evt.key_val);
                             if(keyValEx>0)
							 {
							      m_tick=app_get_tick_count();
								  
                                 
								  if('0'==keyValEx)
								  {
									  if(wnd_wstr_insert_char(p_edit_wnd, ret_char)>0)
									  {
								          return ' ';
									  }
								  }	
								  else if( '#' == keyValEx)
								  {
								      p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
								      return 0;
								  }
								  else if( '*' == keyValEx)
								  {
								      if(wnd_wstr_insert_char(p_edit_wnd, ret_char)>0)
								      {
									      ret_char = wnd_get_sign_option(p_edit_wnd);
										  appsys.byte_need_redraw=0xFF;
										  return ret_char;
								      }
								  }	
								  else
								  {
                                      if(keyVal == keyValEx)
                                      {
                                          //same key
                                          cur_pos= (cur_pos+1)%range;
										  appsys.byte_need_redraw|=EDIT_REDRAW_TOKEN;
									  }
									  else 
									  {
						                  if ( IN_ABC == p_edit_wnd->input_mode   )
						                  {
						                      ret_char =upper_char[start_char_index[keyVal-st_char]+cur_pos];
						                  }
						                  else
						                  {
						                      ret_char =lower_char[start_char_index[keyVal-st_char]+cur_pos];
						                  }

				
										 lcd_goto_xy((SCREEN_WIDTH-tokens_width)/2, 53);
										 lcd_clear_line((SCREEN_WIDTH+tokens_width)/2);

										 

										  if(wnd_wstr_insert_char(p_edit_wnd, ret_char)>0)
										  {
	                                          wnd_edit_init(p_edit_wnd);
											  keyVal=keyValEx;
											  goto KEY_CHANGED;
										  }
										  else 
										  {
                                              return 0;
										  }
									  }
								  }
							 }
						}
			       	}
					else if(EVENT_TIMER == EventPara.eventTyp )
					{
                        if(get_duration_tick(m_tick) >500)
                        {
							if ( IN_ABC == p_edit_wnd->input_mode   )
							{
							   ret_char =upper_char[start_char_index[keyVal-st_char]+cur_pos];
							}
							else
							{
							   ret_char =lower_char[start_char_index[keyVal-st_char]+cur_pos];
							}
							
                            lcd_goto_xy((SCREEN_WIDTH-tokens_width)/2, 53);
						    lcd_clear_line((SCREEN_WIDTH+tokens_width)/2);
                            return ret_char;
						}
					}
		        }
			}
		}
	}
	return 0;
}



unsigned short wnd_edit_IN_PY5D(EDITWND * p_edit_wnd,unsigned char key_code) 
{
    #define CNT_PER_PAGE  6
	
    FlSignalBuffer EventPara;

	unsigned short ret_wchar=0;

	unsigned short gb_buf[128];
	char * gb_lst=NULL;

	char spell_str[7];
	char KeyVal;
	unsigned char i, uni_index,uni_page,uni_max,sel_idx;	
	
	sel_idx =0;
	memset(spell_str,0,sizeof(spell_str));	
    KeyVal=get_key_value(key_code);


	if('1'==KeyVal)
    {
        return ',';
	}
	if('0'==KeyVal)
    {
        return ' ';
	}	
	else if( '#' == KeyVal)
	{
        p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
		return 0;
	}
	else if( '*' == KeyVal)
	{
		ret_wchar=wnd_get_sign_option(p_edit_wnd);
		appsys.byte_need_redraw=0xFF;
	    return ret_wchar;
	}	
	else if( KeyVal>='2'  && KeyVal <='9')
	{
         char KeyBuf[8];
		 unsigned char py_idx=0;
#if 0	 

#else
#define INPUT_USER_MODE 0

        memset(KeyBuf,0,sizeof(KeyBuf));

		if( (py_idx = make_spell_5d( p_edit_wnd, KeyBuf, spell_str,  KeyVal)) >0 )
	    {
	        char hz_buf[127];
			uni_index=0;					
			uni_page=0;
			memset(hz_buf,0,sizeof(hz_buf));
             
		    if(ReadPYHzCode(INPUT_USER_MODE, KeyBuf, (py_idx-1), &gPYCode, (unsigned char *)hz_buf))
			{
				memset(gb_buf,0,sizeof(gb_buf));
				a2w(gb_buf,hz_buf);
				uni_max =wstrlen(gb_buf);	
		    }
			else
			{
                if(1 == appsys.flag_genie_trace)DebugPrintf("spell=%s  no HZ \r\n", spell_str);
			}
	   	}
	   	else
	   	{
	   		return 0;
	   	}
#endif
		

		
		
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
					        else if ( (TFKEY_EXIT== EventPara.sig_p.key_evt.key_val) || (TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val))
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
	}
	return 0;
}

void get_bh_buffer(char * bh_buf, char * keybuf)
{
	unsigned char i,sub;
	unsigned char pBiHuaCode[5][3]=
    {
		"©¤",//ºº×ÖÄÚÂë0xA9A4 -0xA1-0xA1  (8*94+3)*32=0x5E60  falshICµØÖ·
		"©¦",//ºº×ÖÄÚÂë0xA9A6 -0xA1-0xA1  (8*94+5)*32=0x5EA0
		"¥Î",//ºº×ÖÄÚÂë0xA5CE -0xA1-0xA1  (4*94+45)*32=0x34A0
		"¡¢",//ºº×ÖÄÚÂë0xA1A2 -0xA1-0xA1  (0*94+1)*32=0x20
		"¦è" //ºº×ÖÄÚÂë0xA6E8 -0xA1-0xA1  (5*94+71)*32=0x43A0 //´íÎóµØÖ·0x4EE0
    };
	i=0;
	while(keybuf[i]>'0'&&keybuf[i]<'6')
	{
		sub=keybuf[i]-'1';
   
        bh_buf[i*2 +0] =pBiHuaCode[sub][0];
		bh_buf[i*2 +1] =pBiHuaCode[sub][1];
		i++;
	}
}


unsigned short wnd_edit_IN_BX5D(EDITWND * p_edit_wnd,unsigned char key_code) 
{
    #define CNT_PER_PAGE  6
	
    FlSignalBuffer EventPara;

	unsigned short ret_wchar=0;
	unsigned short flag_can_choose=0;

	unsigned short gb_buf[128];
	char * gb_lst=NULL;

	char bh_buf[24];
	char KeyVal;
	unsigned char i, uni_index,uni_page,uni_max,sel_idx;	
	
	sel_idx=0;
    KeyVal=get_key_value(key_code);

	if('0'==KeyVal)
    {
        return ' ';
	}	
	else if( '#' == KeyVal)
	{
        p_edit_wnd->input_mode= get_input_mode(p_edit_wnd);
		return 0;
	}
	else if( '*' == KeyVal)
	{
		ret_wchar=wnd_get_sign_option(p_edit_wnd);
		appsys.byte_need_redraw=0xFF;
	    return ret_wchar;
	}	
	else if( KeyVal>='1'  && KeyVal <='5')  ////±Ê»­ÓÐÐ§¼üÖ»ÓÐ1,2,3,4,5
	{
        char KeyBuf[10];
	    char hz_buf[127];
		unsigned char key_max =0;
        memset(KeyBuf,0,sizeof(KeyBuf));

    	uni_index=0;					
		uni_page=0;
		uni_max =0;
		KeyBuf[key_max] =KeyVal;
		key_max++;
		
		memset(hz_buf,0,sizeof(hz_buf));
		ReadBXHzCode((char *)KeyBuf, (unsigned char *)hz_buf);

		memset(gb_buf,0,sizeof(gb_buf));
		a2w(gb_buf,hz_buf);
		uni_max =wstrlen(gb_buf);	

		
		
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

                appsys.flag_color_reverse = flag_can_choose;
			    clear_bx_rect();
				lcd_goto_xy(BX_POS_LEFT, BX_POS_TOP-1);
				
				memset(bh_buf,0,sizeof(bh_buf));
				get_bh_buffer(bh_buf, KeyBuf);
				
				lcd_put_string(bh_buf);
				appsys.flag_color_reverse =0;

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
					            if(0 == flag_can_choose)
					            {
                                    flag_can_choose =1;
									appsys.byte_need_redraw=1;
								}
								else
								{
						            clear_chs_rect();
									p_edit_wnd->lx_buf[0] =(gb_buf[uni_page *CNT_PER_PAGE +sel_idx]&0xFF00)>>8  ;
									p_edit_wnd->lx_buf[1] =(gb_buf[uni_page *CNT_PER_PAGE +sel_idx]&0x00FF)>>0  ;
									p_edit_wnd->lx_buf[2] =0 ;
									
									return gb_buf[uni_page *CNT_PER_PAGE +sel_idx];
								}
					        }
					        else if ( (TFKEY_EXIT== EventPara.sig_p.key_evt.key_val))
					        {
					            clear_chs_rect();
					        	return 0;
					        }
							else if(TFKEY_SOFT_RIGHT== EventPara.sig_p.key_evt.key_val)
							{
                              	  if(1 == flag_can_choose)
								  {
								      flag_can_choose =0;
                              	  }
								  if(key_max >1)
								  {
								      KeyBuf[key_max-1] =0;
								      key_max--;
									  
									  uni_index=0;					
									  uni_page=0;
									  uni_max =0;
									  memset(hz_buf,0,sizeof(hz_buf));
									  ReadBXHzCode((char *)KeyBuf, (unsigned char *)hz_buf);

									  memset(gb_buf,0,sizeof(gb_buf));
									  a2w(gb_buf,hz_buf);
									  uni_max =wstrlen(gb_buf);	
									  appsys.byte_need_redraw =1;
								  }
								  else
								  {
                                      return 0;
								  }
							}
	                        else if ( (TFKEY_UP== EventPara.sig_p.key_evt.key_val) ) // Move highlight left
					        {
					            if(1 == flag_can_choose)
					            {
						         	if(uni_page>0)
						            {
						            	uni_page--; 
										sel_idx =0;        	
										appsys.byte_need_redraw=1;
						            }
					            }
					        }
					        else if ( (TFKEY_DOWN== EventPara.sig_p.key_evt.key_val)) // Move highlight right
					        {
					            if(1 == flag_can_choose)
					            {
						            if( (uni_page+1) * CNT_PER_PAGE <uni_max)
						            {
						            	uni_page++;
										sel_idx=0;           	
		                                appsys.byte_need_redraw=1;
						            }  
					            }
					        }
							else if  ( TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
							{
							    if(1 == flag_can_choose)
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
							}
							else if(TFKEY_RIGHT==EventPara.sig_p.key_evt.key_val)
							{
							    if(1 == flag_can_choose)
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

								  if(1 == flag_can_choose)
								  {
									  if(KeyVal>='1' && KeyVal<='6')
									  {
										  clear_chs_rect();
										  p_edit_wnd->lx_buf[0] =(gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31]&0xFF00)>>8  ;
									      p_edit_wnd->lx_buf[1] =(gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31]&0x00FF)>>0  ;
									      p_edit_wnd->lx_buf[2] =0 ;
	                                      return gb_buf[uni_page *CNT_PER_PAGE + KeyVal-0x31];
									  }
								  }
								  else if(KeyVal>='1' && KeyVal<='5' && key_max< sizeof(KeyBuf)-1)
								  {
									  KeyBuf[key_max] =KeyVal;
									  KeyBuf[key_max+1] =0;
									  key_max++;

									  for(i =0;i<2;i++)
									  {
										    memset(hz_buf,0,sizeof(hz_buf));
										    if(ReadBXHzCode((char *)KeyBuf, (unsigned char *)hz_buf))
										    {
										    	uni_index=0;					
												uni_page=0;
												uni_max =0;
												memset(gb_buf,0,sizeof(gb_buf));
												a2w(gb_buf,hz_buf);
												uni_max =wstrlen(gb_buf);	
												appsys.byte_need_redraw=1;
												break;
											}
											else
											{
                                                 KeyBuf[key_max-1] =0;
												 key_max--;
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
	return 0;
}


unsigned short wnd_get_input_code( EDITWND * p_edit_wnd, unsigned char key_code)
{
   unsigned short ret_code=0;
   if( TFKEY_MODE == key_code ) 
   {  
       p_edit_wnd->input_mode=get_input_mode(p_edit_wnd);
	   return 0;
   }
   else
   {
       if( IN_123 == p_edit_wnd->input_mode)
       {
           //this mode will have '0' to '+'
           return  wnd_edit_IN_123(p_edit_wnd, key_code);
	   }
	   else if(IN_ABC == p_edit_wnd->input_mode || IN_abc == p_edit_wnd->input_mode)
	   {
           ret_code = wnd_edit_IN_AaBbCc(p_edit_wnd, key_code) ;
		   show_soft_key_prompt(get_multi_string((char * * )text_ok), get_multi_string((char * * )text_del));
		   return ret_code;
	   }
	   else if( IN_PY5D== p_edit_wnd->input_mode)
       {
           ret_code =  wnd_edit_IN_PY5D(p_edit_wnd, key_code);

		   if(0 == ret_code)
		   {
   	            memset(p_edit_wnd->lx_buf,0,sizeof(p_edit_wnd->lx_buf));
		   }
		   
		   show_soft_key_prompt(get_multi_string((char * * )text_ok), get_multi_string((char * * )text_del));
		   return ret_code;
	   }
	   else if( IN_BX5D== p_edit_wnd->input_mode)
       {
           ret_code =  wnd_edit_IN_BX5D(p_edit_wnd, key_code);

		   if(0 == ret_code)
		   {
   	            memset(p_edit_wnd->lx_buf,0,sizeof(p_edit_wnd->lx_buf));
		   }

		   if( get_key_value(key_code) >='1' && get_key_value(key_code) <='5')
		   {
		        clear_bx_rect();
		   }
		   
		   show_soft_key_prompt(get_multi_string((char * * )text_ok), get_multi_string((char * * )text_del));
		   return ret_code;
	   }
   }
   return 0;
}

unsigned char check_valid_money(EDITWND * p_edit_wnd, char key_val)
{
    char money_buf[16];
	memset(money_buf,0,sizeof(money_buf));
  	return check_valid_money_ex(w2a(money_buf, p_edit_wnd->data_buf), p_edit_wnd->data_len, key_val);
}

unsigned char check_valid_money_ex(char * data_buf, unsigned char data_len, char key_val)
{
    unsigned char i,dot_pos;

	i=dot_pos=0;


	for(i=0;i<data_len;i++)
	{
	    if(data_buf[i] == '.')
	    {
			dot_pos=i;
			break;
		}
	}

	if(0 == dot_pos)
	{
        if(data_len >=6)
        {
             if('.' == key_val )
             {
                  return key_val;
			 }
             return 0;
		}
		else
		{
		     if('.' == key_val )
		     {
                 if( 0 == data_len )
                 {
                      return 0;
				 }
			 }
             return key_val;
		}
	}
	else
	{
        if( (data_len-1) - dot_pos  >=4)
        {
             return 0;
		}
		else
		{
              if('.' == key_val )
              {
                  return 0;
			  }
			  return key_val;
		}
	}
}



unsigned char check_valid_ipaddress(EDITWND * p_edit_wnd, char key_val)
{
    unsigned char i,dot_nums, dot_pos[3];
	unsigned short str_cell[4];

	i=dot_nums=0;
	memset(dot_pos,0,3);

	for(i=0;i<p_edit_wnd->data_len;i++)
	{
	    if(p_edit_wnd->data_buf[i] == '.')
	    {
			dot_pos[dot_nums++]=i;
		}
	}

    if('.' == key_val)
    {
        if(dot_nums>=3)
        {
             return 0;
		}
		else
		{
            if(dot_nums>0)
            {
                 if(   dot_pos[dot_nums-1]  == wstrlen(p_edit_wnd->data_buf)  )
                 {
                     return 0;
				 }
				 else
				 {
                     return key_val;
				 }
			}
			else
			{
                 if(wstrlen(p_edit_wnd->data_buf) == 0)
                 {
                     return 0;
				 }
				 else
				 {
                     return key_val;
				 }
			}
		}
	}
	else
	{
        if(dot_nums == 0)
        {
            memset(str_cell, 0,sizeof(str_cell));
            wstrcpy(str_cell, p_edit_wnd->data_buf);
			str_cell[wstrlen(str_cell)]=key_val;
			if(wtoi(str_cell)>255)
			{
               return 0;
			}
			else
			{
               return key_val;
			}
		}
		else
		{
            memset(str_cell, 0,sizeof(str_cell));
		    wstrncpy(str_cell,  & (p_edit_wnd->data_buf[ dot_pos[dot_nums-1]+1] ), wstrlen(p_edit_wnd->data_buf) - dot_pos[dot_nums-1]);
            str_cell[wstrlen(str_cell)]=key_val;
			if( wtoi(str_cell) >255 )
		    {
                return 0;
			}
			else
			{
                return key_val;
			}
		}
	}
}


unsigned char check_valid_decimal(EDITWND * p_edit_wnd, char key_val)
{
    unsigned char i,has_dot,dot_pos;

	i=has_dot=dot_pos=0;


    if(p_edit_wnd->data_len == 0)
    {
		if( key_val != '.')
		{
            return key_val;
		}
	}
	else
	{
		if(p_edit_wnd->data_len< p_edit_wnd->max_len)
		{
	        for(i=0;i<p_edit_wnd->data_len;i++)
	        {
	            if(p_edit_wnd->data_buf[i] == '.')
	            {
	                has_dot=1;
					dot_pos=i;
					break;
				}
			}

			if(1 ==appsys.flag_genie_trace)DebugPrintf("value:%s dot_pos=%d  sysprop->byte_decimal_pos=%d\r\n", p_edit_wnd->data_buf, dot_pos, sysprop->byte_decimal_pos);

            if(sysprop->byte_decimal_pos>0)
            {
				if(has_dot ==0)
				{
				     if(p_edit_wnd->data_len == p_edit_wnd->max_len -1 )
				     {
				         // the last char could not be .
                         if(key_val != '.')
                         {
                             return key_val;
						 }
					 }
					 else
					 {
	                     return key_val;
					 }
				}
				else  if(key_val != '.')
				{
				     if(p_edit_wnd->data_len - dot_pos   <= sysprop->byte_decimal_pos)
				     {
	                      return key_val;
					 }
				}
			}
            else if(key_val != '.')
            {
                return key_val;
			}
		}
	}
    return 0;
}

void wnd_show_password_style()
{
    unsigned char x,y;
    x=13;
	y=22;

	lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);

	lcd_goto_xy(x,y);
	show_bitmap(BMP_PASSWORD_BIN);

	lcd_goto_xy(x+30,y);
	lcd_put_rect(x+29, y, x+103, y+19);

}
void wnd_show_decimal_style()
{
	unsigned char x,y;
    x=5;
	y=19;

	lcd_put_rect(0, 12, SCREEN_WIDTH-1, 51);
}


unsigned char wnd_edit_txt( EDITWND * p_edit_wnd)
{
    FlSignalBuffer EventPara;
	unsigned short input_code=0;
	unsigned char tmp=0;
    unsigned char update_cursor_cnt=0;
	unsigned char edit_st_tick= app_get_tick_count();
	appsys.byte_need_redraw=0xFF;
	
    while(0 == appsys.byte_need_exit)
	{
	    if(appsys.byte_need_redraw>0)
		{
           if (wnd_edit_init( p_edit_wnd) ==0)
           {
               return 0;
		   }
		}
		else
		{
            if(strlen(p_edit_wnd->lx_buf) >0)
            {
                 if((  input_code = make_lianxiang_5d(p_edit_wnd)) >0 )
                 {
                     if(wnd_wstr_insert_char(p_edit_wnd,input_code) >0)
					 {
    					 appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
					 }
				 }
				 else
				 {
				      memset(p_edit_wnd->lx_buf,0,sizeof(p_edit_wnd->lx_buf)); 
				 } 
				 show_soft_key_prompt(get_multi_string((char * * )text_ok), get_multi_string((char * * )text_del));
		    }

		    if(1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{				
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if(p_edit_wnd->input_mode == IN_FIXED)
					    {
                           input_code = get_key_value(EventPara.sig_p.key_evt.key_val);
						   if( (p_edit_wnd->style & EDIT_STYLE_DIGITALONLY) !=0 )
						   {

							   if(input_code == '*' || input_code == '#' )
							   {
                                   if((p_edit_wnd->style & EDIT_STYLE_DECIMALINPUT)>0)
                                   {
                                        input_code='.';
								   }
								   else
								   {
                                        input_code=0;
								   }
							   }
							   else
							   {
	                               if(!(  input_code>='0'  && input_code <='9'))
								   {
	                                    input_code=0;
								   }
							   }
							   if(input_code !=0  )
							   {
                                    if((p_edit_wnd->style & EDIT_STYLE_DECIMALINPUT)>0)
                                    {
                                        if((p_edit_wnd->style & EDIT_STYLE_IPADDRESSINPUT)>0)
                                        {
                                            input_code = check_valid_ipaddress(p_edit_wnd, input_code);
										}
										else if((p_edit_wnd->style & EDIT_STYLE_MONEYINPUT)>0)
										{
										    input_code = check_valid_money(p_edit_wnd, input_code);
											if(input_code != 0)
											{
											    input_code = check_valid_decimal(p_edit_wnd,  input_code);
											}
										}
										else
										{
                                            input_code = check_valid_decimal(p_edit_wnd, input_code);
										}
                                    }
							   }
						   }
					    }
						else
						{
                            input_code=wnd_get_input_code(p_edit_wnd,EventPara.sig_p.key_evt.key_val);
						}
						
						if(input_code != 0)
						{
						    unsigned char flag_redraw_bottom=0;
						    if(0 == p_edit_wnd->data_len)
						    {
                                flag_redraw_bottom =1;
							}
						    if(wnd_wstr_insert_char(p_edit_wnd,input_code) >0)
							{   
    							appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
								if(1 == flag_redraw_bottom)
								{
                                    appsys.byte_need_redraw|=EDIT_REDRAW_BOTTOM;
								}
							}
						}
						else
						{
						    if(TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)
							{
							    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK)
							    {
							        if( p_edit_wnd ->data_len >0)
							        {
	                                    unsigned long down_tick = app_get_tick_count();
										unsigned char flag_batch=0;

										while(KEY_STATE_PRESSED == appsys.flag_key_state)
										{
	                                        delay_ms(100);

											if(get_duration_tick(down_tick) > 900)
											{
		                                        flag_batch=1;
												if(1 ==appsys.flag_genie_trace)DebugPrintf("BATCH DEL --------------------<><><>\r\n",NULL);
												break;
											}
										}

	
									   
								        tmp=0;
	                                    if((p_edit_wnd->style & EDIT_STYLE_PASSWD_CHAR) > 0)
	                                    {
	                                        tmp=(0 ==flag_batch)?wnd_wstr_delete(p_edit_wnd):wnd_batch_wstr_delete(p_edit_wnd);
										}
										else
										{
									        if(p_edit_wnd->data_pos !=0 )
									        {
		                                        tmp=(0 ==flag_batch)?wnd_wstr_delete(p_edit_wnd):wnd_batch_wstr_delete(p_edit_wnd);
											}
											else
											{
		                                        tmp=(0 ==flag_batch)?wnd_wstr_delete(p_edit_wnd):wnd_batch_wstr_delete(p_edit_wnd);
											}
										}
									
									    if(tmp >0 )
										{ 
										    appsys.byte_need_redraw|=EDIT_REDRAW_DATA;
									    }

										if(0 == p_edit_wnd ->data_len)
										{
	                                        appsys.byte_need_redraw|=EDIT_REDRAW_BOTTOM;
										}
							        }
									else
									{
                                        return 0;
									}
							    }
								else if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NO_NULL_YES ||  p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_NULL_NULL_OK)
								{
                                    return 0;
								}
							}
							else if(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val)
							{
							    if(p_edit_wnd->soft_key_type == SOFT_KEY_TYPE_DEL_CANCLE_OK ||  (p_edit_wnd->style & EDIT_STYLE_READONLY)>0)
							    {
							        return 0;
							    }
 							}	
							else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val || (TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val))
							{
							    return 1;
 							}	
							else if(
								      (TFKEY_UP==EventPara.sig_p.key_evt.key_val)
								    ||(TFKEY_LEFT== EventPara.sig_p.key_evt.key_val)
									||(TFKEY_DOWN==EventPara.sig_p.key_evt.key_val)
									||(TFKEY_RIGHT== EventPara.sig_p.key_evt.key_val)
								    )
							{
                                wnd_cursor_move(p_edit_wnd,EventPara.sig_p.key_evt.key_val);
								if((p_edit_wnd->style & EDIT_STYLE_SMS_FEATURE)>0 )
								{
                                    appsys.byte_need_redraw |= EDIT_REDRAW_SIZE;
								}
								update_cursor_cnt=0;
							}
							else if(TFKEY_HANDFREE == EventPara.sig_p.key_evt.key_val)
							{
                                if((p_edit_wnd->style & EDIT_STYLE_SMS_FEATURE)>0 )
                                {
							        unsigned char def_pos=0;
								    if (1 == choose_menu_option((char ***)mem_dy_item, 21, &def_pos))
								    {
								        char * tmp_str=get_multi_string_ex((char * * *)mem_dy_item, (def_pos+1));
						                wnd_str_insert_str( p_edit_wnd, tmp_str);
								    }
 		   						 	appsys.byte_need_redraw=0xFF;
								}
							}
							else
							{
							    //process other keys
							}
						}
					}
					
				}
				else if(EVENT_TIMER == EventPara.eventTyp)
				{
				    if( (p_edit_wnd->style & EDIT_STYLE_HIDE_CURSOR) ==0   )
				    {
				        if(++update_cursor_cnt>5)//520ms per time
				        {
				             update_cursor_cnt=0;
					         wnd_put_cursor(p_edit_wnd);
				        }
					}
					
					if(0xFF== appsys.byte_sysrun_state) 
					{
					    if(appsys.byte_need_exit > 0)
		                {
		                    return 0;
						}
					}
				}
			}
		}
	}
	return 0;
}

void app_show_sms_txt(RECORDWND * p_record_wnd)
{
    T_SMS_RECORD sms_record;
	unsigned char nret=0;
	while(0 == appsys.byte_need_exit)
	{
		memset(&sms_record,0,sizeof(T_SMS_RECORD));
		if( 1 == app_record_read(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *) &sms_record))
		{
	        EDITWND  m_edit_wnd;
	        memset(&m_edit_wnd,0,sizeof(EDITWND));
			 
			m_edit_wnd.pcaption= get_multi_string((char * *) text_text_message);
		    m_edit_wnd.data_font=FONT_T_X_10;
			m_edit_wnd.frame_left =0;
			m_edit_wnd.frame_right =SCREEN_WIDTH-1;
		 	m_edit_wnd.frame_top =12;
			m_edit_wnd.frame_bottom =51;
			
		 	m_edit_wnd.style= EDIT_STYLE_SHOW_FRAME|EDIT_STYLE_VERTICAL_BAR| EDIT_STYLE_SPLIT_WORDS;

		    a2w(m_edit_wnd.data_buf,sms_record.context);
			m_edit_wnd.data_len=m_edit_wnd.max_len =wstrlen(m_edit_wnd.data_buf);

		    m_edit_wnd.data_pos=0;
		 	m_edit_wnd.cursor_x_last=0;
			m_edit_wnd.cursor_y_last=0;
		    m_edit_wnd.input_mode=IN_FIXED;	
		 	m_edit_wnd.soft_key_type=SOFT_KEY_TYPE_DEL_CANCLE_OK;



            if(0 == sms_record.sms_flag.flag_read   && TABLEINFO_SMS_INBOX == p_record_wnd->table_idx )
            {
                sms_record.sms_flag.flag_read=1;
				app_record_edit(p_record_wnd->table_idx, (p_record_wnd->selected_pos+1), (unsigned char *)&sms_record);
			}		

			nret  =wnd_show_sms_txt( &m_edit_wnd);
			
			
            if(2 == nret) //option
			{
                if ( TABLEINFO_SMS_INBOX== p_record_wnd->table_idx)
                {
                    switch ( app_menu_select((char ***)mem_in_sm_detial_opt, 6, NULL) )
                    {
                        case 1:
				            memset(sms_record.context,0, SMS_CONTEXT_LENGTH+1);
							edit_sms_message( &sms_record);
                            break;

                        case 2:
				            memset(sms_record.str_num,0, BOOK_ENTRY_NUM_FIELD_SIZE+1);
							edit_sms_message( &sms_record);
							
                            break;

                        case 3:
							wnd_record_delete(p_record_wnd);
	                        
                            return;
                            break;

                        case 4:
                            appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
							if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
							{
                                 app_dial_out(sms_record.str_num);
							}
                            break;

                        case 5:
							new_book_entry(sms_record.str_num);
                            break;
							
                        case 6:
							show_sms_info(&sms_record, p_record_wnd->table_idx);
                            break;
                        default:
                            break;
                    }
                }
                else if( TABLEINFO_SMS_OUTBOX== p_record_wnd->table_idx)
                {
                    switch ( app_menu_select((char ***)mem_out_sm_detial_opt, 4, NULL) )
                    {
                        case 1:
                            edit_sms_message( &sms_record);
                            break;

                        case 2:
				            memset(sms_record.str_num,0, BOOK_ENTRY_NUM_FIELD_SIZE+1);
							edit_sms_message( &sms_record);
                            break;

                        case 3:
							wnd_record_delete(p_record_wnd);
							return;

                            break;

                        case 4:
							show_sms_info(&sms_record, p_record_wnd->table_idx);
                            break;

                        default:
                            break;
                    }
                }
				else if( TABLEINFO_SMS_DRAFT== p_record_wnd->table_idx)
				{
                    switch ( app_menu_select((char ***)mem_draft_detial_opt, 3, NULL) )
                    {
                        case 1:
							appsys.flag_editing_draft=1;
							appsys.byte_cur_draft_rec_no =(p_record_wnd->selected_pos+1);
                            edit_sms_message( &sms_record);
							appsys.flag_editing_draft=0;
                            break;

                        case 2:
							wnd_record_delete(p_record_wnd);
							return;
                            break;

                        case 3:
							show_sms_info(&sms_record, p_record_wnd->table_idx);
                            break;

                        default:
                            break;
                    }
				}
			}
			else 
			{
	             break;
			}
	
		}
		else
		{
            break;
		}
	}
}

void app_show_sms_record(T_SMS_RECORD * psms_record, unsigned char id_pos)
{
	unsigned char nret=0;
	while(1)
	{
		if(1)
		{
	        EDITWND  m_edit_wnd;
	        memset(&m_edit_wnd,0,sizeof(EDITWND));
			 
			m_edit_wnd.pcaption= get_multi_string((char * *) text_text_message);
		    m_edit_wnd.data_font=FONT_T_X_10;
			m_edit_wnd.frame_left =0;
			m_edit_wnd.frame_right =SCREEN_WIDTH-1;
		 	m_edit_wnd.frame_top =12;
			m_edit_wnd.frame_bottom =51;
			
		 	m_edit_wnd.style= EDIT_STYLE_SHOW_FRAME|EDIT_STYLE_VERTICAL_BAR| EDIT_STYLE_SPLIT_WORDS;

		    a2w(m_edit_wnd.data_buf,psms_record->context);
			m_edit_wnd.data_len=m_edit_wnd.max_len =wstrlen(m_edit_wnd.data_buf);

		    m_edit_wnd.data_pos=0;
		 	m_edit_wnd.cursor_x_last=0;
			m_edit_wnd.cursor_y_last=0;
		    m_edit_wnd.input_mode=IN_FIXED;	
		 	m_edit_wnd.soft_key_type=SOFT_KEY_TYPE_DEL_CANCLE_OK;



            if(0 == psms_record->sms_flag.flag_read )
            {
                psms_record->sms_flag.flag_read=1;
			}		

			nret  =wnd_show_sms_txt( &m_edit_wnd);
			
			
            if(2 == nret) //option
			{
                switch ( app_menu_select((char ***)mem_in_sm_detial_opt, 5, NULL) )
                {
                    case 1:
			            memset(psms_record->context,0, SMS_CONTEXT_LENGTH+1);
						edit_sms_message( psms_record);
                        break;

                    case 2:
			            memset(psms_record->str_num,0, BOOK_ENTRY_NUM_FIELD_SIZE+1);
						edit_sms_message( psms_record);
						
                        break;

                    case 3:
						remove_simsms_id(id_pos);
                        return;
                        break;

                    case 4:
                        appsys.flag_hand_free= ( HOOK_STATE_OFFHOOK == appsys.flag_hook_state)?0:1;
						if(1 ==set_voice_path_by_action(VOICE_ACTION_KEY))
						{
                             app_dial_out(psms_record->str_num);
						}
                        break;
						
                    case 5:
						new_book_entry(psms_record->str_num);
                        break;
						
                    default:
                        break;
                }
                
			}
			else 
			{
	             break;
			}
	
		}
		else
		{
            break;
		}
	}
}



unsigned char wnd_show_sms_txt( EDITWND * p_edit_wnd)
{
    FlSignalBuffer EventPara;
	unsigned char tmp=0;
    unsigned char update_cursor_cnt=0;
	appsys.byte_need_redraw=0xFF;
    while(1)
	{
	    if(appsys.byte_need_redraw>0)
		{
		    if(0xFF == appsys.byte_need_redraw)
		    {
                lcd_clear_screen();
			}
	

		    appsys.byte_font_type=p_edit_wnd->data_font;
	 	
			if(p_edit_wnd->style & EDIT_STYLE_SHOW_FRAME)
			{
				lcd_put_rect(p_edit_wnd->frame_left,p_edit_wnd->frame_top  ,p_edit_wnd->frame_right ,p_edit_wnd->frame_bottom ); 
				
				p_edit_wnd->body_left=p_edit_wnd->frame_left+2;
				p_edit_wnd->body_top=p_edit_wnd->frame_top+1;
				p_edit_wnd->body_right=p_edit_wnd->frame_right-5;
				p_edit_wnd->body_bottom=p_edit_wnd->frame_bottom-1;

			}
			else
			{
				p_edit_wnd->body_left=p_edit_wnd->frame_left;
				p_edit_wnd->body_top=p_edit_wnd->frame_top;
				p_edit_wnd->body_right=p_edit_wnd->frame_right;
				p_edit_wnd->body_bottom=p_edit_wnd->frame_bottom;
			}
		  
			p_edit_wnd->body_lines=  (p_edit_wnd->body_bottom-p_edit_wnd->body_top+1)/get_cursor_height();
			
			
		    wnd_get_lines_info(p_edit_wnd);
			wnd_data_redraw(p_edit_wnd);
			show_caption(p_edit_wnd->pcaption);
            show_soft_key_prompt( get_multi_string((char * *)text_option), get_multi_string((char * *)text_back));

			appsys.byte_need_redraw=0;
		}
		else
		{
		    if(1 == app_get_message( &EventPara) )
			{
		        if(EventPara.eventTyp ==EVENT_KEY)
				{				
					if(EventPara.sig_p.key_evt.isPressed)
					{
					    if((TFKEY_SOFT_RIGHT==EventPara.sig_p.key_evt.key_val)||(TFKEY_EXIT==EventPara.sig_p.key_evt.key_val))
						{
                            return 0;
						}
						else if(TFKEY_SOFT_LEFT==EventPara.sig_p.key_evt.key_val)
						{
                            return 2;
						}	
			        	else if(TFKEY_CONFIRM ==EventPara.sig_p.key_evt.key_val)
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
							update_cursor_cnt=0;
						}
						else
						{
						    //process other keys
						}
						
					}
				}
				else if(EVENT_TIMER ==EventPara.eventTyp )
				{
				    if( (p_edit_wnd->style & EDIT_STYLE_HIDE_CURSOR) ==0   )
				    {
				        if(++update_cursor_cnt>5)//400ms per time
				        {
				             update_cursor_cnt=0;
					         wnd_put_cursor(p_edit_wnd);
				        }
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

