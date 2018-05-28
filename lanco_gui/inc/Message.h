#if !defined(EA_5ED95F3D_E2B9_472f_8C53_23AEBE8BEC44__INCLUDED_)
#define EA_5ED95F3D_E2B9_472f_8C53_23AEBE8BEC44__INCLUDED_

#include <inc/Queue.h>
#include <pthread.h>

#define MESSAGE_ITEM_MAX_NUM						6000
#define MESSAGE_DEBUG
#define MESSAGE_QUEUE_DEFAULT_SIZE				    900
#define MESSAGE_QUEUE_SIZE_BIG						1000

template <TUINT32 inSize>
class CManipulateMessage;

class CMessageComeFrom;


class CMessage
{
public:
	
	virtual ~CMessage();
	static CMessage *Allocate(TUINT32 inMsgSize);
	static void Free(CMessage *inMsg);
	TBOOL IsValid() const;
    
	TBOOL SetMsgComeFrom(CMessageComeFrom *inManMsg);
	CMessageComeFrom *GetMsgComeFrom() const;
	
	TBOOL SetMsgCode(TUINT32 inMsgCode);
	TUINT32 GetMsgCode() const;
	
	TBOOL SetMsgLen(TUINT32 inLen);
	TUINT32 GetMsgLen() const;
	
	TUINT8 *GetMsgBuf() const;



//protected:
	CMessage();
	static CQueue<CMessage *, MESSAGE_ITEM_MAX_NUM> mFreeMsg; 
	static CMessage mMsgItem[MESSAGE_ITEM_MAX_NUM];
	static TBOOL mIsInit;
    static pthread_mutex_t mLock;
    
    CMessageComeFrom *mMsgComeFrom;		/*消息所作用的对象的指针*/

	TUINT32 mMsgCode;					/*消息代码*/
	TUINT32 mMsgLen;					/*消息长度,不包括消息头*/
	TUINT8 *mMsgBuf;					/*消息内容的缓冲区指针*/
	TUINT32 mMsgBufLen;					/*消息缓冲区的长度*/
	
private:
#ifdef MESSAGE_DEBUG
	TBOOL mIsFree;						/*消息空闲标志*/
#endif	
};


typedef CQueue<CMessage *, MESSAGE_QUEUE_DEFAULT_SIZE>   CMsgQueue;

class CMessageComeFrom
{
public:
	CMessageComeFrom(){};
	~CMessageComeFrom(){};
	
    //virtual TBOOL Post(CMessage *mMsg) = 0;
};

template <TUINT32 inSize = MESSAGE_QUEUE_DEFAULT_SIZE>
class CManipulateMessage : public CMessageComeFrom
{
public:
	virtual ~CManipulateMessage()
	{
		mMsgQueue.Clear();
	};

	virtual TBOOL Post(CMessage *mMsg)
	{
		return (mMsgQueue.EnQueue(mMsg));
	};

#ifndef NDEBUG
	friend class CStatisticTask;
#endif

protected:
	CManipulateMessage()
	{
	};

	virtual TBOOL Clear()
	{
		CMessage *pMsg = NULL;
		while(mMsgQueue.DeQueue(pMsg))
		{
			DEBUG_ASSERT(pMsg != NULL);
			CMessage::Free(pMsg);
			pMsg = NULL;
		}

		return (bTRUE);
	};

	CQueue<CMessage *, inSize> mMsgQueue;
};

#endif // !defined(EA_5ED95F3D_E2B9_472f_8C53_23AEBE8BEC44__INCLUDED_)
