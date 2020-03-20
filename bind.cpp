#include "bind.h"

static constexpr unsigned int LENGTH_SIZE = sizeof (unsigned long);
static constexpr unsigned int IS_NULL_SIZE = sizeof (my_bool);

Bind::Bind():
    m_buffer(nullptr)
{}

bool Bind::prepareBind(const std::vector<InitBind> &AInit)
{
    const unsigned long count = AInit.size();
    if (!m_bind.empty()) clear();
    if (m_buffer != nullptr) return false;
    m_bind.resize(count);
    unsigned long buffSize = 0;
    unsigned long &offset = buffSize;
    for (unsigned long i = 0; i < count; i++)
    {
        buffSize += (LENGTH_SIZE + IS_NULL_SIZE + AInit[i].bufferLength);
    }
    m_buffer = new char [buffSize];
    memset(m_buffer, 0, buffSize);
    offset = 0;
    for (unsigned long i = 0; i < count; i++)
    {
        m_bind[i].length = reinterpret_cast<unsigned long*>(m_buffer + offset);
        offset += LENGTH_SIZE;
        m_bind[i].is_null = reinterpret_cast<my_bool*>(m_buffer + offset);
        offset += IS_NULL_SIZE;
        m_bind[i].buffer = m_buffer + offset;
        offset += AInit[i].bufferLength;
        m_bind[i].buffer_type = AInit[i].type;
        m_bind[i].buffer_length = AInit[i].bufferLength;
        m_bind[i].is_unsigned = AInit[i].isUnsigned;
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
    delete [] m_buffer;
    m_buffer = nullptr;
}
