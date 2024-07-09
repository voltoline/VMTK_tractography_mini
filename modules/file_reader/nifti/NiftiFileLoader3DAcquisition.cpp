#include "NiftiFileLoader3DAcquisition.h"

#include "modules/file_reader/dicom/vmtkReorientImageVolumeToLPS.h"
#include "modules/file_reader/dicom/vmtkAcquisition.h"

NiftiFileLoader3DAcquisition::NiftiFileLoader3DAcquisition()
{
}

vmtkAcquisitionAbstract* NiftiFileLoader3DAcquisition::loadImage(std::string file_path)
{
    std::cout << "NiftiFileLoader3DAcquisition::loadImage -->> Load file " + file_path + "..." << std::endl;

    tipl::io::nifti nifti_file_parser;
    if(!nifti_file_parser.load_from_file(file_path))
    {
        std::cout << "NiftiFileLoader3DAcquisition::loadImage -->> Load file failed" << std::endl;
        return nullptr;
    }

    if(nifti_file_parser.nif_header.dim[0] != 3)
        return nullptr;

    vmtkAcquisitionAbstract *acq = nullptr;
    switch(nifti_file_parser.nif_header.datatype)
    {
    case 2: // UINT8
    case 256: // INT8
        acq = extract3DImage<unsigned char>(nifti_file_parser, file_path);
        break;

    case 4: // INT16
        acq = extract3DImage<short>(nifti_file_parser, file_path);
        break;

    case 512: // UINT16
        acq = extract3DImage<unsigned short>(nifti_file_parser, file_path);
        break;

    case 8: // INT32
        acq = extract3DImage<int>(nifti_file_parser, file_path);
        break;

    case 16: // FLOAT32
        acq = extract3DImage<float>(nifti_file_parser, file_path);
        break;

    case 64: // FLOAT32
        acq = extract3DImage<double>(nifti_file_parser, file_path);
        break;

    default:
    {
        std::cout << "NiftiFileLoader3DAcquisition::loadImage -->> Unable to handle datatype = " << nifti_file_parser.nif_header.datatype << "!" << std::endl;
        return nullptr;
    }

    }

    return acq;
}

