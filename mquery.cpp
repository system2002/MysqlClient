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
    memset(m_bind, 0, sizeof(MYSQL_BIND) * 100);
    m_length.clear();
    m_length.resize(cc);
    m_rowData.setSize(cc);
    if (mysql_stmt_store_result(m_stmt)) return false;
    std::vector<InitBind> initBind;
    for (unsigned int i = 0; i < cc; i++)
    {
        unsigned long maxLen = column(i).maxLength();
        unsigned long len = column(i).length();
        //m_bind[i].buffer_type = column(i).type();
        InitBind ib;
        ib.type = column(i).type();
        switch (column(i).type())
        {
            case MYSQL_TYPE_STRING:
            case MYSQL_TYPE_VAR_STRING:
            case MYSQL_TYPE_VARCHAR:
            case MYSQL_TYPE_JSON:
                ib.bufferLength = maxLen;
                break;

            case MYSQL_TYPE_DOUBLE:
            case MYSQL_TYPE_LONGLONG:
                ib.bufferLength = 8; break;

            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_FLOAT:
                ib.bufferLength = 4; break;

            case MYSQL_TYPE_SHORT:
                ib.bufferLength = 2; break;

            case MYSQL_TYPE_TINY:
                ib.bufferLength = 1; break;

            case MYSQL_TYPE_TIME:
            case MYSQL_TYPE_DATETIME:
            case MYSQL_TYPE_DATE:
                ib.bufferLength = sizeof(MYSQL_TIME); break;

            default:
            {
                ib.bufferLength = len;
                break;
            }
        }
        initBind.push_back(ib);
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

    result = std::string(m_rowData.getBufferPtr(0));
    return result;
}

std::string MQuery::valueString(const unsigned int AColumn)
{
    std::string result;
    result = std::string(m_rowData.getBufferPtr(AColumn));
    result.resize(m_length[AColumn]);
    return result;
}

unsigned long MQuery::valueLength(const unsigned int AColumn)
{
    return m_length[AColumn];
}

Value MQuery::value(const unsigned int AColumn)
{
    //return Value(m_bind[AColumn]);
    return Value(m_bindbuffer(AColumn));
}

bool MQuery::initSTMT()
{
    m_stmt = mysql_stmt_init(m_mysql);
    //m_data = new int;
    return m_stmt != nullptr;
}
