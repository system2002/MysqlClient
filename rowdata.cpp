#include "rowdata.h"
unsigned int rowData::defaultSize = 0;

rowData::rowData()
{}

rowData::~rowData()
{
    clear();
}

void rowData::setSize(unsigned int ASize)
{
    if (!m_buffer.empty()) clear();
    m_buffer.resize(ASize);
    m_length.resize(ASize);
    for (char * ptr : m_buffer)
    {
        ptr = nullptr;
    }
}

unsigned int rowData::size() const
{
    return m_buffer.size();
}

void rowData::setItemLength(const unsigned int &AItem, const unsigned long &ASize)
{
    if (AItem < size() && m_buffer[AItem] == nullptr)
    {
        m_buffer[AItem] = new char (ASize+1);
        m_length[AItem] = ASize;
    }
}

unsigned int &rowData::getItemLength(const unsigned int &AItem)
{
    if (AItem < size())
        return m_length[AItem];
    else return defaultSize;
}

char * rowData::getBufferPtr(const unsigned int &AItem)
{
    return m_buffer[AItem];
}

void rowData::clear()
{
    for (char * ptr : m_buffer)
    {
        delete [] ptr;
    }
    m_buffer.clear();
    m_length.clear();
}
