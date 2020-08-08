#include"EasyTcpServer.hpp"
#include<thread>



int main()
{
    
    pid_t pid1 = fork();
    if(pid1 < 0) 
    {
        printf("fork process failed...\n");
        return 0;
    }
    if(pid1 == 0)
    {
        EasyTcpServer server;
        server.initSocket();
        server.Bind(NULL, 3456);
        server.Listen(10);   
        printf("3456\n");   

        while(server.isRun())
        {
            server.OnRun();
        }
        printf("exit...\n");
    }
    else
    {
        pid_t pid2 = fork();

        if(pid2 < 0) 
        {
            printf("fork process failed...\n");
            return 0;
        }
        if(pid2 == 0)
        {
            EasyTcpServer server;
            server.initSocket();
            server.Bind(NULL, 3457);
            server.Listen(10);   
            printf("3457\n");   
            while(server.isRun())
            {
                server.OnRun();
            }
            printf("exit...\n");
        }
        else
        {
            EasyTcpServer server;
            server.initSocket();
            server.Bind(NULL, 3458);
            server.Listen(10);
            printf("3458\n");
            while(server.isRun())
            {
                server.OnRun();
            }
            printf("exit...\n");
        }

    }

    

    
    

    

    return 0;
    
}