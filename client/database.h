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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <mysql/mysql.h>
#include <string>

using namespace std;

class IDatabase
{
public:
    virtual void connect(const string &ip, const string &user, const string &passwd, const string &base) = 0;
    virtual void log(const string &message, const string &type, const string &deviceIP, const string &time) = 0;
    virtual void close() = 0;
};


class Database: public IDatabase
{
private:
    MYSQL *_base;

public:
    Database();

    void connect(const string &ip, const string &user, const string &passwd, const string &base);

    void log(const string &message, const string &type, const string &deviceIP, const string &time);

    void close();
};


#endif
