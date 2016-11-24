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

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <memory>
#include <thread>
#include <Winsock2.h>
#include <Windows.h>
#include <unistd.h>
#include <string.h>
#include "tcpclient.h"


using namespace std;


class ITcpServer
{
public:
    virtual void newSession(shared_ptr<ITcpClient> client) = 0;
    virtual void acceptError() const = 0;
    virtual void serverStarted() const = 0;
    virtual void start(unsigned port, unsigned maxClients) = 0;
};


class TcpServer: public ITcpServer
{
private:
    SOCKET _sock;

public:
    /*
     * New client connection session
     */
    virtual void newSession(shared_ptr<ITcpClient> client) { };

    /*
     * Accepting new client error signal
     */
    virtual void acceptError() const { };

    /*
     * Server started signal
     */
    virtual void serverStarted() const { };

    /**
     * Binding tcp server
     * @port: local tcp port
     *
     * throw: error if fail binding
     */
    void start(unsigned port, unsigned maxClients);
};


#endif