template <typename nifti_image_type>
vmtkAcquisitionAbstract *NiftiFileLoader3DAcquisition::extract3DImage(tipl::io::nifti &nifti_file_parser, std::string &file_path)
{
    tipl::image<nifti_image_type, 3> image;
    nifti_file_parser >> image;

    float scl_inter = nifti_file_parser.nif_header.scl_inter;
    float scl_slope = nifti_file_parser.nif_header.scl_slope;
    float cal_max   = nifti_file_parser.nif_header.cal_max;
    float cal_min   = nifti_file_parser.nif_header.cal_min;

    unsigned int image_dim[3];
    image_dim[0] = image.geometry()[0];
    image_dim[1] = image.geometry()[1];
    image_dim[2] = image.geometry()[2];

    tipl::vector<3, float> image_spacing;
    nifti_file_parser.get_voxel_size(image_spacing);

    vmtkAcquisition *acq = new vmtkAcquisition();

    acq->setType(VMTK_MRI_T1_T2_ACQUISITION);
    acq->setAcquisitionPath(file_path);
    acq->setModalityStr("---");

    acq->setDimensions(0, image_dim[0]);
    acq->setDimensions(1, image_dim[1]);
    acq->setDimensions(2, image_dim[2]);

    acq->setOrigin(0, 0.0);
    acq->setOrigin(1, 0.0);
    acq->setOrigin(2, 0.0);

    acq->setPixelSpacing(0, image_spacing[0]);
    acq->setPixelSpacing(1, image_spacing[1]);
    acq->setPixelSpacing(2, image_spacing[2]);

    acq->setPixelRepresentation(0);
    acq->setSamplesPerPixel(1);
    if(nifti_file_parser.get_bit_count() == 16)
    {
        acq->setBitsAllocated(nifti_file_parser.get_bit_count());
        acq->setBitsStored(nifti_file_parser.get_bit_count());
        acq->setHighBit(nifti_file_parser.get_bit_count() - 1);

    } else {

        std::cout << "NiftiFileLoader3DAcquisition::extract3DImage -->> Bits Allocated " << nifti_file_parser.get_bit_count()  << " not supported! Changing to 16!" << std::endl;

        acq->setBitsAllocated(16);
        acq->setBitsStored(16);
        acq->setHighBit(16 - 1);
    }



    float image_orientation[9];
    nifti_file_parser.get_image_orientation(image_orientation);

    std::cout << "\nnifti RAS coordinate system" << std::endl;
    std::cout << "x: " << image_orientation[0] << " " << image_orientation[3] << " " << image_orientation[6] << std::endl;
    std::cout << "y: " << image_orientation[1] << " " << image_orientation[4] << " " << image_orientation[7] << std::endl;
    std::cout << "z: " << image_orientation[2] << " " << image_orientation[5] << " " << image_orientation[8] << std::endl;

    image_orientation[0] = image_orientation[0] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[1] = image_orientation[1] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[2] = image_orientation[2] / nifti_file_parser.nif_header.pixdim[3];

    image_orientation[3] = image_orientation[3] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[4] = image_orientation[4] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[5] = image_orientation[5] / nifti_file_parser.nif_header.pixdim[3];

    image_orientation[6] = image_orientation[6] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[7] = image_orientation[7] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[8] = image_orientation[8] / nifti_file_parser.nif_header.pixdim[3];

    image_orientation[0] = -image_orientation[0];
    image_orientation[1] = -image_orientation[1];
    image_orientation[2] = -image_orientation[2];

    image_orientation[3] = -image_orientation[3];
    image_orientation[4] = -image_orientation[4];
    image_orientation[5] = -image_orientation[5];

    std::cout << "\ndicom LPS coordinate system" << std::endl;
    std::cout << "x: " << image_orientation[0] << " " << image_orientation[3] << " " << image_orientation[6] << std::endl;
    std::cout << "y: " << image_orientation[1] << " " << image_orientation[4] << " " << image_orientation[7] << std::endl;
    std::cout << "z: " << image_orientation[2] << " " << image_orientation[5] << " " << image_orientation[8] << std::endl;

    acq->setDirCos(0, 0, image_orientation[0]);
    acq->setDirCos(0, 1, image_orientation[3]);
    acq->setDirCos(0, 2, image_orientation[6]);
    acq->setDirCos(1, 0, image_orientation[1]);
    acq->setDirCos(1, 1, image_orientation[4]);
    acq->setDirCos(1, 2, image_orientation[7]);
    acq->setDirCos(2, 0, image_orientation[2]);
    acq->setDirCos(2, 1, image_orientation[5]);
    acq->setDirCos(2, 2, image_orientation[8]);

    acq->setRescaleSlope(1.f);
    acq->setRescaleIntercept(0.f);

    unsigned int num_voxels = image_dim[0] * image_dim[1] * image_dim[2];

    unsigned short *image_buffer = new unsigned short[num_voxels];

    float min_float = getVolumeMinValue(image, image_dim, scl_slope, scl_inter, cal_max, cal_min);

    for (unsigned int iz = 0; iz < image_dim[2]; iz++)
    {
        for (unsigned int iy =0; iy < image_dim[1]; iy++)
        {
            for (unsigned int ix =0; ix < image_dim[0]; ix++)
            {
                unsigned int image_pos = iz * image_dim[1] * image_dim[0] + iy * image_dim[0] + ix;

                float scaled_value;
                if(scl_slope != 0.f)
                    scaled_value = scl_slope * image[image_pos] + scl_inter;
                else
                    scaled_value = image[image_pos] + scl_inter;

                if(cal_min != 0.f || cal_max != 0.f)
                {
                    if(scaled_value < cal_min)
                        scaled_value = 0.f;

                    else if(scaled_value > cal_max)
                        scaled_value = cal_max - cal_min;
                }

                if(min_float < 0)
                    scaled_value += (-min_float);

                unsigned short value = static_cast<unsigned short>(scaled_value);
                image_buffer[image_pos] = value;

                if(acq->getRawUmax() < value)
                    acq->setRawUmax(value);

                if(acq->getRawUmin() > value)
                    acq->setRawUmin(value);
            }
        }
    }

    std::cout << "acq->getRawUmin() " << acq->getRawUmin() << std::endl;
    std::cout << "acq->getRawUmax() " << acq->getRawUmax() << std::endl;

    acq->setImageBuffer(reinterpret_cast<char*>(image_buffer));

    acq->setImageBufferLength(num_voxels * 2); // in bytes

    vmtkReorientImageVolumeToLPS::reorient(acq);

    return acq;
}

template <typename nifti_image_type>
float NiftiFileLoader3DAcquisition::getVolumeMinValue(tipl::image<nifti_image_type, 3> image, unsigned int image_dim[],
                                                      float scl_slope, float scl_inter, float cal_max, float cal_min)
{
    float min_float = 0;
    for (unsigned int iz = 0; iz < image_dim[2]; iz++)
    {
        for (unsigned int iy =0; iy < image_dim[1]; iy++)
        {
            for (unsigned int ix =0; ix < image_dim[0]; ix++)
            {
                unsigned int image_pos = iz * image_dim[1] * image_dim[0] + iy * image_dim[0] + ix;

                float scaled_value;
                if(scl_slope != 0.f)
                    scaled_value = scl_slope * image[image_pos] + scl_inter;
                else
                    scaled_value = image[image_pos] + scl_inter;

                if(cal_min != 0.f || cal_max != 0.f)
                {
                    if(scaled_value < cal_min)
                        scaled_value = 0.f;

                    else if(scaled_value > cal_max)
                        scaled_value = cal_max - cal_min;
                }

                if(min_float > scaled_value)
                    min_float = scaled_value;
            }
        }
    }

    std::cout << "min original value " << min_float << std::endl;

    if(min_float < 0.f)
        return min_float;
    else
        return 0.f;
}
