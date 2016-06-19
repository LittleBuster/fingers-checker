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
    virtual bool checkUser(const string &username, const string &deviceIP, const string &time) = 0;
    virtual void addUser(const string &username, const string &deviceIP, const string &time) = 0;
    virtual void close() = 0;
};


class Database: public IDatabase
{
private:
    MYSQL *_base;

public:
    Database();

    /**
     * Connecting to database
     * @ip: address of database
     * @user: database user
     * @passwd: database user password
     * @base: database name
     *
     * throw: error if fail connecting
     */
    void connect(const string &ip, const string &user, const string &passwd, const string &base);

    /**
     * Remote logging
     * @message: log message
     * @type: type of message
     * @deviceIP: IP address of logging device
     * @time: now time
     *
     * throw: error if fail inserting message to base
     */
    void log(const string &message, const string &type, const string &deviceIP, const string &time);

    /**
     * Checking user in database
     * @username: name of checking user
     * @deviceIP: IP address of user's device
     * @time: now time
     *
     * returns true: if user exists in database
     * returns false: if user not found
     */
    bool checkUser(const string &username, const string &deviceIP, const string &time);

    /**
     * Adding new user in database
     * @username: name of adding user
     * @deviceIP: IP address of user's device
     * @time: now time
     *
     * throw: error if fail inserting user
     */
    void addUser(const string &username, const string &deviceIP, const string &time);

    /*
     * Close database
     */
    void close();
};


#endif
