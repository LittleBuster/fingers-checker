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

#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;


/**
 * Conver boost datetime to string date
 * @time: boost datetime
 */
string dateToNum(const boost::posix_time::ptime &time);

/**
 * Conver boost datetime to string datetime
 * @time: boost datetime
 */
string dateTimeToNum(const boost::posix_time::ptime &time);


#endif
