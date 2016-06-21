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

#ifndef __PRINTCLIENT_H__
#define __PRINTCLIENT_H__

#include "tcpclient.h"

typedef struct {
    unsigned uid;
    char shortName[50];
    char printer[10];
    char date[20];
    char time[20];
    char hash1[8];
    char hash2[8];
} SendData;


class IPrintClient: public ITcpClient
{
public:
    virtual void sendData() = 0;
    virtual string genSendData() = 0;
    virtual void setData(unsigned id, unsigned uID, const string &name, const string &printer, const string &devIP, const string &time) = 0;
};


class PrintClient: public IPrintClient
{
private:
    unsigned _id;
    unsigned _uID;
    string _name;
    string _printer;
    string _devIP;
    string _time;
    SendData sdata;

public:
    inline void setData(unsigned id, unsigned uID, const string &name, const string &printer, const string &devIP, const string &time) {
        _id = id;
        _uID = uID;
        _name = name;
        _devIP = devIP;
        _time = time;
        _printer = printer;
    }

    /*
     * Generating hash and short name
     */
    string genSendData();

    /*
     * Sending data to print server
     */
    void sendData();
};


#endif
