#pragma once
class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inLength);
	~InputMemoryStream();

	uint32_t GetRemainingDataSize();

	void Read(void* outData, uint32_t length);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }


private:
	char* m_buffer;
	uint32_t m_head;
	uint32_t m_capacity;
};