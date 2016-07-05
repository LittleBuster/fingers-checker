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
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


Database::Database()
{    
}

void Database::connect(const string &ip, const string &user, const string &passwd, const string &base)
{
    _base = mysql_init(NULL);

    mysql_options(_base, MYSQL_SET_CHARSET_NAME, "utf8");
    _base = mysql_real_connect(_base, ip.c_str(), user.c_str(), passwd.c_str(), base.c_str(), 3306, NULL, 0);
    if (_base == NULL)
        throw string("Logger can not connect to database.");
}

unsigned Database::getMonthCount() const
{
    int retVal;
    size_t count;
    MYSQL_RES *result;

    retVal = mysql_query(_base, string("SELECT id FROM users WHERE MONTH(time) >= MONTH(CURDATE())").c_str());
    if (retVal != 0)
        throw string("Error while reading database table.");

    result = mysql_store_result(_base);
    count = mysql_num_rows(result);
    mysql_free_result(result);
    return count;
}

void Database::addUser(unsigned idUsr, const string &username, const string &deviceName, const string &time, const string &hash)
{
    int retVal;

    retVal = mysql_query(_base, string("INSERT INTO users(idUsr, name, device, time, hash) VALUES (\"" + boost::lexical_cast<string>(idUsr) +
                                       + "\", \"" + username + "\", \"" + deviceName + "\", \"" + time + "\",\"" + hash + "\")").c_str());
    if (retVal != 0)
        throw string("Error inserting user in database.");
}

tuple<vector<ShortUserData>, bool> Database::checkSurname(const string &surname) const
{
    int retVal;
    size_t count;
    MYSQL_RES *result;
    MYSQL_ROW row;
    vector<ShortUserData> susers;
    bool exists = false;

    retVal = mysql_query(_base, string("SELECT idUsr, name FROM people WHERE name LIKE \"%" + surname + "%\"").c_str());
    if (retVal != 0)
        throw string("Error while reading database table.");

    result = mysql_store_result(_base);
    count = mysql_num_rows(result);
    if (count == 0) {
        exists = false;
        mysql_free_result(result);
        return make_tuple(susers, exists);
    }
    else
        exists = true;

    for (size_t i = 0; i < count; i++) {
        ShortUserData sdata;

        row = mysql_fetch_row(result);
        sdata.id = boost::lexical_cast<unsigned>(row[0]);
        sdata.name = boost::lexical_cast<string>(row[1]);
        susers.push_back(sdata);
    }
    mysql_free_result(result);
    return make_tuple(susers, exists);
}

tuple<vector<User>, bool> Database::checkUser(unsigned id) const
{
    int retVal;
    size_t count;
    MYSQL_RES *result;
    MYSQL_ROW row;
    vector<User> users;
    bool exists = false;

    retVal = mysql_query(_base, string("SELECT * FROM users WHERE idUsr=\"" +
                                       boost::lexical_cast<string>(id) + "\" AND time >= CURDATE()").c_str());
    if (retVal != 0)
        throw string("Error while reading database table.");

    result = mysql_store_result(_base);
    count = mysql_num_rows(result);
    if (count == 0)
        exists = false;
    else
        exists = true;

    for (size_t i = 0; i < count; i++) {
        User user;
        vector<string> parts;

        row = mysql_fetch_row(result);
        user.id = boost::lexical_cast<unsigned>(row[1]);
        user.name = boost::lexical_cast<string>(row[2]);
        user.device = boost::lexical_cast<string>(row[3]);
        user.time = boost::lexical_cast<string>(row[4]);
        user.code = boost::lexical_cast<string>(row[5]);

        users.push_back(user);
    }
    mysql_free_result(result);
    return make_tuple(users, exists);
}

void Database::close()
{
    mysql_close(_base);
}
