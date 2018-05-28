
#include "inc/FindCode.h"

unsigned char bBhLastHzNum;
unsigned char fBhPrevNext;
unsigned char bBhCurPageNo;
unsigned long  lBXCODETBL_OFFSET;
unsigned short nBX_INDEX_TOTAL;
extern unsigned char gHintFormFeed;		/* 汉字上下页指示标志 */

T_BXINDEX_REC tBhIndexRec;//static 
struct T_FINDBXLOG tBhFindLog; //static
T_CODETBL_OFFSET stBhCodeTblOffset[5];//static 

unsigned char gsHzBuffer[MAX_PY_BUFFER+20];

extern unsigned char ChkIfHaveTheHz(unsigned char *pHzBuffer, unsigned char *psHz);
extern void asc_to_bcd(unsigned char *bcd, unsigned char *asc, unsigned char asc_len);
extern unsigned char ReadMaBiao(unsigned long nAddr,unsigned char *pBuffer,unsigned short bReadLen);

/****************************************************/
/*  在设备上电初始化函数中调用该函数，以便将	    */
/*	索引表的所有主键值读出，加快输入汉字的速度	    */
/****************************************************/
unsigned char InitBHIndexTbl()
{
	unsigned char rc, bFirstCode;
	uint16_t i ,j, nIndexTotal;
	uint32_t nPosition;
	T_BXINDEX_REC tIndexRec;
	unsigned char sBuf[4];

	memset(&tBhFindLog, 0, sizeof(struct T_FINDBXLOG));

	rc =ReadMaBiao(0L, sBuf, 4);
	if (rc == FALSE)		return FALSE;
	if(memcmp(sBuf, (const char *)("5D09"), 4)!=0)		return FALSE;

	rc =ReadMaBiao(6L, sBuf, 4);
	if (rc == FALSE)		return FALSE;
	lBXCODETBL_OFFSET=((uint32_t)sBuf[0]*256+(uint32_t)sBuf[1])*65536+((uint32_t)sBuf[2]*256+(uint32_t)sBuf[3]);

	nPosition =lBXCODETBL_OFFSET;
	rc =ReadMaBiao(nPosition, sBuf, 2);
	if (rc == FALSE)	return FALSE;

	nPosition +=INDEX_OFFSET;
	nIndexTotal =(uint16_t)sBuf[0]*256 + (uint16_t)sBuf[1];
	nBX_INDEX_TOTAL =nIndexTotal;

	bFirstCode =0x10;
	memset(stBhCodeTblOffset, 0, sizeof(stBhCodeTblOffset)); //willcheck
	for(i=0,j=0; i<nIndexTotal; i++)
	{
		rc =ReadMaBiao(nPosition, (unsigned char *)&tIndexRec, BXINDEX_REC_SIZE);
		if (rc == FALSE)		return FALSE;
		if ((tIndexRec.sKey[0]&0xf0) !=bFirstCode)
		{
			stBhCodeTblOffset[j].end_no =i-1;
			j ++;
			bFirstCode +=0x10;
			stBhCodeTblOffset[j].start_no =i;
		}
		if (i>=(nIndexTotal-1))
		{
			stBhCodeTblOffset[j].end_no =i;
			break;
		}

		nPosition +=BXINDEX_REC_SIZE;
	}

	return TRUE;
}

unsigned char FindIndexTbl(char *psInputCode,unsigned char *pKeyCode, 
						   unsigned short *nIndexNo)
{
	unsigned char no;
//wjhdelete	int rc;
	int16_t 	rc;
	uint16_t  nFirst, nLast, nMiddle;// uint16_t
	unsigned long nPosition;
	T_BXINDEX_REC tIndexRec;

	if (psInputCode[0]<'1' || psInputCode[0]>'5')
		return FALSE;
	no =psInputCode[0]-'1';
	nFirst = stBhCodeTblOffset[no].start_no;
	nLast = stBhCodeTblOffset[no].end_no;

	while(nFirst <= nLast)
	{
		nMiddle = (nFirst+nLast)/2;

		nPosition =lBXCODETBL_OFFSET+INDEX_OFFSET+nMiddle*BXINDEX_REC_SIZE;
		rc =ReadMaBiao(nPosition, (unsigned char *)&tIndexRec, BXINDEX_REC_SIZE);
		if (rc == FALSE)
			return FALSE;

		rc = memcmp(pKeyCode, tIndexRec.sKey, 2);
		if (rc == 0)	// identical
		{
			nPosition =lBXCODETBL_OFFSET + INDEX_OFFSET;
			nPosition +=nMiddle*BXINDEX_REC_SIZE;
			rc =ReadMaBiao(nPosition, (unsigned char *)&tBhIndexRec, BXINDEX_REC_SIZE);
			*nIndexNo =nMiddle;
			return TRUE;
		}
		if (rc < 0)		//  less than
			nLast = nMiddle - 1;
		else			//  greater than
			nFirst = nMiddle + 1;
	}
	return FALSE;
}

