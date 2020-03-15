﻿#ifndef TMC_H
#define TMC_H
#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#include "column.h"
#include <string>

class tmc
{
public:
    tmc();
    ~tmc();

    static std::string clientInfo();
    bool connect(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort = 3306);
    bool changeUser(const std::string &AUser, const std::string &APasswd, const std::string &ADb);
    bool changeBase(const std::string &ADb);
    std::string info() const;
    std::string lastErrorText() const;
    std::string getServerStatus() const;
    std::string getServerInfo() const;
    my_ulonglong serverUptime();

    std::string value(const unsigned int & AColumn) const;
    unsigned int colCount() const;
    unsigned int affected() const;
    my_ulonglong insertId() const;
    TColumn column(const unsigned int & AColumn) const;


    bool exec(const std::string & Aquery);
    bool next();

    bool isConnected() const;


private:
    bool m_isConnected;
    MYSQL * m_mysql;
    MYSQL_RES * m_result;
    MYSQL_ROW m_currRow;


};

#endif // TMC_H
