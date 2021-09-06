#pragma once

class OutputMemoryStream
{
public:
	OutputMemoryStream();
	~OutputMemoryStream();

	const char* GetBuffer() { return m_buffer; }
	uint32_t GetLength() { return m_head; }

	void Write(const void* inData, size_t inByteCount);
	void Clear();
private:
	void ReallocBuffer(uint32_t a_length);

	char* m_buffer;
	uint32_t m_head;
	uint32_t m_capacity;
};