void RightTrim(char *ptr, unsigned char value)
{
	unsigned char i, bLen;

	bLen =strlen((char *)ptr);
	for (i=bLen-1; i>=0; i--)
	{
		if (*(ptr+i) != value)
			break;
		*(ptr+i) =0;
	}
}

unsigned char GetCompatibleCode(unsigned char *pMainKey, unsigned char *pDescriptor, 
					   unsigned char *pResultStr)
{
	unsigned char i, j, bKeyLen;
	unsigned char sCompatibleCode[5];
	unsigned char cReferData[BX_MAX_REFER_VAL] ={'9', '8', '7', '6', '5'};

	bKeyLen =(unsigned char)strlen((char *)pMainKey);
	if (bKeyLen >3)		return FALSE;
	for (i=0,j=3-bKeyLen; i<bKeyLen; i++,j++)
	{
		if (pDescriptor[j] =='1' && pMainKey[i]<='5')
			sCompatibleCode[i] =cReferData[pMainKey[i]-'1'];
		else
			sCompatibleCode[i] =pMainKey[i];
	}
	sCompatibleCode[i] =0;
	if (memcmp(pResultStr, sCompatibleCode, bKeyLen) >=0)
		return FALSE;
	memcpy(pResultStr, sCompatibleCode, bKeyLen+1);
	return TRUE;
}
unsigned char ChkTailCode(unsigned short *pSrcData, unsigned char bDataLen, 
						  unsigned char *pKeyCode)
{
	short nFirst, nLast, nMiddle;
	unsigned short nKeyCode;

	nFirst = 0;
	nLast = bDataLen-1;
	nKeyCode =atoi((char *)pKeyCode);
	while(nFirst <= nLast)
	{
		nMiddle = (nFirst+nLast)/2;

		if (nKeyCode ==pSrcData[nMiddle])	// identical
			return TRUE;
		if (nKeyCode < pSrcData[nMiddle])		//  less than
			nLast = nMiddle - 1;
		else			//  greater than
			nFirst = nMiddle + 1;
	}
	return FALSE;
}
/*   no wildcard  */
unsigned char ReadHzCode_1(char *pInputCode)
{
//wjh200703014	unsigned char cReferData[BX_MAX_REFER_VAL] ={9, 8, 7, 6, 5};
	unsigned char i, j, k, rc, bReadNum;
	unsigned char bCodeLen, bFlag;
//	unsigned char bCodeFlag, bMaskFlag;
	unsigned short  nIndexNo, bReadLen;
	uint32_t  nPosition;
	unsigned short nHzNum, nBufLen, nReadedNum, nRamainTotal;
	unsigned char sMainKey[BX_MAINKEY_LEN+1], sCode[7], sKeyCode[BX_MAINKEY_LEN+1], sBcdCode[2];
	unsigned char bTailFlag, bBcdTail;
	unsigned char sTemp[7];
	T_BXCODE_REC stBxCodeRec[50];

	if (tBhFindLog.bNum >=MAX_BH_BUFFER/2)		/* 缓冲已满 */
		return TRUE;

	bCodeLen =strlen((char *)pInputCode);
	memcpy(sCode, pInputCode, bCodeLen);
	sCode[bCodeLen] =0;
	RightTrim((char*)sCode, '0');

	memset(sMainKey, 0, sizeof(sMainKey));
	bCodeLen =strlen((char *)sCode);
	if (bCodeLen >6)		return FALSE;

	k =bCodeLen;
	if (k >BX_MAINKEY_LEN)
		k =BX_MAINKEY_LEN;
	memcpy(sMainKey, sCode, k);

	if (bCodeLen > BX_MAINKEY_LEN)
		bCodeLen =BX_MAINKEY_LEN;

	memset(sKeyCode, '0', sizeof(sKeyCode));
	memcpy(sKeyCode, sMainKey, bCodeLen);
	sKeyCode[BX_MAINKEY_LEN] =0;
	asc_to_bcd(sBcdCode, sKeyCode, BX_MAINKEY_LEN);

	rc =FindIndexTbl(pInputCode, sBcdCode, &nIndexNo);	// result in struct("tBhIndexRec")
	if (rc == FALSE)
		return FALSE;


	bTailFlag =0;
	bCodeLen =strlen((char *)sCode);
	if (bCodeLen >BX_MAINKEY_LEN)
	{
		memset(sTemp, '0', 2);
		sTemp[2] =0;
		memcpy(sTemp, &sCode[BX_MAINKEY_LEN], bCodeLen-BX_MAINKEY_LEN);
		asc_to_bcd(&bBcdTail, sTemp, 2);
		bTailFlag=0xf0;
		if (bCodeLen==6)
			bTailFlag=0xff;
		memcpy(tBhFindLog.sTailCode, &bBcdTail, 1);
	}
	memcpy(tBhFindLog.sInpCode, sCode, strlen((char*)sCode));
	tBhFindLog.nTotal =(uint16_t)tBhIndexRec.sRecNum[0]*256+(uint16_t)tBhIndexRec.sRecNum[1];
#if 1	
	tBhFindLog.nCurPosi =lBXCODETBL_OFFSET+INDEX_OFFSET+(uint32_t)nBX_INDEX_TOTAL*BXINDEX_REC_SIZE;
	tBhFindLog.nCurPosi +=(uint32_t)tBhIndexRec.sAddr[0]*65536+(uint32_t)tBhIndexRec.sAddr[1]*256+(uint32_t)tBhIndexRec.sAddr[2];


#else//
	tPyFindLog.nTotal =(uint16_t)tIndexRec.sRecNum[0]*256+(uint16_t)tIndexRec.sRecNum[1];
	tPyFindLog.nCurPosi =lPYCODETBL_OFFSET+INDEX_OFFSET+nPY_INDEX_TOTAL*PYINDEX_REC_SIZE;
	tPyFindLog.nCurPosi +=(uint32_t)tIndexRec.sAddr[0]*256+(uint32_t)tIndexRec.sAddr[1];
	

	tBhFindLog.nCurPosi=lBXCODETBL_OFFSET;
	tBhFindLog.nCurPosi+=INDEX_OFFSET;
	tBhFindLog.nCurPosi+=nBX_INDEX_TOTAL*BXINDEX_REC_SIZE;
	
	tBhFindLog.nCurPosi +=tBhIndexRec.sAddr[0]*65536;
	tBhFindLog.nCurPosi +=tBhIndexRec.sAddr[1]*256;
	tBhFindLog.nCurPosi +=tBhIndexRec.sAddr[2];


	nPosition=lBXCODETBL_OFFSET+(uint32_t)lBXCODETBL_OFFSET+(uint32_t)nBX_INDEX_TOTAL*(uint32_t)BXINDEX_REC_SIZE;
	tBhFindLog.nCurPosi =nPosition;
	
	nPosition=(uint32_t)tBhIndexRec.sAddr[0]*65536+(uint32_t)tBhIndexRec.sAddr[1]*256+(uint32_t)tBhIndexRec.sAddr[2];
	tBhFindLog.nCurPosi +=nPosition;
#endif

	nHzNum =0;
	nPosition =tBhFindLog.nCurPosi;

	switch(bCodeLen)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		j=0;
		nReadedNum =0;
		nBufLen =strlen((char *)tBhFindLog.sHzBuffer);
		while(nReadedNum <tBhFindLog.nTotal)
		{
			bReadNum =15;
			if (tBhFindLog.bCodeFlag)
				bReadNum =50;
			if ((tBhFindLog.nTotal-nReadedNum) <bReadNum)
				bReadNum =(unsigned char)(tBhFindLog.nTotal-nReadedNum);
			bReadLen =bReadNum*BXCODE_REC_SIZE;
			rc =ReadMaBiao(nPosition,(unsigned char*)stBxCodeRec,bReadLen);
			if (rc == FALSE)		return FALSE;
			for(i=0; i<bReadNum; i++)
			{
				if (tBhFindLog.bCodeFlag)
				{
					if ((stBxCodeRec[i].sTailCode[1]&tBhFindLog.bMaskFlag)!=tBhFindLog.bCodeFlag)
						continue;
				}
				if (ChkIfHaveTheHz(tBhFindLog.sHzBuffer, stBxCodeRec[i].sHz) ==TRUE)
					continue;
				memcpy(&tBhFindLog.sHzBuffer[nBufLen+2*j], stBxCodeRec[i].sHz, 2);
				tBhFindLog.bNum ++;
				j++;
			}
			tBhFindLog.nCurPosi +=bReadLen;
			nPosition +=bReadLen;
			tBhFindLog.nReadedNum +=bReadNum;
			nReadedNum +=bReadNum;
			if (tBhFindLog.bNum>10)		break;
		}
		break;
	case 5:
	case 6:
	default:
		nRamainTotal =tBhFindLog.nTotal;
		nReadedNum =0;
		while((nReadedNum <nRamainTotal)||tBhFindLog.bNum<30)
		{
			if ((nRamainTotal-nReadedNum) >50)
				bReadNum =50;
			else
				bReadNum =(unsigned char)(nRamainTotal-nReadedNum);
			if (bReadNum <=0)
				break;

			bReadLen =bReadNum*BXCODE_REC_SIZE;
			rc =ReadMaBiao(nPosition+nReadedNum*BXCODE_REC_SIZE,
							(unsigned char *)stBxCodeRec,bReadLen);
			if (rc == FALSE)			return FALSE;
			nBufLen =strlen((char *)tBhFindLog.sHzBuffer);
			for(i=0,j=0; i<bReadNum; i++)
			{
				rc =TRUE;
				if (tBhFindLog.bCodeFlag)
				{
					rc =FALSE;
					if ((stBxCodeRec[i].sTailCode[1]&tBhFindLog.bMaskFlag)==tBhFindLog.bCodeFlag)
						rc =TRUE;
				}

				bFlag =stBxCodeRec[i].sTailCode[0]&bTailFlag;
				if (bFlag==bBcdTail&&rc==TRUE)
				{
					if (ChkIfHaveTheHz(tBhFindLog.sHzBuffer, stBxCodeRec[i].sHz) ==FALSE)
					{
						memcpy(&tBhFindLog.sHzBuffer[nBufLen+2*j], stBxCodeRec[i].sHz, 2);
						tBhFindLog.bNum ++;
						j++;
					}
				}
				tBhFindLog.nCurPosi +=BXCODE_REC_SIZE;
				tBhFindLog.nReadedNum ++;

				if (tBhFindLog.bNum >=12)
					break;
			}
			if (tBhFindLog.bNum >=12)
				break;
			nReadedNum +=bReadNum;
		}//while
		break;
	} //switch
	if (tBhFindLog.bNum ==0)
		return FALSE;
	return TRUE;
}

