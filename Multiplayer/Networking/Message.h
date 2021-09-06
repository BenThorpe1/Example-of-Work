#pragma once
#include "stdafx.h"


#define MAX_STRING 10
class InputMemoryStream;
class OutputMemoryStream;

struct UpdateData
{
	float playerPos[MAX_PLAYERS];
	float ballXPos = 0;
	float ballYPos = 0;
	int score[2];
	UpdateData(float* a_pos, float ballX, float ballY,int a_score[2])
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			playerPos[i] = a_pos[i];
		}

		ballXPos = ballX;
		ballYPos = ballY;
		score[0] = a_score[0];
		score[1] = a_score[1];
	}
	UpdateData()
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			playerPos[i] = 0;
		}

		ballXPos = 0;
		ballYPos = 0; 
		score[0] =0;
		score[1] =0;
	}	

};

class Message
{
public:
	enum class MessageType
	{
		EMPTY = 0,
		START = 1,
		Upadate = 2,
	};
	Message();

	MessageType GetType() { return m_type; }
	void SetType(MessageType a_v) { m_type = a_v; }

	void SetString(const char* a_value);
	const char* GetString() { return m_string; }

	void SetData(UpdateData a_data);
	UpdateData GetData() { return m_update; }

	void Write(OutputMemoryStream& inStream);
	void Read(InputMemoryStream& inStream);

private:
	MessageType m_type;
	char m_string[MAX_STRING];
	UpdateData m_update;
};