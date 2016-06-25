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

#ifndef __PRINTSERVER_H__
#define __PRINTSERVER_H__

#include <string>
#include "tcpserver.h"
#include "log.h"
#include <QtPrintSupport/QPrinter>

using namespace std;

typedef struct {
    unsigned uid;
    char shortName[50];
    char printer[10];
    char time[40];
    char hash[16];
} RecvData;


class IPrintServer: public ITcpServer
{
public:
    virtual void printTicket(const QString &shortName, const string &printName, const string &time, const string &hash) = 0;
};


class PrintServer final: public IPrintServer, public TcpServer
{
private:
    shared_ptr<ILog> _log;
    QPrinter _printer;

    /**
     * New client connected
     * @client: new client
     */
    virtual void newSession(shared_ptr<ITcpClient> client) override final;

    /*
     * Accepting new client error
     */
    virtual void acceptError(void) const override final;

public:
    PrintServer(const shared_ptr<ILog> &log);

    /**
     * Printing ticket
     * @shortName: Name of user
     * @printName: printer name
     * @time: now datetime
     * @hash: md5 hash sum
     *
     * throw: unknown error if fail printing
     */
    void printTicket(const QString &shortName, const string &printName, const string &time, const string &hash);

    /**
     * Starting tcp server
     * @port: tcp port
     *
     * throw: error if fail binding ip address
     */
    virtual void start(unsigned port) override final { TcpServer::start(port); }    
};


#endif
