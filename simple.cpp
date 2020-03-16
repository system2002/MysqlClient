#include "simple.h"

Simple::Simple():
    tmc()
{}

Simple::Simple(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort):
    tmc(AHost, AUser, APasswd, ADb, APort)
{}

bool Simple::exec(const std::string &Aquery)
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

bool Simple::next()
{
    if (m_result == nullptr) return false;
    m_currRow = mysql_fetch_row(m_result);
    return m_currRow != nullptr;
}

const char *Simple::valuePtr(const unsigned int &AColumn) const
{
    if (m_result == nullptr) return nullptr;
    const unsigned int& columnCount = mysql_num_fields(m_result);
    if (AColumn < columnCount)
    {
        return valueForUser(m_currRow[AColumn]);
    }
    else return nullptr;
}

char **Simple::row() const
{
    if (m_result == nullptr) return nullptr;
    return m_currRow;
}

std::string Simple::valueStr(const unsigned int &AColumn) const
{
    if (m_result == nullptr) return std::string();
    const unsigned int& columnCount = mysql_num_fields(m_result);
    if (AColumn < columnCount)
    {
        return std::string(valueForUser(m_currRow[AColumn]));
    }
    else return std::string("Out of range");;
}

std::vector<std::string> Simple::rowVS() const
{
    const uint32_t & cc = colCount();
    std::vector<std::string> result;
    result.reserve(cc);
    for (uint32_t i = 0; i < cc; i++)
    {
        result.emplace_back(valueForUser(m_currRow[i]));
    }
    return result;
}

std::map<std::string, std::string> Simple::rowMS() const
{

    const uint32_t & cc = colCount();
    std::map<std::string, std::string> result;

    for (uint32_t i = 0; i < cc; i++)
    {
        const std::string & currColName = column(i).name();
        result[currColName] = std::string(valueForUser(m_currRow[i]));
    }

    return result;

}

std::vector<std::vector<std::string> > Simple::getAllrowString()
{
    std::vector<std::vector<std::string> > result;
    const uint32_t & cc = colCount();
    while (next())
    {
        std::vector<std::string> v;
        v.reserve(cc);
        for (uint32_t i = 0; i < cc; i++)
        {
            v.emplace_back(valueForUser(m_currRow[i]));
        }
        result.emplace_back(v);
    }
    return result;
}

const char *Simple::valueForUser(char *value)
{
    if (value != nullptr)   return value;
    else                    return NULL_VALUE;
}

/*
std::string Simple::value(const unsigned int &AColumn) const
{
    if (m_result == nullptr) return "";
    const unsigned int columnCount = mysql_num_fields(m_result);
    if (AColumn < columnCount)
    {
        return std::string(m_currRow[AColumn]);
    }
    else return "Out of range!";
}
*/
