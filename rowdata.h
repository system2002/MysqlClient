#ifndef ROWDATA_H
#define ROWDATA_H
#include <vector>

class rowData
{
public:
    rowData();
    void setSize(unsigned int ASize);
    unsigned int size();
    void setItemSize(const unsigned int & AItem, const unsigned long & AItemSize);
    void clear();
private:
    std::vector<char*> m_buffer;
};

#endif // ROWDATA_H
