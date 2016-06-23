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

#include "checker.h"
#include "printclient.h"
#include <iostream>
#include <tuple>
#include <curl/curl.h>
#include <thread>
#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>
#include "helper.h"


static size_t writer(char *ptr, size_t size, size_t nmemb, string* data)
{
    if (data) {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else
        return 0;
}


void Checker::check()
{
    const auto &wc = _cfg->getWebCfg();
    /*
     * Check data from device
     */
    /*for (unsigned i = 0; i < DEV_COUNT; i++) {
        thread th(bind(&Checker::checkDevice, this, wc.devIps[i], wc.printIps[i], wc.username, wc.passwd,
                              boost::lexical_cast<string>(wc.pages), wc.devNames[i], wc.printNames[i]));
        th.detach();
    }*/

    for (size_t i = 0; i < DEV_COUNT; i++)
        checkDevice(wc.devIps[i], wc.printIps[i], wc.username, wc.passwd,  boost::lexical_cast<string>(wc.pages), wc.devNames[i], wc.printNames[i]);
    /*
     * Next time cycle
     */
    _timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(_interval));
    _timer->async_wait(bind(&Checker::check, this));
}

void Checker::checkDevice(const string &devIp, const string &printer, const string &user, const string &passwd,
                          const string &pageSize, const string &devName, const string &printName)
{
    string userName;
    string userKey;

    while (true) {
        /*
         * Reading Auth.xml
         */
        const tuple<string, bool> &auth = getData("http://" + devIp + "/chk.cgi?userid=" + user + "&userpwd=" + passwd);
        if (get<1>(auth) == true) {
            _log->local(devName + ": Can not connect to authorization server.", LOG_ERROR);
            _log->remote("Can not connect to authorization server.", LOG_ERROR, devName);
            break;
        }
        if (get<0>(auth) == "") {
            _log->local(devName + ": Empty file Auth.xml", LOG_ERROR);
            _log->remote("Empty file Auth.xml", LOG_ERROR, devName);
            break;
        }
        stringstream stream(get<0>(auth));
        boost::property_tree::ptree propertyTree;
        try {
            boost::property_tree::read_xml(stream, propertyTree);
        }
        catch(...) {
            _log->local(devName + ": Fail reading keys.", LOG_ERROR);
            _log->remote("Fail reading keys.", LOG_ERROR, devName);
            break;
        }

        bool isOk = true;
        for(const auto &v : propertyTree) {
            try {
                userName = v.second.get<string>("user");
                userKey = v.second.get<string>("userkey");
            } catch (...) {
                _log->local(devName + ": Fail reading user keys.", LOG_ERROR);
                _log->remote("Fail reading user keys.", LOG_ERROR, devName);
                isOk = false;
                break;
            }
        }
        if (!isOk)
            break;

        cout << "[" +  devName + "] Admin: " << userName << " Key: " << userKey << endl;

        /*
         * Reading Data.xml
         */
        const string &nowdate = dateToNum(boost::posix_time::second_clock::local_time());
        const tuple<string, bool> &data = getData("http://" + devIp + "/query.cgi?userid=" + user + "&sdate="
                                                  + nowdate + "&edate=" + nowdate + "&start=0&pagesize=" + pageSize
                                                  + "&user=" + userName + "&userkey=" + userKey);

        if (get<1>(data) == true) {
            _log->local(devName + ": Can not connect to data server.", LOG_ERROR);
            _log->remote("Can not connect to data server.", LOG_ERROR, devName);
            return;
        }
        if (get<0>(data) == "") {
            _log->local(devName + ": Empty file Data.xml", LOG_ERROR);
            _log->remote("Empty file Data.xml", LOG_ERROR, devName);
            return;
        }
        stringstream streamData(get<0>(data));
        boost::property_tree::ptree p2, p3;
        try {
            boost::property_tree::read_xml(streamData, p2);
            p3 = p2.get_child("document.items");
        }
        catch(...) {
            _log->local(devName + ": Fail reading data.", LOG_ERROR);
            _log->remote("Fail reading data.", LOG_ERROR, devName);
            return;
        }

        const auto &dbc = _cfg->getDatabaseCfg();
        Database db;

        try {
            db.connect(dbc.ip, dbc.user, dbc.passwd, dbc.base);
        }
        catch (const string &err) {
            _log->local(devName + ": CheckUser: " + err, LOG_ERROR);
            _log->remote("CheckUser: " + err, LOG_ERROR, devName);
            break;
        }

        for (const auto &v : p3) {
            bool retVal;

            try {
                retVal = db.checkUser(v.second.get<unsigned>("userid"));
            }
            catch(const string &err) {
                _log->local(devName + ": CheckUser: " + err, LOG_ERROR);
                _log->remote("CheckUser: " + err, LOG_ERROR, devName);
                continue;
            }
            if (!retVal) {
                string hash;

                /*
                 * PRINT TICKET
                 */
                cout << "PRINT TICKET! User: " << v.second.get<unsigned>("userid") << " " << v.second.get<string>("name") <<
                        " Printer: " << printer << endl;
                PrintClient pclient;
                pclient.setData(v.second.get<unsigned>("userid"), v.second.get<string>("name"), printName, devName, nowdate);
                hash = pclient.genSendData();

                /*
                 * Add new user in database
                 */
                try {
                    db.addUser(v.second.get<unsigned>("userid"), v.second.get<string>("name"), devName, v.second.get<string>("date"),
                               hash);
                }
                catch (const string &err) {
                    _log->local(devName + ": CheckUser: " + err, LOG_ERROR);
                    _log->remote("CheckUser: " + err, LOG_ERROR, devName);
                    continue;
                }

                try {
                    pclient.connect(printer, 6000);
                }
                catch(const string &err) {
                    _log->local(devName + ": PrintClient: " + err, LOG_ERROR);
                    _log->remote("PrintClient: " + err, LOG_ERROR, devName);
                    continue;
                }
                try {
                    pclient.sendData();
                }
                catch (const string &err) {
                    pclient.close();
                    _log->local(devName + ": PrintClient: " + err, LOG_ERROR);
                    _log->remote("PrintClient: " + err, LOG_ERROR, devName);
                    continue;
                }
                pclient.close();
            }
        }
        db.close();
        break;
    }
}

tuple<string, bool> Checker::getData(const string &url) const
{
    bool err = false;
    string content;
    CURL *curl_handle;

    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
        err = false;
    } else
        err = true;
    return make_tuple(content, err);
}

Checker::Checker(const shared_ptr<ILog> &log, const shared_ptr<IConfigs> &cfg)
{
    _log = log;
    _cfg = cfg;
}

void Checker::start()
{
    _timer = make_shared<deadline_timer>(_io, boost::posix_time::seconds(_interval));
    _timer->async_wait(bind(&Checker::check, this));
    _io.run();
}
