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

#define DEV_COUNT 3

typedef struct {
    string username;
    string passwd;
    unsigned pages;
    string devIps[DEV_COUNT];
    string devNames[DEV_COUNT];
    string printIps[DEV_COUNT];
    string printNames[DEV_COUNT];
} WebCfg;

typedef struct {
    string ip;
    unsigned port;
} ServerCfg;

typedef struct {
    unsigned interval;
    unsigned devInterval;
} CheckerCfg;

typedef struct {
    unsigned id;
    string key;
} TelegramCfg;

typedef struct {
    string ip;
    string user;
    string passwd;
    string base;
} DatabaseCfg;


class IConfigs
{
public:
    virtual const WebCfg& getWebCfg(void) const = 0;
    virtual const CheckerCfg& getCheckerCfg(void) const = 0;
    virtual const ServerCfg& getServerCfg(void) const = 0;
    virtual const DatabaseCfg& getDatabaseCfg(void) const = 0;
    virtual const TelegramCfg& getTelegramCfg(void) const = 0;
    virtual void load(const string &filename) = 0;
};


class Configs: public IConfigs
{
private:
    WebCfg wc;
    ServerCfg sc;
    CheckerCfg cc;
    DatabaseCfg dbc;
    TelegramCfg tc;

public:
    inline const DatabaseCfg& getDatabaseCfg(void) const { return dbc; }

    inline const CheckerCfg& getCheckerCfg(void) const { return cc; }

    inline const WebCfg& getWebCfg(void) const { return wc; }

    inline const ServerCfg& getServerCfg(void) const { return sc; }

    inline const TelegramCfg& getTelegramCfg(void) const { return tc; }

    /**
     * Loading configs from json file
     * @filename: name of json file
     *
     * throw: errror if file not found or parsing fail
     */
    void load(const string &filename);
};


#endif
