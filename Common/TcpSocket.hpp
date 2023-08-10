#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#define IP "127.0.0.1"

using namespace std;


//通信类
class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();
    void toNoBlack();
    int ConnectToHost(string ip, unsigned short port);
    int sendMsg(string msg);
    string recvMsg();
    
    int getfd(){
        return socketfd;
    }

private:
    int readn (char *buf, int size);
    int writen(char *msg, int size);

private:
    int socketfd;
};

inline TcpSocket::TcpSocket()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
}

inline TcpSocket::TcpSocket(int socket)
{
    socketfd = socket;
}

inline TcpSocket::~TcpSocket()
{
    //close(socketfd);
}

inline int TcpSocket::ConnectToHost(string ip,unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.data(), &saddr.sin_addr.s_addr);
    int ret = connect(socketfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("connect");
        return -1;
    }
    return ret;
}

inline int TcpSocket::sendMsg(string msg)
{
    // 申请内存空间: 数据长度 + 包头4字节(存储数据长度)
    //char* data = new char[msg.size() + 4];
    char *data = (char *)malloc(sizeof(char) * (msg.size() + 4));
    int bigLen = htonl(msg.size());
    memcpy(data, &bigLen, 4);
    memcpy(data + 4, msg.c_str(), msg.size());
    // 发送数据
    int ret = writen(data, msg.size() + 4);
    delete[] data;
    return ret;
}

inline string TcpSocket::recvMsg()
{
    // 接收数据
    int len = 0;
    readn((char*)&len, 4);
    len = ntohl(len);
    
    // 根据读出的长度分配内存
    char* buf = new char[len + 1];
    int ret = readn(buf, len);
    if (ret != len)
    {
        return string();
    }
    buf[len] = '\0';
    string retStr(buf);
    delete[]buf;

    return retStr;
}

inline int TcpSocket::readn(char* buf, int size)
{
    int nread = 0;
    int left = size;
    char* p = buf;

    while (left > 0)
    {
        if ((nread = read(socketfd, p, left)) > 0)
        {
            p += nread;
            left -= nread;
        }
        else if (nread == -1)
        {
            cout << "Rerrno:" << strerror(errno) << endl;
            return -1;
        }
    }
    return size;
}

inline int TcpSocket::writen(char* msg, int size)
{
    int left = size;
    int nwrite = 0;
    const char* p = msg;
   
    while (left > 0)
    {
        if ((nwrite = write(socketfd, msg, left)) > 0)
        {
            p += nwrite;
            left -= nwrite;
        }
        else if (nwrite == -1)
        {
            cout << "Werrno:" << strerror(errno) << endl;
            return -1;
        }
    }
    return size;
}

inline void TcpSocket::toNoBlack()
{
    int flag = fcntl(socketfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(socketfd, F_SETFL, flag);
}

//服务器 = 服务器类+通信类
class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int setListen(unsigned short port);
    TcpSocket* acceptConnect();

    int getfd(){
        return listenfd;
    }

private:
    int listenfd;
};

inline TcpServer::TcpServer()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
}

inline TcpServer::~TcpServer()
{
    //close(listenfd);
}

inline int TcpServer::setListen(unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0 = 0.0.0.0
    int ret = bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    cout << "套接字绑定成功, ip: "
        << inet_ntoa(saddr.sin_addr)
        << ", port: " << port << endl;

    ret = listen(listenfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    cout << "设置监听成功..." << endl;

    return ret;
}

inline TcpSocket *TcpServer::acceptConnect()
{
    int cfd = accept(listenfd, NULL, NULL);
    if (cfd == -1)
    {
        perror("accept");
    }
    printf("成功和客户端建立连接...\n");
    return new TcpSocket(cfd);
}

#endif