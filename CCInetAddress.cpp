﻿
#include "CCInetAddress.h"

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	#pragma comment(lib, "Ws2_32.lib")
#endif

NS_CC_BEGIN

CCInetAddress::CCInetAddress()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	sin_len = sizeof(struct sockaddr_in);
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif
}

CCInetAddress::CCInetAddress(const char* ip, unsigned short port)
{
	sin_family = AF_INET;
	sin_addr.s_addr = inet_addr(ip);
	sin_port = htons(port);
	memset(sin_zero, 0, 8);
}

CCInetAddress::CCInetAddress(const struct sockaddr * addr)
{
	memcpy(&this->sin_family, addr, sizeof(struct sockaddr));
}

CCInetAddress::~CCInetAddress(void)
{
	
}

CCInetAddress::operator struct sockaddr*()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	return (struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return (struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return (struct sockaddr *)(&this->sin_len);
#endif
}

CCInetAddress::operator const struct sockaddr*() const
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	return (const struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return (const struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return (const struct sockaddr *)(&this->sin_len);
#endif
}

const char* CCInetAddress::getHostAddress() const
{
	static char addr[64];
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	sprintf_s(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	snprintf(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	snprintf(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif
	return addr;
}

const char* CCInetAddress::getIp() const
{
	return inet_ntoa(sin_addr);
}

unsigned short CCInetAddress::getPort() const
{
	return ntohs(sin_port);
}

void CCInetAddress::setIp(const char* ip)
{
	sin_addr.s_addr = inet_addr(ip);
}

void CCInetAddress::setIp(unsigned int ip)
{
	sin_addr.s_addr = ip;
}

void CCInetAddress::setPort(unsigned short port)
{
	sin_port = htons(port);
}

void CCInetAddress::setHost(const char* name)
{
	hostent* h = gethostbyname(name);
	if( h != NULL )
	{
		sin_addr.s_addr = *((u_long *)h->h_addr_list[0]);
	}
}

int CCInetAddress::getLength() 
{ 
	return sizeof(sockaddr_in); 
}

NS_CC_END