#include "value.h"

static constexpr unsigned int SIZE8  = sizeof (int8_t);
static constexpr unsigned int SIZE16 = sizeof (int16_t);
static constexpr unsigned int SIZE32 = sizeof (int32_t);
static constexpr unsigned int SIZE64 = sizeof (int64_t);
           const char*        TIME_FORMAT = "%04d.%02d.%02d %02d:%02d:%02d"; //YYYY.MM.DD HH:mm:ss
static constexpr unsigned int TIME_FORMAT_SIZE = 19; // = strlen(TIME_FORMAT)

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
        case MYSQL_TYPE_DOUBLE:         m_nMode = nmDouble; break;
        case MYSQL_TYPE_FLOAT:          m_nMode = nmFloat ; break;
        case MYSQL_TYPE_NEWDECIMAL:     m_nMode = nmNewDemical ; break;

        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIME:
            m_nMode = nmTime; break;

        /*
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_JSON:
            m_nMode = nmString; break;
        */
        default: m_nMode = nmString;
    }
}

Value::~Value()
{}

std::string Value::operator()(const std::string &ifNull) const
{
    return toString(ifNull);
}

std::string Value::toString(const std::string &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmString :
            return getString();

        case nmU64      :
            return std::to_string(fastUInt64());

        case nmS64      :
        case nmU32      :
        case nmS32      :
        case nmU16      :
        case nmU8       :
        case nmS16      :
        case nmS8       :
            return std::to_string(toInt64());

        case nmDouble       :
        case nmNewDemical   :
        case nmFloat        :
            return std::to_string(fastFloat());

        case nmTime:
    {
            const MYSQL_TIME t = fastMySQLTime();
            char tstr[TIME_FORMAT_SIZE];
            sprintf(tstr, TIME_FORMAT, t.year, t.month, t.day, t.hour, t.minute, t.second);
            return std::string(tstr);
    }

        default : return 0;
    }

    /*
    if (isNull()) return std::string(ifNull);
    if (getType(m_type) == trString)
    {
        return getString();
    }
    else return std::string("NO STRING!");
    */
}

uint32_t Value::toUInt(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmU64      : return static_cast<uint32_t>(fastUInt64());
        case nmU32      : return static_cast<uint32_t>(fastUInt32());
        case nmU16      : return static_cast<uint32_t>(fastUInt16());
        case nmU8       : return static_cast<uint32_t>(fastUInt8());
        case nmS64      : return static_cast<uint32_t>(fastInt64());
        case nmS32      : return static_cast<uint32_t>(fastInt32());
        case nmS16      : return static_cast<uint32_t>(fastInt16());
        case nmS8       : return static_cast<uint32_t>(fastInt8());

        case nmDouble   : return static_cast<uint32_t>(fastDouble());
        case nmFloat    : return static_cast<uint32_t>(fastFloat());
        default : return 0;
    }
}

uint64_t Value::toUInt64(const unsigned int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmU64      : return static_cast<uint64_t>(fastUInt64());
        case nmU32      : return static_cast<uint64_t>(fastUInt32());
        case nmU16      : return static_cast<uint64_t>(fastUInt16());
        case nmU8       : return static_cast<uint64_t>(fastUInt8());
        case nmS64      : return static_cast<uint64_t>(fastInt64());
        case nmS32      : return static_cast<uint64_t>(fastInt32());
        case nmS16      : return static_cast<uint64_t>(fastInt16());
        case nmS8       : return static_cast<uint64_t>(fastInt8());

        case nmDouble   : return static_cast<uint64_t>(fastDouble());
        case nmFloat    : return static_cast<uint64_t>(fastFloat());
        default : return 0;
    }
}

MYSQL_TIME Value::toMySQLTime() const
{
    if (m_nMode == nmTime)
    {
        return fastMySQLTime();
    }
    else return {};
}

time_t Value::toCTime() const
{
    if (m_nMode == nmTime)
    {
        return getCTime();
    }
    else return {};
}

std::chrono::time_point<std::chrono::system_clock> Value::toChronoTimePoint() const
{
    if (m_nMode == nmTime)
    {
        return getChronoTimePoint();
    }
    else return {};
}

int32_t Value::toInt(const int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmS64      : return static_cast<int32_t>(fastInt64());
        case nmS32      : return static_cast<int32_t>(fastInt32());
        case nmS16      : return static_cast<int32_t>(fastInt16());
        case nmS8       : return static_cast<int32_t>(fastInt8());
        case nmU64      : return static_cast<int32_t>(fastUInt64());
        case nmU32      : return static_cast<int32_t>(fastUInt32());
        case nmU16      : return static_cast<int32_t>(fastUInt16());
        case nmU8       : return static_cast<int32_t>(fastUInt8());

        case nmDouble   : return static_cast<int32_t>(fastDouble());
        case nmFloat    : return static_cast<int32_t>(fastFloat());
        default : return 0;
    }
}

int64_t Value::toInt64(const int &ifNull) const
{
    if (isNull()) return ifNull;
    switch (m_nMode)
    {
        case nmS64      : return static_cast<int64_t>(fastInt64());
        case nmS32      : return static_cast<int64_t>(fastInt32());
        case nmS16      : return static_cast<int64_t>(fastInt16());
        case nmS8       : return static_cast<int64_t>(fastInt8());
        case nmU64      : return static_cast<int64_t>(fastUInt64());
        case nmU32      : return static_cast<int64_t>(fastUInt32());
        case nmU16      : return static_cast<int64_t>(fastUInt16());
        case nmU8       : return static_cast<int64_t>(fastUInt8());

        case nmDouble   : return static_cast<int64_t>(fastDouble());
        case nmFloat    : return static_cast<int64_t>(fastFloat());
        default : return 0;
    }
}

double Value::toDouble() const
{
    if (isNull()) return 0;
    switch (m_nMode)
    {
        case nmDouble   : return                    (fastDouble());
        case nmFloat    : return static_cast<double>(fastFloat());
        case nmS64      : return static_cast<double>(fastInt64());
        case nmS32      : return static_cast<double>(fastInt32());
        case nmS16      : return static_cast<double>(fastInt16());
        case nmS8       : return static_cast<double>(fastInt8());
        case nmU64      : return static_cast<double>(fastUInt64());
        case nmU32      : return static_cast<double>(fastUInt32());
        case nmU16      : return static_cast<double>(fastUInt16());
        case nmU8       : return static_cast<double>(fastUInt8());

        case nmNewDemical :
        {
            char * begin = static_cast<char*>(m_buffer);
            char * end = begin + size();
            return std::strtod(begin, &end);
        }
        default         : return 0;
    }
}

float Value::toFloat() const
{
    if (isNull()) return {};
    switch (m_nMode)
    {
        case nmDouble   : return static_cast<float>(fastDouble());
        case nmFloat    : return                   (fastFloat());
        case nmS64      : return static_cast<float>(fastInt64());
        case nmS32      : return static_cast<float>(fastInt32());
        case nmS16      : return static_cast<float>(fastInt16());
        case nmS8       : return static_cast<float>(fastInt8());
        case nmU64      : return static_cast<float>(fastUInt64());
        case nmU32      : return static_cast<float>(fastUInt32());
        case nmU16      : return static_cast<float>(fastUInt16());
        case nmU8       : return static_cast<float>(fastUInt8());

        case nmNewDemical :
        {
            char * begin = static_cast<char*>(m_buffer);
            char * end = begin + size();
            return std::strtof(begin, &end);
        }
        default         : return {};
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


time_t Value::getCTime() const
{
    MYSQL_TIME mt = fastMySQLTime();
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
