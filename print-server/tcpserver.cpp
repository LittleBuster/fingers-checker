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


void TcpServer::start(unsigned port)
{
    try {
        _acpt = make_shared<tcp::acceptor>(_service, tcp::endpoint(tcp::v4(), port));
    }
    catch (...) {
        throw string("Binding ip address and port fail.");
    }
    for (;;) {
        auto sock = make_shared<tcp::socket>(_service);
        try {
            _acpt->accept(*sock);
        }
        catch (...) {
            this->acceptError();
            continue;
        }
        auto client = make_shared<TcpClient>(sock);
        thread th(bind(&TcpServer::newSession, this, client));
        th.detach();
    }
}
