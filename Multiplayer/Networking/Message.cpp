#include "stdafx.h"

Message::Message()
{
	m_type = Message::MessageType::EMPTY;
	 const char temp[] = ".";
	int messageLen = strlen(temp) + 1;
	strcpy_s(m_string, messageLen, temp);
	UpdateData dateTemp;
	m_update = dateTemp;
}

void Message::SetString(const char* a_value)
{
	int messageLen = strlen(a_value) + 1;
	strcpy_s(m_string, messageLen, a_value);
}

void Message::Write(OutputMemoryStream& inStream)
{
	inStream.Write(&m_type, sizeof(MessageType));
	inStream.Write(m_string, MAX_STRING);
	inStream.Write(&m_update.ballXPos, sizeof(float));
	inStream.Write(&m_update.ballYPos, sizeof(float));
	inStream.Write(&m_update.playerPos, sizeof(float[MAX_PLAYERS]));
	inStream.Write(&m_update.score, sizeof(int[2]));

}
void Message::Read(InputMemoryStream& inStream)
{
	inStream.Read(&m_type, sizeof(MessageType));
	inStream.Read(m_string, MAX_STRING);
	inStream.Read(&m_update.ballXPos, sizeof(float));
	inStream.Read(&m_update.ballYPos, sizeof(float));
	inStream.Read(&m_update.playerPos, sizeof(float[MAX_PLAYERS]));
	inStream.Read(&m_update.score, sizeof(int[2]));

}	


void Message::SetData(UpdateData a_data)
{
	m_update.ballXPos = a_data.ballXPos;
	m_update.ballYPos = a_data.ballYPos;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_update.playerPos[i] = a_data.playerPos[i];
	}
	m_update.score[0] = a_data.score[0];
	m_update.score[1] = a_data.score[1];
}

