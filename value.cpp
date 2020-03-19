#include "value.h"


Value::Value(const MYSQL_BIND& ABind):
    m_bind(ABind),
    m_length(*ABind.length),
    m_buffer(ABind.buffer),
    m_type(ABind.buffer_type)
{}

std::string Value::toString(const std::string &ifNull) const
{
    if (isNull()) return std::string(ifNull);
    switch (getType(m_type))
    {
        case trString: return getString(m_buffer, m_length); break;
        case trTime:
        {
            time_t t = getCTime(m_buffer);
            std::string str(std::ctime(&t));
            str.resize(m_length);
            return str;
        }
        case trInt: return std::to_string(getUInt64(m_buffer)); break;
        case trDouble : return std::to_string(toDouble()); break;
    }
    return std::string("NO STRING!");
}

uint32_t Value::toUInt(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    if (IS_NUM(m_type))
    {
        return getUInt32(m_buffer);
    }
    else return 0;
}

MYSQL_TIME Value::toMySQLTime() const
{
    if (    m_type == MYSQL_TYPE_TIMESTAMP||
            m_type == MYSQL_TYPE_DATE ||
            m_type == MYSQL_TYPE_DATETIME||
            m_type == MYSQL_TYPE_TIME)
    {
        return getMySQLTime(m_buffer);
    }
    return MYSQL_TIME {};
}

time_t Value::toCTime() const
{
    if (    m_type == MYSQL_TYPE_TIMESTAMP||
            m_type == MYSQL_TYPE_DATE ||
            m_type == MYSQL_TYPE_DATETIME||
            m_type == MYSQL_TYPE_TIME)
    {
        return getCTime(m_buffer);
    }
    return {};
}

std::chrono::time_point<std::chrono::system_clock> Value::toChronoTimePoint() const
{
    if (    m_type == MYSQL_TYPE_TIMESTAMP||
            m_type == MYSQL_TYPE_DATE ||
            m_type == MYSQL_TYPE_DATETIME||
            m_type == MYSQL_TYPE_TIME)
    {

        return getChronoTimePoint(m_buffer);
    }
    return {};
}

int32_t Value::toInt(const int &ifNull) const
{
    if (isNull()) return ifNull;
    if (m_type == MYSQL_TYPE_TINY       ||
        m_type == MYSQL_TYPE_SHORT      ||
        m_type == MYSQL_TYPE_LONG       ||
        m_type == MYSQL_TYPE_LONGLONG   ||
        m_type == MYSQL_TYPE_INT24      ||
        m_type == MYSQL_TYPE_YEAR       ||
        m_type == MYSQL_TYPE_BIT)
    {
        return getInt32(m_buffer);
    }
    else return {};
}

double Value::toDouble() const
{
    switch (m_type)
    {
        case MYSQL_TYPE_DOUBLE: return getDouble(m_buffer);                     break;
        case MYSQL_TYPE_FLOAT:  return static_cast<double>(getFloat(m_buffer)); break;
        default: return {};
    }
}

float Value::toFloat() const
{
    switch (m_type)
    {
        case MYSQL_TYPE_FLOAT:  return getFloat(m_buffer);                      break;
        case MYSQL_TYPE_DOUBLE: return static_cast<float>(getDouble(m_buffer)); break;
        default: return {};
    }
}

std::string Value::getString(void *ABuffer, unsigned long ALength)
{
    std::string result = std::string(static_cast<char*>(ABuffer));
    result.resize(ALength);
    return result;
}

int32_t Value::getInt32(void *ABuffer)
{
    return *static_cast<int32_t*>(ABuffer);
}

int64_t Value::getInt64(void *ABuffer)
{
    return *static_cast<int64_t*>(ABuffer);
}

uint32_t Value::getUInt32(void *ABuffer)
{
    return *static_cast<uint32_t*>(ABuffer);
}

uint64_t Value::getUInt64(void *ABuffer)
{
    return *static_cast<uint64_t*>(ABuffer);
}

MYSQL_TIME Value::getMySQLTime(void *ABuffer)
{
    return *static_cast<MYSQL_TIME*>(ABuffer);
}

time_t Value::getCTime(void *ABuffer)
{
    MYSQL_TIME mt = getMySQLTime(ABuffer);
    std::tm timeTm;
    timeTm.tm_year  = mt.year - 1900;
    timeTm.tm_mon   = mt.month - 1;
    timeTm.tm_mday  = mt.day;
    timeTm.tm_hour  = mt.hour;
    timeTm.tm_min   = mt.minute;
    timeTm.tm_sec   = mt.second;
    return mktime(&timeTm);
}

std::chrono::time_point<std::chrono::system_clock> Value::getChronoTimePoint(void *ABuffer)
{
    return std::chrono::system_clock::from_time_t(getCTime(ABuffer));
}

typeReturn Value::getType(const enum_field_types &AType)
{
    switch (AType)
    {
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_JSON:
            return trString;
            break;

        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_BIT:
            return trInt;
            break;

        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIME:
            return trTime;
            break;
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_FLOAT:
            return trDouble;
            break;

        default: return trString ;
    }
}

double Value::getDouble(void *ABuffer)
{
    return *static_cast<double*>(ABuffer);
}

float Value::getFloat(void *ABuffer)
{
    return *static_cast<float*>(ABuffer);
}
