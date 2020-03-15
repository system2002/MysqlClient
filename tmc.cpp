#include "tmc.h"


tmc::tmc():
    m_mysql(nullptr),
    m_result(nullptr)
{
    m_mysql = mysql_init(nullptr);
}

tmc::~tmc()
{
    mysql_free_result(m_result);
    mysql_close(m_mysql);
}

std::string tmc::clientInfo()
{
    return std::string().append(mysql_get_client_info());
}

bool tmc::connect(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort)
{
    if (mysql_real_connect(m_mysql, AHost.c_str(), AUser.c_str(), APasswd.c_str(), ADb.c_str(), APort, nullptr, 0) != nullptr)
    {
        m_isConnected = true;
    }
    else
    {
        m_isConnected = false;
    }
    return isConnected();
}

bool tmc::changeUser(const std::string &AUser, const std::string &APasswd, const std::string &ADb)
{
    if (!isConnected()) return false;
    if (mysql_change_user(m_mysql, AUser.c_str(), APasswd.c_str(), ADb.c_str()))
    {
        return true;
    }
    else
    {
        m_isConnected = false;
        return false;
    }
}

bool tmc::changeBase(const std::string &ADb)
{
    if (!isConnected()) return false;
    if (mysql_select_db(m_mysql, ADb.c_str()) != 0 )
    {
        return true;
    }
    else
    {
        return false;
    }

}

std::string tmc::info() const
{
    if (mysql_info(m_mysql) != nullptr)
    {
        return std::string().append(mysql_info(m_mysql));
    }
    return "";
}

std::string tmc::lastErrorText() const
{
    return std::string().append(mysql_error(m_mysql));
}

std::string tmc::getServerStatus() const
{
    return std::string().append(mysql_stat(m_mysql));
}

std::string tmc::getServerInfo() const
{
    if (isConnected())
    {
        return std::string().append(mysql_get_server_info(m_mysql));
    }
    return std::string();

}

my_ulonglong tmc::serverUptime()
{
    if (!isConnected()) return 0;
    if (exec("SHOW GLOBAL STATUS LIKE 'Uptime';") && (next()))
    {
        return std::stol(value(1));
    }
    else return 0;
}

std::string tmc::value(const unsigned int &AColumn) const
{
    if (m_result == nullptr) return "";
    const unsigned int columnCount = mysql_num_fields(m_result);
    if (AColumn < columnCount)
    {
        return std::string(m_currRow[AColumn]);
    }
    else return "Out of range!";
}

unsigned int tmc::colCount() const
{

    if (m_result == nullptr || !isConnected()) return 0;
    else return mysql_num_fields(m_result);
}

unsigned int tmc::affected() const
{
    return mysql_affected_rows(m_mysql);
}

my_ulonglong tmc::insertId() const
{
    return mysql_insert_id(m_mysql);
}

/*
MYSQL_FIELD *tmc::columnPtr(const unsigned int &AColumn) const
{
    if (m_result == nullptr || !isConnected() || AColumn >= colCount()) return nullptr;
    else return mysql_fetch_field_direct(m_result, AColumn);
}
*/

TColumn tmc::column(const unsigned int &AColumn) const
{
    return TColumn(mysql_fetch_field_direct(m_result, AColumn));
}

bool tmc::exec(const std::string &Aquery)
{
    if (!isConnected()) return false;
    const char * sql = Aquery.c_str();
    unsigned long length = Aquery.size();
    int result = mysql_real_query(m_mysql, sql, length);
    if (result == 0)
    {
        if (m_result != nullptr)
        {
            mysql_free_result(m_result);
        }
        m_result = mysql_store_result(m_mysql);
        return true;
    }
    else
    {
        switch (result)
        {
            case CR_SERVER_GONE_ERROR:  m_isConnected = false; break;
            case CR_SERVER_LOST:        m_isConnected = false; break;
            default: break;
        }
        return false;
    }
}

bool tmc::next()
{
    if (m_result == nullptr) return false;
    m_currRow = mysql_fetch_row(m_result);
    return m_currRow != nullptr;

}

bool tmc::isConnected() const
{
    return m_isConnected;
}

