/* Fingers Statistics application
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
#include <vector>
#include <tuple>

using namespace std;


typedef struct {
    unsigned id;
    string name;
    string time;
    string device;
    string code;
} User;

typedef struct {
    unsigned id;
    string name;
} ShortUserData;


class IDatabase
{
public:
    virtual void connect(const string &ip, const string &user, const string &passwd, const string &base) = 0;
    virtual void addUser(unsigned idUsr, const string &username, const string &deviceName, const string &time, const string &hash) = 0;
    virtual unsigned getMonthCount() const = 0;
    virtual tuple<vector<ShortUserData>, bool> checkSurname(const string &surname) const = 0;
    virtual tuple<vector<User>, bool> checkUser(unsigned id) const = 0;
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

    unsigned getMonthCount() const;

    void addUser(unsigned idUsr, const string &username, const string &deviceName, const string &time, const string &hash);

    tuple<vector<ShortUserData>, bool> checkSurname(const string &surname) const;

    tuple<vector<User>, bool> checkUser(unsigned id) const;

    /*
     * Close database
     */
    void close();
};


#endif
