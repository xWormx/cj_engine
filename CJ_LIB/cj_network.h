/* date = October 26th 2020 8:26 pm */

#ifndef CJ_NETWORK_H
#define CJ_NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "cj_int.h"

struct NETWORK
{
    WSADATA wsa_data;
    
};

struct CJ_SOCKET
{
    SOCKET socket;
    sockaddr_in addr;
    i32 acceptLen = sizeof(sockaddr);
};

NETWORK StartCJNetwork(u32 version_big, u32 version_low);
void QuitCJNetwork();

void SetAdressAndPort(CJ_SOCKET *s, char *IP, i32 port);
void CreateSocket(CJ_SOCKET *s, i32 type, i32 protocol);
void BindSocket(CJ_SOCKET *s);
void ListenOnSocket(CJ_SOCKET *s, i32 backlog);
void Connect(CJ_SOCKET *connector, CJ_SOCKET *target);
CJ_SOCKET Accept(CJ_SOCKET *s, CJ_SOCKET *cli);

void SendMsg(CJ_SOCKET *s, char *msg, u32 sz_msg);
void RecvMsg(CJ_SOCKET *s, char *msg, u32 sz_msg);


#endif //CJ_NETWORK_H
