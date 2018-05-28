
#include "inc/app_def.h"





DATABASEINFO g_db_info[DATABASEINFO_CNT];
DATABASEINFO * g_table_info;
unsigned char * prop_file;
unsigned char  g_ot_buffer[TFNET_FILE_SIZE]={0};
int  g_ot_id_len[TFNET_ID_MAX]={0};



SYSPROP * sysprop;


void app_test_syn_data(signed char tb_idx)
{
    int i=0;
    T_CALL_RECORD call_record;

	T_SMS_RECORD sms_record;

	T_BOOK_ENTRY book_entry;

	
   
	 if(TABLEINFO_CALL_OUT == tb_idx    || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_CALL_OUT);}
	 
	 if(TABLEINFO_CALL_IN == tb_idx     || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_CALL_IN);}
	 
	 if(TABLEINFO_MISSED_CALL == tb_idx || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_MISSED_CALL);}
	 
	 if(TABLEINFO_PHONEBOOK == tb_idx   || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_PHONEBOOK);}
	 
	 if(TABLEINFO_SMS_INBOX == tb_idx   || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_SMS_INBOX);}
	 	 
     if(TABLEINFO_SMS_OUTBOX == tb_idx  || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_SMS_OUTBOX);}

	 if(TABLEINFO_SMS_DRAFT== tb_idx  || 0x7F ==tb_idx ){reset_table_index(TABLEINFO_SMS_DRAFT);}
	 	 
	
    if(TABLEINFO_PHONEBOOK == tb_idx    || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_PHONEBOOK].tb_info.rec_max_cnt;i++)
		{
	         memset(&book_entry,0,sizeof(book_entry));

			 sprintf(book_entry.num,"137135590%.2d", i);
		     sprintf(book_entry.name,"姓名%.2d", i);

	         app_record_append(TABLEINFO_PHONEBOOK, (unsigned char *)&book_entry, REC_POSTFIX_APPEND);
			 //delay_ms(300);

		}
    }
	
    if(TABLEINFO_CALL_IN == tb_idx     || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_CALL_IN].tb_info.rec_max_cnt;i++)
		{
	         memset(&call_record,0,sizeof(call_record));

			 sprintf(call_record.num,"137135590%.2d", i);
			 call_record.year = appsys.struct_date.year;
			 call_record.month= appsys.struct_date.mon;
			 call_record.day = appsys.struct_date.day;
			 call_record.hour= appsys.struct_time.hour;
			 call_record.minute = appsys.struct_time.min;
			 call_record.second= appsys.struct_time.sec;

	         app_record_append(TABLEINFO_CALL_IN, (unsigned char *)&call_record, REC_PREFIX_APPEND);


		}
    }

    if(TABLEINFO_MISSED_CALL == tb_idx || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_MISSED_CALL].tb_info.rec_max_cnt;i++)
		{
	         memset(&call_record,0,sizeof(call_record));

			 sprintf(call_record.num,"186135590%.2d", i);
			 call_record.year = appsys.struct_date.year;
			 call_record.month = appsys.struct_date.mon;
			 call_record.day = appsys.struct_date.day;
			 call_record.hour= appsys.struct_time.hour;
			 call_record.minute= appsys.struct_time.min;
			 call_record.second= appsys.struct_time.sec;
			 call_record.not_answer=1;
			 app_record_append(TABLEINFO_MISSED_CALL, (unsigned char *)&call_record, REC_PREFIX_APPEND);

		}
    }
	
    if(TABLEINFO_CALL_OUT == tb_idx   || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_CALL_OUT].tb_info.rec_max_cnt;i++)
		{
	         memset(&call_record,0,sizeof(call_record));

			 sprintf(call_record.num,"139135590%.2d", i);
			 call_record.year = appsys.struct_date.year;
			 call_record.month = appsys.struct_date.mon;
			 call_record.day = appsys.struct_date.day;
			 call_record.hour= appsys.struct_time.hour;
			 call_record.minute = appsys.struct_time.min;
			 call_record.second= appsys.struct_time.sec;
			 app_record_append(TABLEINFO_CALL_OUT, (unsigned char *)&call_record, REC_PREFIX_APPEND);

		}
    }


    if(TABLEINFO_SMS_OUTBOX == tb_idx   || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_SMS_OUTBOX].tb_info.rec_max_cnt;i++)
		{
	         memset(&sms_record,0,sizeof(sms_record));

			 sprintf(sms_record.str_num,"139135590%.2d", i);
			 sms_record.date.year = appsys.struct_date.year;
			 sms_record.date.mon = appsys.struct_date.mon;
			 sms_record.date.day = appsys.struct_date.day;
			 sms_record.time.hour= appsys.struct_time.hour;
			 sms_record.time.min= appsys.struct_time.min;
			 sms_record.time.sec= appsys.struct_time.sec;
	         sprintf(sms_record.context,"发送%.2d  1. 下传彩信的时候，不要接电话，不要收短信息； DONE2. 大容量数据通信成功，要告知成功；3. 库里面不要经常写FLASH 数据； DONE5. 有1K的彩信",i);
			 
			 app_record_append(TABLEINFO_SMS_OUTBOX, (unsigned char *)&sms_record, REC_PREFIX_APPEND);
		}
    }


    if(TABLEINFO_SMS_INBOX == tb_idx   || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_SMS_INBOX].tb_info.rec_max_cnt;i++)
		{
	         memset(&sms_record,0,sizeof(sms_record));

			 sprintf(sms_record.str_num,"139135590%.2d", i);
			 sms_record.date.year = appsys.struct_date.year;
			 sms_record.date.mon = appsys.struct_date.mon;
			 sms_record.date.day = appsys.struct_date.day;
			 sms_record.time.hour= appsys.struct_time.hour;
			 sms_record.time.min= appsys.struct_time.min;
			 sms_record.time.sec= appsys.struct_time.sec;
	         sprintf(sms_record.context,"接收%.2d  1. 下传彩信的时候，不要接电话，不要收短信息； DONE2. 大容量数据通信成功，要告知成功；3. 库里面不要经常写FLASH 数据； DONE5. 有1K的彩信",i);
			 
			 app_record_append(TABLEINFO_SMS_INBOX, (unsigned char *)&sms_record, REC_PREFIX_APPEND);
		}
    }


    if(TABLEINFO_SMS_DRAFT== tb_idx   || 0x7F ==tb_idx )
    {
		for(i=0;i<g_table_info[TABLEINFO_SMS_DRAFT].tb_info.rec_max_cnt;i++)
		{
	         memset(&sms_record,0,sizeof(sms_record));

			 sprintf(sms_record.str_num,"181135590%.2d", i);
			 sms_record.date.year = appsys.struct_date.year;
			 sms_record.date.mon = appsys.struct_date.mon;
			 sms_record.date.day = appsys.struct_date.day;
			 sms_record.time.hour= appsys.struct_time.hour;
			 sms_record.time.min= appsys.struct_time.min;
			 sms_record.time.sec= appsys.struct_time.sec;
	         sprintf(sms_record.context,"草稿%.2d  1. 下传彩信的时候，不要接电话，不要收短信息； DONE2. 大容量数据通信成功，要告知成功；3. 库里面不要经常写FLASH 数据； DONE5. 有1K的彩信",i);
			 
			 app_record_append(TABLEINFO_SMS_DRAFT, (unsigned char *)&sms_record, REC_PREFIX_APPEND);
		}
    }



	

	//appsys.flag_having_unread_sms=1;
	//appsys.flag_having_missed_call=1;



	DebugPrintf("=========数据仿真加入完成==========\r\n",NULL);
}
void app_test_function()
{
     
}

