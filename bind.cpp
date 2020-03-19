#include "bind.h"

Bind::Bind():
    m_buffer(nullptr)

{

}

bool Bind::prepareBind(const std::vector<InitBind> &AInit)
{
    unsigned long count = AInit.size();
    if (!m_bind.empty()) clear();
    if (m_buffer != nullptr) return false;
    m_bind.resize(count);
    m_length.resize(count);
    m_dataIsNull.resize(count);

    unsigned long buffSize = 0;
    unsigned long &offset = buffSize;
    for (unsigned long i = 0; i < count; i++)
    {
        buffSize += AInit[i].bufferLength;
    }
    m_buffer = new char [buffSize];
    offset = 0;
    for (unsigned long i = 0; i < count; i++)
    {
        m_bind[i].is_null = &m_dataIsNull[i];
        m_bind[i].length = &m_length[i];
        m_bind[i].buffer = m_buffer + offset;
        m_bind[i].buffer_type = AInit[i].type;
        m_bind[i].buffer_length = AInit[i].bufferLength;
        offset += AInit[i].bufferLength;
    }
    return true;
}

MYSQL_BIND &Bind::operator()(const unsigned int &column)
{
    if (column < m_bind.size())
        return m_bind[column];
    else
        return m_bind[column % m_bind.size()];
}

void Bind::clear()
{
    m_bind.clear();
    m_length.clear();
    m_dataIsNull.clear();
    delete m_buffer;
    m_buffer = nullptr;
}
