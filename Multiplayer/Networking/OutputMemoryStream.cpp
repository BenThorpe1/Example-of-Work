#include "stdafx.h"


OutputMemoryStream::OutputMemoryStream()
{
	ReallocBuffer(32);
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(m_buffer);
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{

	m_buffer = static_cast<char*>(std::realloc(m_buffer, inNewLength));
	//handle realloc failure
	//...
	m_capacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	//make sure we have space...
	uint32_t resultHead = m_head + static_cast<uint32_t>(inByteCount);
	if (resultHead > m_capacity)
	{
		ReallocBuffer(std::max(m_capacity * 2, resultHead));
	}
	//copy into buffer at head
	std::memcpy(m_buffer + m_head, inData, inByteCount);
	//increment head for next write
	m_head = resultHead;
}

void OutputMemoryStream::Clear()
{
	m_head = 0;

}