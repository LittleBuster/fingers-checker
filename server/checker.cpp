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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>


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
    string userName;
    string userKey;

    while (true) {
        /*
         * Reading Auth.xml
         */
        const tuple<string, bool> &auth = getData("http://galynsky.ru/auth.xml");
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
            _log->keys("EMPTY", "EMPTY");
            break;
        }

        bool isOk = true;
        for(const auto &v : propertyTree) {
            try {
                userName = v.second.get<string>("user");
                userKey = v.second.get<string>("userkey");
            } catch (...) {
                _log->local("Fail reading keys.", LOG_ERROR);
                _log->keys("EMPTY", "EMPTY");
                isOk = false;
                break;
            }
        }
        if (!isOk)
            break;

        cout << "Admin: " << userName << " Key: " << userKey << endl;
        _log->keys(userName, userKey);

        /*
         * Reading Data.xml
         */
        const tuple<string, bool> &data = getData("http://galynsky.ru/data.xml");
        if (get<1>(data) == true) {
            _log->local("Can not connect to Data.xml", LOG_ERROR);
            break;
        }
        if (get<0>(data) == "") {
            _log->local("Empty file Data.xml", LOG_ERROR);
            break;
        }
        stringstream streamData(get<0>(data));
        boost::property_tree::ptree p2;
        try {
            boost::property_tree::read_xml(streamData, p2);
        }
        catch(...) {
            _log->local("Fail reading data.", LOG_ERROR);
            break;
        }
        const auto &p3 = p2.get_child("document.items");

        unsigned i = 0;
        string info = "";
        for (const auto &v : p3) {
            info += " User" + boost::lexical_cast<string>(i) + ": [" + boost::lexical_cast<string>(v.second.get<unsigned>("userid"))
                    + "] ";
            i++;
        }
        _log->local(info, LOG_INFORMATION);
        break;
    }

    _timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&Checker::check, this));
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

Checker::Checker(const shared_ptr<ILog> &log)
{
    _log = log;
}

void Checker::start()
{
    _timer = make_shared<deadline_timer>(_io, boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&Checker::check, this));
    _io.run();
}
