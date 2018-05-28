
#include "inc/app_def.h"


unsigned char bLastHzNum;
unsigned char fPrevNext;
unsigned char bCurPageNo;
unsigned long lPYCODETBL_OFFSET;
unsigned short nPY_INDEX_TOTAL;
unsigned short nPYIndex_OFFSET;
unsigned short nPYInfo_OFFSET;


unsigned char   ssPYInfoRef[8][2] =
{
	{0,   44},		//'2', [offset. total]
	{44,  29},		//'3'
	{73,  22},		//'4'
	{95,  35},		//'5'
	{130, 30},		//'6'
	{160, 44},		//'7'
	{204, 26},		//'8'
	{230, 45}		//'9'
};

static struct T_FINDPYLOG tPyFindLog;
static T_CODETBL_OFFSET stPYCodeTblOffset[8];

unsigned char gHintFormFeed;		/* 汉字上下页指示标志 */
extern unsigned char gsHzBuffer[MAX_PY_BUFFER+20];

unsigned char ConvCodeToPY(unsigned char *pInpCode, unsigned char *pResult,  unsigned char *pResult1, unsigned char *pZeroSMLen);
extern void RightTrim(char *ptr, unsigned char value);

extern void asc_to_bcd(unsigned char *bcd, unsigned char *asc, unsigned char asc_len);
extern unsigned char ChkIfHaveTheHz(unsigned char *pHzBuffer, unsigned char *psHz);
//extern unsigned char ReadMaBiao(unsigned long nAddr, unsigned char *pBuffer, unsigned short bReadLen);
extern	unsigned char ReadMaBiao(unsigned long nAddr, unsigned char *pBuffer, unsigned short  bReadLen);

///////////////////////////////////////////////////
unsigned char InitPYIndexTbl()
{
	unsigned char rc;
	uint16_t i, nIndexTotal;
	unsigned long nPosition;
	unsigned char sBuf[32];

	memset(&tPyFindLog, 0, sizeof(struct T_FINDPYLOG));

	rc =ReadMaBiao(0L, sBuf, 32);
	if (rc == FALSE)		return FALSE;
	if(memcmp(sBuf, (const char *)"5D09", 4)!=0)	return FALSE;//[20070402wjh]

	nPYIndex_OFFSET =(uint16_t)sBuf[4]*256+(uint16_t)sBuf[5];
	lPYCODETBL_OFFSET=((uint32_t)sBuf[10]*256+(uint32_t)sBuf[11])*65536+((uint32_t)sBuf[12]*256+(uint32_t)sBuf[13]);
	nPYInfo_OFFSET =(uint16_t)sBuf[18]*256+(uint16_t)sBuf[19];

	nPosition =lPYCODETBL_OFFSET;
	rc =ReadMaBiao(nPosition, sBuf, 2);
	if (rc == FALSE)
		return FALSE;
	nPosition +=INDEX_OFFSET;
	nIndexTotal =(uint16_t)sBuf[0]*256 + (uint16_t)sBuf[1];
	nPY_INDEX_TOTAL =nIndexTotal;

	memset(stPYCodeTblOffset, 0, sizeof(stPYCodeTblOffset));//willcheck
	nPosition =80;
	rc =ReadMaBiao(nPosition, sBuf, 32);
	if (rc == FALSE)		return FALSE;
	for(i=0; i<8; i++)
	{
		stPYCodeTblOffset[i].start_no =(uint16_t)sBuf[i*4]*256+(uint16_t)sBuf[i*4+1];
		stPYCodeTblOffset[i].end_no =(uint16_t)sBuf[i*4+2]*256+(uint16_t)sBuf[i*4+3];
	}
	return TRUE;
}

