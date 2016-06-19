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

#include "tcpclient.h"
#include <stdlib.h>
#include <boost/lexical_cast.hpp>


TcpClient::TcpClient()
{
    _client = make_shared<tcp::socket>(_service);
}

TcpClient::TcpClient(const shared_ptr<tcp::socket> &parent_client)
{
    _client = parent_client;
}

void TcpClient::connect(const string &ip, unsigned port)
{
    boost::system::error_code err;

    tcp::resolver resolver(_service);
    boost::asio::connect(*_client, resolver.resolve({ip, boost::lexical_cast<string>(port)}), err);
    if (err)
        throw string("Can not connect to server.");
}

void TcpClient::send(const void *data, size_t len) const
{
    boost::system::error_code error;

    _client->write_some(boost::asio::buffer(data, len), error);
    if (error)
        throw string("Fail sending data.");
}

void TcpClient::recv(void *data, size_t len) const
{
    size_t r_len;
    boost::system::error_code error;

    r_len = _client->read_some(boost::asio::buffer(data, len), error);
    if (error == boost::asio::error::eof)
        return;
    else if (error)
        throw string("Fail receiving data.");
}

void TcpClient::close() const
{
    _client->close();
}
