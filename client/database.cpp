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

#include "database.h"


Database::Database()
{
    mysql_init(_base);
}

void Database::connect(const string &ip, const string &user, const string &passwd, const string &base)
{
    _base = mysql_real_connect(_base, ip.c_str(), user.c_str(), passwd.c_str(), base.c_str(), 3306, NULL, 0);
    if (_base == NULL)
        throw string("Logger can not connect to database.");
}

void Database::log(const string &message, const string &type, const string &deviceIP, const string &time)
{
    int retVal;
    string table;

    if (deviceIP == "0.0.0.0")
        table = "syslog";
    else
        table = "log";

    retVal = mysql_query(_base, string("INSERT INTO " + table + "(devip, message, type, time) VALUES (\"" + deviceIP +
                                       "\",\"" + message + "\",\"" + type + "\",\"" + time + "\")").c_str());
    if (retVal != 0)
        throw string("Logger can not insert to database.");
}

void Database::close()
{
    mysql_close(_base);
}
