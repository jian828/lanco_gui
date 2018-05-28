/********************************Copyright(c)********************************
*                        Yuewei Song Software Studio
*
*                            136337978@qq.com
*
*
*-------------------------------File Info-----------------------------------
* File Name:               Queue.h
* Last modifyed date: 	    2013/1/31
* Last Version:            V1.0
* Description:
*
*----------------------------------------------------------------------------
* Created By:             Yuewei Song
* Created date:           2013/1/31
* Version:                V1.0
* Descriptions:
*
*----------------------------------------------------------------------------
* Modified by:
* Modified date:          2013/1/31
* Version:
* Description:
*
****************************************************************************/


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "inc/types.h"
#include "inc/Gloabal.h"

#define MEMORY_ALLOCATE				malloc
#define MEMORY_FREE					free
#define Mem_Copy					memcpy


#define QUEUE_BUF_SIZE_DEFAULT          100

#define DEBUG_ASSERT(condition)		do {				 					\
										if(!(condition))					\
										{									\
											assert(0);						\
										}									\
									} while (0)

template <class TType, 
TUINT QUEUE_SIZE = QUEUE_BUF_SIZE_DEFAULT>
class CQueue {
    public:
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.CQueue
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  void
    * 
    *  Description:
    *  ���캯��
    * 
    *--------------------------------------------------------------------------*/
	inline CQueue()
	{
		DEBUG_ASSERT(0 < QUEUE_SIZE);
		mQueueCtr = 0;        
		mQueueInPtr = &mQueueBuf[0];
		mQueueOutPtr = &mQueueBuf[0];
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.CQueue
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  void
    * 
    *  Description:
    *  ������������еĻ���
    * 
    *--------------------------------------------------------------------------*/    
	inline void Clear(void)
	{
		OS_ENTER_CRITICAL();
		mQueueCtr = 0;
		mQueueInPtr = &mQueueBuf[0];
		mQueueOutPtr = &mQueueBuf[0];
		OS_EXIT_CRITICAL();
	};
    
	~CQueue()
	{
	};
    
	/*--------------------------------------------------------------------------
	*  Function:
	*  CQueue.GetTypeSize
	* 
	*  Parameters:
	*  void
	* 
	*  Returns value:
	*  TUINT
	* 
	*  Description:
	*  ��ȡԪ�����ʹ�С
	* 
	*--------------------------------------------------------------------------*/    
	inline TUINT GetTypeSize(void)
	{	
		return (sizeof(TType));
	};
    
	/*--------------------------------------------------------------------------
	*  Function:
	*  CQueue.Copy
	* 
	*  Parameters:
	*  CQueue &inSrcQueue
	* 
	*  Returns value:
	*  void
	* 
	*  Description:
	*  �����������У��������д�С�������
	* 
	*--------------------------------------------------------------------------*/    
	inline void Copy(CQueue &inSrcQueue)
	{	
		//�������д�С�������
		DEBUG_ASSERT((QUEUE_SIZE == inSrcQueue.GetSize())
                     && (sizeof(TType) == inSrcQueue.GetTypeSize()));
        
		mQueueCtr = inSrcQueue.mQueueCtr;
		Mem_Copy(mQueueBuf, inSrcQueue.mQueueBuf, QUEUE_SIZE * sizeof(TType));
		mQueueInPtr = mQueueBuf + (inSrcQueue.mQueueInPtr - inSrcQueue.mQueueBuf);
		mQueueOutPtr = mQueueBuf + (inSrcQueue.mQueueOutPtr - inSrcQueue.mQueueBuf);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetSize
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  TUINT   -���ض��еĴ�С
    * 
    *  Description:
    *  ���ض��еĴ�С
    * 
    *--------------------------------------------------------------------------*/
	inline TUINT GetSize(void)
	{
		return (QUEUE_SIZE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetElemNum
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  TUINT   -���ض��к��ж���Ԫ��
    * 
    *  Description:
    *  ���ض��к��ж���Ԫ��
    * 
    *--------------------------------------------------------------------------*/
	inline TUINT GetElemNum(void)
	{
		return (mQueueCtr);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.IsEmpty
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  TBOOL   -����Ϊ���򷵻�bTRUE, ��Ϊ���򷵻�bFALSE
    * 
    *  Description:
    *  ���ض��е�ǰ��״̬
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL IsEmpty(void)
	{
		OS_ENTER_CRITICAL();
		if (0 < mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			OS_EXIT_CRITICAL();
			return (bTRUE);
		}
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.IsFull
    * 
    *  Parameters:
    *  void
    * 
    *  Returns value:
    *  TBOOL - ����Ϊ���򷵻�bTRUE,��Ϊ���򷵻�bFALSE
    * 
    *  Description:
    *  ���ض��е�ǰ��״̬
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL IsFull(void)
	{
		OS_ENTER_CRITICAL();
		if (QUEUE_SIZE > mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			OS_EXIT_CRITICAL();
			return (bTRUE);
		}
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.EnQueue2
    * 
    *  Parameters:
    *  TType * - ָ��ָ���Ԫ��
    * 
    *  Returns value:
    *  TBOOL -���TType *ָ��ָ���Ԫ�ؽ����гɹ����򷵻�bTRUE,���򷵻�bFALSE.
    * 
    *  Description:
    *  ��ָ����Ԫ�ؽ����У�����ɹ�����bTRUE,ʧ�ܷ���bFALSE,��������������ȫ�ģ�
    *  ֻ�����ж���ʹ�ã����ߵ�����ʹ�á�
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL EnQueue2(const TType *inElemPtr)
	{
		DEBUG_ASSERT(NULL != inElemPtr);
        
		if (mQueueCtr < QUEUE_SIZE)
		{
			mQueueCtr++;
			/*
			memcpy(mQueueInPtr, inElemPtr, sizeof(TType));
			*/
			*mQueueInPtr = *inElemPtr;
			mQueueInPtr++;
			if (mQueueInPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueInPtr = mQueueBuf;
			}
			return (bTRUE);
		}
		return (bFALSE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.EnQueue2
    * 
    *  Parameters:
    *  TType - һ��TType��Ԫ��
    * 
    *  Returns value:
    *  TBOOL -�������Ԫ�ؽ����гɹ����򷵻�bTRUE,���򷵻�bFALSE.
    * 
    *  Description:
    *  ��ָ����Ԫ�ؽ����У�����ɹ�����bTRUE,ʧ�ܷ���bFALSE,��������������ȫ�ģ�
    *  ֻ�����ж���ʹ�ã����ߵ�����ʹ�á�
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL EnQueue2(const TType &inElem)
	{
		if (mQueueCtr < QUEUE_SIZE)
		{
			mQueueCtr++;
			*mQueueInPtr = inElem;
			mQueueInPtr++;
			if (mQueueInPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueInPtr = mQueueBuf;
			}
			return (bTRUE);
		}
		return (bFALSE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.EnQueue
    * 
    *  Parameters:
    *  TType - һ��ָ��TTypeԪ�ص�ָ��
    * 
    *  Returns value:
    *  TBOOL -�������Ԫ�ؽ����гɹ����򷵻�bTRUE,���򷵻�bFALSE.
    * 
    *  Description:
    *  ��ָ����Ԫ�ؽ����У�����ɹ�����bTRUE,ʧ�ܷ���bFALSE,������������ȫ�ģ�
    *  �������ж���ʹ�á�
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL EnQueue(const TType *inElemPtr)
	{
		TBOOL ret;
        
		DEBUG_ASSERT(NULL != inElemPtr);
        
		OS_ENTER_CRITICAL();
		if (mQueueCtr < QUEUE_SIZE)
		{
			mQueueCtr++;
			*mQueueInPtr = *inElemPtr;
			mQueueInPtr++;
			if (mQueueInPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueInPtr = mQueueBuf;
			}           
			ret = bTRUE;
		}
		else
		{
			ret = bFALSE;
		}
		OS_EXIT_CRITICAL();
        
		return (ret);    
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.EnQueue
    * 
    *  Parameters:
    *  TType - һ��TTypeԪ��
    * 
    *  Returns value:
    *  TBOOL -�������Ԫ�ؽ����гɹ����򷵻�bTRUE,���򷵻�bFALSE.
    * 
    *  Description:
    *  ��ָ����Ԫ�ؽ����У�����ɹ�����bTRUE,ʧ�ܷ���bFALSE,������������ȫ�ģ�
    *  �������ж���ʹ�á�
    *
    *--------------------------------------------------------------------------*/
	inline TBOOL EnQueue(const TType &inElem)
	{
		TBOOL ret;
        
		OS_ENTER_CRITICAL();
		if (mQueueCtr < QUEUE_SIZE)
		{
			mQueueCtr++;
			*mQueueInPtr = inElem;
			mQueueInPtr++;
			if (mQueueInPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueInPtr = mQueueBuf;
			}
			ret = bTRUE;
		}
		else
		{
			ret = bFALSE;
		}
		OS_EXIT_CRITICAL();
        
        
		return (ret);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetHead
    * 
    *  Parameters:
    *  outElemPtr - point to a buffer that saved a element in queue.
    * 
    *  Returns value:
    *  TBOOL   - if the queue is empty,returned the bFALSE, else returned the bTRUE
    * 
    *  Description:
    *  returned the first element in queue. don't delete from the queue
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL GetHead(TType *outElemPtr)
	{
		DEBUG_ASSERT(NULL != outElemPtr);
        
		OS_ENTER_CRITICAL();
		if (0 == mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			/*
			memcpy(outElemPtr, mQueueOutPtr, sizeof(TType));
			*/
			*outElemPtr = *mQueueOutPtr; 
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
	}
    
    inline TBOOL GetElem(TUINT32 inOffset, TType *outElemPtr)
    {
		DEBUG_ASSERT(NULL != outElemPtr);
        
		OS_ENTER_CRITICAL();
		if ((0 == mQueueCtr) || (inOffset >= mQueueCtr))
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			/*
			memcpy(outElemPtr, mQueueOutPtr, sizeof(TType));
			*/
            TType* iOut = mQueueOutPtr + inOffset;
            if (iOut >= &mQueueBuf[QUEUE_SIZE])
            {
                TUINT32 ip = (iOut - &mQueueBuf[QUEUE_SIZE]) / sizeof(TType);
                *outElemPtr = mQueueBuf[ip];
            }
            else
            {
			    *outElemPtr = *(mQueueOutPtr + inOffset); 
            }
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
    }
    
    /*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetHead
    * 
    *  Parameters:
    *  outElem -  saved the first element in Queue
    * 
    *  Returns value:
    *  TBOOL   - if the queue is empty,returned the bFALSE, else returned the bTRUE
    * 
    *  Description:
    *  returned the first element in queue. don't delete from the queue
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL GetHead(TType &outElem)
	{
		OS_ENTER_CRITICAL();
		if (0 == mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{           
			outElem = *mQueueOutPtr; 
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
	}
    
    inline TBOOL GetElem(TUINT32 inOffset, TType &outElemPtr)
    {
		OS_ENTER_CRITICAL();
		if ((0 == mQueueCtr) || (inOffset >= mQueueCtr))
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			/*
			memcpy(outElemPtr, mQueueOutPtr, sizeof(TType));
			*/
            TType* iOut = mQueueOutPtr + inOffset;
            if (iOut >= &mQueueBuf[QUEUE_SIZE])
            {
                TUINT32 ip = (iOut - &mQueueBuf[QUEUE_SIZE]) / sizeof(TType);
                outElemPtr = mQueueBuf[ip];
            }
            else
            {
			    outElemPtr = *(mQueueOutPtr + inOffset); 
            }
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
    }
    
    /*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetHead
    * 
    *  Parameters:
    *  outElemPtr - point to a buffer that saved a element in queue.
    * 
    *  Returns value:
    *  TBOOL   - if the queue is empty,returned the bFALSE, else returned the bTRUE
    * 
    *  Description:
    *  returned the first element in queue. don't delete from the queue
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL GetHead2(TType *outElemPtr)
	{
		DEBUG_ASSERT((TType *)0 != outElemPtr);
		if (0 == mQueueCtr)
		{
			return (bFALSE);
		}
		else
		{           
			*outElemPtr = *mQueueOutPtr; 
		}
        
		return (bTRUE);
	}
    
    inline TBOOL GetElem2(TUINT32 inOffset, TType *outElemPtr)
    {
		DEBUG_ASSERT(NULL != outElemPtr);
		if ((0 == mQueueCtr) || (inOffset >= mQueueCtr))
		{
			return (bFALSE);
		}
		else
		{
            TType* iOut = mQueueOutPtr + inOffset;
            if (iOut >= &mQueueBuf[QUEUE_SIZE])
            {
                TUINT32 ip = (iOut - &mQueueBuf[QUEUE_SIZE]) / sizeof(TType);
                *outElemPtr = mQueueBuf[ip];
            }
            else
            {
			    *outElemPtr = *(mQueueOutPtr + inOffset); 
            }
		}
		return (bTRUE);
    }
    
    /*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.GetHead
    * 
    *  Parameters:
    *  outElem -  saved the first element in Queue
    * 
    *  Returns value:
    *  TBOOL   - if the queue is empty,returned the bFALSE, else returned the bTRUE
    * 
    *  Description:
    *  returned the first element in queue. don't delete from the queue
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL GetHead2(TType &outElem)
	{
		if (0 == mQueueCtr)
		{
			return (bFALSE);
		}
		else
		{           
			outElem = *mQueueOutPtr; 
		}
        
		return (bTRUE);
	}
    

    inline TBOOL GetElem2(TUINT32 inOffset, TType &outElemPtr)
    {
		if ((0 == mQueueCtr) || (inOffset >= mQueueCtr))
		{
			return (bFALSE);
		}
		else
		{
            TType* iOut = mQueueOutPtr + inOffset;
            if (iOut >= &mQueueBuf[QUEUE_SIZE])
            {
                TUINT32 ip = (iOut - &mQueueBuf[QUEUE_SIZE]) / sizeof(TType);
                outElemPtr = mQueueBuf[ip];
            }
            else
            {
			    outElemPtr = *(mQueueOutPtr + inOffset); 
            }
		}
		return (bTRUE);
    }

    
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.DeQueue
    * 
    *  Parameters:
    *  outElemPtr - point to a buffer that saved a element in queue.
    * 
    *  Returns value:
    *  TBOOL   - if the queue is empty,returned the bFALSE, else returned the bTRUE
    * 
    *  Description:
    *  returned the first element in queue.
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL DeQueue(TType *outElemPtr)
	{
		DEBUG_ASSERT(NULL != outElemPtr);
        
		OS_ENTER_CRITICAL();
		if (0 == mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			mQueueCtr--;
			/*
			memcpy(outElemPtr, mQueueOutPtr, sizeof(TType));
			*/
			*outElemPtr = *mQueueOutPtr;
			mQueueOutPtr++;
			if (mQueueOutPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueOutPtr = mQueueBuf;
			}        
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.DeQueue
    * 
    *  Parameters:
    *  outElem -  saved the first element in Queue
    * 
    *  Returns value:
    *  TBOOL   - if queue isn't empty, returned bTRUE, else returned bFALSE.
    * 
    *  Description:
    *  DeQueue returned the first element in queue.
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL DeQueue(TType &outElem)
	{
		OS_ENTER_CRITICAL();
		if (0 == mQueueCtr)
		{
			OS_EXIT_CRITICAL();
			return (bFALSE);
		}
		else
		{
			mQueueCtr--;
			/*
			memcpy(&outElem, mQueueOutPtr, sizeof(TType));
			*/
			outElem = *mQueueOutPtr;
			mQueueOutPtr++;
			if (mQueueOutPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueOutPtr = mQueueBuf;
			}        
			OS_EXIT_CRITICAL();   
		}
        
		return (bTRUE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.DeQueue2
    * 
    *  Parameters:
    *  outElemPtr - point to a buffer that saved the first element in queue
    * 
    *  Returns value:
    *  TBOOL   - if the queue is not empty, returned the bTRUE, if the queue is 
    *  empty, returned the bFALSE
    * 
    *  Description:
    *  DeQueue2 returned the first element in queue.
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL DeQueue2(TType *outElemPtr)
	{
		DEBUG_ASSERT(NULL != outElemPtr);
        
		if (0 == mQueueCtr)
		{
			return (bFALSE);
		}
		else
		{
			mQueueCtr--;
			/*
			memcpy(outElemPtr, mQueueOutPtr, sizeof(TType));
			*/
			*outElemPtr = *mQueueOutPtr;
			mQueueOutPtr++;
			if (mQueueOutPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueOutPtr = mQueueBuf;
			}        
		}
        
		return (bTRUE);
	};
    
	/*--------------------------------------------------------------------------
    *  Function:
    *  CQueue.DeQueue2
    * 
    *  Parameters:
    *  outElem -  the Element in queue header 
    * 
    *  Returns value:
    *  TBOOL   - if the queue isn't empty, returned bTRUE, else returned bFALSE
    * 
    *  Description:
    *  DeQueue2 returned the first element in queue.
    * 
    *--------------------------------------------------------------------------*/
	inline TBOOL DeQueue2(TType &outElem)
	{
		if (0 == mQueueCtr)
		{
			return (bFALSE);
		}
		else
		{
			mQueueCtr--;
			/*
			memcpy(&outElem, mQueueOutPtr, sizeof(TType));
			*/
			outElem = *mQueueOutPtr;
			mQueueOutPtr++;
			if (mQueueOutPtr == &mQueueBuf[QUEUE_SIZE])
			{
				mQueueOutPtr = mQueueBuf;
			}        
		}
        
		return (bTRUE);
	};
    
    protected:
    TUINT mQueueCtr;
    TType *mQueueInPtr;
    TType *mQueueOutPtr;
    TType mQueueBuf[QUEUE_SIZE];
};


#endif