void app_check_database()
{

    unsigned char    ret=0;
    unsigned short   i=0;
	unsigned short   tmp_cell_start=1;
	unsigned long    tot_data_size =0;
	unsigned long    cur_data_size =0;

	if(1 ==appsys.flag_genie_trace)DebugPrintf("++++++START TO CHECK DATABASE!+++++\r\n",NULL);	

	

	//LOCAL FILES used 8K
	g_db_info[0].cell_info.cell_len=FLASH_CELL_LENGTH_1K;
	g_db_info[0].cell_info.cell_start=tmp_cell_start;
	g_db_info[0].cell_info.cell_cnt=8;
	g_db_info[0].tb_info.rec_len=FLASH_CELL_LENGTH_1K;
    tmp_cell_start+=g_db_info[0].cell_info.cell_cnt;	

    //TEMP FILES  used 20K
	g_db_info[1].cell_info.cell_len=FLASH_CELL_LENGTH_1K;
	g_db_info[1].cell_info.cell_start=tmp_cell_start;	
	g_db_info[1].cell_info.cell_cnt=20;
	g_db_info[1].tb_info.rec_len=FLASH_CELL_LENGTH_1K;	
	tmp_cell_start+=g_db_info[1].cell_info.cell_cnt;	

    //TABLE INDEXS used 16K
	g_db_info[2].cell_info.cell_len=FLASH_CELL_LENGTH_1K;
	g_db_info[2].cell_info.cell_start=tmp_cell_start;	
	g_db_info[2].cell_info.cell_cnt=16;
	g_db_info[2].tb_info.rec_len=FLASH_CELL_LENGTH_1K;	
	tmp_cell_start+=g_db_info[2].cell_info.cell_cnt;	

//=========================================================ABOVE IS FILES ,BELOW IS RECORDS 

    //SMS INBOX used 20K
	g_db_info[3].cell_info.cell_len=FLASH_CELL_LENGTH_2K; 
	g_db_info[3].cell_info.cell_start=tmp_cell_start;	
	g_db_info[3].cell_info.cell_cnt=10; 
	g_db_info[3].tb_info.rec_len=sizeof(T_SMS_RECORD);
	tmp_cell_start+=g_db_info[3].cell_info.cell_cnt;	
	
    //SMS OUTBOX used 10K
	g_db_info[4].cell_info.cell_len=FLASH_CELL_LENGTH_2K;
	g_db_info[4].cell_info.cell_start=tmp_cell_start;	
	g_db_info[4].cell_info.cell_cnt=5; 
	g_db_info[4].tb_info.rec_len=sizeof(T_SMS_RECORD);	
	tmp_cell_start+=g_db_info[4].cell_info.cell_cnt;	


	
    //PHONE BOOK  used 10K
	g_db_info[5].cell_info.cell_len=FLASH_CELL_LENGTH_2K;
	g_db_info[5].cell_info.cell_start=tmp_cell_start;	
	g_db_info[5].cell_info.cell_cnt=5; 
	g_db_info[5].tb_info.rec_len=sizeof(T_BOOK_ENTRY);	
	tmp_cell_start+=g_db_info[5].cell_info.cell_cnt;	

    //CALL IN    USED 10K
	g_db_info[6].cell_info.cell_len=FLASH_CELL_LENGTH_2K;
	g_db_info[6].cell_info.cell_start=tmp_cell_start;	
	g_db_info[6].cell_info.cell_cnt=5; 
	g_db_info[6].tb_info.rec_len=sizeof(T_CALL_RECORD);	
	tmp_cell_start+=g_db_info[6].cell_info.cell_cnt;	
	
    //MISSED CALL USED 10K
	g_db_info[7].cell_info.cell_len=FLASH_CELL_LENGTH_2K ;
	g_db_info[7].cell_info.cell_start=tmp_cell_start;	
	g_db_info[7].cell_info.cell_cnt=5;
	g_db_info[7].tb_info.rec_len=sizeof(T_CALL_RECORD);	
	tmp_cell_start+=g_db_info[7].cell_info.cell_cnt;	
	
    //CALL OUT   USED 10K
	g_db_info[8].cell_info.cell_len=FLASH_CELL_LENGTH_2K;
	g_db_info[8].cell_info.cell_start=tmp_cell_start;	
	g_db_info[8].cell_info.cell_cnt=5;
	g_db_info[8].tb_info.rec_len=sizeof(T_CALL_RECORD);	
	tmp_cell_start+=g_db_info[8].cell_info.cell_cnt;	


    //SMS DRAFT USED 10K
	g_db_info[9].cell_info.cell_len=FLASH_CELL_LENGTH_2K;
	g_db_info[9].cell_info.cell_start=tmp_cell_start;	
	g_db_info[9].cell_info.cell_cnt=5; 
	g_db_info[9].tb_info.rec_len=sizeof(T_SMS_RECORD);	
	tmp_cell_start+=g_db_info[9].cell_info.cell_cnt;	







	
	if(1 ==appsys.flag_genie_trace)DebugPrintf("++++++CHECKING DATABASE'S REC_MAX_CNT!+++++\r\n",NULL);	

	g_table_info=&g_db_info[DBINFO_TABLE_START];

	

	g_db_info[0].p_flh=(char * *)text_null; 
	g_db_info[1].p_flh=(char * *)text_null;  
    g_db_info[2].p_flh=(char * *)text_null;     
	
	g_table_info[TABLEINFO_SMS_INBOX].p_flh=(char * *)text_inbox;   
	g_table_info[TABLEINFO_SMS_OUTBOX].p_flh=(char * *)text_outbox;  
	g_table_info[TABLEINFO_SMS_DRAFT].p_flh=(char * *)text_draft; 
	g_table_info[TABLEINFO_PHONEBOOK].p_flh=(char * *)text_book;  
	g_table_info[TABLEINFO_CALL_IN].p_flh=(char * *)text_incoming_call;     
	g_table_info[TABLEINFO_MISSED_CALL].p_flh=(char * *)text_missed_calls; 
	g_table_info[TABLEINFO_CALL_OUT].p_flh=(char * *)text_outgoing_call;    




	
	
    for( i=0;i<DATABASEINFO_CNT;i++)
    {
		unsigned short  cell_id=0;
		
        g_db_info[i].cell_info.cell_max_rec_cnt= (g_db_info[i].cell_info.cell_len)/ (g_db_info[i].tb_info.rec_len ) ;

	    tot_data_size += (unsigned long )(g_db_info[i].cell_info.cell_len  *  g_db_info[i].cell_info.cell_cnt);
		
		g_db_info[i].tb_info.rec_max_cnt = g_db_info[i].cell_info.cell_max_rec_cnt * g_db_info[i].cell_info.cell_cnt;
        g_db_info[i].tb_info.rec_max_cnt=(g_db_info[i].tb_info.rec_max_cnt>510)? 510: g_db_info[i].tb_info.rec_max_cnt;

		if(g_db_info[i].tb_info.rec_max_cnt > g_db_info[i].cell_info.cell_max_rec_cnt * g_db_info[i].cell_info.cell_cnt)
		{
            g_db_info[i].tb_info.rec_max_cnt =g_db_info[i].cell_info.cell_max_rec_cnt * g_db_info[i].cell_info.cell_cnt;
		}

		if(g_db_info[i].tb_info.rec_max_cnt >10)
		{
		    g_db_info[i].tb_info.rec_max_cnt =(g_db_info[i].tb_info.rec_max_cnt /10) *10;
		}

		
    }

    for( i=0;i<DATABASEINFO_CNT;i++)
    {
		unsigned short  cell_id=0;
		for(cell_id=0;cell_id<g_db_info[i].cell_info.cell_cnt;cell_id++)
		{
		    g_ot_id_len[cell_id+g_db_info[i].cell_info.cell_start] = g_db_info[i].cell_info.cell_len/1024;
		}
    }
	
    if(1 ==appsys.flag_genie_trace)
    {DebugPrintf("+++++start to check db+++++ total size = %d /%d (Kbytes)\r\n", tot_data_size/1024,  TFNET_ID_MAX);}

	gu_load_data_from_flash();
	
	// appsys.flag_need_reset_flash=1;

    if(0 == app_check_prop() ||  (1 == appsys.flag_need_reset_flash))
    {
        sysprop->byte_language_type =LANGUAGE_SPANISH;
		
		msg(get_combined_string(get_multi_string((char * *)text_checking_database), "\n", get_multi_string((char * *)text_please_wait)));

	    for( i=0;i<DATABASEINFO_CNT;i++)
	    {
			unsigned short  cell_id=0;
		    unsigned char * huge_buf  = app_alloc_memory(g_db_info[i].cell_info.cell_len);

		    //if(1 ==appsys.flag_genie_trace)DebugPrintf("******  db_idx =%d start to alloc records cell_cnt=%d ****** \r\n", i,g_db_info[i].cell_info.cell_cnt);
		    memset(huge_buf,0,g_db_info[i].cell_info.cell_len);
            
			
			for(cell_id=0;cell_id<g_db_info[i].cell_info.cell_cnt;cell_id++)
			{		
		
				if(1 == flash_cell_write( cell_id+g_db_info[i].cell_info.cell_start, g_db_info[i].cell_info.cell_len, huge_buf ))
		        {
#if 0
					char str_show[8];
					memset(str_show,0,sizeof(str_show));
	                cur_data_size+=g_db_info[i].cell_info.cell_len;
					sprintf(str_show," %d%% ", cur_data_size*100/tot_data_size);
					lcd_goto_x_line((SCREEN_WIDTH-get_str_dots_width(str_show))/2 , 4);
					lcd_put_string(str_show);
#endif
		            if(1 ==appsys.flag_genie_trace)DebugPrintf("------> db_idx=%d id_no=%d alloced succeed \r\n", i ,cell_id);
				}
		   
			}
			app_free_memory(huge_buf);huge_buf=NULL;

			if( i >= DBINFO_TABLE_START)
			{
				 reset_table_index(i-DBINFO_TABLE_START);
			}
		}

	    ///////////////////////////////////////关键数据恢复出厂设置需要保护
        sysprop->flag_encyped_only  = 0 ;
	    sysprop->flag_lock_operator = 0 ;
		sysprop->flag_lock_base     = 0 ;
			
	  	load_from_factory();
		
		if(1 ==appsys.flag_genie_trace)DebugPrintf("++++open database succeed++++\r\n",NULL);
		delay_ms(1000);

        app_test_syn_data(0x7F);
	}
	else
	{
        if(1 ==appsys.flag_genie_trace)DebugPrintf("++++open database succeed++++\r\n",NULL);
	}




    for(i=0;i<(DATABASEINFO_CNT -DBINFO_TABLE_START);i++)
    {
       if( TABLEINFO_PHONEBOOK == i )
       {
	       if(g_table_info[i].tb_info.rec_max_cnt >50)  
	       {
	            // g_table_info[i].tb_info.rec_max_cnt=50;
		   }
	   }
	   else
	   {
	       if(g_table_info[i].tb_info.rec_max_cnt >30)  
	       {
	            // g_table_info[i].tb_info.rec_max_cnt=30;
		   }
	   }
	   
	   if(g_table_info[i].tb_info.rec_max_cnt >100)
	   {
          // g_table_info[i].tb_info.rec_max_cnt = 100;
	   }


	}




	
}


