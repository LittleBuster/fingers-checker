#include "helper.h"


string dateToNum(const boost::posix_time::ptime &time)
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

string dateTimeToNum(const boost::posix_time::ptime &time)
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

    out += "-" + d[2] + " " + dt[1];
    return out;
}
