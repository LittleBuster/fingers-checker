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

#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <memory>

using namespace std;

typedef struct {
    string ip;
    unsigned port;
} PrintServerCfg;

typedef struct {
    string ip;
    string user;
    string passwd;
    string base;
} DatabaseCfg;


class IConfigs
{
public:
    virtual void load(const string &filename) = 0;
    virtual const PrintServerCfg& getPrintServerCfg() const = 0;
    virtual const DatabaseCfg& getDatabaseCfg() const = 0;
};


class Configs: public IConfigs
{
private:
    PrintServerCfg psc;
    DatabaseCfg dbc;

public:
    inline const PrintServerCfg& getPrintServerCfg() const { return psc; }

    inline const DatabaseCfg& getDatabaseCfg() const { return dbc; }

    /**
     * Loading configs from json file
     * @filename: name of json file
     *
     * throw: errror if file not found or parsing fail
     */
    void load(const string &filename);
};


#endif