void ConvToNumeral(char *psInpCode, char *psResult)
{
	unsigned char cReferData[BX_MAX_REFER_VAL]={'5', '4', '3', '2', '1'};
	unsigned char i, j, bCodeLen, bCodeFlag, bMaskFlag;

	bCodeLen =strlen(psInpCode);
	bCodeFlag =0;
	bMaskFlag =0;
	j =bCodeLen;
	if (j>4)	j=4;
	for(i=0; i<j; i++)
	{
		if (psInpCode[i] >'5')
		{
			bCodeFlag |=1<<(4+i);
			psResult[i] =cReferData[psInpCode[i]-'5'];
		}
		else
			psResult[i]=psInpCode[i];
		bMaskFlag |=1<<(4+i);
	}
	psResult[bCodeLen] =0;

	for(i=4,j=0; (bCodeLen>4)&&(i<bCodeLen); i++,j++)
	{
		if (psInpCode[i] >'5')
		{
			bCodeFlag |=1<<j;
			psResult[i] =cReferData[psInpCode[i]-'5'];
		}
		else
			psResult[i]=psInpCode[i];
		bMaskFlag |=1<<j;
	}
	tBhFindLog.bCodeFlag =bCodeFlag;
	tBhFindLog.bMaskFlag =bMaskFlag;
}

