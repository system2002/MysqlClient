#include "value.h"

static constexpr int SIZE8  = sizeof (int8_t);
static constexpr int SIZE16 = sizeof (int16_t);
static constexpr int SIZE32 = sizeof (int32_t);
static constexpr int SIZE64 = sizeof (int64_t);
static constexpr int SIZE_BYTE = sizeof (char);

Value::Value(const MYSQL_BIND& ABind):
    m_bind(ABind),
    m_length(*ABind.length),
    m_buffer(ABind.buffer),
    m_type(ABind.buffer_type),
    m_isUnsigned(ABind.is_unsigned)
{
    switch (m_type)
    {
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_BIT:
        {
            if (m_isUnsigned)
            {
                switch (size())
                {
                    case SIZE64 : m_nMode = nmU64; break;
                    case SIZE32 : m_nMode = nmU32; break;
                    case SIZE16 : m_nMode = nmU16; break;
                    case SIZE8  : m_nMode = nmU8 ; break;
                }
                break;
            }
            else
            {
                switch (size())
                {
                    case SIZE64 : m_nMode = nmS64; break;
                    case SIZE32 : m_nMode = nmS32; break;
                    case SIZE16 : m_nMode = nmS16; break;
                    case SIZE8  : m_nMode = nmS8 ; break;
                }
                break;
            }
        }
        case MYSQL_TYPE_DOUBLE: m_nMode = nmDouble; break;
        case MYSQL_TYPE_FLOAT:  m_nMode = nmFloat ; break;
        case MYSQL_TYPE_NEWDECIMAL:  m_nMode = nmNewDemical ; break;
        default: m_nMode = nmNone;
    }
}

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
    switch (m_nMode)
    {
        case nmU64      : return static_cast<uint32_t>(getUInt64());
        case nmU32      : return static_cast<uint32_t>(getUInt32());
        case nmU16      : return static_cast<uint32_t>(getUInt16());
        case nmU8       : return static_cast<uint32_t>(getUInt8());
        case nmS64      : return static_cast<uint32_t>(getInt64());
        case nmS32      : return static_cast<uint32_t>(getInt32());
        case nmS16      : return static_cast<uint32_t>(getInt16());
        case nmS8       : return static_cast<uint32_t>(getInt8());

        case nmDouble   : return static_cast<uint32_t>(getDouble());
        case nmFloat    : return static_cast<uint32_t>(getFloat());
        case nmNewDemical : return 0;
        case nmNone     : return 0;
    }
}

uint64_t Value::toUInt64(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmU64      : return static_cast<uint64_t>(getUInt64());
        case nmU32      : return static_cast<uint64_t>(getUInt32());
        case nmU16      : return static_cast<uint64_t>(getUInt16());
        case nmU8       : return static_cast<uint64_t>(getUInt8());
        case nmS64      : return static_cast<uint64_t>(getInt64());
        case nmS32      : return static_cast<uint64_t>(getInt32());
        case nmS16      : return static_cast<uint64_t>(getInt16());
        case nmS8       : return static_cast<uint64_t>(getInt8());

        case nmDouble   : return static_cast<uint64_t>(getDouble());
        case nmFloat    : return static_cast<uint64_t>(getFloat());
        case nmNewDemical : return 0;
        case nmNone     : return 0;
    }
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
    switch (m_nMode)
    {
        case nmS64      : return static_cast<int32_t>(getInt64());
        case nmS32      : return static_cast<int32_t>(getInt32());
        case nmS16      : return static_cast<int32_t>(getInt16());
        case nmS8       : return static_cast<int32_t>(getInt8());
        case nmU64      : return static_cast<int32_t>(getUInt64());
        case nmU32      : return static_cast<int32_t>(getUInt32());
        case nmU16      : return static_cast<int32_t>(getUInt16());
        case nmU8       : return static_cast<int32_t>(getUInt8());

        case nmDouble   : return static_cast<int32_t>(getDouble());
        case nmFloat    : return static_cast<int32_t>(getFloat());
        case nmNewDemical : return 0;
        case nmNone     : return 0;
    }
}

int64_t Value::toInt64(const int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmS64      : return static_cast<int64_t>(getInt64());
        case nmS32      : return static_cast<int64_t>(getInt32());
        case nmS16      : return static_cast<int64_t>(getInt16());
        case nmS8       : return static_cast<int64_t>(getInt8());
        case nmU64      : return static_cast<int64_t>(getUInt64());
        case nmU32      : return static_cast<int64_t>(getUInt32());
        case nmU16      : return static_cast<int64_t>(getUInt16());
        case nmU8       : return static_cast<int64_t>(getUInt8());

        case nmDouble   : return static_cast<int64_t>(getDouble());
        case nmFloat    : return static_cast<int64_t>(getFloat());
        case nmNewDemical : return 0;
        case nmNone     : return 0;
    }
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

int8_t Value::getInt8() const
{
    return *static_cast<int8_t*>(m_buffer);
}

int16_t Value::getInt16() const
{
    return *static_cast<int16_t*>(m_buffer);
}

int32_t Value::getInt32() const
{
    /*
    int32_t result = *static_cast<int32_t*>(m_buffer);
    if (size() < SIZE32)
    {
        int move = SIZE32 - size() * SIZE_BYTE;
        result = (result << move) >> move;
    }
    return result;
    */
    return *static_cast<int32_t*>(m_buffer);
}

int64_t Value::getInt64() const
{
    return *static_cast<int64_t*>(m_buffer);
}

uint8_t Value::getUInt8() const
{
    return (*static_cast<uint8_t*>(m_buffer));
}

uint16_t Value::getUInt16() const
{
    return (*static_cast<uint8_t*>(m_buffer));
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
