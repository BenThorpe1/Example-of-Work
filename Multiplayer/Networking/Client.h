#pragma once
#include "stdafx.h"
class Message;
class Scene;
class OutputMemoryStream;

class cClient
{
public:
	cClient();
	~cClient();
	
	bool Init(const char* a_ipAddress,Scene* a_scene);
	int StartClient(unsigned short a_port, const char* a_serverName);
	static void EndClient(int a_clientSocket);
	static void Unpack(char* a_inMsg, int a_size);
	static void Pack(OutputMemoryStream& a_outMsg);
	static void MessageLoop(int* a_mysocket);
	static void SetMessage(Message* a_message);

private:
	int			m_serverSocket;
	int			m_clientSocket;
};