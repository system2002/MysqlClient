#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <ctime>
#include <cstring>
#include <chrono>
#include "mysql/mysql.h"
#include <iostream>

enum numberMode :uint8_t
{
    nmNone,
    nmU8,   // unsigned  8 bit (1 byte)
    nmS8,   //   signed  8 bit (1 byte)
    nmU16,  // unsigned 16 bit (2 byte)
    nmS16,  //   signed 16 bit (2 byte)
    nmU32,  // unsigned 32 bit (3 byte)
    nmS32,  //   signed 32 bit (3 byte)
    nmU64,  // unsigned 64 bit (4 byte)
    nmS64,  //   signed 64 bit (4 byte)

    nmDouble,
    nmFloat,
    nmNewDemical,

    nmTime,
    nmString,

};

class Value
{
public:
    Value(const MYSQL_BIND& ABind);
    ~Value();

    std::string operator() (const std::string &ifNull = std::string()) const;
    std::string toString(const std::string &ifNull = std::string()) const;
    uint32_t    toUInt(const unsigned int &ifNull = 0) const;
    uint64_t    toUInt64(const unsigned int &ifNull = 0) const;
    int32_t     toInt(const int &ifNull = 0) const;
    int64_t     toInt64(const int &ifNull = 0) const;
    double      toDouble() const;
    float       toFloat() const;
    void        *toVoidPtr() const;
    MYSQL_TIME  toMySQLTime() const;
    std::time_t toCTime()const;

    std::chrono::time_point<std::chrono::system_clock> toChronoTimePoint() const;

    inline int8_t      fastInt8()   const { return *static_cast<int8_t*>  (m_buffer); }
    inline int16_t     fastInt16()  const { return *static_cast<int16_t*> (m_buffer); }
    inline int32_t     fastInt32()  const { return *static_cast<int32_t*> (m_buffer); }
    inline int64_t     fastInt64()  const { return *static_cast<int64_t*> (m_buffer); }
    inline uint8_t     fastUInt8()  const { return *static_cast<uint8_t*> (m_buffer); }
    inline uint16_t    fastUInt16() const { return *static_cast<uint16_t*>(m_buffer); }
    inline uint32_t    fastUInt32() const { return *static_cast<uint32_t*>(m_buffer); }
    inline uint64_t    fastUInt64() const { return *static_cast<uint64_t*>(m_buffer); }
    inline double      fastDouble() const { return *static_cast<double*>  (m_buffer); }
    inline float       fastFloat()  const { return *static_cast<float*>   (m_buffer); }
    inline MYSQL_TIME  fastMySQLTime() const {return *static_cast<MYSQL_TIME*>(m_buffer);}

    inline unsigned long    length()     const { return m_length; }
    inline unsigned long    size()       const { return m_length; }
    inline bool             isNull()     const { return *m_bind.is_null;}
    inline bool             isUnsigned() const { return m_bind.is_unsigned;}

private:

    const MYSQL_BIND        & m_bind;
    const unsigned long     & m_length;
    void                    * m_buffer;
    const enum_field_types  & m_type;
    const my_bool           & m_isUnsigned;

    numberMode m_nMode;

    std::string getString() const;
    time_t getCTime() const;

    std::chrono::time_point<std::chrono::system_clock> getChronoTimePoint() const;
//    typeReturn getType(const enum_field_types& AType) const;
};

#endif // VALUE_H
