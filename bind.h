#ifndef BIND_H
#define BIND_H
#include <vector>
#include "mysql/mysql.h"

struct InitBind
{
    enum_field_types type;
    unsigned long bufferLength;
};

class Bind
{
public:
    Bind();
    bool prepareBind(const std::vector<InitBind>& AInit);
    MYSQL_BIND & operator() (const unsigned int & column);

private:
    std::vector<MYSQL_BIND> m_bind;
    std::vector<unsigned long> m_length;
    std::vector<char> m_dataIsNull;
    char * m_buffer;
    void clear();
};

#endif // BIND_H
