#ifndef _HTTPMessage_hpp_
#define _HTTPMessage_hpp_
#include<string>
#include<map>

using namespace std;
enum Method
{
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    OPTIONS,
    TRACE,
    CONNECT
};

class HttpRequest
{
    public:

        HttpRequest();
        ~HttpRequest();

        string getUrl();
        Method getMethod();
        string getVersion();
        string getHead(const string& str);
        string getBody();

        int setMethod(const Method& method);
        int setUrl(const string& str);
        int setVersion(const string& str);
        int setHead(const string& str);
        int setBody(const string& str);


        int setMessage();
        int parseMessage(const unsigned char* str);
    private:

        unsigned char* message;
        Method method;
        string url = "";
        string version = "HTTP/1.1";
        map<string, string> requestHead;
        string body = "";
        
    
};



class HttpRespond
{
    public:
        HttpRespond();
        ~HttpRespond();

        string getStatus();
        Method getDescription();
        string getVersion();
        string getHead(const string& str);

        int setStatus(const Method& method);
        int setDescription(const string& str);
        int setVersion(const string& str);
        int setHead(const string& str);

        int setMessage();
        int parseMessage();
    private:

        unsigned char* message;
        string version = "HTTP/1.1";
        string status;
        string description;
        map<string, string> requestHead;
        
};

#endif