#include "stdafx.h"
InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inLength)
{
	m_capacity =inLength;
	m_head =0;
	m_buffer = inBuffer;

}

InputMemoryStream::~InputMemoryStream()
{
	//std::free(m_buffer);
}
uint32_t InputMemoryStream::GetRemainingDataSize()
{
	return m_capacity - m_head;
}

void InputMemoryStream::Read(void* outData, uint32_t length)
{
	uint32_t resultHead = m_head + length;
	if (resultHead > m_capacity)
	{
		//handle error, no data to read!
		//...
	}

	std::memcpy(outData, m_buffer + m_head, length);

	m_head = resultHead;
}