int ftnet_get_id_address(int flash_id)
{
	int i;
	unsigned long file_adr=0;

	for(i=0;i<=flash_id;i++)
	{
	    file_adr += g_ot_id_len[i];
	}
	return file_adr*1024;
}


void gu_flash_read(int id, int offset,unsigned char *data_buf, int data_buf_len)
{
	int file_address;
	
	//计算ID的地址;
	file_address=ftnet_get_id_address(id);
	memcpy(data_buf,&g_ot_buffer[file_address+offset],data_buf_len);
	
	//if(1 == appsys.flag_genie_trace)tfnet_print_log(" (gu_flash_read) read out  %d bytes data  ",data_buf_len);
		
}


void gu_flash_write(int id, int offset,unsigned char *data_buf, int data_buf_len)
{
	int file_address;
	
	//计算ID的地址;
	file_address=ftnet_get_id_address(id);
	memcpy(&g_ot_buffer[file_address+offset],data_buf,data_buf_len);
	
	if(1 == appsys.flag_genie_trace)DebugPrintf(" (gu_flash_write) address =%d (k) write %d bytes data to file !  \r\n",file_address/1024,data_buf_len);

}



unsigned char flash_cell_read(unsigned short cell_id, unsigned short data_len, unsigned char * pdata )
{
	 unsigned long tmp_size=data_len;
     //if(1 ==appsys.flag_genie_trace)DebugPrintf("flash read ---------->: %d cell_id=%d \r\n" , result,cell_id ); 
	 
	 gu_flash_read(cell_id, 0, (void *)pdata,tmp_size);

	 return 1;
}