unsigned char ReadBXHzCode(char *pInpCode,unsigned char *pResult)
{
	unsigned char rc,i,j,k;
	unsigned char bCodeLen;
	unsigned short nReadTotal;
	char pNewInpCode[8];
	unsigned char *pPoint;

	memset(&tBhFindLog, 0, sizeof(struct T_FINDBXLOG));

	memset(pNewInpCode, 0, sizeof(pNewInpCode));
	tBhFindLog.sHzBuffer =gsHzBuffer;
	memset(gsHzBuffer, 0, sizeof(gsHzBuffer));

	ConvToNumeral(pInpCode, pNewInpCode);
	pPoint =(unsigned char *)strchr((char *)pNewInpCode, '0');
	if (pPoint != NULL)
		*pPoint =0;

	bCodeLen =strlen((char *)pNewInpCode);
	k =bCodeLen;
	if (bCodeLen >BX_MAINKEY_LEN)
		k =BX_MAINKEY_LEN;

	rc =ReadHzCode_1(pNewInpCode);
	if (rc ==FALSE)
		return FALSE;

	nReadTotal =strlen((char *)tBhFindLog.sHzBuffer);
	if (nReadTotal ==0)
		return FALSE;

	memset(pResult, 0, FETCH_HZNUM*2+1);
	i =0;
	j =0;
	while(1)
	{
		if ((tBhFindLog.bPoint >=nReadTotal) || (i >=FETCH_HZNUM))
			break;
		if (tBhFindLog.sHzBuffer[tBhFindLog.bPoint] !=0x20)
		{
			memcpy(&pResult[i*2], &tBhFindLog.sHzBuffer[tBhFindLog.bPoint], 2);
			i ++;
		}
		tBhFindLog.bPoint +=2;
		j ++;
	}

	gHintFormFeed =0;
	if (tBhFindLog.sHzBuffer[tBhFindLog.bPoint] >0x80)
		gHintFormFeed =2;

	bBhLastHzNum =j;
	fBhPrevNext =NEXT_PAGE;
	bBhCurPageNo =0;
	return TRUE;
}

