#include "cj_network.h"

NETWORK StartCJNetwork(u32 version_big, u32 version_low)
{
    NETWORK network = {};
    WORD version = MAKEWORD(version_big, version_low);
    if(WSAStartup(version, &network.wsa_data) != 0)
    {
        printf("WSAStartup failed\n");
    }
    
    return network;
    
}

void SetAdressAndPort(CJ_SOCKET *s, char *IP, i32 port)
{
    
    s->addr.sin_family = AF_INET;
    s->addr.sin_addr.s_addr = inet_addr(IP);
    s->addr.sin_port = htons(port);
    
}

void CreateSocket(CJ_SOCKET *s, i32 type, i32 protocol)
{
    
    s->socket = socket(AF_INET, type, protocol);
    if(s->socket == INVALID_SOCKET)
    {
        printf("CreateSocket failed -  (error: %d)\n", WSAGetLastError());
    }
}

void BindSocket(CJ_SOCKET *s)
{
    
    if(bind(s->socket, (sockaddr*)&s->addr, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        printf("Binding socket failed - (error:%d)\n", WSAGetLastError());
    }
    
}

void ListenOnSocket(CJ_SOCKET *s, i32 backlog)
{
    
    if(listen(s->socket, backlog) == SOCKET_ERROR)
    {
        printf("Listen on socket failed - (error:%d)\n", WSAGetLastError());
    }
    
}

void Connect(CJ_SOCKET *connector, CJ_SOCKET *target)
{
    connect(connector->socket, (sockaddr*)&target->addr, sizeof(sockaddr));
}

CJ_SOCKET Accept(CJ_SOCKET *s, CJ_SOCKET *cli)
{
    CJ_SOCKET result = {};
    
    result.socket = accept(s->socket, (sockaddr*)&cli->addr, &cli->acceptLen);
    result.addr = cli->addr;
    
    if(result.socket == INVALID_SOCKET)
    {
        printf("Accepting on socket failed - (error:%d)\n", WSAGetLastError());
    }
    
    
    
    return result;
    
}

void SendMsg(CJ_SOCKET *s, char *msg, u32 sz_msg)
{
    
    send(s->socket, msg, sz_msg, 0);
}

void RecvMsg(CJ_SOCKET *s, char *msg, u32 sz_msg)
{
    
    recv(s->socket, msg, sz_msg, 0);
}


void QuitCJNetwork()
{
    WSACleanup();
    
}

