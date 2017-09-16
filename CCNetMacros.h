#ifndef __CCNET_MACROS_H__
#define __CCNET_MACROS_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef unsigned int          SOCKET;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#endif

#ifndef USING_PACKAGE_HEAD_LENGTH
#define USING_PACKAGE_HEAD_LENGTH 1
#endif
#ifndef SOCKET_SOTIMEOUT
#define SOCKET_SOTIMEOUT 30.0f
#endif
#ifndef SOCKET_READ_BUFFER_SIZE
#define SOCKET_READ_BUFFER_SIZE 8192
#endif
#ifndef MEMORYTYPE_REVERSE
#define MEMORYTYPE_REVERSE 1
#endif
#ifndef HANDLE_ON_SINGLE_FRAME
#define HANDLE_ON_SINGLE_FRAME 1
#endif
#ifndef CCBUFFER_ORGINAL_SIZE
#define CCBUFFER_ORGINAL_SIZE 512
#endif

#ifndef SINGLE_DELEGATE_INSTANCE_FUNC
#define SINGLE_DELEGATE_INSTANCE_FUNC(_CLASS_) \
static _CLASS_* getInstance() \
{ \
	static _CLASS_* pRet = NULL; \
	if( !pRet ) pRet = new _CLASS_(); \
	return pRet; \
}
#endif //SINGLE_DELEGATE_INSTANCE_FUNC

#endif //__CCNET_MACROS_H__