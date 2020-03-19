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
    std::string lastErrorText() const;
    std::string rowString();
    std::string valueString(const unsigned int AColumn);
    unsigned long valueLength(const unsigned int AColumn);
    Value value(const unsigned int AColumn);



private:
    bool initSTMT();
    MYSQL_STMT * m_stmt;
    MYSQL_BIND m_bind[100];
    char m_nullData[100];
    Bind m_bindbuffer;

    std::vector<unsigned long> m_length;
    std::vector<char> m_buffer;
    rowData m_rowData;
    char * m_data;
    char m_text[256];
};

#endif // MQUERY_H
