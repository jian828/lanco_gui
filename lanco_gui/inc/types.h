/*
 * types.h
 *
 *  Created on: 2015Äê11ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef signed char		TSINT8;//8bit
typedef unsigned char		TUINT8;
typedef char			TINT8;

typedef char			TCHAR;//8bit

typedef signed short		TSINT16;//16bit
typedef unsigned short		TUINT16;
typedef short			TINT16;

typedef signed long		TSINT32;//32bit
typedef unsigned long		TUINT32;
typedef long			TINT32;

typedef int					TINT;		/* ...., It depended the CPU structrue*/
typedef unsigned int		TUINT;

typedef signed long long	TSINT64;//64bit
typedef unsigned long long 	TUINT64;
typedef long long		TINT64;

typedef float			TFP32;//32bit
typedef double			TFP64;//64bit

typedef TUINT32       		THandle;	/* the device handle */
typedef enum {
    bFALSE = (0 == 1),
    bTRUE = (1 == 1),
} TBOOL;


#endif /* TYPES_H_ */
