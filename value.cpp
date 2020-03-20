#include "value.h"

static constexpr int SIZE32 = sizeof (int32_t);
static constexpr int SIZE64 = sizeof (int64_t);
static constexpr int SIZE_BYTE = sizeof (char);

Value::Value(const MYSQL_BIND& ABind):
    m_bind(ABind),
    m_length(*ABind.length),
    m_buffer(ABind.buffer),
    m_type(ABind.buffer_type),
    m_isUnsigned(ABind.is_unsigned)
{}

Value::~Value()
{
    for (char* ptr : m_autoremove)
    {
        delete ptr;
    }
}

std::string Value::operator()(const std::string &ifNull) const
{
    if (isNull()) return std::string(ifNull);
    switch (getType(m_type))
    {
        case trString: return getString(); break;
        case trTime:
        {
            time_t t = getCTime();
            unsigned int len = strlen(std::ctime(&t));
            std::string str(std::ctime(&t));
            str.resize(len-1);
            return str;
        }
        case trInt:
    {
        if (m_isUnsigned)
        {
            uint64_t result = getUInt64();
            int move = SIZE64 - size() * SIZE_BYTE;
            result = (result << move) >> move;
            return std::to_string(result); break;
        }
        else
        {
            int64_t result = getInt64();
            int move = SIZE64 - size() * SIZE_BYTE;
            result = (result << move) >> move;
            return std::to_string(result); break;
        }
    }
        case trDouble : return std::to_string(toDouble()); break;
    }
    return std::string("NO STRING!");
}

std::string Value::toString(const std::string &ifNull) const
{
    if (isNull()) return std::string(ifNull);
    if (getType(m_type) == trString)
    {
        return getString();
    }
    else return std::string("NO STRING!");
}

uint32_t Value::toUInt(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    if (IS_NUM(m_type))
    {
        return getUInt32();
    }
    else return 0;
}

uint64_t Value::toUInt64(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    uint64_t result;
    if (m_type == MYSQL_TYPE_TINY       ||
        m_type == MYSQL_TYPE_SHORT      ||
        m_type == MYSQL_TYPE_LONG       ||
        m_type == MYSQL_TYPE_LONGLONG   ||
        m_type == MYSQL_TYPE_INT24      ||
        m_type == MYSQL_TYPE_YEAR       ||
        m_type == MYSQL_TYPE_BIT)
    {
        result = getUInt64();
        if (size() == SIZE64)
        {
            return result;
        }
        else
        {
            int move = SIZE64 - size() * SIZE_BYTE;
            result = (result << move) >> move;
            return result;
        }
    }
    else return {};
}

MYSQL_TIME Value::toMySQLTime() const
{
    if (    m_type == MYSQL_TYPE_TIMESTAMP||
            m_type == MYSQL_TYPE_DATE ||
            m_type == MYSQL_TYPE_DATETIME||
            m_type == MYSQL_TYPE_TIME)
    {
        return getMySQLTime();
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
        return getCTime();
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
        return getChronoTimePoint();
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
        return getInt32();
    }
    else return {};
}

int64_t Value::toInt64(const int &ifNull) const
{
    if (isNull()) return ifNull;
    int64_t result;
    if (m_type == MYSQL_TYPE_TINY       ||
        m_type == MYSQL_TYPE_SHORT      ||
        m_type == MYSQL_TYPE_LONG       ||
        m_type == MYSQL_TYPE_LONGLONG   ||
        m_type == MYSQL_TYPE_INT24      ||
        m_type == MYSQL_TYPE_YEAR       ||
        m_type == MYSQL_TYPE_BIT)
    {
        result = getInt64();
        if (size() == SIZE64)
        {
            return result;
        }
        else
        {
            int move = SIZE64 - size() * SIZE_BYTE;
            result = result << move >> move;
            return result;
        }
    }
    else return {};
}

double Value::toDouble() const
{
    switch (m_type)
    {
        case MYSQL_TYPE_DOUBLE: return getDouble();
        case MYSQL_TYPE_FLOAT:  return static_cast<double>(getFloat());
        case MYSQL_TYPE_NEWDECIMAL:
        {
            char * begin = static_cast<char*>(m_buffer);
            char * end = begin + size();
            return std::strtod(begin, &end);
        }
        default: if (getType(m_type) == trInt)
        {
            return static_cast<double>(getInt64());
        }
        else return 0;
    }
}

float Value::toFloat() const
{
    switch (m_type)
    {
        case MYSQL_TYPE_FLOAT:  return getFloat();
        case MYSQL_TYPE_DOUBLE: return static_cast<float>(getDouble()); break;
        default:  if (getType(m_type) == trInt)
        {
            return static_cast<float>(getInt64());
        }
        else return 0;
    }
}

void *Value::toVoidPtr() const
{
    return m_buffer;
}

std::string Value::getString() const
{
    std::string result = std::string(static_cast<char*>(m_buffer));
    result.resize(m_length);
    return result;
}

int32_t Value::getInt32() const
{

    int32_t result = *static_cast<int32_t*>(m_buffer);
    if (size() < SIZE32)
    {
        int move = SIZE32 - size() * SIZE_BYTE;
        result = (result << move) >> move;
    }
    return result;
}

int64_t Value::getInt64() const
{
    return *static_cast<int64_t*>(m_buffer);
}

uint32_t Value::getUInt32() const
{
    return (*static_cast<uint32_t*>(m_buffer));
}

uint64_t Value::getUInt64() const
{
    return *static_cast<uint64_t*>(m_buffer);
}

MYSQL_TIME Value::getMySQLTime() const
{
    return *static_cast<MYSQL_TIME*>(m_buffer);
}

time_t Value::getCTime() const
{
    MYSQL_TIME mt = getMySQLTime();
    std::tm timeTm;
    timeTm.tm_year  = mt.year - 1900;
    timeTm.tm_mon   = mt.month - 1;
    timeTm.tm_mday  = mt.day;
    timeTm.tm_hour  = mt.hour;
    timeTm.tm_min   = mt.minute;
    timeTm.tm_sec   = mt.second;
    return mktime(&timeTm);
}

std::chrono::time_point<std::chrono::system_clock> Value::getChronoTimePoint() const
{
    return std::chrono::system_clock::from_time_t(getCTime());
}

typeReturn Value::getType(const enum_field_types &AType) const
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

double Value::getDouble() const
{
    return *static_cast<double*>(m_buffer);
}

float Value::getFloat() const
{
    return *static_cast<float*>(m_buffer);
}