unsigned char flash_cell_write( unsigned short cell_id, unsigned short data_len, unsigned char * pdata )
{
     unsigned long tmp_size=data_len;

	 if(1 ==appsys.flag_genie_trace)DebugPrintf("flash cell write ---------->:cell_id=%d data_len=%d\r\n" , cell_id ,data_len); 
	 gu_flash_write(cell_id,  0, (void *)pdata,tmp_size);
     appsys.word_change_times++;
     return 1;
}

void app_flash_might_save(char * str_reason)
{
     if(appsys.word_change_times >0)
     {
         appsys.word_change_times=0;
	     if(1 ==appsys.flag_genie_trace)DebugPrintf("app_flash_might_save, reason =%s\r\n" , str_reason); 
		 gu_save_data_2_flash();
     }
}






		


unsigned char app_flash_read( unsigned char dbinfo_idx, unsigned short rec_id, unsigned char * buf)
{
     unsigned char nret=0;
     unsigned char * huge_buf=NULL;     
	 unsigned char cell_id=0;

	 cell_id = rec_id/ (g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt);
	 if(cell_id >=g_db_info[dbinfo_idx].cell_info.cell_cnt)
	 {
          if(1 ==appsys.flag_genie_trace)DebugPrintf("[read] could not read table =[%s] , rec_id =%d ,cell_max_cnt=%d ,cell_cnt=%d \r\n", get_multi_string((char * *)g_db_info[dbinfo_idx].p_flh), rec_id, g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt, g_db_info[dbinfo_idx].cell_info.cell_cnt);
          return 0;
	 }

	 huge_buf=app_alloc_memory( g_db_info[dbinfo_idx].cell_info.cell_len);
     nret=flash_cell_read(cell_id+g_db_info[dbinfo_idx].cell_info.cell_start, g_db_info[dbinfo_idx].cell_info.cell_len, huge_buf);  
     memcpy(buf,&huge_buf[  (rec_id % (g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt)) *g_db_info[dbinfo_idx].tb_info.rec_len   ], g_db_info[dbinfo_idx].tb_info.rec_len);
     app_free_memory(huge_buf);huge_buf=NULL;
     return nret;
}


