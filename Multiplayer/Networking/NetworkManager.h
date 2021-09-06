#pragma once
#include "stdafx.h"

#define INTEGER_SIZE	8
#define PACKET_COUNT	6
#define MESSAGE_SIZE	PACKET_COUNT * INTEGER_SIZE

class Scene;

class cNetworkManager
{
public:
	cNetworkManager();
	~cNetworkManager();

	void Add(bool a_startServer, const char *a_ipAddress,Scene* a_scene);	// adds a server or client to the network
	void Update();
	void Deinit();
	
	const char* m_serverName = "serverName";
	bool		m_accepted[32];
	int			m_client_id;
	DWORD		m_thread_id[2];
	HANDLE		m_thread_handle[2];
	HANDLE		m_mutex;			// lockable memory spot - prevents overwriting during a read
	FD_SET		m_masterSet;		// list of sockets
	cServer		m_server;
	cClient		m_client;
	bool		m_serverExists;
	SOCKET		m_serverSocket;
protected:
	int			m_smarties; // keeps count of the number of smarties in each players pocket

	
};