unsigned char ReadBXNextHz(unsigned char *pResult)
{
	unsigned char i,j,rc,bReadLen,bCodeLen,bReadNum;
	unsigned char bFlag,bTailFlag;
	uint16_t  nReadTotal, nBufLen;
	T_BXCODE_REC stBxCodeRec[30];
	bTailFlag=0;//[20070418wjh]add
	memset(&stBxCodeRec,0,sizeof(stBxCodeRec));//[20070417wjh]add
	i =0;
	gHintFormFeed =0;
	if (tBhFindLog.bPoint >(uint16_t)i)
		gHintFormFeed =1;

	nReadTotal =strlen((char *)tBhFindLog.sHzBuffer);
	if (nReadTotal ==0)
		return FALSE;

	if (fBhPrevNext ==PREV_PAGE && bBhCurPageNo!=0)
	{
		i =0;
		j =0;
		while(1)
		{
			if ((tBhFindLog.bPoint >=nReadTotal) || (i >=FETCH_HZNUM))
				break;
			if (tBhFindLog.sHzBuffer[tBhFindLog.bPoint] !=0x20)
				i ++;
			tBhFindLog.bPoint +=2;
			j ++;
		}
	}

	bCodeLen =strlen((char*)tBhFindLog.sInpCode);
	if (bCodeLen>BX_MAINKEY_LEN)
	{
		bTailFlag=0xf0;
		if (bCodeLen==6)
			bTailFlag=0xff;
	}
	memset(pResult, 0, FETCH_HZNUM*2+1);
	i =0;
	j =0;
	if ((tBhFindLog.bNum<MAX_BH_BUFFER/2)&&(tBhFindLog.nTotal>tBhFindLog.nReadedNum))
	{
		if (bCodeLen<=4)
		{
			j=0;
			nBufLen =strlen((char *)tBhFindLog.sHzBuffer);
			while((tBhFindLog.nReadedNum <tBhFindLog.nTotal)&&(j<FETCH_HZNUM))
			{
				bReadNum =10;
				if (tBhFindLog.bCodeFlag)
					bReadNum =30;
				if ((tBhFindLog.nTotal-tBhFindLog.nReadedNum) <10)
					bReadNum =(unsigned char)(tBhFindLog.nTotal-tBhFindLog.nReadedNum);
				if ((tBhFindLog.bNum+bReadNum)>MAX_BH_BUFFER/2)
					bReadNum =(unsigned char)(MAX_BH_BUFFER/2-tBhFindLog.bNum);
				
				if (bReadNum ==0)
				{  
						return FALSE;
				}
				bReadLen =bReadNum*BXCODE_REC_SIZE;
				rc =ReadMaBiao(tBhFindLog.nCurPosi,(unsigned char*)stBxCodeRec,(uint16_t)bReadLen);
				if (rc == FALSE)	
				{
						return FALSE;
				}
				for(i=0; i<bReadNum; i++)
				{
					if (tBhFindLog.bCodeFlag)
					{
						if ((stBxCodeRec[i].sTailCode[1]&tBhFindLog.bMaskFlag)!=tBhFindLog.bCodeFlag)
							continue;
					}
					if (tBhFindLog.bNum <20)
					{
						if (ChkIfHaveTheHz(tBhFindLog.sHzBuffer, stBxCodeRec[i].sHz) ==TRUE)
							continue;
					}
					memcpy(&tBhFindLog.sHzBuffer[nBufLen+2*j], stBxCodeRec[i].sHz, 2);
					tBhFindLog.bNum ++;
					j++;
				}
				tBhFindLog.nCurPosi +=(uint32_t)bReadNum*BXCODE_REC_SIZE;
				tBhFindLog.nReadedNum +=(uint16_t)bReadNum;
			}
		}
		else
		{
			j=0;
			nBufLen =strlen((char *)tBhFindLog.sHzBuffer);
			while((tBhFindLog.nReadedNum <tBhFindLog.nTotal)&&(j<FETCH_HZNUM))
			{
				if ((tBhFindLog.nTotal-tBhFindLog.nReadedNum) >30)
					bReadNum =30;
				else
					bReadNum =(unsigned char)(tBhFindLog.nTotal-tBhFindLog.nReadedNum);
				if ((tBhFindLog.bNum+bReadNum)>MAX_BH_BUFFER/2)
					bReadNum =(unsigned char)(MAX_BH_BUFFER/2-tBhFindLog.bNum);
				if (bReadNum <=0)
					break;

				bReadLen =bReadNum*BXCODE_REC_SIZE;
				rc =ReadMaBiao(tBhFindLog.nCurPosi,
								(unsigned char *)stBxCodeRec,(uint16_t)bReadLen);
				if (rc == FALSE)		
				{
						return FALSE;
				}
				for(i=0; i<bReadNum; i++)
				{
					rc =TRUE;
					if (tBhFindLog.bCodeFlag)
					{
						rc =FALSE;
						if ((stBxCodeRec[i].sTailCode[1]&tBhFindLog.bMaskFlag)==tBhFindLog.bCodeFlag)
							rc =TRUE;
					}
					bFlag =stBxCodeRec[i].sTailCode[0]&bTailFlag;
					if (bFlag==tBhFindLog.sTailCode[0]&&rc==TRUE)
					{
						rc =FALSE;
						if (tBhFindLog.bNum <20)
							rc =ChkIfHaveTheHz(tBhFindLog.sHzBuffer, stBxCodeRec[i].sHz);
						if (rc ==FALSE)
						{
							memcpy(&tBhFindLog.sHzBuffer[nBufLen+2*j], stBxCodeRec[i].sHz, 2);
							tBhFindLog.bNum ++;
							j++;
							if (j >=(2*FETCH_HZNUM))	i=bReadNum;
						}
					}
					tBhFindLog.nCurPosi +=BXCODE_REC_SIZE;
					tBhFindLog.nReadedNum ++;

					if (j >=(2*FETCH_HZNUM))
						break;
				}
			}//while
		} //if
	}
	nReadTotal =strlen((char *)tBhFindLog.sHzBuffer);
	if (tBhFindLog.bPoint >=nReadTotal)
	{
		return FALSE;
	}

	bReadLen =FETCH_HZNUM*2;
	if ((tBhFindLog.bPoint+FETCH_HZNUM*2)>nReadTotal)
		bReadLen =nReadTotal-tBhFindLog.bPoint;

	memcpy(pResult, &tBhFindLog.sHzBuffer[tBhFindLog.bPoint], bReadLen);
	tBhFindLog.bPoint +=(uint16_t)bReadLen;
	j =bReadLen/2;

	bBhLastHzNum =j;
	fBhPrevNext =NEXT_PAGE;
	bBhCurPageNo ++;
	if (tBhFindLog.bPoint <nReadTotal)
		gHintFormFeed +=2;
	else if ((nReadTotal<MAX_BH_BUFFER)&&(tBhFindLog.bPoint==nReadTotal)&&(tBhFindLog.nTotal>tBhFindLog.nReadedNum))
		gHintFormFeed +=2;

	return TRUE;
}

