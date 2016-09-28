#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>
#include <sstream>

#include <QString>
#include <QDebug>

using namespace std;


namespace StringHelper
{
bool isFloat(const string & str);


template<typename T>
bool fromString(const string & str, T& dest)
{
    // creer un flux Ã  partir de la chaÃ®ne donnee
    istringstream iss(str);
    // tenter la conversion vers Dest
    return iss >> dest != 0;
}


template <class T>
string toString (const T& t)
{
    stringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
QString toQString (const T& t)
{
    QString str;
    QDebug(&str) << t;
    return str;
}
}

#endif //STRINGHELPER_H
