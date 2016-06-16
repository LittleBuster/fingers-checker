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
#include <QDebug>


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
        boost::property_tree::read_xml(stream, propertyTree);

        bool isOk = true;
        for(const auto &v : propertyTree) {
            try {
                userName = v.second.get<string>("user");
                userKey = v.second.get<string>("userkey");
            } catch (...) {
                _log->local("Fail parsing Auth.xml", LOG_ERROR);
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
        boost::property_tree::read_xml(streamData, p2);
        const auto &p3 = p2.get_child("document.items");

        try {
            _db->open("C:\\Qt\\my.sqlite");
        } catch (const string &err) {
            _log->local(err, LOG_ERROR);
            break;
        }
        size_t checkCnt = 0;
        for (const auto &v : p3) {
            checkCnt++;

            bool res = _db->checkUser(v.second.get<unsigned>("userid"), v.second.get<string>("date"));
            if (!res) {
                try {
                    _db->addUser(v.second.get<unsigned>("userid"), v.second.get<string>("name"), v.second.get<string>("date"));
                } catch (const string &err) {
                    _log->local(err, LOG_ERROR);
                }
            }
            else {
                try {
                    const tuple<bool, unsigned> &retVal = _db->incUser(v.second.get<unsigned>("userid"), v.second.get<string>("date"));
                    if (get<0>(retVal) == true) {
                        //unique id get<1>(retVal)
                        //Printing check
                    }
                } catch (const string &err) {
                    _log->local(err, LOG_ERROR);
                }
            }

            if (checkCnt == 3)
                break;
        }
        _db->close();
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

Checker::Checker(const shared_ptr<IDatabase> &db, const shared_ptr<ILog> &log)
{
    _log = log;
    _db = db;
}

void Checker::start()
{
    _timer = make_shared<deadline_timer>(_io, boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&Checker::check, this));
    _io.run();
}
