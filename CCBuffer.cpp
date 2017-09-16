#include "CCBuffer.h"
#include <stdlib.h>
#include <algorithm>

NS_CC_BEGIN

#define MAXA(a,b) a = (((a) < (b)) ? (b) : (a))
#define MINA(a,b) a = (((a) < (b)) ? (a) : (b))

CCBuffer::CCBuffer()
: _p_buffer(NULL)
, _u_write_pos(0)
, _u_read_pos(0)
, _u_content_size(0)
, _u_buffer_size(CCBUFFER_ORGINAL_SIZE)
, _u_mark_pos(0)
{
	_p_buffer = (char*)malloc(_u_buffer_size);
}

CCBuffer::CCBuffer(unsigned int n_capacity)
: _p_buffer(NULL)
, _u_write_pos(0)
, _u_read_pos(0)
, _u_content_size(0)
, _u_mark_pos(0)
{
	_u_buffer_size = n_capacity;
	_p_buffer = (char*)malloc(_u_buffer_size);
}

CCBuffer::CCBuffer(const char* p_data, unsigned int u_len)
: _p_buffer(nullptr)
, _u_read_pos(0)
, _u_mark_pos(0)
{
	CCASSERT(p_data && u_len > 0, "p_data && u_len > 0");

	_u_buffer_size = u_len;
	_u_write_pos = u_len;
	_u_content_size = u_len;
	_p_buffer = (char*)malloc(u_len);

	memcpy(_p_buffer, p_data, u_len);
}

CCBuffer::~CCBuffer()
{
	CC_SAFE_FREE(_p_buffer);
}

CCBuffer* CCBuffer::create()
{
	CCBuffer* pRet = new CCBuffer();
	pRet->autorelease();
	return pRet;
}

CCBuffer* CCBuffer::create(unsigned int n_capacity)
{
	CCBuffer* pRet = new CCBuffer(n_capacity);
	pRet->autorelease();
	return pRet;
}

CCBuffer* CCBuffer::create(const char* p_data, unsigned int u_len)
{
	CCBuffer* pRet = new CCBuffer(p_data, u_len);
	pRet->autorelease();
	return pRet;
}

Ref* CCBuffer::copy()
{
	if (_u_content_size > 0) {
		CCBuffer* p_buf = new CCBuffer(_p_buffer, _u_content_size);
		return p_buf;
	} else {
		return nullptr;
	}
}

unsigned int CCBuffer::getWriterIndex() const
{
	return _u_write_pos;
}

unsigned int CCBuffer::getContentSize() const
{
	return _u_content_size;
}

unsigned int CCBuffer::getReadableSize() const
{
	return _u_content_size - _u_read_pos;
}

unsigned int CCBuffer::getReaderIndex() const
{
	return _u_read_pos;
}

unsigned int CCBuffer::getBufferSize() const
{
	return _u_buffer_size;
}

void CCBuffer::discardReadContent()
{
	if (_u_read_pos > 0) {
		moveLeft(_u_read_pos);
	}
}

void CCBuffer::skipData(unsigned int u_len)
{
	if (_u_read_pos + u_len <= _u_content_size) {
		_u_read_pos += u_len;
	} else {
		_u_read_pos += _u_content_size - _u_read_pos;
	}
}

void CCBuffer::moveWriterIndexToFront()
{
	_u_write_pos = 0;
}

void CCBuffer::moveWriterIndexToBack()
{
	_u_write_pos = _u_content_size;
}

void CCBuffer::moveReaderIndexToFront()
{
	_u_read_pos = 0;
}

void CCBuffer::moveReaderIndexToBack()
{
	_u_read_pos = _u_content_size;
}

void CCBuffer::reset()
{
	moveWriterIndexToFront();
	moveReaderIndexToFront();
	_u_mark_pos = 0;
	_u_content_size = 0;
}

void CCBuffer::markReaderIndex()
{
	_u_mark_pos = _u_read_pos;
}

void CCBuffer::resetReaderIndex()
{
	_u_read_pos = _u_mark_pos;
}

void CCBuffer::setReaderIndex(unsigned int u_pos)
{
	CCASSERT(u_pos <= _u_content_size, "u_pos <= _u_content_size");
	_u_read_pos = u_pos;
}

void CCBuffer::setWriterIndex(unsigned int u_pos)
{
	CCASSERT(u_pos <= _u_content_size, "u_pos <= _u_content_size");
	_u_write_pos = u_pos;
}

