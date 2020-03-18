#include "mquery.h"

MQuery::MQuery():
    tmc()
{
    initSTMT();
}

MQuery::MQuery(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort):
    tmc(AHost, AUser, APasswd, ADb, APort)
{
    initSTMT();
}

bool MQuery::exec(const std::string &Aquery)
{
    if (!isConnected()) return false;
    const char* sql = Aquery.c_str();
    if (mysql_stmt_prepare(m_stmt, sql, Aquery.size()) != 0) return false;
    m_result = mysql_stmt_result_metadata(m_stmt);
    if (m_result == nullptr) return false;
    if (mysql_stmt_execute(m_stmt)) return false;
    my_bool setMax = true;
    mysql_stmt_attr_set(m_stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &setMax);
    const unsigned int cc = colCount();
    memset(m_bind, 0, sizeof(MYSQL_BIND) * 10);
    m_length.clear();
    m_length.resize(cc);
    if (mysql_stmt_store_result(m_stmt)) return false;
    for (unsigned int i = 0; i < cc; i++)
    {
        m_bind[i].buffer_type = MYSQL_TYPE_STRING;
        unsigned long maxLen = column(i).maxLength();
        m_bind[i].buffer_length = maxLen;
        m_data = new char (maxLen);
        m_bind[i].buffer = m_data;
        m_bind[i].length = &m_length[i];
    }
    if (mysql_stmt_bind_result(m_stmt, m_bind)) return false;
    return true;
}

bool MQuery::next()
{
    if (m_result == nullptr) return false;
    return mysql_stmt_fetch(m_stmt) != MYSQL_NO_DATA;
}

std::string MQuery::lastErrorText() const
{
    std::string result;
    result = tmc::lastErrorText() + " | " + mysql_stmt_error(m_stmt);
    return result;
}

std::string MQuery::rowString()
{
    std::string result;
    /*
    const unsigned int cc = m_bind.size();
    //result = std::to_string( *(int*)m_bind[0].buffer);
    unsigned long len = *m_bind[0].length;
    result = std::to_string(len);
    */
    /*
    unsigned long len = m_length[0];
    std::cout <<"len"<< len << std::endl;
    char* data = new char (len * 2 + 1);
    m_bind[0].buffer = m_data;
    */
    //mysql_stmt_fetch_column(m_stmt, m_bind, 0, 0);

    //std::cout <<"fetch column:"<< mysql_stmt_fetch_column(m_stmt, m_bind, 0, 0) << std::endl;

    //std::cout << std::string(m_data);
    //std::cout << std::string("абвгд").data() << std::endl;
    result = std::string(m_data);
    return result;
}

bool MQuery::initSTMT()
{
    m_stmt = mysql_stmt_init(m_mysql);
    //m_data = new int;
    return m_stmt != nullptr;
}