unsigned char ReadBXPrevHz(unsigned char *pResult)
{
	unsigned char j;
	uint16_t nLen, nReadTotal, nOffsetVal; 

	nOffsetVal =0;

	gHintFormFeed =0;
	if (tBhFindLog.bPoint ==nOffsetVal ||bBhCurPageNo ==0)		/* 无预想字时==2  */
		return FALSE;

	nReadTotal =strlen((char *)tBhFindLog.sHzBuffer);
	if (nReadTotal <=FETCH_HZNUM*2)
		return FALSE;

	gHintFormFeed =2;
	if (fBhPrevNext ==NEXT_PAGE)
	{
		j =bBhLastHzNum;
		while(1)
		{
			if (tBhFindLog.bPoint <=nOffsetVal)
			{
				fBhPrevNext =PREV_PAGE;
				tBhFindLog.bPoint =nOffsetVal;
				return FALSE;
			}
			tBhFindLog.bPoint -=2;
			j--;
			if (j ==0)
				break;
		}
	}
	j =FETCH_HZNUM;
	nLen =tBhFindLog.bPoint;
	while(1)
	{
		if (tBhFindLog.bPoint <=nOffsetVal)
		{
			fBhPrevNext =PREV_PAGE;
			tBhFindLog.bPoint =nLen;
			return FALSE;
		}
		tBhFindLog.bPoint -=2;
		if (tBhFindLog.sHzBuffer[tBhFindLog.bPoint] !=0x20)
		{
			j--;
			memcpy(&pResult[j*2], &tBhFindLog.sHzBuffer[tBhFindLog.bPoint], 2);
		}
		if (j ==0)
			break;
	}
	if (tBhFindLog.bPoint ==nOffsetVal)	
	{
		tBhFindLog.bPoint =nLen;
		bBhCurPageNo =0;
	}
	bBhLastHzNum =0;
	fBhPrevNext =PREV_PAGE;
	if (bBhCurPageNo >0)
		bBhCurPageNo --;

	if (bBhCurPageNo>0)
		gHintFormFeed +=1;
	return TRUE;
}

