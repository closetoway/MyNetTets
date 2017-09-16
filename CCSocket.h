#ifndef __CCNET_SOCKET_H__
#define __CCNET_SOCKET_H__

#include "cocos2d.h"
#include "CCNetMacros.h"
#include "CCInetAddress.h"
#include "CCBuffer.h"
#include <string>

NS_CC_BEGIN

enum CCSocketStatus
{
	eSocketConnected			=  1,
	eSocketConnecting			=  2,
	eSocketDisconnected			=  3,
	eSocketConnectFailed		=  4,
	eSocketConnectTimeout		=  5,
	eSocketReceive				=  6,
	eSocketIoClosed				=  0,
	eSocketIoError				= -1,
	eSocketCreateFailed			= -2,
};

class CCSocket
{
public:
	CCSocket();
	virtual ~CCSocket();
	bool ccInit();
	int  ccRead(char* _buff, int _len);
	int  ccWrite(char* _buff, int _len);
	int  ccIsConnected();
	bool ccIsReadable();
	bool ccIsWritable();
	void ccClose();
	bool ccConnect();
	void ccDisconnect();
	void setInetAddress(const CCInetAddress& oInetAddress);

protected:
	SOCKET m_uSocket;
	CCInetAddress m_oInetAddress;
};


NS_CC_END

#endif //__CCNET_SOCKET_H__