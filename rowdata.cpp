#include "rowdata.h"


rowData::rowData()
{

}

void rowData::setSize(unsigned int ASize)
{
    clear();
    m_buffer.resize(ASize);
    for (char * ptr : m_buffer)
    {
        ptr = nullptr;
    }

}

unsigned int rowData::size()
{
    return m_buffer.size();
}

void rowData::setItemSize(const unsigned int &AItem, const unsigned long &ASize)
{
    if (AItem < size() && m_buffer[AItem] == nullptr)
}

void rowData::clear()
{
    for (char * ptr : m_buffer)
    {
        delete [] ptr;
    }
}
