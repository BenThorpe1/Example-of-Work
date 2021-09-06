#pragma once
#include "stdafx.h"

class Message;
class Scene;
class OutputMemoryStream;

class cServer
{
public:
	cServer();
	~cServer();

	bool Init(Scene* a_scene);
	void Quit();
	void CloseThread();
	int StartServerListening(unsigned short a_port);

	static void EndServer(int a_socket);
	static void Pack(OutputMemoryStream& a_outMsg, unsigned int a_csock, int a_i);
	static void UnpackMessage(char* a_inMsg,int a_size,unsigned int a_csock);
	static void AcceptThread(int* a_serverSocket, int* a_clientSocket);
	static void MessageFeeder(int* a_serverSocket);
	static void RemoveSocket(int a_clientSocket);

	static void SetMessage(Message* a_message);

	int GetCSocket()
	{
		return m_clientSocket;
	}
	int GetSSocket()
	{
		return m_serverSocket;
	}

	void SetClientSocket(int socket)
	{
		m_clientSocket = socket;
	}
private:
	int		m_serverSocket;
	int		m_clientSocket;
	static int m_id[MAX_PLAYERS];
};