unsigned char app_flash_write( unsigned char dbinfo_idx, unsigned short rec_id, unsigned char * buf)
{

     int nret = 0;
     unsigned char * huge_buf=NULL;
	 unsigned char cell_id= rec_id/(g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt);

	 if(cell_id>=g_db_info[dbinfo_idx].cell_info.cell_cnt)
	 {
          if(1 ==appsys.flag_genie_trace)DebugPrintf("[write] could not write table =[%s] , rec_id =%d ,cell_max_rec_cnt=%d ,cell_cnt=%d \r\n",get_multi_string((char * *)g_db_info[dbinfo_idx].p_flh), rec_id, g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt, g_db_info[dbinfo_idx].cell_info.cell_cnt);
          return 0;
	 }

     huge_buf=app_alloc_memory(g_db_info[dbinfo_idx].cell_info.cell_len);

     if(g_db_info[dbinfo_idx].tb_info.rec_len  != g_db_info[dbinfo_idx].cell_info.cell_len )
     {
	    nret=flash_cell_read( cell_id+g_db_info[dbinfo_idx].cell_info.cell_start, g_db_info[dbinfo_idx].cell_info.cell_len, huge_buf);  
     }
	 memcpy(&huge_buf[  (rec_id % (g_db_info[dbinfo_idx].cell_info.cell_max_rec_cnt)) *g_db_info[dbinfo_idx].tb_info.rec_len ],buf, g_db_info[dbinfo_idx].tb_info.rec_len);

	 nret =flash_cell_write( cell_id + g_db_info[dbinfo_idx].cell_info.cell_start, g_db_info[dbinfo_idx].cell_info.cell_len, huge_buf);

     app_free_memory(huge_buf);huge_buf=NULL;
 	 return nret;
}


unsigned char * app_alloc_memory(unsigned short mem_size)
{ 
	return (unsigned char *) malloc(mem_size);
}
void app_free_memory(void * mem_buf)
{
    free(mem_buf);
}




void reset_table_index( unsigned char tb_idx)
{
    reset_table_index_ex( tb_idx, 0);

	switch(tb_idx)
	{
        case TABLEINFO_CALL_IN:
		case TABLEINFO_CALL_OUT:
		case TABLEINFO_MISSED_CALL:
		    if(TABLEINFO_MISSED_CALL == tb_idx)
		    {
                  appsys.flag_having_missed_call=0;
			}
			break;
		case TABLEINFO_SMS_INBOX:
		case TABLEINFO_SMS_OUTBOX:
	    case TABLEINFO_SMS_DRAFT:			
		    if(TABLEINFO_SMS_INBOX == tb_idx)
		    {
                  appsys.flag_having_unread_sms=0;
				  appsys.flag_sms_full=0;
			}
			
			break;
		case TABLEINFO_PHONEBOOK:
			break;


	}

}

void reset_table_index_ex( unsigned char tb_idx, unsigned short rec_used)
{
      unsigned short tmp=0;
	  unsigned char * pbuf =app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);
      unsigned short * p_idx=(unsigned short *)pbuf;
    
	  for(tmp=1;tmp<g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len/2;tmp++)
	  {
          p_idx[tmp]=tmp;
	  }
	  p_idx[0] =rec_used;

	  if(1 == flash_cell_write(tb_idx+g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf))
	  {
          if(1 ==appsys.flag_genie_trace)DebugPrintf("reset table index of <%s> succeed!\r\n",get_multi_string((char * *)g_table_info[tb_idx].p_flh));
	  }
      app_free_memory(pbuf);pbuf=NULL;

	  app_flash_might_save("RESET_TABLE_INDEX");
}


unsigned short app_get_record_used(unsigned char tb_idx)
{
    unsigned long tmp_long = app_get_record_used_and_index(tb_idx,0);
    return (unsigned short)(tmp_long>>16);
}

unsigned short app_get_record_index(unsigned char tb_idx, unsigned short rec_no)
{
    unsigned long tmp_long = app_get_record_used_and_index(tb_idx,rec_no);
    return (unsigned short)(tmp_long&0xFFFF);
}


unsigned long app_get_record_used_and_index(unsigned char tb_idx, unsigned short rec_no)
{
     unsigned long ret_idx=0;
     unsigned char * pbuf   = app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);

	 if(NULL != pbuf)
	 {
	     if(1 ==  flash_cell_read(tb_idx  + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf))
	     {
			 unsigned short * p_idx=(unsigned short *)pbuf;

			 ret_idx= p_idx[0];
			 ret_idx <<=16;
			 ret_idx +=p_idx[rec_no];
		 }
		 app_free_memory(pbuf);pbuf=NULL;
	 }
	 return ret_idx;
}




