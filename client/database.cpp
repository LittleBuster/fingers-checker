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
#include <boost/lexical_cast.hpp>
#include <iostream>
using namespace std;


void Database::connect(const string &ip, const string &user, const string &passwd, const string &base)
{
    _base = mysql_init(NULL);

    mysql_options(_base, MYSQL_SET_CHARSET_NAME, "utf8");
    _base = mysql_real_connect(_base, ip.c_str(), user.c_str(), passwd.c_str(), base.c_str(), 3306, NULL, 0);
    if (_base == NULL)
        throw string("Logger can not connect to database.");
}

void Database::log(const string &message, const string &type, const string &deviceName, const string &time)
{
    int retVal;
    string table;

    if (deviceName == "")
        table = "syslog";
    else
        table = "log";

    retVal = mysql_query(_base, string("INSERT INTO " + table + "(devip, message, type, time) VALUES (\"" + deviceName +
                                       "\",\"" + message + "\",\"" + type + "\",\"" + time + "\")").c_str());
    if (retVal != 0)
        throw string("Logger can not insert to database.");
}

bool Database::checkUser(unsigned idUser)
{
    int retVal;
    size_t count;
    MYSQL_RES *result;

    retVal = mysql_query(_base, string("SELECT idUsr FROM users WHERE idUsr=\"" + boost::lexical_cast<string>(idUser) +
                                       "\" AND time >= CURDATE()").c_str());
    if (retVal != 0)
        throw string("Error while reading database table.");

    result = mysql_store_result(_base);
    count = mysql_num_rows(result);

    if (count != 0) {
        mysql_free_result(result);
        return true;
    }
    mysql_free_result(result);
    return false;
}

void Database::addUser(unsigned idUsr, const string &username, const string &deviceIP, const string &time, const string &hash)
{
    int retVal;

    retVal = mysql_query(_base, string("INSERT INTO users(idUsr, name, device, time, hash) VALUES (\"" + boost::lexical_cast<string>(idUsr) +
                                       + "\", \"" + username + "\", \"" + deviceIP + "\", \"" + time + "\",\"" + hash + "\")").c_str());
    if (retVal != 0)
        throw string("Error inserting user in database.");
}

void Database::close()
{
    mysql_close(_base);
}
