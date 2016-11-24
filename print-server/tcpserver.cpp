/* Fingers checker
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "tcpserver.h"
#include <functional>
#include <thread>


void TcpServer::start(unsigned port, unsigned maxClients)
{
    int retVal;
    struct sockaddr_in sock_addr;

    WSADATA wsaData;
    WORD version_wanted = MAKEWORD(1, 1);

    if (WSAStartup(version_wanted, &wsaData) != 0)
        throw string("Fail WSA init.");

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock == INVALID_SOCKET)
        throw string("Fail Socket init.");

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(port);

    retVal = bind(_sock, reinterpret_cast<struct sockaddr *>(&sock_addr), sizeof(sock_addr));
    if (retVal == SOCKET_ERROR)
        throw string("Fail binding server.");

    retVal = listen(_sock, maxClients);
    if (retVal == SOCKET_ERROR)
        throw string("Fail starting listening.");

    serverStarted();

    for (;;) {
        SOCKET sClient = accept(_sock, NULL, NULL);

        if (sClient == SOCKET_ERROR) {
            acceptError();
            continue;
        }

        auto client = make_shared<TcpClient>(sClient);
        thread th(bind(&TcpServer::newSession, this, client));
        th.detach();
    }
    if (WSACleanup() == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAEINPROGRESS) {
#ifndef _WIN32_WCE
        WSACancelBlockingCall();
#endif
        WSACleanup();
           }
    }
}
