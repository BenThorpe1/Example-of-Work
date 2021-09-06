#include "stdafx.h"

#define _CRT_SECURE_NO_DEPRECATE
#define RCVBUFF_SIZE 32
#define MESSAGE_COUNT 1

extern cNetworkManager networkManager;

const char* severAdress = "localHost";
struct addrinfo hints, *res;

static Message outMessage;
static Scene* sceneManager;
//////////////////////////////////////////////////////////////////////////////////////////////
// constructor() - 
//////////////////////////////////////////////////////////////////////////////////////////////
cClient::cClient()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// destructor() - 
//////////////////////////////////////////////////////////////////////////////////////////////
cClient::~cClient()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// init() - 
//////////////////////////////////////////////////////////////////////////////////////////////
bool cClient::Init(const char* a_ipAddress,Scene* a_scene)
{
    if (a_ipAddress != nullptr)
    {
        severAdress = a_ipAddress;
    }
    else
    {
        severAdress = "localHost";
    }

    sceneManager = a_scene;

    WSAData wsaData;

    printf("CLIENT:\n");

    int error = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (error == SOCKET_ERROR)
    {
        printf("CLIENT: WinSock startup failed\n");
        return 1;
    }

    printf("CLIENT: WinSocket Loaded.\n");

    int mySocket = StartClient(7700, severAdress);

    if (mySocket == SOCKET_ERROR)
    {
        EndClient(mySocket);
        return 1;
    }

    m_clientSocket = mySocket;

    // Create the thread
    networkManager.m_thread_handle[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageLoop, &m_clientSocket, 0, (LPDWORD)&networkManager.m_thread_id[0]);

    if (networkManager.m_thread_handle == NULL)
    {
        printf("CLIENT: Unable to create a new thread!\n");
        EndClient(m_clientSocket);
        return 1;
    }
    Sleep(100);// Let the AcceptThread start up, cheap method

    ////Create first message
    //MessageString stringe;
    //stringe.SetText("I am new  \n");
    //m_message[0] = stringe;
    //printf("c18 \n");
    //printf("%s c19 \n", stringe.GetText());
    //printf("%i c83 \n", (int)m_message->GetType());
    //printf("%i c88 \n", stringe.Size());
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// EndClient() - 
//////////////////////////////////////////////////////////////////////////////////////////////
void cClient::EndClient(int a_clientSocket)
{
    sceneManager->Disconnection();
    closesocket(a_clientSocket);

    WSACleanup();

    printf("CLIENT: Shutting down\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////
// StartClient() - 
//////////////////////////////////////////////////////////////////////////////////////////////
int cClient::StartClient(unsigned short a_port, const char* a_serverName)
{
    int error;

    WSAData wsaData;
    ZeroMemory(&wsaData, sizeof(wsaData));

    if ((error = WSAStartup(MAKEWORD(2, 0), &wsaData)) == SOCKET_ERROR)
    {
        printf("CLIENT: Failed to start client\n");
        return -1;
    }

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == SOCKET_ERROR)
    {
        printf("CLIENT: Error\n");
        return -1;
    }

    struct hostent* host_entry;

    if ((host_entry = gethostbyname(a_serverName)) == NULL)
    {
        printf("CLIENT: Who shall I connect to now?\n");     
        printf(a_serverName);
    }

    
    struct sockaddr_in server;
    memset(server.sin_zero, 0, sizeof(server.sin_zero));
    server.sin_family = AF_INET;
    server.sin_port = htons(a_port);

    // TO DO> Replace the following line with the actual server's IP address - in dotted quad form - i.e. 255.46.1.128
    // This value will need to be passed through by the client when executing the program
   
    if (severAdress != "localHost")
    {
    server.sin_addr.s_addr = inet_addr(severAdress);
    }
    else
    {
        server.sin_addr.s_addr = *(unsigned long*)host_entry->h_addr;
    }
 //   char servIP[] = "255.128.12.1"; // example dotted quad
 //   server.sin_addr.s_addr = inet_addr(servIP);


    if (connect(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("CLIENT: Failed to connect to server\n");
        return -1;
    }
    else
    {
        m_clientSocket = mySocket;
        printf("CLIENT: OK\n");
        sceneManager->AddConnection();
    }

    return mySocket;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// unpack() - unpack data packet
//////////////////////////////////////////////////////////////////////////////////////////////
void cClient::Unpack(char* a_inMsg, int a_size)
{
    Message message;
    InputMemoryStream stream(a_inMsg,static_cast<uint32_t> (a_size));
    message.Read(stream);

    //if (message.GetType() != Message::MessageType::EMPTY)
    //{
    //    printf("%i Client 181 received \n", message.GetType());

    //}

   sceneManager->RecieveData(&message,-1);
   
   
   // UpdateData data = a_msg->GetData();
     //Message* message = reinterpret_cast<Message*> (a_msg);
    //if (message->GetType() ==Message::MessageType::STRING)
    //{
    //    MessageString* string = static_cast<MessageString*>(message);
    //    // unpack the incoming message here...
    //    printf("%s \n", string->GetText());
    //}
}
//////////////////////////////////////////////////////////////////////////////////////////////
// pack() - creates a data packet
//////////////////////////////////////////////////////////////////////////////////////////////
void cClient::Pack(OutputMemoryStream& a_outMsg)
{
    outMessage.Write(a_outMsg);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// pack() - creates a data packet
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// MessageLoop() - message loop
//////////////////////////////////////////////////////////////////////////////////////////////
void cClient::MessageLoop(int* a_mysocket)
{
    int nBytes = 0;

    ULONGLONG timeLast = GetTickCount64();
  
    char* tempBuffer;
    const uint32_t kMaxPacketSize = 1470;
    tempBuffer = static_cast<char*>(std::malloc(kMaxPacketSize));
     OutputMemoryStream outStream;

    while (1)
    {
        ULONGLONG timeLapsed = GetTickCount64() - timeLast;

        if (timeLapsed > 16)
        {

            Pack(outStream);
            // send string to the server
            nBytes = send(*a_mysocket,outStream.GetBuffer(),outStream.GetLength(), 0);

            outMessage.SetType(Message::MessageType::EMPTY);

            if (nBytes == SOCKET_ERROR)
            {
                printf("CLIENT: Failed to send message\n");
            }

            if (nBytes == SOCKET_ERROR)
            {
                printf("CLIENT: Failed to send message\n");
            }
         
          
            // receive string from the server
            nBytes = recv(*a_mysocket,(tempBuffer), kMaxPacketSize, 0);
            Unpack(tempBuffer, nBytes);

            timeLast = GetTickCount64();   
            
            outStream.Clear();

        }
    }
    std::free(tempBuffer);


}


void cClient::SetMessage(Message* a_message)
{
    outMessage = *a_message;
}