unsigned char app_record_read(unsigned char tb_idx, unsigned short  rec_no, unsigned char * pbuf )
{
	 unsigned short rec_used=0;
	 unsigned short rec_idx=0;
	 unsigned long tmp_long =app_get_record_used_and_index(tb_idx, rec_no);

	 rec_used= (unsigned short)((tmp_long>>16)&0xFFFF);
	 rec_idx = (unsigned short)((tmp_long>>0 )&0xFFFF);

	 if(rec_used> g_table_info[tb_idx].tb_info.rec_max_cnt  ||  rec_no> rec_used ||  rec_idx ==0)
	 {
         if(1 ==appsys.flag_genie_trace)DebugPrintf("error1 parameteres rec_no=%d  used=%d  idx=%d  cell_cnt=%d   in <app_record_read>\r\n", rec_no,rec_used, rec_idx,g_table_info[tb_idx].tb_info.rec_max_cnt);
		 return 0;
	 }
	 if(app_flash_read((tb_idx +DBINFO_TABLE_START), (rec_idx-1),  pbuf))
	 {
	     return 1;
	 }
	 return 0;
}


unsigned char app_record_edit(unsigned char tb_idx, unsigned short  rec_no,  unsigned char * pbuf )
{
	unsigned short rec_used=0;
	unsigned short rec_idx=0;



	rec_used=app_get_record_used(tb_idx);
	rec_idx =app_get_record_index(tb_idx, rec_no);
	appsys.flag_prerec_changed=1;
	if(rec_used> g_table_info[tb_idx].tb_info.rec_max_cnt||  rec_no> rec_used ||  rec_idx ==0)
	{
	     if(1 ==appsys.flag_genie_trace)DebugPrintf("error parameteres rec_no=%d  used=%d  idx=%d  cell_cnt=%d   in <app_record_edit>\r\n", rec_no,rec_used, rec_idx,g_table_info[tb_idx].cell_info.cell_cnt);
	     return 0;
	}

    if(app_flash_write(  (tb_idx+DBINFO_TABLE_START), (rec_idx-1), pbuf))
    {
        app_flash_might_save("APP_RECORD_EDDIT");
		
        if(1 ==appsys.flag_genie_trace)DebugPrintf("<%s> edit rec_no =%d\r\n",get_multi_string((char * *)g_table_info[tb_idx].p_flh), rec_no);

		
		
	    if(TABLEINFO_SMS_INBOX == tb_idx)
	    {
	 	    appsys.flag_having_unread_sms=check_if_having_new_sms();
	    }
	    else if(TABLEINFO_MISSED_CALL == tb_idx)
	    {
	 	    appsys.flag_having_missed_call=check_if_having_missed_call();
	    }


	
	    return 1;
    }
	return 0;
}

unsigned char app_record_append(unsigned char tb_idx,  unsigned char * pdata, unsigned char flag_fix_type )
{
   unsigned char nret =0;
   unsigned short  rec_used=app_get_record_used(tb_idx);
   appsys.flag_prerec_changed=1;
   if(rec_used == g_table_info[tb_idx].tb_info.rec_max_cnt)
   {
	   if(REC_PREFIX_APPEND == flag_fix_type)
	   {
	       app_record_delete( tb_idx, rec_used);
	   }
	   else if(REC_POSTFIX_APPEND == flag_fix_type)
	   {
           app_record_delete( tb_idx, 1);
	   }
	   rec_used=app_get_record_used(tb_idx);
   }

   if(REC_PREFIX_APPEND == flag_fix_type)
   {
       nret = app_record_insert( tb_idx, 1, pdata);
   }
   else if(REC_POSTFIX_APPEND == flag_fix_type)
   {
       nret = app_record_insert( tb_idx, (rec_used+1), pdata );
   }

   app_flash_might_save("APP_RECORD_APPEND");

   return nret;
}

unsigned char app_record_insert(unsigned char tb_idx, unsigned short  rec_no, unsigned char * pdata )
{
	 unsigned short rec_used=0;
	 unsigned short rec_idx=0;


	 rec_used=app_get_record_used(tb_idx);
	 rec_idx =app_get_record_index(tb_idx, rec_no);
	 appsys.flag_prerec_changed=1;
	 if(rec_used> g_table_info[tb_idx].tb_info.rec_max_cnt  ||  rec_no> (rec_used+1) ||  rec_idx ==0)
	 {
	     if(1 ==appsys.flag_genie_trace)DebugPrintf("error parameteres rec_no=%d  used=%d  idx=%d  cell_cnt=%d   in <app_record_insert>\r\n", rec_no,rec_used, rec_idx,g_table_info[tb_idx].cell_info.cell_cnt);
		 return 0;
	 }
	 else
	 {
		 if(rec_used == g_table_info[tb_idx].tb_info.rec_max_cnt)
		 {
	         //must delete first record fifo
	         app_record_delete(tb_idx, 1);
			 if(rec_no > rec_used)
			 {
	             rec_no= rec_used;
			 }
		 }
		 if(1)
		 {
		      unsigned short i=0;
			  unsigned short rec_new_idx=0;
	          unsigned char * pbuf   = app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);
	          unsigned short * pindex_array;

			  flash_cell_read(tb_idx +g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf);


			  pindex_array =(unsigned short *) pbuf;

			  rec_used=pindex_array[0];
			  rec_new_idx = pindex_array[rec_used+1];

		      for ( i=((g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len-2)/2); i>=1; i-- )
		      {
		          //from 1,because 0 is rec_used
		          if ( i< rec_no)
		          {
		               //no need change
		          }
		          else if ( i==rec_no )
		          {
		               continue;
		          }
		          else if ( i<=rec_used+1 )
		          {
		               //s[i]=s[i-1]
		               pindex_array[i]=pindex_array[i-1];
		          }
		          else
		          {
                        //no need change
		          }
		     }
			 pindex_array[rec_no]=rec_new_idx;
			 
		     // rec_used++
	         pindex_array[0]++;

			 //save index

			 flash_cell_write(tb_idx +g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf);

			 app_free_memory(pbuf);pbuf=NULL;

			 //save data record
			
			 if(1 == app_flash_write( (tb_idx+DBINFO_TABLE_START) , (rec_new_idx-1) , pdata))
			 {
				 if(1 ==appsys.flag_genie_trace)DebugPrintf("<%s> insert rec_no =%d\r\n",get_multi_string((char * *)g_table_info[tb_idx].p_flh), rec_no);

                 if(TABLEINFO_SMS_INBOX == tb_idx)
                 {
				     appsys.flag_sms_full =(app_get_record_used(TABLEINFO_SMS_INBOX) == g_table_info[TABLEINFO_SMS_INBOX].tb_info.rec_max_cnt)?1:0;
                 }


				 return 1;
			 }
			 else
			 {
				 if(1 ==appsys.flag_genie_trace)DebugPrintf("<%s> insert FAILED!! rec_no =%d\r\n",get_multi_string((char * *)g_table_info[tb_idx].p_flh), rec_no);
			 }
		 } 
	 }
	 return 0;
}


