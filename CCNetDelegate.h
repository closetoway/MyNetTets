#ifndef __CCNET_NETDELEGATE_H__
#define __CCNET_NETDELEGATE_H__

#include "cocos2d.h"
#include "CCNetMacros.h"
#include "CCSocket.h"

NS_CC_BEGIN

class CCNetDelegate : public Ref
{
public:
	CCNetDelegate();
	virtual ~CCNetDelegate();

public:
	// will calling when a package is coming
	virtual void onMessageReceived(CCBuffer& oBuffer) = 0;

	// when connected will calling
	virtual void onConnected(){}

	// when connect time out will calling
	virtual void onConnectTimeout(){}

	// on disconnected will call
	virtual void onDisconnected(){}

	// on exception
	virtual void onExceptionCaught(CCSocketStatus eStatus){}

public:
	// set target address
	void setInetAddress(const CCInetAddress& oInetAddress);

	// get target address
	const CCInetAddress& getInetAddress() const;
	
	// the time out of connecting
	void setSoTimeout(float fSoTimeout);

	// get the time out value
	float getSoTimeout() const;

	// send package to target address
	void send(char* pBuffer, unsigned int uLen);

	// send package to target address
	void send(CCBuffer* pBuffer);

	// check the net status
	bool isConnected();

	// close the socket
	void close();

	// connect to target address
	bool connect();

	// disconnect as close for now
	void disconnect();

private:
	// read data on every frame, if needed
	bool runRead();

	// send data on every frame, if needed
	bool runWrite();

	// frame call
	void runSchedule(float dt);
	
	// registe the function of frame called
	void registerScheduler();

	// unregiste the function of frame called
	void unregisterScheduler();

private:

	/**
	 * struct : _SENDBUFFER
	 * author : Jason lee
	 * email  : jason.lee.c@foxmail.com
	 * descpt : send data
	 */
	struct _SENDBUFFER
	{
		char* pBuffer;       // the data
		int nOffset;         // the send data offset
		int nLength;         // data's length
	};
	
private:
	bool                   m_bRunSchedule;
	float                  m_fConnectingDuration;
	float                  m_fSoTimeout;
	std::list<_SENDBUFFER> m_lSendBuffers;
	CCBuffer               m_oReadBuffer;
	CCInetAddress          m_oInetAddress;
	CCSocket               m_oSocket;
	char                   m_pReadBuffer[SOCKET_READ_BUFFER_SIZE];

protected:
	CCSocketStatus         m_eStatus;
};

NS_CC_END

#endif //__CCNET_NETDELEGATE_H__