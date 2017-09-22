#define WINVER 0x600

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib");

using namespace std;

void recv_message(const SOCKET&);
std::mutex m1;
std::mutex m2;

int main() {

string ipAddress = "127.0.0.1"; //ip address of the server
int port = 54000; //Listening port number on server

// Initialize Winsock

WSAData data;
WORD ver = MAKEWORD(2,2);

int wsResult = WSAStartup(ver, &data);

if (wsResult != 0)
{
    cerr << "Can't start winsock, Err #" << wsResult << endl;
}

//Create socket

SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

//run separate thread for receiving message!
std::thread recv_thread(recv_message, std::ref(sock));
recv_thread.detach(); //this thread will run freely on its own

if (sock == INVALID_SOCKET)
{
    cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
}

// Fill in a hint structure
sockaddr_in hint;

hint.sin_family = AF_INET;
hint.sin_port = htons(port);
hint.sin_addr.s_addr = inet_addr("127.0.0.1");

//inet_pton mysteriously does not work for some reason?

//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

// connect to server
int connResult = connect(sock, (sockaddr*) &hint, sizeof(hint));

if(connResult == SOCKET_ERROR)
{
    cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
    closesocket(sock);
    WSACleanup();
}

//Do-while loop to send and recieve data
char buf[4096];

int bytesIn = recv(sock, buf, 4096, 0);

//welcome message to client
if (bytesIn > 0) {
    cout << buf << endl;

}

/*
string userName = "";
/*
while(userName == "") {

cout << "Enter your username: " << endl;
getline(cin, userName);
}

string header = userName + " says: ";
*/
string userInput;


do {
    //prompt user for some text
    //std::lock_guard<std::mutex> guard1(m1);
    //guard
    cout << "> ";

    getline(cin, userInput);

    //header += userInput;

    if (userInput.size() > 0) {
    //send text
    int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);


    cin.clear();
    }

} while (userInput.size() > 0);
// Gracefully close down everything

closesocket(sock);
WSACleanup();
}

void recv_message(const SOCKET& s) {

 char buf[4096];
 ZeroMemory(buf, 4096);

 for (;;) {

 int bytesReceived = recv(s, buf, 4096, 0);

  if (bytesReceived > 0)
  {
    cout << buf << endl;
    ZeroMemory(buf, 4096);
  }

 }

}
