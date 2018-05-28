#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "inc/app_def.h"

#define LCD_LINE_CNT             0x06                /* line amount */
#define LCD_START_POS            0x00

unsigned char lcd_x;
unsigned char lcd_y;
unsigned char page_line[LCD_LINE_CNT]={0, 14, 24, 34,44,54};   /* Page start line */   

int lcd_fd = -1;
char* fbp = NULL;
int screensize = 0;

unsigned char uni_buf[26];
unsigned short * ctw_buf;
unsigned char  * wtc_buf;



void lcd_backlight_on()
{

}
void lcd_backlight_off()
{

}

void lcd_init_my()
{
    struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	if ((lcd_fd = open("/dev/graphics/fb1", O_RDWR)) < 0) {
	
		return;
	}

	ioctl(lcd_fd, FBIOGET_FSCREENINFO, &finfo);
	ioctl(lcd_fd, FBIOGET_VSCREENINFO, &vinfo);
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	fbp=(char*)mmap(0,screensize,PROT_READ | PROT_WRITE,MAP_SHARED, lcd_fd, 0);
	
}





void lcd_set_contrast_my(unsigned char value)
{ 

 


}

void lcd_clear_screen(void)
{
	memset(lcd_buf,0 ,sizeof(lcd_buf));

    if(1)
    {
	 	int m_page,m_col;
	  	for(m_page=0;m_page<8;m_page++)
	  	{  
	  	    for(m_col=0;m_col<SCREEN_WIDTH;m_col++)// some other lcd is from 0 to 127..
			{
	   	  		//lcd_send_data(lcd_buf[m_col][m_page]);  
	   	  		*(fbp + 128*m_page + m_col) = lcd_buf[m_col][m_page];
			}	  
	  	} 
    }
} 


void lcd_update_screen_area(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1)
{
	 unsigned char page_st,page_et;
	 int i,j;
	 unsigned char tmp_val;

	 page_st=y0/8;
	 page_et=(y1-1)/8;
     page_et%=8;
	 

	 // show the data!
     for(i=page_st ; i<=page_et ;i++)
	 {	
		 for(j=x0;j<x1;j++)
		 {
		     *(fbp + 128*i + j) = lcd_buf[j][i];
		 }
	 }
  
	 
     //restore to the original position
	 lcd_goto_xy(x0,y0);
}



void lcd_goto_x_line(unsigned char x, unsigned line)
{
	lcd_x=x;
	lcd_y=y_dem_of_row[line];   
}
void lcd_goto_xy(unsigned char x, unsigned char y)
{
	lcd_x=x;
	lcd_y=y;
}



unsigned char lcd_get_x(void)
{
   return lcd_x;
}
unsigned char lcd_get_y(void)
{
   return lcd_y;
}


void lcd_add_x( unsigned char step )
{
    if(lcd_x+step<SCREEN_WIDTH)
	{
        lcd_x += step;
	}
}


unsigned char get_bit_from_bytes(const unsigned char * data,unsigned char pos)
{
   unsigned char tmp_val =data[pos/8];
   tmp_val<<=(pos%8);
   tmp_val>>=7;
   return tmp_val;
}

void set_bit_to_bytes(unsigned char * data,unsigned char pos,unsigned char bitval)
{
   unsigned char mask=0x01;
   if(pos<64)
   {
	   bitval&=0x01;
	   bitval<<=(7- pos%8);
	   mask  <<=(7- pos%8);

	   data[pos/8] &=~mask;
	   data[pos/8] |=bitval;
   }
}

unsigned char get_bits_num_from_bytes(const unsigned char * data)
{
	unsigned char cnt=0;
	unsigned char i=0;
	unsigned char tmp_val=0;
    for(i=0;i<64;i++)
	{
		tmp_val=data[i/8];
		tmp_val<<=(i%8);
		tmp_val>>=7;

	    if(  tmp_val==1)
		{
		   cnt++;
		}
	}
	return cnt;
}