unsigned char FindPYIndexTbl(char *pInputCode, unsigned char *pKeyCode,
							 T_PYINDEX_REC *ptIndexRec)
{
	unsigned char no, bCodeLen;
//wjhdelete	int rc;
	int16_t rc;
	short nFirst, nLast, nMiddle;
	unsigned long nPosition;
	T_PYINDEX_REC tIndexRec;

	bCodeLen =strlen((char *)pInputCode);
//	if (pInputCode[0]<'2' || pInputCode[0]>'9')
//		return FALSE;
	no =pInputCode[0]-'2';
	
	nFirst = stPYCodeTblOffset[no].start_no;
	nLast = stPYCodeTblOffset[no].end_no;
	while(nFirst <= nLast)
	{
		nMiddle = (nFirst+nLast)/2;

		nPosition =lPYCODETBL_OFFSET+INDEX_OFFSET+nMiddle*PYINDEX_REC_SIZE;
		rc =ReadMaBiao(nPosition, (unsigned char *)&tIndexRec, PYINDEX_REC_SIZE);
	
		if (rc == FALSE)	
		{
		    return FALSE;
		}
		rc =memcmp(pKeyCode, tIndexRec.sKey, 2);
		if (rc == 0)	// identical
		{
			memcpy(ptIndexRec, &tIndexRec, PYINDEX_REC_SIZE);
			return TRUE;
		}
		if (rc < 0)		//  less than
		{
			nLast = nMiddle - 1;
		}
		else			//  greater than
		{
			nFirst = nMiddle + 1;
		}
	}
	return FALSE;
}

unsigned char GetYunMu(char *pCode, char pResult[2][6])
{
	unsigned long nCodeVal;

	nCodeVal =atol(pCode);
	switch(nCodeVal)
	{
	case 5:			//[20070402wjh]
		strcpy(pResult[0], (const char *)("46"));strcpy(pResult[1], (const char *)("io"));		break;
	case 7:
		strcpy(pResult[0], (const char *)("42"));strcpy(pResult[1], (const char *)("ia"));		break;
	case 9:
		strcpy(pResult[0], (const char *)("82"));strcpy(pResult[1], (const char *)("ua"));		break;
	case 45:
		strcpy(pResult[0], (const char *)("464"));strcpy(pResult[1], (const char *)("ing"));	break;
	case 55:
	case 465:
	case 564:
		strcpy(pResult[0],(const char *)("4664"));strcpy(pResult[1], (const char *)("iong"));	break;
	case 56:
		strcpy(pResult[0], (const char *)("466"));strcpy(pResult[1], (const char *)("ion"));	break;
	case 65:
		strcpy(pResult[0], (const char *)("664"));strcpy(pResult[1], (const char *)("ong"));	break;
	case 75:
	case 425:
	case 764:
		strcpy(pResult[0], (const char *)("4264"));strcpy(pResult[1], (const char *)("iang"));	break;
	case 76:
		strcpy(pResult[0], (const char *)("426"));strcpy(pResult[1], (const char *)("ian"));	break; //ian
	case 77:
		strcpy(pResult[0], (const char *)("427"));strcpy(pResult[1], (const char *)("iao"));	break; //iao
	case 94:
		strcpy(pResult[0], (const char *)("824"));strcpy(pResult[1], (const char *)("uai"));	break;
	case 95:
	case 825:
	case 964:
		strcpy(pResult[0], (const char *)("8264"));strcpy(pResult[1],(const char *)("uang"));	break;
	case 96:
		strcpy(pResult[0], (const char *)("826"));strcpy(pResult[1], (const char *)("uan"));	break;
	case 25:
		strcpy(pResult[0], (const char *)("264"));strcpy(pResult[1], (const char *)("ang"));	break;
	case 35:
		strcpy(pResult[0], (const char *)("364"));strcpy(pResult[1], (const char *)("eng"));	break;
	default:
		return FALSE;
	}
	return TRUE;
}

