#include "mquery.h"
#include "tmc.h"

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
    if (mysql_stmt_prepare(m_stmt, Aquery.c_str(), Aquery.size()) != 0) return false;
    m_result = mysql_stmt_result_metadata(m_stmt);
    if (m_result == nullptr) return false;
    if (mysql_stmt_execute(m_stmt)) return false;
    my_bool setMax = true;
    mysql_stmt_attr_set(m_stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &setMax);
    const unsigned int cc = colCount();
    if (mysql_stmt_store_result(m_stmt)) return false;
    std::vector<InitBind> initBind(cc);
    for (unsigned int i = 0; i < cc; i++)
    {
        unsigned long maxLen = column(i).maxLength();
//        unsigned long len = column(i).length();
        initBind[i].type = column(i).type();
        initBind[i].isUnsigned = column(i).isUnsigned();
        initBind[i].bufferLength = tmc::sizeOfMyType(column(i).type());
        if (initBind[i].bufferLength == 0)
            initBind[i].bufferLength = maxLen;
    }
    if (!m_bindbuffer.prepareBind(initBind))
    {
        std::cout << "Error init bind" << std::endl;
        return false;
    }

    MYSQL_BIND * bind = &m_bindbuffer(0);

    if (mysql_stmt_bind_result(m_stmt, bind)) return false;
    return true;
}

bool MQuery::next()
{
    if (m_result == nullptr) return false;
    return mysql_stmt_fetch(m_stmt) != MYSQL_NO_DATA;
}

/*
std::string MQuery::lastErrorText() const
{
    std::string result;
    result = tmc::lastErrorText() + " | " + mysql_stmt_error(m_stmt);
    return result;
}
*/
Value MQuery::value(const unsigned int AColumn)
{
    if (AColumn < colCount())
    return Value(m_bindbuffer(AColumn));
    else return Value(m_bindbuffer(AColumn % colCount()));
}

bool MQuery::initSTMT()
{
    m_stmt = mysql_stmt_init(m_mysql);
    return m_stmt != nullptr;
}
