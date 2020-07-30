#include"EasyTcpClient.hpp"

int main()
{
    EasyTcpClient client;
    client.Connect("127.0.0.1", 3457);

    EasyTcpClient client1;
    client1.Connect("127.0.0.1", 4567);

    EasyTcpClient client2;
    client2.Connect("127.0.0.1", 4567);


    Login login;
    char name[] = {"guofan"};
    char password[] = {"123"};
    memcpy(&(login.userName), name, sizeof(name));
    memcpy(&(login.password), password, sizeof(password));

   
    while(client.isRun())
    {
        client.SendData(&login);
        // client2.SendData(&login);
        // client1.SendData(&login);
        sleep(1);
        client.OnRun();
        // client2.OnRun();
        // client1.OnRun();
        sleep(1);
        
    }
    return 0;
}