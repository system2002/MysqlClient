#ifndef MQUERY_H
#define MQUERY_H
#include "tmc.h"
#include "vector"
#include <cstring>
#include <iostream>
#include "rowdata.h"
#include "value.h"
#include "bind.h"

class MQuery  : public tmc
{
public:

    MQuery();
    MQuery(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort = 3306);

    bool exec(const std::string & Aquery);
    bool next();
    //std::string lastErrorText() const;
    Value value(const unsigned int AColumn);



private:
    bool initSTMT();
    MYSQL_STMT * m_stmt;
    Bind m_bindbuffer;
};

#endif // MQUERY_H