unsigned char app_record_delete(unsigned char tb_idx, unsigned short  rec_no)
{
	 unsigned short rec_used=0;
	 unsigned short rec_idx=0;

	
	 rec_used=app_get_record_used(tb_idx);
	 rec_idx =app_get_record_index(tb_idx, rec_no);
	 appsys.flag_prerec_changed=1;

	 if(rec_used> g_table_info[tb_idx].tb_info.rec_max_cnt  ||  rec_no> rec_used ||  rec_idx ==0 || rec_used == 0)
	 {
	     if(1 ==appsys.flag_genie_trace)DebugPrintf("error parameteres rec_no=%d  used=%d  idx=%d  cell_cnt=%d   in <app_record_delete>\r\n", rec_no,rec_used, rec_idx,g_table_info[tb_idx].tb_info.rec_max_cnt);
	 }
	 else
	 {   
	       unsigned short i=0;
           unsigned char * pbuf   = app_alloc_memory(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len);
           unsigned short * pindex_array;




		   flash_cell_read(tb_idx + g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf);

		   pindex_array =(unsigned short *) pbuf;

   	       for(i=1; i<(g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len)/2 ;i++)
		   {
		        //from 1,because 0 is rec_used
		        if ( i<rec_no )
		        {
		            //no need change
		        }
		        else if ( i<rec_used )
		        {
		            //s[i]=s[i+1] 
                    pindex_array[i]=pindex_array[i+1];
				}
		        else if ( i==rec_used )
		        { 
		            //move the delete value to the end of array
		            pindex_array[i]=rec_idx;
		        }
				else
				{
                    //no need change
				}
		   }
		   // rec_used--
           pindex_array[0]--;

		  
		   flash_cell_write(tb_idx +g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_start, g_db_info[DBINFO_INDEXS_TABLE].cell_info.cell_len, pbuf);

           app_free_memory(pbuf);pbuf=NULL;


		   if(1 ==appsys.flag_genie_trace)DebugPrintf("<%s> delete rec_no =%d\r\n",get_multi_string((char * *)g_table_info[tb_idx].p_flh), rec_no);

		   if(TABLEINFO_SMS_INBOX == tb_idx)
		   {
		 	    appsys.flag_having_unread_sms=check_if_having_new_sms();
			    appsys.flag_sms_full =0;
		   }
		   else if(TABLEINFO_MISSED_CALL == tb_idx)
		   {
		 	    appsys.flag_having_missed_call=check_if_having_missed_call();
		   }

		   app_flash_might_save("APP_RECORD_DELETE");

		   return 1;
	}
	return 0;
}



void app_save_prop()
{
    unsigned short prop_size= sizeof(SYSPROP);
	sysprop->prop_size=prop_size;
	sysprop->prop_size<<=16;
	sysprop->prop_size+=(unsigned short)(~prop_size);

	strcpy(sysprop->dversion, database_ver);
	
    save_file_to_flash(DBINFO_LOCAL_FILES, DB_LOCAL_PROP_CELL_POS, (unsigned char *)sysprop);

	app_flash_might_save("APP_SAVE_PROP");
}






void load_from_factory()
{

    unsigned char bkup_val1;
	unsigned char bkup_val2;
	unsigned char bkup_val3;
	unsigned char bkup_val4;

	char str_bkup_xh_ip[TCP_IP_ADDRESS_LENGTH+1];


	if(1 ==appsys.flag_genie_trace)DebugPrintf("++++load_from_factory++++\r\n",NULL);

	memset(str_bkup_xh_ip,0,sizeof(str_bkup_xh_ip));
	
    bkup_val1 = sysprop->flag_encyped_only ;
	bkup_val2 = sysprop->flag_lock_operator;
	bkup_val3 = sysprop->flag_lock_base;


    memset(sysprop, 0, FLASH_CELL_LENGTH_1K); 
	
    fill_prop_with_pri(); 

    sysprop->flag_encyped_only  = bkup_val1 ;
    sysprop->flag_lock_operator = bkup_val2 ;
	sysprop->flag_lock_base     = bkup_val3 ;



	
    app_save_prop();	


}


