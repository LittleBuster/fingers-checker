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
    for (size_t i = 0; i < DEV_COUNT; i++) {
        thread th(bind(&Checker::checkDevice, this, i));
        th.detach();
    }
    /*
     * Next time cycle
     */
    _timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(_interval));
    _timer->async_wait(bind(&Checker::check, this));
}

void Checker::checkDevice(size_t index)
{
    string userName;
    string userKey;
    const auto &wc = _cfg->getWebCfg();

    /*
     * Reading Auth.xml
     */
    const tuple<string, bool> &auth = getData("http://" + wc.devIps[index] + "/chk.cgi?userid=" + wc.username + "&userpwd=" + wc.passwd);
    if (get<1>(auth) == true) {
        mtx.lock();
        _log->local(wc.devNames[index] + ": Can not connect to authorization server.", LOG_ERROR);
        _log->remote("Can not connect to authorization server.", LOG_ERROR, wc.devNames[index]);
        mtx.unlock();
        return;
    }
    if (get<0>(auth) == "") {
        mtx.lock();
        _log->local(wc.devNames[index] + ": Empty file Auth.xml", LOG_ERROR);
        _log->remote("Empty file Auth.xml", LOG_ERROR, wc.devNames[index]);
        mtx.unlock();
        return;
    }
    stringstream stream(get<0>(auth));
    boost::property_tree::ptree propertyTree;
    try {
        boost::property_tree::read_xml(stream, propertyTree);
        _isKeysErr[index] = false;
    }
    catch(...) {
        mtx.lock();
        if (!_isKeysErr[index]) {
            _isKeysErr[index] = true;
            _log->local(wc.devNames[index] + ": Fail reading keys.", LOG_ERROR);
            _log->remote("Fail reading keys.", LOG_ERROR, wc.devNames[index]);
        }
        mtx.unlock();
        return;
    }

    bool isOk = true;
    for(const auto &v : propertyTree) {
        try {
            userName = v.second.get<string>("user");
            userKey = v.second.get<string>("userkey");
        } catch (...) {
            mtx.lock();
            _log->local(wc.devNames[index] + ": Fail reading user keys.", LOG_ERROR);
            _log->remote("Fail reading user keys.", LOG_ERROR, wc.devNames[index]);
            mtx.unlock();
            isOk = false;
            return;
        }
    }
    if (!isOk)
        return;

    mtx.lock();
    cout << "[" +  wc.devNames[index] + "] Admin: " << userName << " Key: " << userKey << endl;
    mtx.unlock();

    /*
     * Reading Data.xml
     */
    const string &nowdate = dateToNum(boost::posix_time::second_clock::local_time());
    const tuple<string, bool> &data = getData("http://" + wc.devIps[index] + "/query.cgi?userid=" + wc.username + "&sdate="
                                              + nowdate + "&edate=" + nowdate + "&start=0&pagesize=" + boost::lexical_cast<string>(wc.pages)
                                              + "&user=" + userName + "&userkey=" + userKey);

    if (get<1>(data) == true) {
        mtx.lock();
        _log->local(wc.devNames[index] + ": Can not connect to data server.", LOG_ERROR);
        _log->remote("Can not connect to data server.", LOG_ERROR, wc.devNames[index]);
        mtx.unlock();
        return;
    }
    if (get<0>(data) == "") {
        mtx.lock();
        _log->local(wc.devNames[index] + ": Empty file Data.xml", LOG_ERROR);
        _log->remote("Empty file Data.xml", LOG_ERROR, wc.devNames[index]);
        mtx.unlock();
        return;
    }
    stringstream streamData(get<0>(data));
    boost::property_tree::ptree p2, p3;
    try {
        boost::property_tree::read_xml(streamData, p2);
        p3 = p2.get_child("document.items");
        mtx.lock();
        _isReadErr[index] = false;
        mtx.unlock();
    }
    catch(...) {
        mtx.lock();
        if (!_isReadErr[index]) {
            _isReadErr[index] = true;
            _log->local(wc.devNames[index] + ": Fail reading data.", LOG_ERROR);
            _log->remote("Fail reading data.", LOG_ERROR, wc.devNames[index]);
        }
        mtx.unlock();
        return;
    }

    const auto &dbc = _cfg->getDatabaseCfg();
    Database db;
    try {
        db.connect(dbc.ip, dbc.user, dbc.passwd, dbc.base);
    }
    catch (const string &err) {
        mtx.lock();
        _log->local(wc.devNames[index] + ": CheckUser: " + err, LOG_ERROR);
        _log->remote("CheckUser: " + err, LOG_ERROR, wc.devNames[index]);
         mtx.unlock();
        return;
    }

    for (const auto &v : p3) {
        bool retVal;
        PrintClient pClient;

        boost::gregorian::date dt(boost::gregorian::day_clock::local_day());
        if (boost::lexical_cast<string>(dt.day_of_week()) == "Sat" || boost::lexical_cast<string>(dt.day_of_week()) == "Sun") {
            mtx.lock();
            cout << "Weekend!" << endl;
            mtx.unlock();
            continue;
        }

        try {
            retVal = db.checkUser(v.second.get<unsigned>("userid"));
        }
        catch(const string &err) {
            mtx.lock();
            _log->local(wc.devNames[index] + ": CheckUser: " + err, LOG_ERROR);
            _log->remote("CheckUser: " + err, LOG_ERROR, wc.devNames[index]);
            mtx.unlock();
            continue;
        }
        if (!retVal) {
            string hash;

            /*
             * PRINT TICKET
             */
            mtx.lock();
            cout << "PRINT TICKET! User: " << v.second.get<unsigned>("userid") << " " << v.second.get<string>("name") <<
                    " Printer: " << wc.printNames[index] << endl;
            _log->ticket("PRINT TICKET! User: " + boost::lexical_cast<string>(v.second.get<unsigned>("userid")) + " " + v.second.get<string>("name") +
                    " Printer: " + wc.printNames[index], LOG_INFORMATION);
            mtx.unlock();
            pClient.setData(v.second.get<unsigned>("userid"), v.second.get<string>("name"), wc.printNames[index], wc.devNames[index],
                              v.second.get<string>("date"));
            hash = pClient.genSendData();

            /*
             * Add new user in database
             */
            try {
                db.addUser(v.second.get<unsigned>("userid"), v.second.get<string>("name"), wc.devNames[index], v.second.get<string>("date"),
                           hash);
            }
            catch (const string &err) {
                mtx.lock();
                _log->local(wc.devNames[index] + ": CheckUser: " + err, LOG_ERROR);
                _log->remote("CheckUser: " + err, LOG_ERROR, wc.devNames[index]);
                mtx.unlock();
                continue;
            }

            const auto &sc = _cfg->getServerCfg();
            try {
                pClient.connect(sc.ip, sc.port);
                mtx.lock();
                _isPrintErr[index] = false;
                mtx.unlock();
            }
            catch(const string &err) {
                mtx.lock();
                if (!_isPrintErr[index]) {
                    _isPrintErr[index] = true;
                    _log->local(wc.devNames[index] + ": PrintClient: " + err, LOG_ERROR);
                    _log->remote("PrintClient: " + err, LOG_ERROR, wc.devNames[index]);
                }
                mtx.unlock();
                continue;
            }
            try {
                pClient.sendData();
            }
            catch (const string &err) {
                mtx.lock();
                pClient.close();
                _log->local(wc.devNames[index] + ": PrintClient: " + err, LOG_ERROR);
                _log->remote("PrintClient: " + err, LOG_ERROR, wc.devNames[index]);
                mtx.unlock();
                continue;
            }
            pClient.close();
        }
    }
    db.close();
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
