
#ifndef _MessageHeader_hpp_
#define _MessageHeader_hpp_

enum CMD
{
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_ERROR,
    CMD_NEWUSER_JOIN
};            
         

struct DataHeader
{
    DataHeader(){}
    DataHeader(short _length, CMD _cmd): dataLength(_length), cmd(_cmd){}
    short dataLength;
    short cmd;
    virtual ~DataHeader(){}
};

//DataPackage
struct Login : public DataHeader
{
    Login()
    {
        dataLength = sizeof(Login);
        cmd = CMD_LOGIN;
    }
    char userName[32];
    char password[32];
};

struct LoginResult : public DataHeader
{
    LoginResult()
    {
        dataLength = sizeof(LoginResult);
        cmd = CMD_LOGIN_RESULT;
    }
    int result;
};



struct Logout : public DataHeader
{
    Logout()
    {
        dataLength = sizeof(Login);
        cmd = CMD_LOGIN;
    }
    char userName[32];
};


struct LogoutResult : public DataHeader
{
    LogoutResult()
    {
        dataLength = sizeof(LogoutResult);
        cmd = CMD_LOGOUT_RESULT;
    }
    int result;
};


struct NewUserJoin : public DataHeader
{
    NewUserJoin()
    {
        dataLength = sizeof(NewUserJoin);
        cmd = CMD_NEWUSER_JOIN;      
    }
    char userName[32];
};

#endif // !_MessageHeader_hpp_

