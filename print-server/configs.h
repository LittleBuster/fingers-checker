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
    unsigned port;
    unsigned clients;
} ServerCfg;

class IConfigs
{
public:
    virtual const ServerCfg& getServerCfg(void) const=0;
    virtual void load(const string &filename)=0;
};


class Configs: public IConfigs
{
private:
    ServerCfg sc;

    string readString(ifstream &is) const;

    int readInt(ifstream &is) const;

public:
    inline const ServerCfg& getServerCfg(void) const { return sc; }

    /**
     * Loading configs from json file
     * @filename: name of json file
     *
     * throw: errror if file not found or parsing fail
     */
    void load(const string &filename);
};


#endif
