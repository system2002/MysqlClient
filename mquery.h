#ifndef MQUERY_H
#define MQUERY_H
#include "tmc.h"
#include "vector"
#include <cstring>
#include <iostream>
#include "rowdata.h"
#include "value.h"
#include "bind.h"
#include <string>
#include <set>
#include <regex>

class MQuery  : public tmc
{
public:

    MQuery();
    MQuery(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort = 3306);

    bool exec(const std::string & Aquery);
    bool next();
    Value value(const unsigned int AColumn);
    static std::set<std::string> strToSet(const std::string & AStr);



private:
    bool initSTMT();
    MYSQL_STMT * m_stmt;
    Bind m_bindbuffer;
    std::vector<Value> m_value;
};

#endif // MQUERY_H