void lcd_display_data(unsigned char width,unsigned char height ,const unsigned char * pdata)
{
     unsigned char x0,y0,x1,y1;
	 unsigned char page_st,page_et;
	 int i,j;
	 unsigned char ori_buf[8];
	 unsigned char cur_buf[8];
	 unsigned char tmp_val;

	 x0=lcd_get_x();
	 y0=lcd_get_y();



	 x1=x0+width;
	 y1=y0+height;

	 page_st=y0/8;
	 page_et=(y1-1)/8;
     page_et%=8;
	 

 
 
	 for(j=x0;j<x1;j++)
	 {
		 memset(ori_buf,0,8);
         for(i=0;i<8;i++)
		 {
		     ori_buf[i]=lcd_buf[j][i];
		 }
		 memset(cur_buf,0,8);
		 for(i=0;i<= (height-1)/8;i++)
		 {
			 if(appsys.flag_color_reverse== 0)
			 {
                 cur_buf[i]= pdata[ i *width +j-x0];
			 }
			 else
			 {
                 cur_buf[i]=~pdata[ i *width +j-x0];
			 }
		 }

		 for(i=0;i<height;i++)
		 {
			 tmp_val=get_bit_from_bytes(cur_buf,i);

             set_bit_to_bytes(ori_buf,y0 +i,tmp_val);
		 }


         for(i=0;i<8;i++)
		 {
		     lcd_buf[j][i]=ori_buf[i];
		 }
	 }
     
     if(1)
     {
		 // show the data!
	     for(i=page_st ; i<=page_et ;i++)
		 {	
			 for(j=x0;j<x1;j++)
			 {
			      *(fbp + 128*i + j) = lcd_buf[j][i];
			 }
		 }
     }
	 
     //restore to the original position
	 lcd_goto_xy(x0,y0);
}



unsigned char lcd_show_to_colum(char * pstring, unsigned char left, unsigned char top, unsigned char dst_col)
{
    unsigned char * pdata=NULL;
	unsigned char i=0;
	unsigned short * pshort = a2w(NULL, pstring);

	if(dst_col >=SCREEN_WIDTH)
	{
	     if(1==appsys.flag_genie_trace)DebugPrintf("tooo big width (lcd_show_to_colum) w=%d\r\n",dst_col);
         dst_col =SCREEN_WIDTH -1;
	}
	
	for(i=0;i<wstrlen(pshort);i++)
	{
	    pdata=(unsigned char *)get_char_font_data(pshort[i]);
		if(pdata !=0)
		{
		    if(left+pdata[0] <dst_col )
		    {
				lcd_goto_xy(left,top);
	            left+=lcd_put_wchar( pshort[i]);
			}
			else
			{
			    break;
			}
		}
	}
	return left;
}



unsigned char lcd_put_wchar(unsigned short data)
{
	unsigned char w,h;
	unsigned char * pdata =0;
	unsigned char ret_width=0;

	w=h=0;

	if(data <=get_charset_cmp())
	{
       return  lcd_put_char((unsigned char)data);
	}


    pdata=(unsigned char *)get_uni_font(data);

	if(pdata !=0)
	{
	    w=pdata[0];
		h=pdata[1];
	
		if(w<SCREEN_WIDTH && h< SCREEN_HEIGHT)
		{ 
			unsigned char font_slot[SCREEN_PAGES];
			memset(font_slot,0,sizeof(font_slot));
	 		lcd_display_data(w,h ,&pdata[2]);			
		}
		ret_width=w;
	}

	return ret_width ;

}




unsigned char lcd_put_char(unsigned char data)// return width
{
	unsigned char w,h;
	unsigned char * pdata;

	w=h=0;
	pdata=(unsigned char *)get_char_font_data(data);

	if(pdata !=0)
	{
	    w=pdata[0];
		h=pdata[1];
		
		if(w<SCREEN_WIDTH && h< SCREEN_HEIGHT)
		{
	 		lcd_display_data(w,h ,&pdata[2]);
		}
	}

	return w;
}


void lcd_put_string( char * data)
{
    if(data !=NULL && strlen(data)!=0)
    {	
		 unsigned char w,i;
		 unsigned short * pshort = a2w(NULL, data);

	     for(i=0;i<wstrlen(pshort);i++)
		 {
		     w = lcd_put_wchar(pshort[i]);
	         lcd_add_x(w);
		 }
    }
}


