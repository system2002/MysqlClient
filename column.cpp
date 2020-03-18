#include "column.h"

TColumn::TColumn(MYSQL_FIELD *AColumn) :
    m_column(AColumn)
{

}

enum_field_types TColumn::type() const
{
    return m_column->type;
}

std::string TColumn::typeString(const enum_field_types &AType)
{
    switch (AType)
    {
        case MYSQL_TYPE_DECIMAL:        return std::string("Demical");
        case MYSQL_TYPE_TINY:           return std::string("Tiny");
        case MYSQL_TYPE_SHORT:          return std::string("Short");
        case MYSQL_TYPE_LONG:           return std::string("Long");
        case MYSQL_TYPE_FLOAT:          return std::string("Float");
        case MYSQL_TYPE_DOUBLE:         return std::string("Double");
        case MYSQL_TYPE_NULL:           return std::string("Null");
        case MYSQL_TYPE_TIMESTAMP:      return std::string("Time stamp");
        case MYSQL_TYPE_LONGLONG:       return std::string("Long long");
        case MYSQL_TYPE_INT24:          return std::string("Int24");
        case MYSQL_TYPE_DATE:           return std::string("Date");
        case MYSQL_TYPE_TIME:           return std::string("Time");
        case MYSQL_TYPE_DATETIME:       return std::string("Date Time");
        case MYSQL_TYPE_YEAR:           return std::string("Year");
        case MYSQL_TYPE_NEWDATE:        return std::string("New date");
        case MYSQL_TYPE_VARCHAR:        return std::string("Varchar");
        case MYSQL_TYPE_BIT:            return std::string("Bit");
        case MYSQL_TYPE_TIMESTAMP2:     return std::string("TIMESTAMP2");
        case MYSQL_TYPE_DATETIME2:      return std::string("Date time 2");
        case MYSQL_TYPE_TIME2:          return std::string("TIME2");
        case MYSQL_TYPE_JSON:           return std::string("JSON");
        case MYSQL_TYPE_NEWDECIMAL:     return std::string("NEW DEMICAL");
        case MYSQL_TYPE_ENUM:           return std::string("ENUM");
        case MYSQL_TYPE_SET:            return std::string("SET");
        case MYSQL_TYPE_TINY_BLOB:      return std::string("TINY BLOB");
        case MYSQL_TYPE_MEDIUM_BLOB:    return std::string("Medium Blob");
        case MYSQL_TYPE_LONG_BLOB:      return std::string("Long BLOB");
        case MYSQL_TYPE_BLOB:           return std::string("BLOB");
        case MYSQL_TYPE_VAR_STRING:     return std::string("Var string");
        case MYSQL_TYPE_STRING:         return std::string("String");
        case MYSQL_TYPE_GEOMETRY:       return std::string("Geometry");
    }
}

unsigned long TColumn::length() const
{
    return m_column->length;
}

unsigned long TColumn::maxLength() const
{
    return m_column->max_length;
}

bool TColumn::isPrimaryKey() const
{
    return IS_PRI_KEY(m_column->flags);
}

bool TColumn::isNotNul() const
{
    return IS_NOT_NULL(m_column->flags);
}

bool TColumn::isBlob() const
{
    return IS_BLOB(m_column->flags);
}

bool TColumn::isUnsigned() const
{
    return m_column->flags & UNSIGNED_FLAG;
}

std::string TColumn::name() const
{
    return std::string(m_column->name);
}
