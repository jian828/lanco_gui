#include "inc/FindCode.h"

unsigned char fLxPrevNext;
unsigned long lLXCODETBL_OFFSET;
unsigned short nLX_INDEX_OFFSET;
unsigned short nLX_INDEX_TOTAL;
T_FINDLXLOG tLxFindLog;
extern unsigned char gsHzBuffer[MAX_PY_BUFFER+20];
extern unsigned char gHintFormFeed;		/* 汉字上下页指示标志 */

//extern unsigned char ReadMaBiao(unsigned long nAddr, unsigned char *pBuffer, unsigned short bReadLen);
extern	unsigned char ReadMaBiao(unsigned long nAddr, unsigned char *pBuffer, unsigned short  bReadLen);

unsigned char InitLXIndexTbl()
{
	unsigned char  rc;
	unsigned short  nIndexTotal ;//wjhdelete  i, nBlockNum; nOffset;
//wjhdelete	unsigned char bLastNum; j, bReadNum;
	unsigned long nPosition;
	unsigned char sBuf[4];
//wjhdelete	T_LX_INDEX_REC stLxIndexRec[30];

	rc =ReadMaBiao(0L, sBuf, 4);
	if (rc == FALSE)		return FALSE;
	if(memcmp(sBuf, (const char *)("5D09"), 4)!=0)		return FALSE;

	rc =ReadMaBiao(14L, sBuf, 4);
	if (rc == FALSE)		return FALSE;
	lLXCODETBL_OFFSET=((uint32_t)sBuf[0]*256+(uint32_t)sBuf[1])*65536+((uint32_t)sBuf[2]*256+(uint32_t)sBuf[3]);

	rc =ReadMaBiao(20L, sBuf, 4);
	if (rc == FALSE)		return FALSE;
	nLX_INDEX_OFFSET=(uint16_t)sBuf[0]*256+(uint16_t)sBuf[1];

	nPosition =lLXCODETBL_OFFSET;
	rc =ReadMaBiao(nPosition, sBuf, 2);
	if (rc == FALSE)	return FALSE;

	nPosition +=INDEX_OFFSET;
	nIndexTotal =(uint16_t)sBuf[0]*256 + (uint16_t)sBuf[1];
	nLX_INDEX_TOTAL =nIndexTotal;

	return TRUE;
}

unsigned char FindLXIndexTbl(unsigned char *pHzCode, T_LX_INDEX_REC *ptIndexRec)
{
	unsigned char bCodeLen;
//wjhdelete	int rc, i;
	int16_t rc,i;
	
	uint16_t  nFirst, nLast, nMiddle;  // short
	unsigned long nPosition;
	T_LX_INDEX_REC tIndexRec;
	unsigned char sBuf[4];

	bCodeLen =(unsigned char)strlen((char *)pHzCode);

	i= pHzCode[0];
	if (pHzCode[0]>=0xe0)
		i =0xe0;
	nPosition =(uint32_t)nLX_INDEX_OFFSET+(i-0xb0)*4;//[20070409wjh]
	rc =(int16_t)ReadMaBiao(nPosition, sBuf, 4);
	if (rc == FALSE)		return FALSE;

	nFirst = (uint16_t)sBuf[0]*256+(uint16_t)sBuf[1];
	nLast = (uint16_t)sBuf[2]*256+(uint16_t)sBuf[3];

	nPosition =lLXCODETBL_OFFSET+INDEX_OFFSET;
	while(nFirst <= nLast)
	{
		nMiddle = (nFirst+nLast)/2;

		rc =(int16_t)ReadMaBiao(nPosition+nMiddle*LX_INDEX_REC_SIZE, (unsigned char *)&tIndexRec, LX_INDEX_REC_SIZE);
		if (rc == FALSE)		return FALSE;
		rc = memcmp(pHzCode, tIndexRec.sKey, 2);
		if (rc == 0)	// identical
		{
			memcpy(ptIndexRec, &tIndexRec, LX_INDEX_REC_SIZE);
			return TRUE;
		}
		if (rc < 0)		//  less than
			nLast = nMiddle - 1;
		else			//  greater than
			nFirst = nMiddle + 1;
	}
	return FALSE;
}