void lcd_put_rect(unsigned char left,unsigned char top ,unsigned char right,unsigned char bottom)
{
    lcd_put_rect_ex(left, top , right, bottom, 1);
}

void lcd_put_rect_ex(unsigned char left,unsigned char top ,unsigned char right,unsigned char bottom, unsigned char bshow)
{
     unsigned char page_st,page_et;
 	 unsigned char ori_buf[8];
	 unsigned char cur_buf[8];
	 int x,y,i;
   	 page_st=top/8;
	 page_et=bottom/8;

	if(right>=SCREEN_WIDTH)
	{
	     if(1 == appsys.flag_genie_trace)DebugPrintf("tooo big width (lcd_put_rect_ex) w=%d\r\n",right);
         right =SCREEN_WIDTH -1;
	}


	 //left
	 memset(ori_buf,0,8);
     for(i=0;i<8;i++)
	 {
		 ori_buf[i]=lcd_buf[left][i];
	 }
	 for(y=top;y<=bottom;y++)
	 {
		 set_bit_to_bytes(ori_buf,y,bshow);
	 }
	 for(i=0;i<8;i++)
	 {
		 lcd_buf[left][i]=ori_buf[i];
	 }
 

	 //top
	 for(x=left;x<=right;x++)
	 {
	  	 memset(ori_buf,0,8);
         for(i=0;i<8;i++)
		 {
		     ori_buf[i]=lcd_buf[x][i];
		 }
		 set_bit_to_bytes(ori_buf,top,bshow);

	     for(i=0;i<8;i++)
		 {
		     lcd_buf[x][i]=ori_buf[i];
		 }
	 }

	 //right
	 memset(ori_buf,0,8);
     for(i=0;i<8;i++)
	 {
		 ori_buf[i]=lcd_buf[right][i];
	 }
	 for(y=top;y<=bottom;y++)
	 {
		 set_bit_to_bytes(ori_buf,y,bshow);
	 }
	 for(i=0;i<8;i++)
	 {
		 lcd_buf[right][i]=ori_buf[i];
	 }

	 //bottom
	 for(x=left;x<=right;x++)
	 {
	  	 memset(ori_buf,0,8);
         for(i=0;i<8;i++)
		 {
		     ori_buf[i]=lcd_buf[x][i];
		 }
		 set_bit_to_bytes(ori_buf,bottom,bshow);

	     for(i=0;i<8;i++)
		 {
		     lcd_buf[x][i]=ori_buf[i];
		 }
	 }
	 
     if(1)
     {
		 // show the data!
	     for(i=page_st ; i<=page_et ;i++)
		 {		
			 for(x=left;x<=right;x++)
			 {
			      *(fbp + 128*i + x) = lcd_buf[x][i];
			 }
		 }
     }

}

void lcd_fill_area( unsigned char def_data,unsigned char x,unsigned char y,unsigned char width,unsigned char height )
{
    unsigned char i=0;


	if(width >=SCREEN_WIDTH)
	{
	     if(1==appsys.flag_genie_trace)DebugPrintf("tooo big width (lcd_fill_area) w=%d\r\n",width);
         width =SCREEN_WIDTH -1;
	}
	
   	for(i = x ; i<(x+width) ;i++)
	{
	    lcd_goto_xy(i,y);
	    lcd_draw_vertical(def_data,height);
	}
}
void lcd_clear_line(unsigned char dest_col )
{
   lcd_clear_line_ex(get_cursor_height(),dest_col);
}

void lcd_clear_line_ex(unsigned char height,unsigned char dest_col)
{
    unsigned char x,y,h,i;


    if(dest_col >=SCREEN_WIDTH)
	{
	     if(1==appsys.flag_genie_trace)DebugPrintf("tooo big width (lcd_clear_line_ex) w=%d\r\n",dest_col);
         dest_col =SCREEN_WIDTH -1;
	}
    
	x=lcd_get_x();
	y=lcd_get_y();

	for(i = x ; i<=dest_col ;i++)
	{
	    lcd_goto_xy(i,y);
	    lcd_draw_vertical(0x00,height);
	}
}