unsigned char  char_to_bin(unsigned char bchar)
{
	if ((bchar >= '0')&&(bchar <= '9'))
		return(bchar-'0');
	else
	{
		if ((bchar >= 'A')&&(bchar <= 'F'))
			return(bchar-'A'+10);
		else
			return(0xf);
	}
}

void asc_to_bcd(unsigned char *bcd, unsigned char *asc, unsigned char asc_len)
{
	unsigned char i,j;
	unsigned char bOddFlag, bchar, bchar1, bchar2;

	if (asc_len%2)
		bOddFlag = 1;
	else
		bOddFlag = 0;

	for (i=0,j=0; j<asc_len; i++)
	{
		if ((i==0)&&(bOddFlag))
		{
			bchar1 = asc[j++];
			bcd[i] = char_to_bin(bchar1);
		}
		else
		{
			bchar1 = asc[j++];
			bchar = char_to_bin(bchar1);
			bchar *= 16;
			bchar1 = asc[j++];
			bchar2 = char_to_bin(bchar1);
			bcd[i] = bchar + bchar2;
		}
	}
}

unsigned char ChkIfHaveTheHz(unsigned char *pHzBuffer, unsigned char *psHz)
{
	unsigned short nHzLen;
	unsigned char sTemp[3];
	unsigned char *pTmp, *pPoint;

	memcpy(sTemp, psHz, 2);
	sTemp[2] =0;
	pPoint =pHzBuffer;
	while(1)
	{
		pTmp =(unsigned char *)strstr((char *)pPoint, (char *)sTemp);
		if (pTmp ==NULL)		break;
		nHzLen =pTmp-pHzBuffer;
		if ((nHzLen%2) ==0)			return TRUE;
		pPoint =pTmp+1;
	}
	return FALSE;
}


