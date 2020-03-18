#ifndef MQUERY_H
#define MQUERY_H
#include "tmc.h"
#include "vector"
#include <cstring>
#include <iostream>

class MQuery  : public tmc
{
public:

    MQuery();
    MQuery(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort = 3306);

    bool exec(const std::string & Aquery);
    bool next();
    std::string lastErrorText() const;
    std::string rowString();


private:
    bool initSTMT();
    MYSQL_STMT * m_stmt;
    MYSQL_BIND m_bind[10];

    std::vector<unsigned long> m_length;
    std::vector<char> m_buffer;
    char * m_data;
    char m_text[256];
};

#endif // MQUERY_H
