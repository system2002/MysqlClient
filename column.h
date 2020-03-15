#ifndef TCOLUMN_H
#define TCOLUMN_H
#include "mysql/mysql.h"
#include <string>
#include <type_traits>

class TColumn
{
public:
    TColumn(MYSQL_FIELD * AColumn);
    std::string name() const;
    enum_field_types type() const;
    inline std::string typeString() const { return typeString(type()); }
    unsigned long length() const;
    bool isPrimaryKey() const;
    bool isNotNul() const;
    bool isBlob() const;
    bool isUnsigned() const;

    static std::string typeString(const enum_field_types& AType);

private:
    MYSQL_FIELD * m_column;

};

#endif // TCOLUMN_H
