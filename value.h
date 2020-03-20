#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <ctime>
#include <cstring>
#include <chrono>
#include <vector>
#include "mysql/mysql.h"
#include <iostream>

enum typeReturn : uint8_t
{
    trString,
    trInt,
    trTime,
    trDouble

};

class Value
{
public:
    Value(const MYSQL_BIND& ABind);
    ~Value();

    std::string operator() (const std::string &ifNull = std::string()) const;
    std::string toString(const std::string &ifNull = std::string()) const;
    uint32_t toUInt(const unsigned int &ifNull = 0) const;
    uint64_t toUInt64(const unsigned int &ifNull = 0) const;
    int32_t toInt(const int &ifNull = 0) const;
    int64_t toInt64(const int &ifNull = 0) const;
    double toDouble() const;
    float toFloat() const;
    void *toVoidPtr() const;

    inline unsigned long length() const         { return m_length; }
    inline unsigned long size() const           { return m_length; }
    inline bool isNull() const                  { return *m_bind.is_null;}
    inline bool isUnsigned() const              { return m_bind.is_unsigned;}

    MYSQL_TIME toMySQLTime() const;
    std::time_t toCTime()const;
    std::chrono::time_point<std::chrono::system_clock> toChronoTimePoint() const;

private:
    const MYSQL_BIND& m_bind;
    const unsigned long &m_length;
    void* m_buffer;
    const enum_field_types& m_type;
    const my_bool& m_isUnsigned;
    std::vector<char*> m_autoremove;

    std::string getString() const;
    int32_t getInt32() const;
    int64_t getInt64() const;
    uint32_t getUInt32() const;
    uint64_t getUInt64() const;
    MYSQL_TIME getMySQLTime() const;
    time_t getCTime() const;
    std::chrono::time_point<std::chrono::system_clock> getChronoTimePoint() const;
    typeReturn getType(const enum_field_types& AType) const;
    double getDouble() const;
    float getFloat() const;
};

#endif // VALUE_H