unsigned char CheckIfLegalPY(char *psInpCode, unsigned char bYunMu)
{
	if (bYunMu==0)		return TRUE;
	if (bYunMu>'5' || bYunMu=='0')		return FALSE;
	switch(psInpCode[0])
	{
	case '2':
		if (bYunMu >'4')		return FALSE;
		break;
	case '3':
	case '5':
	case '6':
		if (bYunMu >'3')		return FALSE;
		break;
	case '4':
		if (bYunMu >'2')		return FALSE;
		break;
	case '8':
		if (bYunMu >'1')		return FALSE;
		break;
	case '7':
	case '9':
		if (bYunMu >'5')		return FALSE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

unsigned char FindPYCodeTbl(unsigned char bOffset, char *pInpCode, 
							 T_PYINPUTCODE *ptPYInpCode)
{
	//char sYunmuTbl[5] ={'5','4','1','2','3'};//'0','6','7','8','9'
    signed char  rc;
	unsigned char i, j, k, bCodeLen, bCount, bYMStart;
	unsigned char bPYTotal, bYunMu, bxHFlag, bYSMFlag;
	uint32_t nFirst, nLast, nMiddle;  // int
	unsigned short nPosition;
	T_PYBASE_INFO tBaseInfo;
	char sKeyCode[10], sResult[2][6];
	unsigned char ssPYInfo[6][18]; 
	char *psPoint;
	
	bxHFlag =FALSE;
	bCodeLen =strlen(pInpCode);
	if (bCodeLen==2 && bOffset==2)
	{
		nFirst =atoi(pInpCode);
		if (nFirst==64)
		{												////[20070402wjh]
			memcpy(ptPYInpCode->ssPinYun[ptPYInpCode->bPYTotal], (const char *)("ng    "), 6);
			memcpy(ptPYInpCode->ssPYCode[ptPYInpCode->bPYTotal], (const char *)("64  "), 4);
			ptPYInpCode->bPYTotal ++;
			return TRUE;
		}
	}
	
	if (bCodeLen==(bOffset+1))
	{
		nFirst =atoi(pInpCode);
		if ((bOffset==1 && nFirst==25) || (bOffset==2 && nFirst==215))
		{
			memcpy(ptPYInpCode->ssPinYun[ptPYInpCode->bPYTotal], (const char *)("ang   "), 6);
			memcpy(ptPYInpCode->ssPYCode[ptPYInpCode->bPYTotal], (const char *)("215 "), 4);
			ptPYInpCode->bPYTotal ++;
			return TRUE;
		}
/*		if ((bOffset==1 && nFirst==35) || (bOffset==2 && nFirst==325))
		{
			memcpy(ptPYInpCode->ssPinYun[ptPYInpCode->bPYTotal], "eng   ", 6);
			memcpy(ptPYInpCode->ssPYCode[ptPYInpCode->bPYTotal], "325 ", 4);
			ptPYInpCode->bPYTotal ++;
			return TRUE;
		}
*/	}

	for(i=0; i<2; i++)
	{
		memset(sKeyCode, 0x20, 5);
		memset(sResult, 0, sizeof(sResult));////willcheck
		sKeyCode[5] =0;
		bYunMu =0;
		if (bOffset <=1)
		{
			if (i==0 && bCodeLen!=6)
				memcpy(sKeyCode, pInpCode, bCodeLen);
			else
			{
				memcpy(sKeyCode, pInpCode, bCodeLen-i);
				if (i==1)
				{

					if (bCodeLen>2 && (pInpCode[0]=='2' || pInpCode[0]=='7' || pInpCode[0]=='9'))
					{
						if (pInpCode[1] =='4')
						{
							j=1;
							memcpy(sKeyCode, pInpCode, bCodeLen);
							if (pInpCode[bCodeLen-2] =='5')
							{
								memcpy(sKeyCode, pInpCode, bCodeLen-1);
								sKeyCode[bCodeLen-1] =0;
								bYunMu =pInpCode[bCodeLen-1];
							}
						}
						else
						{
							memcpy(sKeyCode, pInpCode, bCodeLen-i);
							bYunMu =pInpCode[bCodeLen-1];
						}
					}
					else
					{
						memcpy(sKeyCode, pInpCode, bCodeLen-i);
						bYunMu =pInpCode[bCodeLen-1];
					}
				}
				if (strlen(sKeyCode)>=6)
				{
					bYunMu =sKeyCode[5];
					sKeyCode[5] =0;
				}
			}

			j=1;
			if (i==0)
			{
				if (pInpCode[0]=='2' || pInpCode[0]=='7' || pInpCode[0]=='9')
				{
					if (pInpCode[1] =='4')
						j=2;
				}
			}
		}
		else
		{
			j =1;
			sKeyCode[0] =pInpCode[0];
			if (bCodeLen >1)
			{
				if (pInpCode[0]=='2' || pInpCode[0]=='7' || pInpCode[0]=='9')
				{
					if (pInpCode[1] =='4') // || pInpCode[1] =='6')
					{
						sKeyCode[1]='4';
						memcpy(&sKeyCode[2], &pInpCode[2], bCodeLen-2);
						bxHFlag =TRUE;
						j =2;
						if (strlen(sKeyCode)>=6)
						{
							bYunMu =sKeyCode[5];
							sKeyCode[5] =0;
						}
					}
					else
					{
						memcpy(&sKeyCode[1], &pInpCode[2], bCodeLen-2);
						bYunMu =pInpCode[1];
					}
				}
				else
				{
					memcpy(&sKeyCode[1], &pInpCode[2], bCodeLen-2);
					bYunMu =pInpCode[1];
				}
			}
		}
		if (CheckIfLegalPY(pInpCode, bYunMu) ==FALSE)
			continue;

		bYMStart =j;
		bYSMFlag =FALSE;
		memset(sResult, 0, sizeof(sResult));////willcheck
		if (GetYunMu(&sKeyCode[j], sResult)==TRUE)
		{
			//memcpy(&sKeyCode[j], sResult[0], strlen(sResult[0]));
			bYSMFlag =TRUE;
		}

/*		if ((bYunMu>='6' && bYunMu<='9') || bYunMu=='0')
		{
			if (bYunMu =='0')
				bYunMu =sYunmuTbl[0];
			else
				bYunMu =sYunmuTbl[bYunMu-'5'];
		}
*/

		bCount =0;
		for(k=0; k<2; k++)
		{
			if (k==1)
			{
				if (bYSMFlag==FALSE)
					break;
				memcpy(&sKeyCode[bYMStart], sResult[0], strlen(sResult[0]));
			}

			j =pInpCode[0]-'2';

			nFirst = ssPYInfoRef[j][0]; //// [20070402wjh]
			nLast = ssPYInfoRef[j][0]+ssPYInfoRef[j][1]-1;



			while(nFirst <=nLast)
			{
				nMiddle =(nFirst+nLast)/2;
				nPosition =nPYIndex_OFFSET+nMiddle*PYBASE_INFO_SIZE;		
				rc =ReadMaBiao(nPosition, (unsigned char *)&tBaseInfo, PYBASE_INFO_SIZE);

				
				if (rc == FALSE)		return FALSE;
				rc = memcmp(sKeyCode, tBaseInfo.sKey, 5);
				
				if (rc ==0)		// identical
					break;

				if (rc <0)		//  less than
					nLast =nMiddle - 1;
				else			//  greater than
					nFirst =nMiddle + 1;
			}

			if ((bOffset==2 || bCodeLen==1 || bCodeLen==6) && rc)
			{
				if (bYSMFlag==TRUE)		continue;
				return FALSE;
			}
			if (rc==0)
			{
				bPYTotal =tBaseInfo.sPYTotal[0];
				nPosition =nPYInfo_OFFSET;
				nPosition +=((uint16_t)tBaseInfo.sOffset[0]*256+(uint16_t)tBaseInfo.sOffset[1])*18;
				rc =ReadMaBiao(nPosition, (unsigned char*)ssPYInfo, bPYTotal*18);
				if (rc == FALSE)		return FALSE;
//				memcpy(ssPYInfo, ssPinYunInfo[nPosition], bPYTotal*18);
				for(j=0; j<bPYTotal; j++)
				{
					if (bYunMu)
					{
						if (bYunMu !=ssPYInfo[j][5])
							continue;
					}
					if (bOffset<=1)
					{
						if (ssPYInfo[j][16]=='2')
							continue;
					}
					else //if (bOffset==2)
					{
						if (bxHFlag==FALSE)
						{
							if (ssPYInfo[j][16]=='1')
								continue;
						}
						else
						{
							psPoint =strchr((char*)ssPYInfo[j], 'h');
							if (psPoint ==NULL)
								continue;
						}
					}

					//if (bYMStart==2)
					//{
						//if (pInpCode[bCodeLen-1]=='7' && ssPYInfo[j][16]=='3')
						//	continue;
					//}
				//	if (bOffset==2 && ssPYInfo[j][16]=='3' && bxHFlag==TRUE)
				//		continue;
					if (k==1 && bYSMFlag==TRUE)
					{
						memcpy(sKeyCode, &ssPYInfo[j][6], 6);
						sKeyCode[6]=0;
						RightTrim(sKeyCode, 0x20);
						psPoint =strchr(sKeyCode, 0x20);
						if (psPoint !=NULL)
							*psPoint =0;
						psPoint =strchr(sKeyCode, sResult[1][0]);
						if (psPoint ==NULL)
							continue;
						if (strcmp(psPoint, sResult[1])!=0)
						{
							if (strcmp(sResult[1],(const char *)("ian"))!=0)
								continue;
							if (strcmp(psPoint, (const char *)("iao"))!=0)
								continue;
						}
					}

					memcpy(ptPYInpCode->ssPinYun[ptPYInpCode->bPYTotal], &ssPYInfo[j][6], 6);
					memcpy(ptPYInpCode->ssPYCode[ptPYInpCode->bPYTotal], &ssPYInfo[j][12], 4);
					ptPYInpCode->bPYTotal ++;
					bCount ++;
				}
			}
		}
		if (bCodeLen>=6)		break;
		if (bCount)		break;
		if (bCodeLen==1 || bOffset==2)	break;
	}
	if (ptPYInpCode->bPYTotal==0)		return FALSE;
	if (ptPYInpCode->bPYTotal==2)
	{
		if (memcmp(ptPYInpCode->ssPinYun[0], ptPYInpCode->ssPinYun[1], 4)==0)
			ptPYInpCode->bPYTotal=1;
		return TRUE;
	}
	if (bOffset==2 && bxHFlag==TRUE )
	{
		if (pInpCode[bCodeLen-1]!='6')
			ptPYInpCode->bPYTotal =1;
		else
		{
			if (bCodeLen>=3 && pInpCode[2]!='2'&& pInpCode[2]!='8')
				ptPYInpCode->bPYTotal=1;
			else
				ptPYInpCode->bPYTotal=2;
		}
	}

	return TRUE;
}

unsigned char ConvPYInpCode(unsigned char bInpMode, char *pInpCode, T_PYINPUTCODE *ptPYInpCode)
{
	unsigned char rc, bCodeLen, bOffset;
	T_PYINPUTCODE tPYCode;
	char sNewCode[7], sTemp[7];

	bCodeLen =strlen(pInpCode);
	if (bCodeLen==0 || bCodeLen>6)		return FALSE;
	if (pInpCode[0]<'2' || pInpCode[0]>'9')	return FALSE;

	bOffset =2;
	if (bInpMode==DAN_PIN)
		bOffset =1;

	memset(&tPYCode, 0, sizeof(T_PYINPUTCODE));
	memcpy(sTemp, pInpCode, bCodeLen+1);
	if (bInpMode ==SHUANG_PIN)
	{
		if (pInpCode[0]=='8'&& bCodeLen>=2)
		{
			if (pInpCode[0] !='1')
			{
				sTemp[0]=pInpCode[0];
				sTemp[1]='1';
				memcpy(&sTemp[2], pInpCode+1, bCodeLen);
			}

		}
	}
	else
	{
		if (bCodeLen>1 && pInpCode[0]=='8' && pInpCode[bCodeLen-1]=='1')
			return FALSE;
	}
	memcpy(sNewCode, sTemp, strlen(sTemp)+1);
	rc =FindPYCodeTbl(bOffset, sNewCode, &tPYCode);
	if (rc==FALSE)		return rc;
	if (tPYCode.bPYTotal==0)  return FALSE;

	memcpy(ptPYInpCode, &tPYCode, sizeof(T_PYINPUTCODE));
	return TRUE;
}

/*   no wildcard  */
/*   mode =0: 查出所有的汉字 */
/*         1: 检查是否有汉字[仅针对带笔形、单韵母的组合] */
unsigned char ReadHzInfo(char bInpMode, char *pInputCode, 
						 unsigned char bSelectNo, T_PYINPUTCODE *ptPYInpCode)
{
	unsigned char i,j,rc, bReadNum;
	unsigned char bCodeLen, bReadLen, bCurSelNo;
	unsigned short  bReadTotal, nHzTotal, nLength;
	unsigned char sCode[10], sBcdKey[3];
	T_PYINDEX_REC tIndexRec;
	T_PYCODE_REC stPyCodeRec[FETCH_HZNUM+5];
	bCurPageNo =0;

	bCurSelNo =bSelectNo;
	if (bSelectNo >=ptPYInpCode->bPYTotal)
		bCurSelNo =0;

	bCodeLen =strlen((char *)pInputCode);
	memset(sCode, '0', PY_MAINKEY_LEN);
	sCode[PY_MAINKEY_LEN] =0;
	RightTrim(ptPYInpCode->ssPYCode[bCurSelNo], 0x20);

	if (bCodeLen==1 && bInpMode==SHUANG_PIN)
		memcpy(sCode, pInputCode, bCodeLen);
	else
		memcpy(sCode, ptPYInpCode->ssPYCode[bCurSelNo], strlen(ptPYInpCode->ssPYCode[bCurSelNo]));

	memcpy(tPyFindLog.sInpCode, pInputCode, bCodeLen+1);
	asc_to_bcd(sBcdKey, sCode, PY_MAINKEY_LEN);
	rc =FindPYIndexTbl(pInputCode, sBcdKey, &tIndexRec);
	if (rc==FALSE)		return rc;
	tPyFindLog.nTotal =(uint16_t)tIndexRec.sRecNum[0]*256+(uint16_t)tIndexRec.sRecNum[1];
	tPyFindLog.nCurPosi =lPYCODETBL_OFFSET+INDEX_OFFSET+nPY_INDEX_TOTAL*PYINDEX_REC_SIZE;
	tPyFindLog.nCurPosi +=(uint32_t)tIndexRec.sAddr[0]*256+(uint32_t)tIndexRec.sAddr[1];

	nHzTotal =tPyFindLog.nTotal;
	if (nHzTotal >30)
		nHzTotal =30;

	j =0;
	bReadTotal =0;
	while(bReadTotal <nHzTotal)
	{
		if ((nHzTotal-bReadTotal) <(FETCH_HZNUM+5))
			bReadNum =nHzTotal-bReadTotal;
		else
			bReadNum =FETCH_HZNUM+5;
		if (bReadNum == 0)		continue;

		bReadLen =bReadNum*PYCODE_REC_SIZE;
		rc =ReadMaBiao(tPyFindLog.nCurPosi,(unsigned char *)stPyCodeRec,bReadLen);
		if (rc == FALSE)	return FALSE;

		nLength =strlen((char*)tPyFindLog.sHzBuffer);
		for(i=0,j=0; i<bReadNum; i++)
		{
			if (ChkIfHaveTheHz(tPyFindLog.sHzBuffer, stPyCodeRec[i].sHz) ==TRUE)
				continue;
			memcpy(&tPyFindLog.sHzBuffer[nLength+j*2], stPyCodeRec[i].sHz, 2);
			j ++;
		}
		bReadTotal +=bReadNum;
		tPyFindLog.nCurPosi +=bReadLen;
		tPyFindLog.nHzNum +=j;
		if (tPyFindLog.nHzNum >FETCH_HZNUM)		break;
	}//while(

	if (tPyFindLog.nHzNum ==0)	return FALSE;
	fPrevNext =NEXT_PAGE;

	tPyFindLog.nReadedTotal =bReadTotal;
	return TRUE;
}

unsigned char ReadPYNextHz(unsigned char *pResult)
{
	unsigned char i, j, rc, bCodeLen=0, bHzCount, bReadNum, bReadLen;
	unsigned short  nReadTotal, nLength;
	T_PYCODE_REC stPyCodeRec[FETCH_HZNUM+2];

	i =0;
	gHintFormFeed =0;
	if (tPyFindLog.bPoint>(unsigned short)i || bCurPageNo>0)
		gHintFormFeed =1;

	nReadTotal =strlen((char *)tPyFindLog.sHzBuffer);
	if (nReadTotal ==0)		return FALSE;

	if (bCodeLen== 2 && tPyFindLog.sInpCode[1] =='0')
		tPyFindLog.bPoint =0;

	if (fPrevNext ==PREV_PAGE && bCurPageNo!=0)
	{
		i =0;
		j =0;
		while(1)
		{
			if ((tPyFindLog.bPoint >=nReadTotal) || (i >=FETCH_HZNUM))
				break;
			if (tPyFindLog.sHzBuffer[tPyFindLog.bPoint] !=0x20)
				i ++;
			tPyFindLog.bPoint +=2;
			j ++;
		}
	}
	i =0;
	bHzCount =0;
	while((tPyFindLog.nTotal>tPyFindLog.nReadedTotal)&&(tPyFindLog.nHzNum<MAX_PY_BUFFER/2))
	{
		if ((tPyFindLog.bPoint+FETCH_HZNUM*2)<nReadTotal)
			break;

		if ((tPyFindLog.nTotal-tPyFindLog.nReadedTotal) <(FETCH_HZNUM+2))
			bReadNum =tPyFindLog.nTotal-tPyFindLog.nReadedTotal;
		else
			bReadNum =FETCH_HZNUM+2;

		if ((tPyFindLog.nHzNum+bReadNum) >=(MAX_PY_BUFFER/2))
			bReadNum =MAX_PY_BUFFER/2-tPyFindLog.nHzNum;

		if (bReadNum >0)
		{
			bReadLen =bReadNum*PYCODE_REC_SIZE;
			rc =ReadMaBiao(tPyFindLog.nCurPosi,(unsigned char *)stPyCodeRec,bReadLen);
			if (rc ==TRUE)
			{
				nLength =strlen((char*)tPyFindLog.sHzBuffer);
				for(i=0,j=0; i<bReadNum; i++)
				{
					if (tPyFindLog.nHzNum <20)
					{
						if (ChkIfHaveTheHz(tPyFindLog.sHzBuffer, stPyCodeRec[i].sHz) ==TRUE)
							continue;
					}
					memcpy(&tPyFindLog.sHzBuffer[nLength+j*2], stPyCodeRec[i].sHz, 2);
					j++;
					bHzCount ++;
					tPyFindLog.nHzNum ++;
					if (bHzCount >FETCH_HZNUM)
					{
						i++;
						break;
					}
				}
				tPyFindLog.nCurPosi +=(long)i*PYCODE_REC_SIZE;
				tPyFindLog.nReadedTotal +=i;
			}
		}
		nReadTotal =strlen((char *)tPyFindLog.sHzBuffer);
	}
	nReadTotal =strlen((char *)tPyFindLog.sHzBuffer);
	if (tPyFindLog.bPoint>=nReadTotal)
		return FALSE;

	memset(pResult, 0, FETCH_HZNUM*2+1);
	bReadLen =FETCH_HZNUM*2;
	if ((tPyFindLog.bPoint+FETCH_HZNUM*2)>nReadTotal)
		bReadLen =nReadTotal-tPyFindLog.bPoint;
	memcpy(pResult, &tPyFindLog.sHzBuffer[tPyFindLog.bPoint], bReadLen);
	pResult[bReadLen] =0;
	tPyFindLog.bPoint +=bReadLen;

	bLastHzNum =bReadLen/2;
	fPrevNext =NEXT_PAGE;

	if (tPyFindLog.bPoint <nReadTotal)
		gHintFormFeed +=2;
	else if ((nReadTotal<MAX_PY_BUFFER)&&(tPyFindLog.bPoint==nReadTotal)&&(tPyFindLog.nTotal>tPyFindLog.nReadedTotal))
		gHintFormFeed +=2;
	bCurPageNo ++;

	return TRUE;
}

unsigned char ReadPYPrevHz(unsigned char *pResult)
{
	unsigned char j;
	unsigned short nLen, nReadTotal, nOffserVal; 

	gHintFormFeed =0;
	if (tPyFindLog.sInpCode[0] == '1')		return FALSE;
	nOffserVal =0;
	if (tPyFindLog.bPoint ==nOffserVal ||bCurPageNo ==0)
		return FALSE;

	nReadTotal =strlen((char *)tPyFindLog.sHzBuffer);
	if (nReadTotal <=FETCH_HZNUM*2)
		return FALSE;

	if (fPrevNext ==NEXT_PAGE)
	{
		j =bLastHzNum;
		while(1)
		{
			if (tPyFindLog.bPoint <=nOffserVal)
			{
				fPrevNext =PREV_PAGE;
				tPyFindLog.bPoint =nOffserVal;
				return FALSE;
			}
			tPyFindLog.bPoint -=2;
			j--;
			if (j ==0)
				break;
		}
	}
	j =FETCH_HZNUM;
	nLen =tPyFindLog.bPoint;
	while(1)
	{
		if (tPyFindLog.bPoint <=nOffserVal)
		{
			fPrevNext =PREV_PAGE;
			tPyFindLog.bPoint =nLen;
			return FALSE;
		}
		tPyFindLog.bPoint -=2;
		if (tPyFindLog.sHzBuffer[tPyFindLog.bPoint] !=0x20)
		{
			j--;
			memcpy(&pResult[j*2], &tPyFindLog.sHzBuffer[tPyFindLog.bPoint], 2);
		}
		if (j ==0)
			break;
	}
	if (tPyFindLog.bPoint ==nOffserVal)	
	{
		tPyFindLog.bPoint =nLen;
		bCurPageNo =0;
	}
	bLastHzNum =0;
	fPrevNext =PREV_PAGE;
	if (bCurPageNo >0)		bCurPageNo --;

	gHintFormFeed =2;
	if (bCurPageNo>0)
		gHintFormFeed +=1;
	return TRUE;
}

unsigned char ReadPYHzCode(char bInpMode, char *pInputCode, unsigned char bSelectNo, T_PYINPUTCODE *ptPYInpCode, unsigned char *psResult)
{
	memset(&tPyFindLog, 0, sizeof(struct T_FINDPYLOG));
	tPyFindLog.sHzBuffer =gsHzBuffer;
	memset(gsHzBuffer, 0, sizeof(gsHzBuffer));
	if (ReadHzInfo(bInpMode, pInputCode, bSelectNo, ptPYInpCode)==FALSE)
	{
		return FALSE;
	}
	if (ReadPYNextHz(psResult)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

