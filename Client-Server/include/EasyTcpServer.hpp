#ifndef _EasyTcpServer_hpp_
#define _EasyTcpServer_hpp_


#include<iostream>
#include"MessageHeader.hpp"
#include<vector>

#ifdef _WIN32 

#else
    #include<unistd.h>
    #include<arpa/inet.h>
    #include<string.h>

    #define SOCKET int
    #define INVALID_SOCKET (SOCKET) (0)
    #define SOCKET_ERROR (-1)
#endif 

class EasyTcpServer
{
    public:
        EasyTcpServer()
        {

        }

        virtual ~EasyTcpServer()
        {
            Close();
        }

        // 初始化socket
        int initSocket()
        {
            if(INVALID_SOCKET != _sock)
            {
                printf("close last connection\n");
                Close();
            }
            // 启动
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if(INVALID_SOCKET == _sock)
            {
                printf("error, creat socket failed!\n");
            }
            else
            {
                // printf("creat socket sucessfully!\n");  
            }
             
            return _sock;
        }
        // 绑定IP和端口号
        int Bind(const char* ip, unsigned short port)
        {
            sockaddr_in _sin = {};
            _sin.sin_family = AF_INET;
            _sin.sin_port = htons(port);
            
            if(ip)
            {
                _sin.sin_addr.s_addr = inet_addr(ip);
            }
            else
            {
                _sin.sin_addr.s_addr = INADDR_ANY;
            }
            
            if(SOCKET_ERROR == bind(_sock, reinterpret_cast<sockaddr*>(&_sin), sizeof(_sin)))
            {
                printf("error, bind internet port failed...\n");
            }    
            else
            {
                printf("bind internet port succesfully...\n");
                this->port = port;
            }
            
        }
        // 监听端口号, 支持n个连接
        int Listen(int n)
        {
            int ret = listen(_sock, n);
            if(SOCKET_ERROR == ret)
            {
                printf("listen port failed...\n");
            }
            else
            {
                printf("listen port successfully...\n");
            }
            return ret;
        }
        // 接受客户端连接
        SOCKET Accept()
        {
            sockaddr_in clientAddr = {};
            int nAddrLen = sizeof(sockaddr_in);
            SOCKET _csock = INVALID_SOCKET;
            _csock = accept(_sock, reinterpret_cast<sockaddr*>(&clientAddr), reinterpret_cast<socklen_t*>(&nAddrLen));

            if(INVALID_SOCKET == _csock)
            {
                printf("socket = <%d> error, received invalid client socket...\n", _sock);
            }
            else
            {
                for(int n = (int)g_clients.size() - 1; n >= 0; n--)
                {
                    NewUserJoin userJoin;
                    memcpy(userJoin.userName, inet_ntoa(clientAddr.sin_addr), sizeof(clientAddr.sin_addr));
                    SendData(_csock, &userJoin);
                }
                g_clients.push_back(_csock);
                printf("port = <%d> new client add in : socket = %d, IP = %s \n", port, (int)_sock, inet_ntoa(clientAddr.sin_addr));
                
            }

            return _csock;
            
        }

        // 关闭
        void Close()
        {
            close(_sock); 
            _sock = INVALID_SOCKET;
        }
        // 处理网络消息
        bool OnRun()
        {
            if(isRun())
            {
                // BSD socket
                fd_set fdReads;//描述符集合
                fd_set fdWrites;
                fd_set fdExp;

                // 清理集合
                FD_ZERO(&fdReads);
                FD_ZERO(&fdWrites);
                FD_ZERO(&fdExp);

                // 将描述符(socket)加入集合
                FD_SET(_sock, &fdWrites);
                FD_SET(_sock, &fdReads);
                FD_SET(_sock, &fdExp);

                SOCKET maxSock = _sock;

                for(int n = g_clients.size() - 1; n >= 0; n--)
                {
                    FD_SET(g_clients[n], &fdReads);
                    if(maxSock < g_clients[n])
                    {
                        maxSock = g_clients[n];
                    }
                }

                timeval t = {1,0};
                int ret = select(maxSock + 1, &fdReads, &fdWrites, &fdExp, &t);

                if(ret < 0)
                {
                    printf("select mission over. \n");
                    Close();
                }

                if(FD_ISSET(_sock, &fdReads))
                {
                    FD_CLR(_sock, &fdReads);
                    Accept();
                }

                for(int n = g_clients.size() - 1; n >= 0; n--)
                {
                    if(FD_ISSET(g_clients[n], &fdReads))
                    {
                        if(-1 == RecvData(g_clients[n]))
                        {
                            auto iter = g_clients.begin() + n;
                            if(iter != g_clients.end())
                            {
                                close(*iter);
                                g_clients.erase(iter);
                            }

                        }
                    }
                }

                return true;

            }
            return false;
        }

        // 判断是否正在运行
        bool isRun()
        {
            return _sock != INVALID_SOCKET;
        }
        // 接受消息
        int RecvData(SOCKET _csock)
        {
            char szRecv[4096] = {};
            int nLen = (int)recv(_csock, szRecv, sizeof(DataHeader), 0);
            DataHeader* header = reinterpret_cast<DataHeader*>(szRecv);
            if(nLen <= 0)
            {
                // printf("client<socket=%d> exit, mission over...\n", _csock);
                return -1;
            }
            if(header->dataLength - sizeof(DataHeader) > 0)
                recv(_csock, szRecv+sizeof(DataHeader), header->dataLength-sizeof(DataHeader), 0);
            OnNetMsg(_csock, header);
            return 1;
        }

        // 响应网络消息
        virtual void OnNetMsg(SOCKET _csock, DataHeader* header)
        {
            switch (header->cmd)
            {
            case CMD_LOGIN:
            {
                Login* login = static_cast<Login*>(header);
                // printf("received client <socket=%d> request: CMD_LOGIN, data length: %d, userName = %s, PassWord = %s...\n",
                        // _csock, login->dataLength, login->userName, login->password);
                LoginResult ret;
                ret.result = 666;  
                SendData(_csock, &ret);
                break;
            }
            case CMD_LOGOUT:
            {
                Logout* logout = static_cast<Logout*>(header);
                printf("received client <socket=%d> request: CMD_LOGOUT, data length: %d, userName = %s..\n",
                        _csock, logout->dataLength, logout->userName);
                LogoutResult ret;
                ret.result = 666;
                SendData(_csock, &ret);
                break;
            }   
            default:
            {
                DataHeader ret = {0, CMD_ERROR};
                SendData(_csock, &ret);
            }
                break;
            }
        }
        // 发送消息
        int SendData(SOCKET _csock, DataHeader* header)
        {
            if(isRun() && header)
            {
                // printf("send successfully...\n");
                return send(_csock, static_cast<const void*>(header), header->dataLength, 0);
            }
            return SOCKET_ERROR;
        }

        // 群发
        int SendDataToAll(DataHeader* header)
        {
            if(isRun() && header)
            {
                for(int n = g_clients.size() - 1; n >= 0; n--)
                {
                    SendData(g_clients[n], header);
                }
                
            return true;
            }
            return SOCKET_ERROR;
        }
         
        private:
            SOCKET _sock;
            std::vector<SOCKET> g_clients;

            int port;
};

#endif // !_EasyTcpServer_hpp_