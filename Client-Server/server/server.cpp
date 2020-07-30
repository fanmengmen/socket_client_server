#include"EasyTcpServer.hpp"
#include<thread>



int main()
{
    EasyTcpServer server;
    server.initSocket();
    server.Bind(NULL, 3457);
    server.Listen(10);

    while(server.isRun())
    {
        server.OnRun();
    }
    printf("exit...\n");

    return 0;
    
}