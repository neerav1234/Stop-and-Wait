#include <iostream>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <time.h>
using namespace std;

int main()
{
    std::cout << "Creating server socket..." << std::endl;
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Can't create a socket!";
        return -1;
    }

    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(7980);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    std::cout << "Binding socket to sockaddr..." << std::endl;
    if (bind(listening, (struct sockaddr *)&hint, sizeof(hint)) == -1) 
    {
        std::cerr << "Can't bind to IP/port\n";
        return -2;
    }

    std::cout << "Mark the socket for listening..." << std::endl;
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen !";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    std::cout << "Accept client call..." << std::endl;
    int clientSocket = accept(listening, (struct sockaddr *)&client, &clientSize);

    std::cout << "Received call..." << std::endl;
    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting!";
        return -4;
    }
 
    std::cout << "Client address: " << inet_ntoa(client.sin_addr) << " and port: " << client.sin_port << std::endl;

    close(listening);

    char buf[4096];
    int flag;
    while (true) {
        // clear buffer
        memset(buf, 0, 4096);

        // wait for a message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            std::cerr << "There was a connection issue." << std::endl;
        }
        if (bytesRecv == 0)
        {
            std::cout << "The client disconnected" << std::endl;
        }
        
        // display message

        //sleep(10);
        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << "\r\n";
        //for(long long i=0;i<=1e10;i++);

        // return message
        if(buf[strlen(buf)-1]=='0') flag=1;
        else flag=0;
        
        send(clientSocket, ("ack " + to_string(flag)).c_str(), bytesRecv+1, 0);
    }
    // close socket
    close(clientSocket);

    return 0;

}
