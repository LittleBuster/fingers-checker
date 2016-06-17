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
#include <iostream>
#include <tuple>
#include <curl/curl.h>
#include <thread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>


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

    for (unsigned i = 0; i < DEV_COUNT; i++) {
        thread th(boost::bind(&Checker::checkDevice, this, wc.devices[i], wc.printers[i]));
        th.detach();
    }

    _timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&Checker::check, this));
}

void Checker::checkDevice(const string &devIp, const string &printer)
{
    string userName;
    string userKey;
    const auto &wc = _cfg->getWebCfg();

    while (true) {
        /*
         * Reading Auth.xml
         */
        const tuple<string, bool> &auth = getData("http://" + devIp + "/chk.cgi?userid=" + wc.username
                                                  + "&userpwd=" + wc.passwd);
        if (get<1>(auth) == true) {
            _log->local("Can not connect to Auth.xml", LOG_ERROR);
            break;
        }
        if (get<0>(auth) == "") {
            _log->local("Empty file Auth.xml", LOG_ERROR);
            break;
        }
        stringstream stream(get<0>(auth));
        boost::property_tree::ptree propertyTree;
        try {
            boost::property_tree::read_xml(stream, propertyTree);
        }
        catch(...) {
            _log->local("Fail reading keys.", LOG_ERROR);
            break;
        }

        bool isOk = true;
        for(const auto &v : propertyTree) {
            try {
                userName = v.second.get<string>("user");
                userKey = v.second.get<string>("userkey");
            } catch (...) {
                _log->local("Fail reading keys.", LOG_ERROR);
                isOk = false;
                break;
            }
        }
        if (!isOk)
            break;

        cout << "Admin: " << userName << " Key: " << userKey << endl;

        /*
         * Reading Data.xml
         */
        const string &nowdate = dateToNum(boost::posix_time::second_clock::local_time());
        const tuple<string, bool> &data = getData("http://" + devIp + "/query.cgi?userid=" + wc.username + "&sdate="
                                                  + nowdate + "&edate=" + nowdate + "&start=0&pagesize=3&user="
                                                  + userName + "&userkey=" + userKey);
        if (get<1>(data) == true) {
            _log->local("Can not connect to Data.xml", LOG_ERROR);
            break;
        }
        if (get<0>(data) == "") {
            _log->local("Empty file Data.xml", LOG_ERROR);
            break;
        }
        stringstream streamData(get<0>(data));
        boost::property_tree::ptree p2, p3;
        try {
            boost::property_tree::read_xml(streamData, p2);
            p3 = p2.get_child("document.items");
        }
        catch(...) {
            _log->local("Fail reading data.", LOG_ERROR);
            break;
        }

        for (const auto &v : p3) {
            v.second.get<unsigned>("userid");
            // check if user exists
        }
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

string Checker::dateToNum(const boost::posix_time::ptime &time)
{
    vector<string> dt;
    vector<string> d;

    const string &datetime = boost::lexical_cast<string>(time);
    boost::split(dt, datetime, boost::is_any_of(" "));
    boost::split(d, dt[0], boost::is_any_of("-"));

    string out = d[0] + "-";

    if (d[1] == "Jan")
        out += "1";
    if (d[1] == "Feb")
        out += "2";
    if (d[1] == "Mar")
        out += "3";
    if (d[1] == "Apr")
        out += "4";
    if (d[1] == "May")
        out += "5";
    if (d[1] == "Jun")
        out += "6";
    if (d[1] == "Jul")
        out += "7";
    if (d[1] == "Aug")
        out += "8";
    if (d[1] == "Sep")
        out += "9";
    if (d[1] == "Oct")
        out += "10";
    if (d[1] == "Nov")
        out += "11";
    if (d[1] == "Dec")
        out += "12";

    out += "-" + d[2];
    return out;
}

Checker::Checker(const shared_ptr<ILog> &log, const shared_ptr<IConfigs> &cfg)
{
    _log = log;
    _cfg = cfg;
}

void Checker::start()
{
    _timer = make_shared<deadline_timer>(_io, boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&Checker::check, this));
    _io.run();
}
