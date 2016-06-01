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


void Database::open(const string &filename)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(filename.c_str());
    if (!_db.open())
        throw string(_db.lastError().text().toStdString());
}

void Database::addUser(unsigned id, string name, string date)
{
    QSqlQuery q;
    string uid = boost::lexical_cast<string>(id);
    string str = "INSERT INTO users (uid, name, date) VALUES ('" + uid + "', '" + name + "', '" + date + "')";

    bool f = q.exec(str.c_str());
    if (!f)
        throw string("Fail add new user in Database");
}

void Database::incUser(unsigned id)
{
    //UPDATE VALUES
}

void Database::close()
{
    _db.close();
}

bool Database::checkUser(unsigned id)
{
    string uid = boost::lexical_cast<string>(id);

    QSqlQuery query(string("SELECT uid FROM users WHERE uid='" + uid + "'").c_str());
    while (query.next()) {
        unsigned _uid = query.value(0).toUInt();
        if (id == _uid)
            return true;
    }
    return false;
}
