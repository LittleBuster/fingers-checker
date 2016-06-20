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

#include "notify.h"
#include <boost/date_time.hpp>
#include <curl/curl.h>


Notify::Notify(const shared_ptr<IConfigs> &cfg, const shared_ptr<ITcpClient> &client)
{
    _cfg = cfg;
    _client = client;
}

bool Notify::checkDevice(const string &deviceIP)
{
    try {
        _client->connect(deviceIP, 80);
        _client->close();
    }
    catch (const string &err) {
        return false;
    }
    return true;
}

void Notify::sendTelegram(const string &message)
{
    CURL *curl_handle;
    const auto &tc = _cfg->getTelegramCfg();
    string url = "https://api.telegram.org/bot" + tc.key + "/sendMessage?chat_id=" + boost::lexical_cast<string>(tc.id) +
                 "&text=" + message;

    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
        return;
    }
    throw string("Fail sending telegram.");
}
