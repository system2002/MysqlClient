#ifndef SIMPLE_H
#define SIMPLE_H
#include "tmc.h"
#include <vector>
#include <map>


class Simple : public tmc
{
public:
    Simple();
    Simple(const std::string &AHost, const std::string &AUser, const std::string &APasswd, const std::string &ADb, unsigned int APort = 3306);

    bool exec(const std::string & Aquery);
    bool next();
    const char *valuePtr(const unsigned int & AColumn) const;
    char **row() const;

    std::string valueStr(const unsigned int & AColumn) const;
    std::vector<std::string> rowVS() const;
    std::map<std::string, std::string> rowMS() const;
    std::vector<std::vector<std::string>> getAllrowString();

protected:
    static const char* valueForUser(char* value);
};

#endif // SIMPLE_H
