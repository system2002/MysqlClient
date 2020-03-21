#ifndef BIND_H
#define BIND_H
#include <vector>
#include <cstring>
#include "mysql/mysql.h"
#include <ctime>


#include <string>
#include <ctime>
#include <cstring>
#include <chrono>
#include <vector>
#include "mysql/mysql.h"
#include <iostream>
#include "value.h"

struct InitBind
{
    enum_field_types type;
    unsigned long bufferLength;
    my_bool isUnsigned;
};

class Bind
{
public:
    Bind();
    bool prepareBind(const std::vector<InitBind>& AInit, std::vector<Value> & AValue);
    MYSQL_BIND & operator() (const unsigned int & column);

private:
    std::vector<MYSQL_BIND> m_bind;
    char * m_buffer;
    void clear();
};

#endif // BIND_H
