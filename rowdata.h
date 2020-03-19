#ifndef ROWDATA_H
#define ROWDATA_H
#include <vector>

class rowData
{
public:
    rowData();
    ~rowData();
    void setSize(unsigned int ASize);
    unsigned int size() const;
    void setItemLength(const unsigned int & AItem, const unsigned long & AItemSize);
    unsigned int &getItemLength(const unsigned int & AItem);
    char *getBufferPtr(const unsigned int & AItem);
    void clear();
private:
    std::vector<char*> m_buffer;
    std::vector<unsigned int> m_length;

    static unsigned int defaultSize;
};

#endif // ROWDATA_H
