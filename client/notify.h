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

#ifndef __NOTIFY_H__
#define __NOTIFY_H__

#include "configs.h"
#include "tcpclient.h"


class INotify
{
public:
    virtual bool checkDevice(const string &deviceIP) = 0;
    virtual void sendTelegram(const string &message) = 0;
};


class Notify: public INotify
{
private:
    shared_ptr<IConfigs> _cfg;
    shared_ptr<ITcpClient> _client;

public:
    explicit Notify(const shared_ptr<IConfigs> &cfg, const shared_ptr<ITcpClient> &client);

    /**
     * Try to connect to device
     *
     * returns true: if device is live
     * returns false: if device is dead
     */
    bool checkDevice(const string &deviceIP);

    /**
     * Send message to telegram bot
     * @message: text of message
     *
     * throw: error if fail sending msg
     */
    void sendTelegram(const string &message);
};


#endif