void lcd_draw_vertical(unsigned char def_val,unsigned char height)
{
    unsigned char data[4];
    height %=32;
    memset(data, def_val, 4);
	lcd_display_data(1,height,data);
}

void lcd_draw_horizontal(unsigned char width)
{
 
 	 unsigned char ori_buf[8];
	 int x,y,i,s_x,e_x;
	 y=lcd_get_y();
	 s_x=lcd_get_x();


    if(width >=SCREEN_WIDTH)
	{
	     if(1==appsys.flag_genie_trace)DebugPrintf("tooo big width (lcd_draw_horizontal) w=%d\r\n",width);
         width =SCREEN_WIDTH -1;
	}
    


	 
	 e_x=s_x+width;
	 
 
	 //top
	 for(x=s_x ;x<=e_x;x++)
	 {
	  	 memset(ori_buf,0,8);
         for(i=0;i<8;i++)
		 {
		     ori_buf[i]=lcd_buf[x][i];
		 }
		 set_bit_to_bytes(ori_buf,y, (1 == appsys.flag_color_reverse)?0:1);

	     for(i=0;i<8;i++)
		 {
		     lcd_buf[x][i]=ori_buf[i];
		 }
	 }

	 if(1)
     {
		 for(x=s_x ;x<=e_x;x++)
		 {
			  *(fbp + 128* (y/8) + x) = lcd_buf[x][y/8];
		 }
	 }
}



void lcd_mem_set_pixel(unsigned char x, unsigned char y, unsigned char color)
{
   unsigned char tmp_byte=lcd_buf[x][y/8];
   if(1 == color)
   {
       tmp_byte |=( 1 <<( 7- (y%8) ));
   }
   else
   {
       tmp_byte &= ~( 1 <<( 7- (y%8) ));
   }
   lcd_buf[x][y/8]=tmp_byte;
}

unsigned char lcd_mem_get_pixel(unsigned char x, unsigned char y)
{
   unsigned char tmp_byte=lcd_buf[x][y/8];
   return ( (tmp_byte & ( 1 <<( 7- (y%8) )))>0)? 1:0;
}

