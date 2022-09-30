#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<ctime>
#include <thread>
#include <chrono>

#include<time.h>

using namespace std;

class timer{
    private:
    unsigned long begTime;
    public:
    void start(){
        begTime = clock();
    }
    unsigned long elapsedTime(){
        return((unsigned long) clock() - begTime)/CLOCKS_PER_SEC;
    }
    bool isTimeout(unsigned long seconds){
        return seconds >= elapsedTime();
    }
};

int main()
{
    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    int port = 7980;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    //	While loop:
    char buf[4096];
    bool f = false;
    string userInput;
    char ack='0';
    timer t;
    time_t tim=time(NULL);
    struct tm *tm = localtime(&tim);
    int time_out = 3;
    do {
        //		Enter lines of text
       if(f!=true){
         cout << "> ";
        userInput = "";
        getline(cin, userInput);
       }

        //		Send to server
        // if(ack=='0') ack='1';
        // else ack='1';
        t.start();
        //this_thread::sleep_for(chrono::milliseconds(5000));
        struct tm *tm2 = localtime(&tim);
        //cout << endl << tm2-tm << endl;
        long long r=rand();
        for(long long i=0;i<=r;i++);     
        int sendRes = send(sock, (userInput + ack).c_str(), userInput.size() + 1, 0);
        
        if (sendRes == -1)
        {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }
        
        // if(t.elapsedTime())
        //		Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = 0;
        while(bytesReceived==0){
            f = false;
            bytesReceived = recv(sock, buf, 4096, 0);
            //cout << t.elapsedTime();
            if(t.elapsedTime()>=time_out){
                cout<<"Timed out.\n";
                f = true;
                break;
            }
        }
        if(f){
            continue;
        }
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            //		Display response
           // cout << bytesReceived << "\r\n";
            cout << "SERVER> " << buf << "\r\n";
        }
        ack=buf[strlen(buf)-1];
    } while(true);

    //	Close the socket
    close(sock);

    return 0;
}

