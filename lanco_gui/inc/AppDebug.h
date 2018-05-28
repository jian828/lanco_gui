/*
 * AppDebug.h
 *
 *  Created on: 2015Äê11ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef APPDEBUG_H_
#define APPDEBUG_H_

#include <android/log.h>
#include <time.h>

#define TAG				"NativeCode"
#define TAG_CSOCKET		"NativeSocketTcp"
#define TAG_UART		"NativeUart"


#define LOG_TAG_ENDLINE
#define LOG_INFO_TAG			"[I]" LOG_TAG_ENDLINE
#define LOG_DEBUG_TAG			"[D]" LOG_TAG_ENDLINE
#define LOG_WARNING_TAG			"[W]" LOG_TAG_ENDLINE
#define LOG_ERROR_TAG			"[E]" LOG_TAG_ENDLINE
#define LOG_OTHER_TAG			"[O]" LOG_TAG_ENDLINE

#define END_LINE                    "\r\n"



#define TRACE_ATTACH()		time_t timep;\
							time (&timep);\
							printf(" %s-%d %s", __FILE__,__LINE__, ctime(&timep))

//#define TRACE_ERR(TAG, ...)			__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
//#define TRACE_INFO(TAG, ...)		__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
//#define TRACE_WARNING(TAG, ...)		__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
//#define TRACE_DEBUG(TAG, ...)		__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
//#define TRACE_OTHER(TAG, ...)		__android_log_print(ANDROID_LOG_UNKNOWN, TAG, __VA_ARGS__)

#define TRACE_ERR(...)	\
	do {\
		__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);\
		__android_log_print(ANDROID_LOG_ERROR, TAG, "%s - %d", __FILE__,__LINE__);\
	}while(0)

#define TRACE_INFO(...) \
	do {\
		__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);\
		__android_log_print(ANDROID_LOG_INFO, TAG, "%s - %d", __FILE__,__LINE__);\
	}while(0)

#define TRACE_DEBUG(...) \
	do {\
		__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);\
		__android_log_print(ANDROID_LOG_DEBUG, TAG, "%s - %d", __FILE__,__LINE__);\
	}while(0)

#define TRACE_WARNING(...) \
	do {\
		__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__);\
		__android_log_print(ANDROID_LOG_WARN, TAG, "%s - %d", __FILE__,__LINE__);\
	}while(0)



#define LOG_ERR(...)	\
	do {\
		memset(mLogBuf, 0, sizeof(mLogBuf));\
		mLogLen = snprintf(mLogBuf, 255, __VA_ARGS__);\
		snprintf(mLogBuf + mLogLen, 255, "%s - %d",  __FILE__,__LINE__);\
		Log(env, 1, mLogBuf);\
	}while(0)

#define LOG_INFO(...)	\
	do {\
		memset(mLogBuf, 0, sizeof(mLogBuf));\
		mLogLen = snprintf(mLogBuf, 255, __VA_ARGS__);\
		snprintf(mLogBuf + mLogLen, 255, "%s - %d",  __FILE__,__LINE__);\
		Log(env, 3, mLogBuf);\
	}while(0)

#define LOG_WARNING(...)	\
	do {\
		memset(mLogBuf, 0, sizeof(mLogBuf));\
		mLogLen = snprintf(mLogBuf, 255, __VA_ARGS__);\
		snprintf(mLogBuf + mLogLen, 255, "%s - %d",  __FILE__,__LINE__);\
		Log(env, 4, mLogBuf);\
	}while(0)

#define LOG_DEBUG(...)	\
	do {\
		memset(mLogBuf, 0, sizeof(mLogBuf));\
		mLogLen = snprintf(mLogBuf, 255, __VA_ARGS__);\
		snprintf(mLogBuf + mLogLen, 255, "%s - %d",  __FILE__,__LINE__);\
		Log(env, 5, mLogBuf);\
	}while(0)

#include <assert.h>
#define DEBUG_ASSERT(condition)		do {				 					\
										if(!(condition))					\
										{									\
											__android_log_print(ANDROID_LOG_ERROR, TAG_CSOCKET,"assert fail");\
											assert(0);						\
										}									\
									} while (0)




#endif /* APPDEBUG_H_ */