void CCBuffer::moveLeft(unsigned int u_len)
{
	if (_u_content_size == 0) {
		return;
	}
	CCAssert(u_len != 0, "u_len!=0");

	if (u_len >= _u_content_size)
	{
		clear();
	}
	else
	{
		for (unsigned int i = u_len; i < _u_content_size; i++) {
			*(_p_buffer + (i - u_len)) = *(_p_buffer + i);
			*(_p_buffer + i) = 0;
		}
		_u_write_pos = (unsigned int)MAX(0, (int)_u_write_pos - (int)u_len);
		_u_read_pos = (unsigned int)MAX(0, (int)_u_read_pos - (int)u_len);
		_u_mark_pos = (unsigned int)MAX(0, (int)_u_mark_pos - (int)u_len);
		_u_content_size = _u_content_size - u_len;
	}
}

void CCBuffer::moveRight(unsigned int u_len)
{
	if (_u_content_size == 0) {
		return;
	}
	_reallocBufferSizeInChanged(u_len);
	for (int i = _u_content_size - 1; i >= 0; i--) {
		*(_p_buffer + i + (int)u_len) = *(_p_buffer + i);
		*(_p_buffer + i) = 0;
	}
	_u_write_pos += u_len;
	_u_read_pos  +=  u_len;
	_u_mark_pos += u_len;
	_u_content_size += u_len;
}

bool CCBuffer::isReadable() const
{
	if (_u_content_size == 0) {
		return false;
	}
	return _u_read_pos < _u_content_size;
}

bool CCBuffer::isReadable(unsigned int u_len) const
{
	if (_u_content_size == 0 || u_len == 0) 
	{
		return false;
	}
	return _u_read_pos + u_len <= _u_content_size;
}

unsigned int CCBuffer::length() const
{
	return getContentSize();
}

unsigned int CCBuffer::capacity() const
{
	return getBufferSize();
}

bool CCBuffer::empty() const
{
	return length() == 0;
}

char* CCBuffer::data() const
{
	return _p_buffer;
}

void CCBuffer::clear()
{
	_u_content_size = 0;
	_u_write_pos = 0;
	_u_read_pos = 0;
	_u_mark_pos = 0;
	memset(_p_buffer, 0, _u_buffer_size);
}

void CCBuffer::_reallocBufferSize()
{
	_u_buffer_size = _u_buffer_size * 2;
	_p_buffer = (char*) realloc(_p_buffer, _u_buffer_size);
}

void CCBuffer::_reallocBufferSizeInChanged(unsigned int u_len)
{
	while (true) {
		if (_u_write_pos + u_len <= _u_buffer_size) {
			break;
		} else {
			_reallocBufferSize();
		}
	}
}

void CCBuffer::writeData(const char* p_data, unsigned int u_len)
{
	CCASSERT(p_data && u_len > 0, "p_data && u_len > 0");
	_reallocBufferSizeInChanged(u_len);
	memcpy(_p_buffer + _u_write_pos, p_data, u_len);
	_u_write_pos += u_len;
	MAXA(_u_content_size, _u_write_pos);
}

void CCBuffer::writeLengthAndData(const char* p_data, unsigned int u_len)
{
	writeUInt(u_len);
	writeData(p_data, u_len);
}

void CCBuffer::writeChar(char data)
{
	_reallocBufferSizeInChanged(sizeof(char));
	*(_p_buffer + _u_write_pos) = data;
	++_u_write_pos;
	MAXA(_u_content_size, _u_write_pos);
}

void CCBuffer::writeUChar(unsigned char data)
{
	writeChar((char)data);
}

void CCBuffer::writeShort(short data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(short)] = {0};
	memcpy(p_data, (short*)(&data), sizeof(short));
	std::reverse(&(p_data[0]), &(p_data[sizeof(short)]));
	writeData(p_data, sizeof(short));
#else
	char* p_data = (char*)(&data);
	writeData(p_data, sizeof(short));
#endif
}

void CCBuffer::writeUShort(unsigned short data)
{
	writeShort((short)data);
}

void CCBuffer::writeInt(int data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(int)] = {0};
	memcpy(p_data, (char*)(&data), sizeof(int));
	std::reverse(&(p_data[0]), &(p_data[sizeof(int)]));
	writeData(p_data, sizeof(int));
#else
	char* p_data = (char*)(&data);
	writeData(p_data, sizeof(int));
#endif
}

void CCBuffer::writeUInt(unsigned int data)
{
	writeInt((int)(data));
}

void CCBuffer::writeLongLong(long long data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(long long)] = {0};
	memcpy(p_data, (char*)(&data), sizeof(long long));
	std::reverse(&(p_data[0]), &(p_data[sizeof(long long)]));
	writeData(p_data, sizeof(long long));
#else
	char* p_data = (char*)(&data);
	writeData(p_data, sizeof(long long));
#endif
}

void CCBuffer::writeULongLong(unsigned long long data)
{
	writeLongLong((long long)(data));
}

