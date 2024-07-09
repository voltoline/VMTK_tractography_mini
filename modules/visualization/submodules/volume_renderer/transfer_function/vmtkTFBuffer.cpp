#include "vmtkTFBuffer.h"

#include <fstream>

vmtkTFBuffer::vmtkTFBuffer():
    m_tranfer_function_buffer(NULL),
    m_buffer_size(0),
    m_format(VMTK_TF_NO_FORMAT)
{

}

vmtkTFBuffer::vmtkTFBuffer(unsigned char *buffer, unsigned short size, vmtkTFFormat format)
{
    // Remove the existing one
    if (m_tranfer_function_buffer) delete[] m_tranfer_function_buffer; //added by Ting (22/11/2019)

    m_tranfer_function_buffer = buffer;
    m_buffer_size = size;
    m_format = format;
}

vmtkTFBuffer::~vmtkTFBuffer()
{
    delete[] m_tranfer_function_buffer;
    m_tranfer_function_buffer = nullptr;                 //added by Ting (22/11/2019)
}

unsigned short vmtkTFBuffer::getBufferSize() const
{
    return m_buffer_size;
}

void vmtkTFBuffer::setBufferSize(unsigned short buffer_size)
{
    m_buffer_size = buffer_size;
}

vmtkTFBuffer::vmtkTFFormat vmtkTFBuffer::getFormat() const
{
    return m_format;
}

void vmtkTFBuffer::setFormat(const vmtkTFFormat &format)
{
    m_format = format;
}

void vmtkTFBuffer::save(std::string file_path)
{
    if(m_buffer_size <= 0)
    {
        std::cout << "vmtkTFBuffer::save -> buffer is empty!" << std::endl;
        return;
    }

    if(m_tranfer_function_buffer == nullptr)
    {
        std::cout << "vmtkTFBuffer::save -> buffer not allocated!" << std::endl;
        return;
    }

    std::ofstream out(file_path.c_str());

    out << m_buffer_size << "\n";

    for (int i = 0; i < m_buffer_size; i++)
    {
        out << m_tranfer_function_buffer[i * 4] << "\n";
        out << m_tranfer_function_buffer[i * 4 + 1] << "\n";
        out << m_tranfer_function_buffer[i * 4 + 1] << "\n";
        out << m_tranfer_function_buffer[i * 4 + 2] << "\n";
    }

    out.close();
}

void vmtkTFBuffer::load(std::string file_path)
{
    if(m_tranfer_function_buffer != nullptr)
    {
        delete[] m_tranfer_function_buffer;
    }

    std::ifstream in(file_path.c_str());

    in >> m_buffer_size;

    m_tranfer_function_buffer = new unsigned char[m_buffer_size * 4];

    for (int i = 0; i < m_buffer_size; i++)
    {
        in >> m_tranfer_function_buffer[i * 4];
        in >> m_tranfer_function_buffer[i * 4 + 1];
        in >> m_tranfer_function_buffer[i * 4 + 1];
        in >> m_tranfer_function_buffer[i * 4 + 2];
    }

    in.close();
}

unsigned char *vmtkTFBuffer::getTFBuffer()
{
    return m_tranfer_function_buffer;
}

void vmtkTFBuffer::setTFBuffer(unsigned char *tranfer_function_buffer)
{
    m_tranfer_function_buffer = tranfer_function_buffer;
}


