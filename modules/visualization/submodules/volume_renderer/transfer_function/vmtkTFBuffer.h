#ifndef VMTK_TF_BUFFER_H
#define VMTK_TF_BUFFER_H

#include <iostream>

class vmtkTFBuffer
{
public:

    enum vmtkTFFormat
    {
        VMTK_TF_NO_FORMAT = 1,
        VMTK_TF_RGBA,
        VMTK_TF_RED
    };

    vmtkTFBuffer();
    vmtkTFBuffer(unsigned char *buffer, unsigned short size, vmtkTFFormat format);
    ~vmtkTFBuffer();

    unsigned char *getTFBuffer();
    void setTFBuffer(unsigned char *tranfer_function_buffer);

    unsigned short getBufferSize() const;
    void setBufferSize(unsigned short buffer_size);

    vmtkTFFormat getFormat() const;
    void setFormat(const vmtkTFFormat &format);

    void save(std::string file_path);
    void load(std::string file_path);

private:
    unsigned char *m_tranfer_function_buffer = nullptr; // initilize with nullptr (added by Ting (22/11/20190
    unsigned short m_buffer_size;
    vmtkTFFormat m_format;
};

#endif // VMTK_TF_BUFFER_H
