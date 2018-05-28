/*
 * Gloabal.h
 *
 *  Created on: 2015��12��9��
 *      Author: Administrator
 */

#ifndef GLOABAL_H_
#define GLOABAL_H_

#include <pthread.h>


extern pthread_mutex_t mGloabalLock;

#define OS_ENTER_CRITICAL()			pthread_mutex_lock(&mGloabalLock)
#define OS_EXIT_CRITICAL()			pthread_mutex_unlock(&mGloabalLock)



#endif /* GLOABAL_H_ */
