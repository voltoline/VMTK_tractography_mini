#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <memory>

class DataBuffer
{
public:
    typedef std::shared_ptr<DataBuffer> sPtr;
    typedef std::unique_ptr<DataBuffer> uPtr;

    enum class Type
    {
        SHORT,
        USHORT,
        FLOAT,
        NONE
    };

    DataBuffer(char* data, Type type, unsigned int length);
    DataBuffer(Type type, unsigned int length);
    virtual ~DataBuffer();

    char* getBuffer() const;
    Type getDataType() const;
    unsigned int getLength() const;

    float getValueAsFloat(unsigned int index);
    unsigned short getValueAsUshort(unsigned int index);
    short getValueAsShort(unsigned int index);

    void setValue(unsigned int index, unsigned short value);
    void setValue(unsigned int index, short value) { setValueAs(index, value); }
    void setValue(unsigned int index, float value) { setValueAs<float>(index, value); }

private:

    typedef struct _variant
    {
        Type type;

        union
        {
            short *ptr_short;
            unsigned short *ptr_ushort;
            float *ptr_float;
        };
    } Variant;

    template<typename T>
    void setValueAs(unsigned int index, T value);

    template<typename T, typename buffer_type>
    void setConvertedValue(unsigned int index, T value, buffer_type *data_buffer);

    template<typename return_type>
    return_type getValue(unsigned int index);

    template<typename return_type, typename buffer_type>
    return_type getConvertedValue(unsigned int index, buffer_type *data_buffer);

    Variant m_data;
    unsigned int m_length;
};

#endif // DATA_BUFFER_H
