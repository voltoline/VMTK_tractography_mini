#include "DataBuffer.h"

#include <iostream>
#include <assert.h>
#include <type_traits>
#include <cstring>

DataBuffer::DataBuffer(char *data, DataBuffer::Type type, unsigned int length)
{
    switch(type)
    {
    case DataBuffer::Type::SHORT:
    {
        m_data.ptr_short = reinterpret_cast<short*>(data);
        break;
    }

    case DataBuffer::Type::USHORT:
    {
        m_data.ptr_ushort = reinterpret_cast<unsigned short*>(data);;
        break;
    }

    case DataBuffer::Type::FLOAT:
    {
        m_data.ptr_float = reinterpret_cast<float*>(data);;
        break;
    }

    default:
        break;

    }

    m_data.type = type;
    m_length = length;
}

DataBuffer::DataBuffer(/*char *data, */DataBuffer::Type type, unsigned int length)
{
    switch(type)
    {
    case DataBuffer::Type::SHORT:
    {
        m_data.ptr_short = new short[length];
        std::memset(m_data.ptr_short, 0, length * sizeof(short));
        break;
    }

    case DataBuffer::Type::USHORT:
    {
        m_data.ptr_ushort = new unsigned short[length];
        std::memset(m_data.ptr_ushort, 0, length * sizeof(unsigned short));
        break;
    }

    case DataBuffer::Type::FLOAT:
    {
        m_data.ptr_float = new float[length];
        std::memset(m_data.ptr_float, 0, length * sizeof(float));

        break;
    }

    default:
        break;

    }

    m_data.type = type;
    m_length = length;
}

DataBuffer::~DataBuffer()
{
    switch(m_data.type)
    {
    case DataBuffer::Type::SHORT:
    {
        if(m_data.ptr_short != nullptr)
            delete[] m_data.ptr_short;

        break;
    }

    case DataBuffer::Type::USHORT:
    {
        if(m_data.ptr_ushort != nullptr)
            delete[] m_data.ptr_ushort;

        break;
    }

    case DataBuffer::Type::FLOAT:
    {
        if(m_data.ptr_float != nullptr)
            delete[] m_data.ptr_float;

        break;
    }

    default:
        break;

    }

}

char *DataBuffer::getBuffer() const
{
    switch(m_data.type)
    {
    case DataBuffer::Type::SHORT:
    {
        return reinterpret_cast<char*>(m_data.ptr_short);
    }

    case DataBuffer::Type::USHORT:
    {
        return reinterpret_cast<char*>(m_data.ptr_ushort);
    }

    case DataBuffer::Type::FLOAT:
    {
        return reinterpret_cast<char*>(m_data.ptr_float);
    }

    default:
        std::cerr << "DataBuffer::getBuffer -->> Buffer not found! " << std::endl;
        return nullptr;
    }
}

DataBuffer::Type DataBuffer::getDataType() const
{
    return m_data.type;
}

unsigned short DataBuffer::getValueAsUshort(unsigned int index)
{
    if(m_data.type == DataBuffer::Type::USHORT)
    {
        if(index < m_length)
        {
            return m_data.ptr_ushort[index];

        } else {

            std::cerr << "DataBuffer::getValue -->> Index out of range!" << std::endl;
            return 0;
        }
    }

    return getValue<unsigned short>(index);
}

short DataBuffer::getValueAsShort(unsigned int index)
{
    if(m_data.type == DataBuffer::Type::SHORT)
    {
        if(index < m_length)
        {
            return m_data.ptr_short[index];

        } else {

            std::cerr << "DataBuffer::getValue -->> Index out of range!" << std::endl;
            return 0;
        }
    }

    return getValue<short>(index);
}

void DataBuffer::setValue(unsigned int index, unsigned short value)
{
    setValueAs<unsigned short>(index, value);
}

float DataBuffer::getValueAsFloat(unsigned int index)
{
    if(m_data.type == DataBuffer::Type::FLOAT)
    {
        if(index < m_length)
        {
            return m_data.ptr_float[index];

        } else {

            std::cerr << "DataBuffer::getValue -->> Index out of range!" << std::endl;
            return 0;
        }
    }

    return getValue<float>(index);
}

unsigned int DataBuffer::getLength() const
{
    return m_length;
}

template<typename T>
void DataBuffer::setValueAs(unsigned int index, T value)
{
    if(index < m_length)
    {
        switch(getDataType())
        {

        case DataBuffer::Type::SHORT:
        {
            if(std::is_same<short, T>::value)
                m_data.ptr_short[index] = value;
            else
                setConvertedValue<T>(index, value, m_data.ptr_short);
            break;
        }

        case DataBuffer::Type::USHORT:
        {
            if(std::is_same<unsigned short, T>::value)
                m_data.ptr_ushort[index] = value;
            else
                setConvertedValue<T>(index, value, m_data.ptr_ushort);
            break;
        }

        case DataBuffer::Type::FLOAT:
        {
            if(std::is_same<float, T>::value)
                m_data.ptr_float[index] = value;
            else
                setConvertedValue<T>(index, value, m_data.ptr_float);
            break;
        }

        default:
            assert(!"DataBuffer::setValue -->> Type not handled!");
        }

    } else {

        std::cerr << "DataBuffer::setValue -->> Index out of range!" << std::endl;
    }
}

template<typename T, typename buffer_type>
void DataBuffer::setConvertedValue(unsigned int index, T value, buffer_type *data_buffer)
{
    data_buffer[index] = static_cast<T>(value);
}

template<typename return_type>
return_type DataBuffer::getValue(unsigned int index)
{    
    switch(getDataType())
    {

    case DataBuffer::Type::SHORT:
        return getConvertedValue<return_type>(index, m_data.ptr_short);

    case DataBuffer::Type::USHORT:
        return getConvertedValue<return_type>(index, m_data.ptr_ushort);

    case DataBuffer::Type::FLOAT:
        return getConvertedValue<return_type>(index, m_data.ptr_float);

    default:
        assert(!"DataBuffer::getValue -->> Type not handled!");
        return 0;
    }
}

template<typename return_type, typename buffer_type>
return_type DataBuffer::getConvertedValue(unsigned int index, buffer_type *data_buffer)
{
    if(index > m_length)
    {
        std::cerr << "DataBuffer::getConvertedValue -->> Index out of range!" << std::endl;
        return 0;
    }

    return static_cast<return_type>(data_buffer[index]);
}