void save_file_to_flash(unsigned char dbinfo_idx, unsigned short cell_pos,unsigned char * pdata)
{
     if(cell_pos< g_db_info[dbinfo_idx].cell_info.cell_cnt)
     {
		 flash_cell_write(cell_pos + g_db_info[dbinfo_idx].cell_info.cell_start,g_db_info[dbinfo_idx].cell_info.cell_len,  pdata);
     }
	 else
	 {
         if(1 ==appsys.flag_genie_trace)DebugPrintf("[save file] too big index for %s, cell_pos=%d  cell_cnt=%d\r\n",get_multi_string((char * *)g_db_info[dbinfo_idx].p_flh), cell_pos,g_table_info[dbinfo_idx].cell_info.cell_cnt);
	 }
	 
}
unsigned char load_file_from_flash(unsigned char dbinfo_idx, unsigned short cell_pos, unsigned char * pdata)
{
     if(cell_pos<g_db_info[dbinfo_idx].cell_info.cell_cnt)
     { 
         return flash_cell_read(cell_pos + g_db_info[dbinfo_idx].cell_info.cell_start,g_db_info[dbinfo_idx].cell_info.cell_len,  pdata);
     }
	 else
	 {
	     if(1 ==appsys.flag_genie_trace)DebugPrintf("[load file] too big index for %s, cell_pos=%d  cell_cnt=%d\r\n",get_multi_string((char * *)g_db_info[dbinfo_idx].p_flh), cell_pos,g_table_info[dbinfo_idx].cell_info.cell_cnt);
	 }
	 return 0;
}
void fill_prop_with_pri()
{
    unsigned char i=0;
    unsigned short prop_size= sizeof(SYSPROP);
	sysprop->prop_size=prop_size;
	sysprop->prop_size<<=16;
	sysprop->prop_size+=(unsigned short)(~prop_size);

	sysprop->struct_alarm.alarm_on=0;
	sysprop->struct_alarm.alarm_ring=0;
	sysprop->struct_alarm.alarm_time.hour=6;
	sysprop->struct_alarm.alarm_time.min=30;
	
	sysprop->byte_backlightoff_timeout=0;
	sysprop->byte_contrast_volume=DEFAULT_LCD_CONTRAST;
	sysprop->byte_decimal_pos=2;
	
	
	sysprop->byte_income_ring =8;
	sysprop->byte_sms_ring=3;
	
	sysprop->byte_talk_volume=2;
    sysprop->byte_ring_volume=2;
    sysprop->byte_sms_volume=2;

	sysprop->byte_sleep_timeout=1;
	
	


    sysprop->dword_call_in_tm_total=0;
    sysprop->dword_call_out_tm_total=0;
	

	
	

	sysprop->flag_auto_lock_keypad=0;
	sysprop->flag_auto_redial=0;
	sysprop->flag_clock_format=1;
	sysprop->flag_incomecall_allow=1;
	sysprop->flag_outcall_allow=1;
	sysprop->flag_internationalcall_allow=1;
	
	sysprop->flag_lock_phone=0;





	sysprop->byte_network_type =NETWORK_TYPE_2G;
	
	sysprop->flag_key_tone =1;

	sysprop->byte_language_type =LANGUAGE_SPANISH;



    sysprop->byte_call_delay_s =4;


	
	sysprop->auto_redial.redial_cnt= AUTO_REDIAL_CNT;
	sysprop->auto_redial.redial_interval=AUTO_REDIAL_INTERVAL;




   


}

unsigned char app_check_prop()
{
    char tmp_buf[32];
	memset(tmp_buf,0,sizeof(tmp_buf));
	sprintf(tmp_buf, "prop_size=%d", sizeof(SYSPROP));
    if(1 ==appsys.flag_genie_trace)DebugPrintf("%s\r\n",tmp_buf); 

	
	if(load_file_from_flash(DBINFO_LOCAL_FILES, DB_LOCAL_PROP_CELL_POS,(unsigned char *)sysprop))
	{
        unsigned short hi_word,low_word;
		hi_word =  (unsigned short)((sysprop->prop_size&0xFFFF0000)>>16);
		low_word=  (unsigned short)((sysprop->prop_size&0x0000FFFF)>>0);
        if(1 ==appsys.flag_genie_trace)DebugPrintf("---------------PROP ID already created-------------\r\n",NULL);
		if(hi_word == (unsigned short)(~low_word))
		{ 
	
            if(0 == strcmp(sysprop->dversion,  database_ver))
            {
			    if(1 ==appsys.flag_genie_trace)DebugPrintf("load prop file succeed!\r\n",NULL);
				appsys.flag_key_tone=sysprop->flag_key_tone;
	            return 1;
            }
			else
			{
			    if(strlen(sysprop->dversion)>0)
			    {
                     if(1 ==appsys.flag_genie_trace)DebugPrintf("prop version not match old=%s new=%s\r\n", sysprop->dversion, database_ver);
			    }
			}		
		}
		else
		{
            if(1 ==appsys.flag_genie_trace)DebugPrintf("prop crc error,prop_size=0x%.8X ,hi=0x%.4X ,low=0x%.4X\r\n",sysprop->prop_size, hi_word,low_word);
		}
	}
	else
	{
	    if(1 == appsys.flag_genie_trace)DebugPrintf(" can not read out prop id!\r\n",NULL);
	}
	
	return 0;
}



