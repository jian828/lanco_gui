/********************************Copyright(c)********************************
 *                        Yuewei Song Software Studio
 *
 *                            136337978@qq.com
 *
 *
 *-------------------------------File Info-----------------------------------
 * File Name:               Message.cpp
 * Last modified date:      2013/3/27
 * Last Version:            V1.0
 * Description:
 *
 *----------------------------------------------------------------------------
 * Created By:              Yuewei Song
 * Created date:            2013/3/27
 * Version:                 V1.0
 * Descriptions:
 *
 *----------------------------------------------------------------------------
 * Modified by:
 * Modified date:           2013/3/27
 * Version:
 * Description:
 *
 ****************************************************************************/


/****************************************************************************
 * Include Files
 ****************************************************************************/
#include <inc/Message.h>

#include <inc/types.h>
#include <malloc.h>
#include <string.h>

#define OS_CRITICAL_METHOD			(-1)
#define MEMORY_ALLOCATE				malloc
#define MEMORY_FREE					free
#define Mem_Copy					memcpy


/****************************************************************************
 * Static Variables
 ****************************************************************************/
CQueue<CMessage *, MESSAGE_ITEM_MAX_NUM> CMessage::mFreeMsg;
CMessage CMessage::mMsgItem[MESSAGE_ITEM_MAX_NUM];
TBOOL CMessage::mIsInit = bFALSE;
pthread_mutex_t CMessage::mLock;
/*--------------------------------------------------------------------------
 *  Function:
 *  CMessage.CMessage
 * 
 *  Parameters:
 * 		void
 * 
 *  Returns value:
 * 		void
 * 
 *  Description:
 * 		构造函数
 * 
 *--------------------------------------------------------------------------*/
CMessage::CMessage()
{
	mMsgComeFrom 	= (CMessageComeFrom *)0;
	mMsgCode 		= 0;
	mMsgLen 		= (TUINT8)0;
	mMsgBuf			= (TUINT8 *)0;
	mMsgBufLen		= 0;

#ifdef MESSAGE_DEBUG
	mIsFree			= bTRUE;
#endif

	if (!mIsInit) {
		pthread_mutex_init(&mLock, NULL);
		for (TUINT i = 0; i < MESSAGE_ITEM_MAX_NUM; i++)
		{
			mFreeMsg.EnQueue2(&mMsgItem[i]);
		}
	}
}


/*--------------------------------------------------------------------------
 *  Function:
 *  CMessage.~CMessage
 * 
 *  Parameters:
 * 		void
 * 
 *  Returns value:
 * 		void
 * 
 *  Description:
 * 		析构函数
 * 
 *--------------------------------------------------------------------------*/
CMessage::~CMessage()
{
	mIsInit = bFALSE;
}

/*--------------------------------------------------------------------------
 *  Function:
 *  CMessage.Allocate
 * 
 *  Parameters:
 * 		TUINT32 inMsgSize- 消息体的大小
 * 
 *  Returns value:
 * 		CMessage *
 * 
 *  Description:
 * 		申请CMessage
 * 
 *--------------------------------------------------------------------------*/
CMessage *CMessage::Allocate(TUINT32 inMsgSize)
{
	CMessage *ptr;
	
	/*初始化mFreeMsg队列*/
	OS_ENTER_CRITICAL();
	if (!mIsInit)
	{
		mIsInit = bTRUE;

		OS_EXIT_CRITICAL();

		for (TUINT i = 0; i < MESSAGE_ITEM_MAX_NUM; i++)
		{
			mFreeMsg.EnQueue2(&mMsgItem[i]);
		}
	}
	else
	{
		OS_EXIT_CRITICAL();
	}

	if (mFreeMsg.DeQueue(ptr))
	{
#ifdef MESSAGE_DEBUG 
		if (!ptr->mIsFree)
			assert(0);//DEBUG_ASSERT(0);
		ptr->mIsFree = bFALSE;
#endif
		if (0 != inMsgSize)
		{
			ptr->mMsgBuf = (TUINT8 *)MEMORY_ALLOCATE(inMsgSize);
			DEBUG_ASSERT(NULL != ptr->mMsgBuf);
			ptr->mMsgBufLen = inMsgSize;
		}
		else
		{
			ptr->mMsgBuf = NULL;
			ptr->mMsgBufLen = 0;	
		}

		return (ptr);
	}
	else
	{
		return ((CMessage *)0);
	}
}

/*--------------------------------------------------------------------------
 *  Function:
 *  	CMessage.Free
 * 
 *  Parameters:
 * 		CMessage *
 * 
 *  Returns value:
 * 		void
 * 
 *  Description:
 * 		释放CMessage
 * 
 *--------------------------------------------------------------------------*/
void CMessage::Free(CMessage *inMsg)
{
    if (NULL == inMsg)
        return ;
    
    inMsg->mMsgComeFrom 	= (CMessageComeFrom *)0;
	inMsg->mMsgCode 		= 0;
	inMsg->mMsgLen 			= (TUINT8)0;
	
	if (NULL != inMsg->mMsgBuf)
	{
		MEMORY_FREE(inMsg->mMsgBuf);
	}
	
	inMsg->mMsgBuf			= (TUINT8*)0;
	inMsg->mMsgBufLen		= 0;

#ifdef MESSAGE_DEBUG
	if (inMsg->mIsFree)
		assert(0);//DEBUG_ASSERT(0);
	else
		inMsg->mIsFree = bTRUE;
#endif

	mFreeMsg.EnQueue(inMsg); 
}

/*--------------------------------------------------------------------------
 *  Function:
 *  CMessage.IsValid
 * 
 *  Parameters:
 * 		void	
 * 
 *  Returns value:
 * 		TBOOL - bTRUE,合法的CMessage.
 * 
 *  Description:
 * 		判断CMessage 是否合法
 * 
 *--------------------------------------------------------------------------*/
TBOOL CMessage::IsValid(void) const
{
#ifdef MESSAGE_DEBUG
	if (mIsFree)
		return (bFALSE); 
#endif
	if ((CMessageComeFrom*)0 == mMsgComeFrom || 0 == mMsgCode
		|| mMsgCode >= 20000)
		return (bFALSE);
		
	return (bTRUE);
}

TBOOL CMessage::SetMsgComeFrom(CMessageComeFrom *inManMsg)
{
	mMsgComeFrom = inManMsg;
	return (bTRUE);
}

CMessageComeFrom* CMessage::GetMsgComeFrom() const
{
	return (mMsgComeFrom);
}


TBOOL CMessage::SetMsgCode(TUINT32 inMsgCode)
{
	mMsgCode = inMsgCode;
	return (bTRUE);
}


TUINT32 CMessage::GetMsgCode() const
{
	return (mMsgCode);
}

TBOOL CMessage::SetMsgLen(TUINT32 inLen)
{
	if (inLen <= mMsgBufLen)
	{
		mMsgLen = inLen;
		return (bTRUE);
	}
	else
	{
		DEBUG_ASSERT(inLen <= mMsgBufLen);
		return (bFALSE);
	}
}

TUINT32 CMessage::GetMsgLen() const
{
	return (mMsgLen);
}

TUINT8 *CMessage::GetMsgBuf() const
{
	return (mMsgBuf);
}




