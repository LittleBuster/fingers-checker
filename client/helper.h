#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;


string dateToNum(const boost::posix_time::ptime &time);

string dateTimeToNum(const boost::posix_time::ptime &time);


#endif
