#include "stdafx.h"

#define MAX_MESSAGE_SIZE 4096
#define MAX_PLAYER_COUNT 10
extern cNetworkManager networkManager;

static Message messageBuffer;
static Scene* sceneManger;


cServer::cServer()
{
   	m_serverSocket = -1;
	m_clientSocket = -1;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_id[i] = -1;
        printf("%i \n",m_id[i]);
    }
}

cServer::~cServer()
{
    // close all threads before exiting..
    Quit();
}

int cServer::m_id[] = { -1,-1,-1,-1 };

bool cServer::Init(Scene* a_scene)
{
    sceneManger = a_scene;
    int error;
    WSAData wsaData;

    error = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (error == SOCKET_ERROR)
    {
        printf("SERVER: Winsock Failed to start up!\n");
        return false;
    }

    printf("SERVER: WinSocket Started Correctly!\n");

    m_serverSocket = StartServerListening(7700); // Calls our function StartServerListen(); and passes the port 7700

    if (m_serverSocket == SOCKET_ERROR)
    {
        printf("SERVER: Found nothing\n");
        return false;
    }
    else
    {
        networkManager.m_serverSocket = m_serverSocket;
    }

    networkManager.m_mutex = CreateMutex(NULL, false, NULL); // create a mutex

    if (networkManager.m_mutex == NULL)
    {
        printf("SERVER: Could not create our Mutex\n");
        EndServer(m_serverSocket);
        return false;
    }

    // Create the thread
    networkManager.m_thread_handle[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AcceptThread, &m_serverSocket, 0, (LPDWORD)&networkManager.m_thread_id[0]);
    
    if (networkManager.m_thread_handle[0] == NULL)
    {
        printf("SERVER: Unable to create a new thread!\n");
        EndServer(m_serverSocket);
        return 1;
    }
    Sleep(100);// Let the AcceptThread start up, cheap method



    // Create the thread
    networkManager.m_thread_handle[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageFeeder, &m_serverSocket, 0, (LPDWORD)&networkManager.m_thread_id[1]);

    if (networkManager.m_thread_handle == NULL)
    {
        printf("SERVER: Unable to create a new thread!\n");
        EndServer(m_serverSocket);
        return 1;
    }
    Sleep(100);// Sleep time to let the AcceptThread start up, cheap method

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// AcceptThread() - checks for new sockets (clients)
//////////////////////////////////////////////////////////////////////////////////////////
void cServer::AcceptThread(int* a_serverSocket, int* pClientSocket)
{
    int mySocket = *a_serverSocket;    // Copy my socket over to a local variable

    for (;;)// Loop forever
    {
        unsigned int clientSocket = accept(mySocket, 0, 0);    // Do a normal accept

        if (clientSocket == SOCKET_ERROR)
        {
            printf("SERVER: Connection Denied!\n");
            return;
        }
        else
        {
            WaitForSingleObject(networkManager.m_mutex, INFINITE);// Lock the mutex and wait for it
            FD_SET(clientSocket, &networkManager.m_masterSet);// Add this socket to the master set using FD_SET()
            ReleaseMutex(networkManager.m_mutex);// Unlock the mutex

            printf("SERVER: Client on %d connected\n", clientSocket);
            sceneManger->AddConnection();
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                printf("Sever added player id %i \n", m_id[i]);
                if (m_id[i] == -1)
                {
                    printf("Sever added player 00 id %i \n", i);

                    m_id[i] = clientSocket;
                    break;
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
// closeThread() - close thread
//////////////////////////////////////////////////////////////////////////////////////////
void cServer::CloseThread()
{
    CloseHandle(networkManager.m_thread_handle);
    sceneManger->Disconnection();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// unpack() - unpacks data and converts from ascii to integer
//////////////////////////////////////////////////////////////////////////////////////////////
void cServer::UnpackMessage(char* a_inMsg,int a_size, unsigned int a_csock)
{
    Message message;
    InputMemoryStream stream(a_inMsg, static_cast<uint32_t> (a_size));
    message.Read(stream);

    int i = -1;
    for (float f = 0; f < MAX_PLAYERS; f++)
    {
        if (m_id[(int)f] == a_csock)
        {
            i = (int)f;
            break;
        }
    }


  //  Message* message = reinterpret_cast<char*>(a_inMsg);
  sceneManger->RecieveData(&message,i);
    
    // Message* message = reinterpret_cast<Message*> (a_inMsg);
   // if (message->GetType() == Message::MessageType::STRING)
    //{reinterpret_cast<char*>
    //    MessageString* string = static_cast<MessageString*>(message);
    //    // unpack the incoming message here...
    //}
    //else
    //{
    //    // unpack the incoming message here...
    //}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// pack() - creates a data packet
//////////////////////////////////////////////////////////////////////////////////////////////
void cServer::Pack(OutputMemoryStream& a_outMsg,unsigned int a_csock, int a_i)
{
    if (messageBuffer.GetType() == Message::MessageType::START)
    {
        for (float i = 0; i < MAX_PLAYERS; i++)
        {
            if (m_id[(int)i] == a_csock)
            {
                UpdateData data = messageBuffer.GetData();
              data.playerPos[1] = i;
              messageBuffer.SetData(data);
               printf("Sever 1803  send start %f \n", messageBuffer.GetData().playerPos[1]);
                break;
            }
        }
    }

    messageBuffer.Write(a_outMsg);    
    /*const char* charMess = reinterpret_cast<const char*>(&a_inMsg);
    int messageLen = strlen(charMess) + 1;
    strcpy_s(a_outMsg, messageLen, charMess);*/
    // // get the socket from the network manager's m_masterSet 

    char id[32];
    _itoa_s(a_csock, id, 32, 10);
    int sz = strlen(id) + 1;
  //  strcat_s(a_outMsg, strlen(&a_outMsg) + sz, id);
    a_outMsg.Write(id, sz);

    //    default:
//    {
//         char message[] = "s Empty Message \n";
//        int messageLen = strlen(message) + 1;
//        strcpy_s(a_outMsg, messageLen, message);
//        break;
//    }
//}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// StartServerListening() - starts up winsock and opens up a socket for listening
//////////////////////////////////////////////////////////////////////////////////////////////
int cServer::StartServerListening(unsigned short a_port)
{
    WSAData wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
    {
       printf("SERVER: Could Not Start Up Winsock!\n");
        return -1;
    }

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == SOCKET_ERROR)
    {
        printf("SERVER: Socket is stuck again, wont open...\n");
        return -1;
    }

    SOCKADDR_IN serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(a_port);

    if (bind(mySocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("SERVER: Can't Bind the Socket\n");
        closesocket(mySocket);
        return -1;
    }

    if (listen(mySocket, 5) == SOCKET_ERROR)
    {
        printf("SERVER: No listening socket open!\n");
        closesocket(mySocket);
        return -1;
    }

    return mySocket;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// EndServer() - closes down a socket and cleans up winsock
//////////////////////////////////////////////////////////////////////////////////////////////
void cServer::EndServer(int a_socket)
{
    WaitForSingleObject(networkManager.m_thread_handle, INFINITE);// Kill thread and handle
    CloseHandle(networkManager.m_thread_handle[0]);
    CloseHandle(networkManager.m_thread_handle[1]);
    CloseHandle(networkManager.m_mutex);

    // Close our socket
    closesocket(a_socket);

    // Shut down winsock
    WSACleanup();

    printf("SERVER: Shutting down\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////
// quit() - close sockets and end server..
//////////////////////////////////////////////////////////////////////////////////////////////
void cServer::Quit()
{
    closesocket(m_clientSocket);
    EndServer(m_serverSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////
// removeSocket() - remove client socket from m_masterSet
//////////////////////////////////////////////////////////////////////////////////////////
void cServer::RemoveSocket(int a_clientSocket)
{
    WaitForSingleObject(networkManager.m_mutex, INFINITE);		    // Lock our mutex
    FD_CLR(a_clientSocket, &networkManager.m_masterSet);			    // Remove clientSocket from our master set
    ReleaseMutex(networkManager.m_mutex);						    // Unlock our mutex
    closesocket(a_clientSocket);					                    // Close the socket on our side, so our computer cleans up properly
    networkManager.m_server.SetClientSocket(0);
    printf("\nSERVER: Client on %d has disconnected\n", a_clientSocket);// Quick Message to the rest in the room
    sceneManger->Disconnection();

}

#define RCVBUFF_SIZE 32

//////////////////////////////////////////////////////////////////////////////////////////
// MessageFeeder() - sends messages to each sockets on request
//////////////////////////////////////////////////////////////////////////////////////////
void cServer::MessageFeeder(int* a_serverSocket)
{
    FD_ZERO(&networkManager.m_masterSet); // zero set before use

    //Create temp variables
    char* tempBuffer;
    OutputMemoryStream stream;
    const uint32_t kMaxPacketSize = 1470;
    tempBuffer = static_cast<char*>(std::malloc(kMaxPacketSize));

    for (;;) // Forever!!!!
    {
        WaitForSingleObject(networkManager.m_mutex, INFINITE);        // Lock the mutex

        FD_SET TempSet = networkManager.m_masterSet;	// Make a temp location so we can unlock our mutex and let our accept thread run ASAP
        ReleaseMutex(networkManager.m_mutex);			// Unlock the mutex

        if (TempSet.fd_count == 0)		// If a set is empty and we call for it, it returns a error, thats too much coding for me
        {								// So we will simply check if there is anything in it, if so we will do something, else screw it -.-;
            continue;
        }

        

        timeval waitTime; // Set up our interval
        waitTime.tv_sec = 0; // Set it to 0
        waitTime.tv_usec = 0; // Set it to 0

        // Select a socket in out TempSet
        int result = select(TempSet.fd_count, &TempSet, NULL, NULL, &waitTime);

        if (result == 0)
        {
            continue; // Empty socket
        }

        if (result == SOCKET_ERROR)
        {
            printf("SERVER: Error in the Select() macro!\n");
            continue;
        }

        // loops through all sockets (clients) and check for new messages..
        for (unsigned int i = 0; i < TempSet.fd_count; i++)
        {
            unsigned int clientSocket = TempSet.fd_array[i];
            int nBytes = 0;

            networkManager.m_server.SetClientSocket(clientSocket);

            // receive string from the server
            nBytes = recv(clientSocket, 
               (tempBuffer), kMaxPacketSize, 0);
            UnpackMessage(tempBuffer,nBytes, clientSocket);

            if (nBytes == SOCKET_ERROR)
            {
                RemoveSocket(clientSocket);
                continue;// On to the next one!
            }

            // Called if the user exits and calls closesocket() on THEIR side. They stop sending data, thus 0 bytes.... remove them from the set
            if (nBytes == 0)
            {
                RemoveSocket(clientSocket);
                continue;// On to the next one!
            }

         Pack(stream,clientSocket, i);
          nBytes = send(clientSocket,stream.GetBuffer(),stream.GetLength(), 0);

            
          stream.Clear();
        messageBuffer.SetType(Message::MessageType::EMPTY);

        }
    }
    std::free(tempBuffer);


    EndServer(*a_serverSocket);
    printf("SERVER: Press any key to shut me down....\n");
}
//
//void cServer::SendString(const char* a_message, int a_clientSocket)
//{
//
//}



void cServer::SetMessage(Message* a_message)
{
    if (messageBuffer.GetType() != Message::MessageType::EMPTY)
    {
        return;
    }
    
    messageBuffer = *a_message;
}
