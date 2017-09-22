#define _WIN32_WINNT 0x501

#include <iostream>
#include <sstream>
#include <Ws2tcpip.h> //includes winsock header file and other jazz
#pragma comment (lib, "ws2_32.lib");
using namespace std;


int main() {

 //initialize winsock
 WSADATA wsData;
 WORD ver = MAKEWORD(2, 2);

 int wsOK = WSAStartup(ver, &wsData);

 if (wsOK != 0)
 {
     cerr << "can't initialize winsock! Quitting" << endl;
 }

 //create a socket
 SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

 if (listening == INVALID_SOCKET)
 {
     cerr << "Can't create a socket! Quitting" << endl;
 }

 // Bind ip address and port to socket
 sockaddr_in hint;
 hint.sin_family = AF_INET;
 hint.sin_port = htons(54000);
 hint.sin_addr.S_un.S_addr = INADDR_ANY;

 bind(listening, (sockaddr*)&hint, sizeof hint);

 //Tell Winsock the socket is for listening
 listen(listening, SOMAXCONN);

 fd_set master;
 FD_ZERO(&master);

 FD_SET(listening, &master); //adds listening socket to master set of file descriptors

 char host[NI_MAXHOST]; //client's remote name
 char service[NI_MAXHOST]; //Service (i.e port) the client is connected on

  while (true) {

  ZeroMemory(host, NI_MAXHOST);
  ZeroMemory(service, NI_MAXHOST);

  fd_set copy = master;
  int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
   //synchronous queue of client connections

   for (int num = 0; num < socketCount; num++) {

        SOCKET socket = copy.fd_array[num];

        if (socket == listening) {

        //accept a new connection
        SOCKET client = accept(listening, nullptr, nullptr);

        if (getnameinfo((sockaddr*)&hint, sizeof hint, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
         cout << "NEW CONNECTION: " << host << " connected on port " << service << "\r\n";
        }

        //add the new connection to the list of connected clients
        FD_SET(client, &master);

        //send a welcome message to connected client
        string welcomeMsg = "Welcome to the chatRoom \r\n";

        send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

        //TODO: Broadcast new connection
        }

        else {

            char buf[4096];
            ZeroMemory(buf, 4096);

             //accept a new message if one has been sent
            int bytesIn = recv(socket, buf, 4096, 0);

            if (bytesIn <= 0) {

                //drop the client
                closesocket(socket);
                FD_CLR(socket, &master);

                cout << "SOCKET: " << socket << " has disconnected" << endl;
            }

           else {

            for (int i = 0; i < master.fd_count; ++i) {

               SOCKET outSock = master.fd_array[i];

               if (outSock != listening and outSock != socket) {

                ostringstream ss;
                ss << "SOCKET: " << socket << ":" << buf << "\r\n";
                string strOut = ss.str();
                send(outSock, strOut.c_str(), strOut.size() + 1, 0);

               }
            }
            //Send message to other client, and NOT listening socket

            }

        }
   }

 }

 WSACleanup();
 return 0;

}

  /* SINGLE CLIENT ONLY

 cout << "Waiting for connection....." << endl;
 // Wait for a connection
 sockaddr_in client;
 int clientSize = sizeof client;

 SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

 if (clientSocket == INVALID_SOCKET)
 {
     cerr << "ERROR OPENING CLIENTSOCKET" << endl;
 }

 char host[NI_MAXHOST]; //client's remote name
 char service[NI_MAXHOST]; //Service (i.e port) the client is connected on

 ZeroMemory(host, NI_MAXHOST);
 ZeroMemory(service, NI_MAXHOST);

 if (getnameinfo((sockaddr*)&client, sizeof client, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
 {
     cout << host << "connected on port " << service << endl;
 }
 else
{
     //inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
     auto result = inet_ntoa(client.sin_addr);
     //cout << "someone connected on " << result << endl;
     cout << host << " connected on port " << ntohs(client.sin_port)
     << endl;
}

 // Close listening socket
 closesocket(listening);

 // While loop: accept and echo message to client
 char buf[4096];

 for (;;) {

    ZeroMemory(buf, 4096);

    // Wait for client to send data
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    cout << "client says: " << buf << endl;

    if (bytesReceived == SOCKET_ERROR)
    {
        cerr << "Error in recv(). Quitting" << endl;
        break;
    }

    if (bytesReceived == 0) {
        cout << "Client disconnected" << endl;
        break;
    }

    // Echo message back to client
    send(clientSocket, buf, 4096 + 1, 0);

 }

    // Close the socket
    closesocket(clientSocket);
    WSACleanup();
    return 0
*/
    //clean up WinSock