// this is from bresenham ,good way to deal with lines
void lcd_mem_bresenham_Line(int x1,int y1,int x2,int y2,int color)
{
    int dx,dy,e;
    dx=x2-x1;
    dy=y2-y1;

    if(dx>=0)
    {
        if(dy >= 0) 
        {
            if(dx>=dy)
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
	
                    if(e>0){y1+=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else     
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else      
        {
            dy=-dy;  

            if(dx>=dy) 
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else  
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
    else 
    {
        dx=-dx;       
        if(dy >= 0) 
        {
            if(dx>=dy) 
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else  
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else          
        {
            dy=-dy;   

            if(dx>=dy) 
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else    
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    lcd_mem_set_pixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}





unsigned short lcd_mem_put_ellipse(unsigned char Ox, unsigned char Oy, unsigned char radii, unsigned char el_rate, unsigned char mask)
{

	unsigned char max_len_x, max_len_y;
	unsigned short ret_xy;	
	short x0,y0,d;
	
	max_len_x = max_len_y=0;


	x0=0,y0=radii,d=0;
	if((mask&0x03)>0)//1st && 2nd
	{
		lcd_mem_set_pixel((Ox+x0),(Oy+y0),(mask&0x80)>0?1:0);
		lcd_mem_set_pixel((Ox-x0),(Oy+y0),(mask&0x80)>0?1:0);
	}
	if((mask&0x0C)>0)//3rd && 4th
	{
		lcd_mem_set_pixel((Ox+x0),(Oy-y0),(mask&0x80)>0?1:0);
		lcd_mem_set_pixel((Ox-x0),(Oy-y0),(mask&0x80)>0?1:0);
	}

	while(y0)
	{
		if(d>radii/2)
		{
			y0--;
			if( (el_rate&0x80) == 0)
			{
			    d=d-2* ( el_rate &0x7F ) *  y0+1;
			}
			else
			{
                d=d-2*y0+1;
			}

		}
		else
		{
			x0++;
            if( (el_rate&0x80) == 0)
			{
			    d=d+2*x0+1;
			}
			else
			{
			    d=d+2*(el_rate &0x7F)*x0+1;
			}
		}
		if( (mask & 0x01) >0)// 1st mask
		{
			lcd_mem_set_pixel((Ox+x0),(Oy+y0),(mask&0x80)>0?1:0 );
		}
		if( (mask & 0x02) >0)// 2nd mask
		{
		    lcd_mem_set_pixel((Ox-x0),(Oy+y0),(mask&0x80)>0?1:0);
		}
    	if( (mask & 0x04) >0)// 3rd mask
		{
		    lcd_mem_set_pixel((Ox-x0),(Oy-y0),(mask&0x80)>0?1:0);
		}
		if( (mask & 0x08) >0)// 4th mask
		{
		    lcd_mem_set_pixel((Ox+x0),(Oy-y0),(mask&0x80)>0?1:0);
		}

		if(max_len_x<x0)
		{
		     max_len_x=x0;
		}
		if(max_len_y<y0)
		{
		     max_len_y=y0;
		}

	}

	ret_xy=max_len_x;
	ret_xy<<=8;
	ret_xy+=max_len_y;
	return ret_xy;
}

void lcd_mem_put_circle(unsigned char Ox,unsigned char Oy, unsigned char radii)
{
	unsigned char el_rate=0x00;
	el_rate+=1;
    lcd_mem_put_ellipse( Ox,  Oy, radii, el_rate, 0x8F);
}

unsigned short lcd_put_percent_pillar(unsigned char percent)
{
   unsigned char Ox,Oy, radii, height,el_rate, real_h;
   unsigned short tmp_short= percent;
   unsigned short ret_xy;
   unsigned char delta_x;
   unsigned char delta_y;
   int i=0,k=0;
   Ox=20;
   Oy=21;
   radii=6;
   height=21;
   el_rate=0x00 + 6;

   // build the area fit for eyes
   tmp_short *= height;
   
   real_h = (tmp_short /100)&0xFF;

   if(0 == real_h)
   {
	  real_h=1;
   }
   else if(real_h>height)
   {
	  real_h=height;
   }
   else if(real_h >= height-1)
   {
	  real_h=height-2;
   }


   ret_xy=lcd_mem_put_ellipse(Ox,Oy, radii, el_rate, 0x8F);
   delta_x= ret_xy>>8;
   delta_y= ret_xy&0xFF;

   lcd_mem_bresenham_Line(Ox-delta_x, Oy, Ox-delta_x , Oy+height, 1);
   lcd_mem_bresenham_Line(Ox+delta_x, Oy, Ox+delta_x , Oy+height, 1);
   lcd_mem_put_ellipse(Ox,(Oy+height       ), radii, el_rate, 0x83);


   if(percent >0 )
   {
       lcd_mem_put_ellipse(Ox,(Oy+height-real_h), radii, el_rate, 0x83);
	   for(i=(Ox-delta_x)+1; i<Ox+delta_x; i++)
	   {
	       for( k =radii;  k>0;k--)
		   {
		       if(1 == lcd_mem_get_pixel(i, Oy+height+ k))
			   {
			       lcd_mem_bresenham_Line(i, (Oy+height-real_h)+ k , i, Oy+height+ k, 1);
				   break;
			   }
		   }
	   }
   }

   // disp the mem to lcd
   if(1)
   {
       unsigned char page_st,page_et;
	   page_st=(Oy-radii)/8;
	   page_et=(Oy+radii+ height)/8;
	   page_et%=8;
	   
	   for(i=page_st ; i<=page_et ;i++)
	   {	
		   for(k=(Ox-delta_x);k<=(Ox+delta_x);k++)
		   {
		        *(fbp + 128*i + k) = lcd_buf[k][i];
		   }
	   }
   }
   return 0;
}





