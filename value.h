#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <ctime>
#include <cstring>
#include <chrono>
#include "mysql/mysql.h"

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
    std::string toString(const std::string &ifNull = std::string()) const;
    uint32_t toUInt(const unsigned int &ifNull = 0) const;
    int32_t toInt(const int &ifNull = 0) const;
    double toDouble() const;
    float toFloat() const;

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

    static std::string getString(void* ABuffer, unsigned long ALength);
    static int32_t getInt32(void* ABuffer);
    static int64_t getInt64(void* ABuffer);
    static uint32_t getUInt32(void* ABuffer);
    static uint64_t getUInt64(void* ABuffer);
    static MYSQL_TIME getMySQLTime(void* ABuffer);
    static time_t getCTime(void* ABuffer);
    static std::chrono::time_point<std::chrono::system_clock> getChronoTimePoint(void* ABuffer);
    static typeReturn getType(const enum_field_types& AType);
    static double getDouble(void* ABuffer);
    static float getFloat(void* ABuffer);
};

#endif // VALUE_H
