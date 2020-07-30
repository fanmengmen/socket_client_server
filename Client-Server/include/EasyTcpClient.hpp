#ifndef _EasyTcpClient_hpp_
#define _EasyTcpClient_hpp_


#include<iostream>
#include<signal.h>
#include"MessageHeader.hpp"
#ifdef _WIN32 

#else
    #include<unistd.h>
    #include<arpa/inet.h>
    #include<string.h>

    #define SOCKET int
    #define INVALID_SOCKET (0)
    #define SOCKET_ERROR (-1)
#endif 
class EasyTcpClient
{
    public:
        EasyTcpClient()
        {
        }
        virtual ~EasyTcpClient()
        {
            if(_sock != INVALID_SOCKET)
            {
                Close();
            }
        }
        //初始化socket
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
                printf("creat socket sucessfully!\n");  
            }
             
            return _sock;
        }

        //连接Server
        int Connect(char* ip, unsigned short port)
        {

            if(INVALID_SOCKET == _sock)
            {
                initSocket();
            }
            sockaddr_in _sin = {};
            _sin.sin_family = AF_INET;
            _sin.sin_port = htons(port);  //?
            _sin.sin_addr.s_addr = inet_addr(ip);

            int ret = connect(_sock, reinterpret_cast<sockaddr*>(&_sin), sizeof(sockaddr_in));
            if(SOCKET_ERROR == ret)
            {
                printf("error, connect server failed!\n");
            }
            else
            {
                printf("connect server sucessfully!\n");
            }
            return ret;
        }

        // close socket
        void Close()
        {
            close(_sock); 
            _sock = INVALID_SOCKET;
        }

        // receive data
        // send data
        // 处理数据
        bool OnRun()
        {
            if(isRun())
            {
                fd_set fdReads;
                FD_ZERO(&fdReads);
                FD_SET(_sock, &fdReads);
                timeval t = {1,0};
                int ret = select(_sock + 1, &fdReads, 0, 0, &t);
                
                if(ret < 0)
                {
                    printf("<socket =%d>select task over!\n", _sock);
                    Close();
                    return false;
                }
                
                if(FD_ISSET(_sock, &fdReads))
                {
                    FD_CLR(_sock, &fdReads);
                    if(-1 == RecvData(_sock))
                    {
                        printf("<socket =%d>select task over!\n", _sock);
                        return false;
                    }
                }            
            }
            return true;
        }

        // 判断是否正在运行
        bool isRun()
        {
            return _sock != INVALID_SOCKET;
        }

        // receive data 处理粘包拆分包
        int RecvData(SOCKET _csock)
        {
            // 缓冲区
            char szRecv[4096] = {};

            ssize_t nLen = recv(_csock, szRecv, sizeof(DataHeader),0);
            DataHeader* header = reinterpret_cast<DataHeader*>(szRecv);
        
            if(nLen <= 0)
            {
                printf("disconnect with server, mission over.\n");
                return -1;
            }  
            // 如果剩余长度 大于0， 将生于的长度从缓冲区拷贝出来
            if(header->dataLength - sizeof(DataHeader) > 0)
                recv(_csock, szRecv+sizeof(DataHeader), header->dataLength-sizeof(DataHeader), 0);
            OnNetMsg(header);

        }

        //响应网络消息
        void OnNetMsg(DataHeader* header)
        {
            
            switch (header->cmd)
            {
            case CMD_LOGIN_RESULT:
            {
                LoginResult* login = static_cast<LoginResult* >(header);
                printf("have received server message: CMD_LOGIN_RESULT, data length: %d\n",login->dataLength);
                break;
            }
            case CMD_LOGOUT_RESULT:
            {
                LogoutResult* login = static_cast<LogoutResult* >(header);
                printf("have received server message: CMD_LOGOUT_RESULT, data length: %d\n",login->dataLength);
                break;
            } 
            default:
                break;
            }
        }

        //发送消息
        int SendData(DataHeader* header)
        {
            if(isRun() && header)
            {
                printf("send successfully...\n");
                return send(_sock, static_cast<const void* >(header), header->dataLength,0);
            }
            else
            {
                printf("send failed");
                return -1;
            }
        }
    private:
        SOCKET _sock = 0;
};

#endif // !_EasyTcpClient_hpp_
