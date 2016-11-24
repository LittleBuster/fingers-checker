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

#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <memory>
#include <Winsock2.h>
#include <Windows.h>

using namespace std;

class ITcpClient
{
public:
    virtual void connect(const string &ip, unsigned port)=0;
    virtual void send(const void *data, size_t len) const=0;
    virtual void recv(void *data, size_t len) const=0;
    virtual void close(void) const=0;
};


class TcpClient: public ITcpClient
{
private:
    SOCKET _client;

public:
    TcpClient();

    TcpClient(SOCKET sock);

    /**
     * Connect to remote server
     * @ip: remote ip address
     * @port: remote socket port
     *
     * throw: error if fail connecting
     */
    void connect(const string &ip, unsigned port);

    /**
     * Send data to server
     * @data: sending data
     * @len: data length
     *
     * throw: error if fail sending data
     */
    void send(const void *data, size_t len) const;

    /**
     * Receive data from server
     * @data: receiving data
     * @len: data length
     *
     * throw: error if fail receiving data
     */
    void recv(void *data, size_t len) const;

    /*
     * Close connection
     */
    void close(void) const;
};


#endif
