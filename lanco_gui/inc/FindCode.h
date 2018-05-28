#include <stdio.h>
#include <stdlib.h>
#include <string.h>





#define MAX_KEYTABLE		8

#define RESULT_OK			0x01
#define RESULT_CANCEL		0x02
#define RESULT_END			0x03
#define RESULT_CONTINUE		0x04

#define BLACK				0
#define RED					1
#define BLUE				2
#define NORMAL_DISP			0
#define REVERSE_DISP		0x80

#define BX_INPUT			1
#define PY_INPUT			2

#define CHK_PY_FLAG			0x80

//////////////////////////////////////////////////////////


#define TRUE					1
#define FALSE				0
#define INDEX_OFFSET		2

#define FETCH_HZNUM		   60		/* ����ÿ����ȡ���ֵ���Ŀ */

#define NEXT_PAGE			0
#define PREV_PAGE			1

#define INDEX_REC_SIZE		7

typedef struct {
	unsigned short start_no;
	unsigned short end_no;
}T_CODETBL_OFFSET;


typedef struct {
	unsigned char sKey[2];
	unsigned char sRecNum[2];
	unsigned char sAddr[3];
} T_INDEX_REC;

// Define BX
#define BX_MAINKEY_LEN		4
#define MAX_BH_BUFFER		300		/*  600   [20070209] �����������󻺳�,�����ɲ鵽100���� */

#define BX_MAX_REFER_VAL	5

#define BXCODE_REC_SIZE		4

#define BXINDEX_REC_SIZE		7

typedef struct {
	unsigned char sKey[2];
	unsigned char sRecNum[2];
	unsigned char sAddr[3];
} T_BXINDEX_REC;

typedef struct {
	unsigned char sTailCode[2];		/* �ʻ������2λ��ת����BCD */
	unsigned char sHz[2];
} T_BXCODE_REC;

struct T_FINDBXLOG {
	unsigned char sInpCode[7];		/* �����5�룬���в���'*' */
	unsigned char sTailCode[1];
	unsigned char bCodeFlag;
	unsigned char bMaskFlag;
	uint16_t  bNum;				/* �����к��ֵ���Ŀ */
	uint16_t bPoint;			/* ����sHzBuffer�ĵ�ǰλ�� */
	unsigned char *sHzBuffer;		/* ��Ų�ѯ����Ļ��� */
	uint16_t nTotal;			/* ���������ĺ������� int addbywjh */
	uint32_t   nCurPosi;			/* FLASH�ĵ�ǰλ�� */  //wjhdelete
	uint16_t  nReadedNum;
};

// Define PY
#define DAN_PIN				1
#define SHUANG_PIN			2

#define IS_SHENGMU			1
#define IS_YUNMU			2

#define USE_ZEYIN			0x01
#define NOT_MISTINESS_PY	0x02

#define PY_MAINKEY_LEN		4
#define MAX_PY_BUFFER		400	/* 800 [20070209] ƴ���������󻺳�,�����ɲ鵽600���� */

#define MAX_PY_NUM			7

#define PY_INDEX_TOTAL		0x1E1		// 885
//#define PYCODETBL_OFFSET	0x25d00

#define PYCODE_REC_SIZE		2
#define PYBASE_INFO_SIZE	8

#define PYINDEX_REC_SIZE		6
typedef struct {
	unsigned char sKey[2];
	unsigned char sRecNum[2];
	unsigned char sAddr[2];
} T_PYINDEX_REC;

typedef struct {
//	unsigned char sInpCode[1];			/* �����������λ��ת����BCD[�Ҷ���] */
	unsigned char sHz[2];
} T_PYCODE_REC;

struct T_FINDPYLOG {
	unsigned char sInpCode[8];		/* �����ƽ���룬���в���'*' */
	unsigned short nHzNum;			/* �����к��ֵ���Ŀ */
	unsigned short bPoint;			/* ����sHzBuffer�ĵ�ǰλ�� */
	unsigned char *sHzBuffer;		/* ��Ų�ѯ����Ļ��� */

	unsigned short  nTotal;		/* ���������ĺ������� */
	unsigned long   nCurPosi;	/* FLASH�ĵ�ǰλ�� */
	unsigned short  nReadedTotal;
};

typedef struct {
	unsigned char sKey[5];			/* �������� */
	unsigned char sPYTotal[1];		/* ƴ������ */
	unsigned char sOffset[2];		/* �ڲ�����Ϣ�е�ƫ�� */
} T_PYBASE_INFO;					/* ������Ϣ */

typedef struct {
	unsigned char bPYTotal;					/* ƴ����ϵ����� */
	unsigned char fHintInput[MAX_PY_NUM];	/* �Ƿ���ʾ���� */
	char ssPYCode[MAX_PY_NUM][5];
	char ssPinYun[MAX_PY_NUM][7];
} T_PYINPUTCODE;

struct T_FIELD {
	unsigned short snField[2];
};
struct T_CODEINDEXTBL {
	unsigned char sPYCode[1];
	unsigned char sIndexNo[1];
};


#define LX_INDEX_REC_SIZE		5

typedef struct {
	unsigned char sKey[2];
	unsigned char sRecNum[1];
	unsigned char sAddr[2];
} T_LX_INDEX_REC;

typedef struct {
	unsigned short  bNum;				/* �����к��ֵ���Ŀ */
	unsigned short  bPoint;				/* ����sHzBuffer�ĵ�ǰλ�� */
	unsigned char	*sHzBuffer;			/* ��Ų�ѯ����Ļ��� */
	unsigned short  nTotal;				/* ���������ĺ������� */
	unsigned long	nCurPosi;			/* FLASH�ĵ�ǰλ�� */
}T_FINDLXLOG;



