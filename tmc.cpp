#include "tmc.h"
tmc::tmc():
    m_mysql(nullptr),
    m_result(nullptr)
{
    m_mysql = mysql_init(nullptr);
    setCharset("utf8");
}

tmc::tmc(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort):
    tmc()
{
    connect(AHost, AUser, APasswd, ADb, APort);
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
    /*
    if (!isConnected()) return 0;
    if (exec("SHOW GLOBAL STATUS LIKE 'Uptime';") && (next()))
    {
        return std::stol(value(1));
    }
    else return 0;
    */
}

int tmc::setCharset(const std::string &ACharsetName)
{
    return mysql_set_character_set(m_mysql, ACharsetName.c_str());
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
    if (!isConnected()) return 0;
    return mysql_insert_id(m_mysql);
}

TColumn tmc::column(const unsigned int &AColumn) const
{
    return TColumn(mysql_fetch_field_direct(m_result, AColumn));
}

bool tmc::isConnected() const
{
    return m_isConnected;
}

unsigned int tmc::sizeOfMyType(const enum_field_types &AType)
{
    unsigned int result;
    switch (AType)
    {
        /*
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_JSON:
            result = 0;
            break;
        */
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_LONGLONG:
            result = 8; break;

        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_FLOAT:
        case MYSQL_TYPE_INT24:
            result = 4; break;

        case MYSQL_TYPE_SHORT:
            result = 2; break;

        case MYSQL_TYPE_TINY:
            result = 1; break;

        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_DATE:
            result = sizeof(MYSQL_TIME); break;
        default:
        {
            result = 0; break;
        }
     }
    return result;
}

