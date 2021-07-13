// TCP_SERVER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib,"ws2_32.lib")
using namespace std;

int main()
{
    //initalize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cout << "Cant initlize winsock" << endl;
    }
    else
    {
        cout << "WinSock Initilized" << endl;
    }

    //create a socket 
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cout << "cant create a socket" << endl;
    }
    else
    {
        cout << "created a socket" << endl;
    }

    //bind the ip address and port to the socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //tell winsock the socket is for listening
    listen(listening, SOMAXCONN);

    //wait for a connection
    sockaddr_in client;
    int clientsize = sizeof(client);
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if(getnameinfo((sockaddr*)&client ,sizeof(client) , host ,NI_MAXHOST , service , NI_MAXSERV,0))
    {
        cout << host << "connected on port" << service << endl;

    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on port : " << ntohs(client.sin_port) << endl;

    }
    //close listening socket
    closesocket(listening);

    //while loop : accept and echo massage back to client
    char buf[4096];
    while (true)
    {
        ZeroMemory(buf, 4096);
        int bytesRecived = recv(clientSocket, buf, 4096, 0);
        if(bytesRecived == SOCKET_ERROR)
        {
            cout << "ERROR in recive" << endl;
        }
        else if (bytesRecived == 0)
        {
            cout << "client disconnected" << endl;
        }
        cout << string(buf, 0, bytesRecived) << endl;

        //echo massage back to client
        send(clientSocket, buf, bytesRecived + 1, 0);
    }
    //close the socket
    closesocket(clientSocket);

    //cleanup winsock
    WSACleanup();

    return 0;

}