unsigned char ReadLXCode(unsigned char *pHzCode, unsigned char *pResult)
{
	unsigned char sAddLxHz[21] ="的一了是我人在不有中";
	unsigned char i, rc;
	unsigned short nReadLen, nReadNum;
	unsigned short nItem;
	unsigned long nPosition;
	T_LX_INDEX_REC tIndexRec;
	unsigned char *pTmp, sTemp[3];

	if (strlen((char*)pHzCode)<2 || pHzCode[0]<0xa1 || pHzCode[1]<0xa1)
		return FALSE;
	memset(pResult, 0, FETCH_HZNUM*2+1);
	memset(&tLxFindLog, 0, sizeof(T_FINDLXLOG));
	tLxFindLog.sHzBuffer =gsHzBuffer;
	memset(tLxFindLog.sHzBuffer, 0, MAX_PY_BUFFER+20);

	gHintFormFeed =0;
	rc =(uint16_t)FindLXIndexTbl(pHzCode, &tIndexRec);//[20070409wjhadd (uint16_t)  ]
	if (rc == FALSE)
	{
		tLxFindLog.bNum =10;
		memcpy(tLxFindLog.sHzBuffer, sAddLxHz, tLxFindLog.bNum*2);
		if (tLxFindLog.bNum >=FETCH_HZNUM)
			memcpy(pResult, tLxFindLog.sHzBuffer, FETCH_HZNUM*2);
		else
			memcpy(pResult, tLxFindLog.sHzBuffer, tLxFindLog.bNum*2);
		if (tLxFindLog.bNum >FETCH_HZNUM)
			gHintFormFeed =2;
		return TRUE;
	}


	nPosition =lLXCODETBL_OFFSET+INDEX_OFFSET+(uint32_t)nLX_INDEX_TOTAL*LX_INDEX_REC_SIZE;//[20070409wjhadd (uint32_t)  ]
	nPosition +=((uint32_t)tIndexRec.sAddr[0]*256+(uint32_t)tIndexRec.sAddr[1])*2;//[20070409wjhadd (uint32_t)  ]

	nItem =tIndexRec.sRecNum[0];
	tLxFindLog.nTotal =nItem;
	tLxFindLog.nCurPosi =nPosition;
	if (nItem ==0)
	{
		tLxFindLog.bNum =10;
		memcpy(tLxFindLog.sHzBuffer, sAddLxHz, tLxFindLog.bNum*2);
		if (tLxFindLog.bNum >=FETCH_HZNUM)
			memcpy(pResult, tLxFindLog.sHzBuffer, FETCH_HZNUM*2);
		else
			memcpy(pResult, tLxFindLog.sHzBuffer, tLxFindLog.bNum*2);
		if (tLxFindLog.bNum >FETCH_HZNUM)
			gHintFormFeed =2;
		return TRUE;
	}

	while (nItem >0)
	{
		if (nItem < 100)
			nReadNum =nItem;
		else
			nReadNum =100;

		nReadLen =nReadNum*2;
		rc =ReadMaBiao(tLxFindLog.nCurPosi, 
							&tLxFindLog.sHzBuffer[tLxFindLog.bNum*2], nReadLen);
		if (rc == FALSE)		return FALSE;

		tLxFindLog.bNum +=nReadNum;
		tLxFindLog.nCurPosi +=nReadLen;
		nItem -=nReadNum;
	}

	if (tLxFindLog.bNum <10)
	{
		memset(sTemp, 0, sizeof(sTemp));
		for(i=0; i<10; i++)
		{
			memcpy((char *)sTemp, (char *)&sAddLxHz[2*i], 2);
			pTmp =tLxFindLog.sHzBuffer;
			while((unsigned short)(pTmp-tLxFindLog.sHzBuffer)<(tLxFindLog.bNum*2))
			{
				pTmp =(unsigned char *)strstr((char *)pTmp, (char *)sTemp);
				if (pTmp ==NULL)
				{
					memcpy(&tLxFindLog.sHzBuffer[tLxFindLog.bNum*2], sTemp, 2);
					tLxFindLog.bNum ++;
					break;
				}
				if (((pTmp-tLxFindLog.sHzBuffer)%2) ==0)
					break;
				pTmp ++;
			} //while
		} //for
	}
	if (tLxFindLog.bNum >=FETCH_HZNUM)
		memcpy(pResult, tLxFindLog.sHzBuffer, FETCH_HZNUM*2);
	else
		memcpy(pResult, tLxFindLog.sHzBuffer, tLxFindLog.bNum*2);

	gHintFormFeed =0;
	if (tLxFindLog.bNum >FETCH_HZNUM)
		gHintFormFeed =2;
	return TRUE;
}

unsigned char ReadLXNextHz(unsigned char *pResult)
{
	unsigned char i ;//wjhdelete, rc;
	unsigned char bReadNum;//wjhdelete, bReadLen;

	i =0;
	gHintFormFeed =0;

	memset(pResult, 0, FETCH_HZNUM*2+1);
	bReadNum = 0;
	if ((tLxFindLog.bPoint+FETCH_HZNUM*2) < tLxFindLog.bNum*2)
	{
		gHintFormFeed =1;
		tLxFindLog.bPoint +=FETCH_HZNUM*2;
		memcpy(pResult, &tLxFindLog.sHzBuffer[tLxFindLog.bPoint], FETCH_HZNUM*2);
		if ((tLxFindLog.bPoint+FETCH_HZNUM*2) <tLxFindLog.nTotal*2)
			gHintFormFeed +=2;
		return TRUE;
	}
	if (tLxFindLog.nTotal <= tLxFindLog.bNum)
		return FALSE;
	return TRUE;
}

unsigned char ReadLXPrevHz(unsigned char *pResult)
{
	unsigned short i;

	gHintFormFeed =0;

	memset(pResult, 0, FETCH_HZNUM*2+1);
	if (tLxFindLog.bPoint == 0)
		return FALSE;

	gHintFormFeed =2;
	if (tLxFindLog.bPoint >= FETCH_HZNUM*2)
		tLxFindLog.bPoint -=FETCH_HZNUM*2;

	memcpy(pResult, &tLxFindLog.sHzBuffer[tLxFindLog.bPoint], FETCH_HZNUM*2);
	i=0;
	if (tLxFindLog.sHzBuffer[0]==' ')	i=2;
	if (tLxFindLog.bPoint>i)
		gHintFormFeed +=1;
	return TRUE;
}