void CCBuffer::writeFloat(float data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(float)] = {0};
	memcpy(p_data, (char*)(&(data)), sizeof(float));
	std::reverse(&(p_data[0]), &(p_data[sizeof(float)]));
	writeData(p_data, sizeof(float));
#else
	char* p_data = (char*)(&data);
	writeData(p_data, sizeof(float));
#endif
}

void CCBuffer::writeDouble(double data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(double)] = {0};
	memcpy(p_data, (char*)(&data), sizeof(double));
	std::reverse(&(p_data[0]), &(p_data[sizeof(double)]));
	writeData(p_data, sizeof(double));
#else
	char* p_data = (char*)(&data);
	writeData(p_data, sizeof(double));
#endif
}

void CCBuffer::writeString(const char* p_data)
{
	CCASSERT(p_data && strlen(p_data) > 0, "p_data && strlen(p_data) > 0");
	writeData(p_data, strlen(p_data));
}

void CCBuffer::writeLengthAndString(const char* p_data)
{
	CCASSERT(p_data && strlen(p_data) > 0, "p_data && strlen(p_data) > 0");
	writeInt(strlen(p_data));
	writeString(p_data);
}

char* CCBuffer::readWholeData()
{
	if (_u_content_size != 0) {
		char* p_buffer = new char[_u_content_size];
		memcpy(p_buffer, _p_buffer, _u_content_size);
		_u_read_pos = _u_content_size;
		return p_buffer;
	} else {
		return nullptr;
	}
}

CCBuffer* CCBuffer::readData(unsigned int u_len)
{
	if (isReadable(u_len)) {
		char* p_data = (char*)malloc(u_len);
		readData(p_data, u_len);
		CCBuffer* p_ret = new CCBuffer(p_data, u_len);
		p_ret->autorelease();
		CC_SAFE_FREE(p_data);
		return p_ret;
	}else {
		return nullptr;
	}
}

void CCBuffer::readData(char* p_out_data, unsigned int u_len)
{
	if (isReadable(u_len)) {
		memcpy(p_out_data, _p_buffer + _u_read_pos, u_len);
		_u_read_pos += u_len;
	}else if (_u_content_size - _u_read_pos >= 0 && u_len != 0) {
		u_len =  _u_content_size - _u_read_pos;
		memcpy(p_out_data, _p_buffer+_u_read_pos, u_len);
		_u_read_pos += u_len;
	}
}

char CCBuffer::readChar()
{
	if (isReadable(sizeof(char))) {
		char ret = *(_p_buffer + _u_read_pos);
		_u_read_pos += sizeof(char);
		return ret;
	}
	return 0;
}

unsigned char CCBuffer::readUChar()
{
	return (unsigned char)readChar();
}

short CCBuffer::readShort()
{
	if (isReadable(sizeof(short))) {
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(short)] = { 0 };
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(short));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(short)]));
		short ret = *((short*)p_temp);
#else
		short ret = *((short*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(short);
		return ret;
	}
	else {
		return 0;
	}
}

unsigned short CCBuffer::readUShort()
{
	return (unsigned short)readShort();
}

int CCBuffer::readInt()
{
	if (isReadable(sizeof(int))) {
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(int)] = { 0 };
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(int));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(int)]));
		int ret = *((int*)p_temp);
#else
		int ret = *((int*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(int);
		return ret;
	} else {
		return 0;
	}
}

unsigned int CCBuffer::readUInt()
{
	return (unsigned int)readInt();
}

long long CCBuffer::readLongLong()
{
	if (isReadable(sizeof(long long))) {
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(long long)] = { 0 };
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(long long));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(long long)]));
		long long ret = *((long long*)p_temp);
#else
		long long ret = *((long long*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(long long);
		return ret;
	} else {
		return 0;
	}
}

unsigned long long CCBuffer::readULongLong()
{
	return (unsigned long long)readLongLong();
}

float CCBuffer::readFloat()
{
	if (isReadable(sizeof(float))) {
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(float)] = { 0 };
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(float));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(float)]));
		float ret = *((float*)p_temp);
#else
		float ret = *((float*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(float);
		return ret;
	} else {
		return 0;
	}
}

double CCBuffer::readDouble()
{
	if (isReadable(sizeof(double))) {
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(double)] = { 0 };
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(double));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(double)]));
		double ret = *((double*)p_temp);
#else
		double ret = *((double*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(double);
		return ret;
	}
	return 0;
}

std::string CCBuffer::readString(unsigned int u_len)
{
	CCASSERT(u_len > 0, "u_len > 0");
	std::string str;
	char* p_data = (char*)malloc(u_len + 1);
	readData(p_data, u_len);
	p_data[u_len] = 0;
	str.assign(p_data);
	CC_SAFE_FREE(p_data);
	return str;
}

std::string CCBuffer::readLengthAndString()
{
	return readString(readUInt());
}


NS_CC_END