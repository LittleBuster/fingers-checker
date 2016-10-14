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

#ifndef __LOG_H__
#define __LOG_H__


#include <string>
#include <memory>
#include "configs.h"
#include "database.h"

using namespace std;


typedef enum {
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFORMATION
} LogType;


class ILog
{
public:
    virtual void setLogFile(const string &filepath) = 0;
    virtual void setTicketFile(const string &filepath) = 0;
    virtual void local(const string &message, const LogType log_type) = 0;
    virtual void remote(const string &message, const LogType logType) = 0;
    virtual void remote(const string &message, const LogType logType, const string &devName) = 0;
};


class Log: public ILog
{
private:
    shared_ptr<IConfigs> _cfg;
    shared_ptr<IDatabase> _db;
    string _logPath;
    string _ticketPath;

    using ILog::setLogFile;

    /**
     * Create log string for writing to file
     * @msg: logging message
     * @type: log message type
     *
     * returns full log string width datetime and type
     */
    string makeLogMsg(const string &msg, const LogType type) const;

public:
    explicit Log(const shared_ptr<IConfigs> &cfg, const shared_ptr<IDatabase> &db);

    /*
     * Set path for saving log data in local file
     */
    inline void setLogFile(const string &filepath) { _logPath = filepath; }

    /*
     * Set path for saving tickets log data in local file
     */
    inline void setTicketFile(const string &filepath) { _ticketPath = filepath; }

    /**
     * Saving log data in local file
     * @message: log message
     * @logType: message type
     *
     * throw: error if fail access to file
     */
    void local(const string &message, const LogType logType);

    /**
     * Saving ticket log data in local file
     * @message: log message
     * @logType: message type
     *
     * throw: error if fail access to file
     */
    void ticket(const string &message, const LogType logType);

    /**
     * Saving log data to database
     * @message: log message
     * @logType: message type
     *
     * throw: error if fail connect to db or insert log fail
     */
    void remote(const string &message, const LogType logType);

    /**
     * Saving log data to database
     * @message: log message
     * @logType: message type
     * @devName: name of device
     *
     * throw: error if fail connect to db or insert log fail
     */
    void remote(const string &message, const LogType logType, const string &devName);
};


#endif
