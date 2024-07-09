#include "NiftiFileLoaderDiffusionAcquisition.h"

#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"
#include "modules/file_reader/dicom/vmtkReorientImageVolumeToLPS.h"

NiftiFileLoaderDiffusionAcquisition::NiftiFileLoaderDiffusionAcquisition()
{

}

vmtkAcquisitionAbstract* NiftiFileLoaderDiffusionAcquisition::loadImage(std::string file_path)
{
    std::cout << "NiftiFileLoaderDiffusionAcquisition -->> load file..." << std::endl;

    tipl::io::nifti nifti_file_parser;
    if(!nifti_file_parser.load_from_file(file_path))
    {
        std::cout << "NiftiFileLoaderDiffusionAcquisition::loadImage -->> load file failed" << std::endl;
        return nullptr;
    }

    if(nifti_file_parser.nif_header.dim[0] != 4)
        return nullptr;

    std::cout << nifti_file_parser.nif_header.datatype << std::endl;

    vmtkAcquisitionAbstract *acq = nullptr;
    switch(nifti_file_parser.nif_header.datatype)
    {
    case 2: // UINT8
        acq = extractDiffusionImage<unsigned char>(nifti_file_parser, file_path);
        break;

    case 4: // INT16
        acq = extractDiffusionImage<short>(nifti_file_parser, file_path);
        break;

    case 8: // INT32
        acq = extractDiffusionImage<int>(nifti_file_parser, file_path);
        break;

    case 16: // FLOAT32
        acq = extractDiffusionImage<float>(nifti_file_parser, file_path);
        break;

    case 512: // UINT16
        acq = extractDiffusionImage<unsigned short>(nifti_file_parser, file_path);
        break;

    default:
    {
        std::cout << "NiftiFileLoaderDiffusionAcquisition -->> Unable to handle datatype = " << nifti_file_parser.nif_header.datatype << "!" << std::endl;
        return nullptr;
    }

    }

    return acq;
}

template <typename nifti_image_type>
vmtkAcquisitionAbstract *NiftiFileLoaderDiffusionAcquisition::extractDiffusionImage(tipl::io::nifti &nifti_file_parser, std::string &file_path)
{
    tipl::image<nifti_image_type, 4> image;
    nifti_file_parser >> image;

    unsigned int image_dim[4];
    image_dim[0] = image.geometry()[0];
    image_dim[1] = image.geometry()[1];
    image_dim[2] = image.geometry()[2];
    image_dim[3] = image.geometry()[3];

    tipl::vector<3, float> image_spacing;
    nifti_file_parser.get_voxel_size(image_spacing);

    vmtkDiffAcquisition *diff_acq = new vmtkDiffAcquisition();

    diff_acq->setType(VMTK_DIFF_ACQUISITION);
    diff_acq->setAcquisitionPath(file_path);
    diff_acq->setModalityStr("MRI-DIFFUSION");

    diff_acq->setDimensions(0, image_dim[0]);
    diff_acq->setDimensions(1, image_dim[1]);
    diff_acq->setDimensions(2, image_dim[2]);
    diff_acq->setDimensions(3, image_dim[3]);

    diff_acq->setOrigin(0, 0.0);
    diff_acq->setOrigin(1, 0.0);
    diff_acq->setOrigin(2, 0.0);

    diff_acq->setPixelSpacing(0, image_spacing[0]);
    diff_acq->setPixelSpacing(1, image_spacing[1]);
    diff_acq->setPixelSpacing(2, image_spacing[2]);

    diff_acq->setPixelRepresentation(0);
    diff_acq->setSamplesPerPixel(1);

    diff_acq->setBitsAllocated(16);
    diff_acq->setBitsStored(16);
    diff_acq->setHighBit(15);

    std::cout << "setBitsAllocated " << diff_acq->getBitsAllocated() << std::endl;
    std::cout << "setBitsStored " << diff_acq->getBitsStored() << std::endl;
    std::cout << "setHighBit " << diff_acq->getHighBit() << std::endl;

    float image_orientation[9];
    nifti_file_parser.get_image_orientation(image_orientation);

    image_orientation[0] = image_orientation[0] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[1] = image_orientation[1] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[2] = image_orientation[2] / nifti_file_parser.nif_header.pixdim[3];

    image_orientation[3] = image_orientation[3] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[4] = image_orientation[4] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[5] = image_orientation[5] / nifti_file_parser.nif_header.pixdim[3];

    image_orientation[6] = image_orientation[6] / nifti_file_parser.nif_header.pixdim[1];
    image_orientation[7] = image_orientation[7] / nifti_file_parser.nif_header.pixdim[2];
    image_orientation[8] = image_orientation[8] / nifti_file_parser.nif_header.pixdim[3];

    std::cout << "\nnifti RAS coordinate system" << std::endl;
    std::cout << "x: " << image_orientation[0] << " " << image_orientation[3] << " " << image_orientation[6] << std::endl;
    std::cout << "y: " << image_orientation[1] << " " << image_orientation[4] << " " << image_orientation[7] << std::endl;
    std::cout << "z: " << image_orientation[2] << " " << image_orientation[5] << " " << image_orientation[8] << std::endl;

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


    diff_acq->setDirCos(0, 0, image_orientation[0]);
    diff_acq->setDirCos(0, 1, image_orientation[3]);
    diff_acq->setDirCos(0, 2, image_orientation[6]);
    diff_acq->setDirCos(1, 0, image_orientation[1]);
    diff_acq->setDirCos(1, 1, image_orientation[4]);
    diff_acq->setDirCos(1, 2, image_orientation[7]);
    diff_acq->setDirCos(2, 0, image_orientation[2]);
    diff_acq->setDirCos(2, 1, image_orientation[5]);
    diff_acq->setDirCos(2, 2, image_orientation[8]);


    diff_acq->setRescaleSlope(1.f);
    diff_acq->setRescaleIntercept(0.f);

    if(!loadBvalsAndBvecs(diff_acq, file_path))
        return nullptr;

    recalculateGradients(diff_acq, image_orientation);

    unsigned short num_volumes = image_dim[3];
    unsigned int num_voxels = image_dim[0] * image_dim[1] * image_dim[2];

    for(unsigned short i = 0; i < num_volumes; i++)
    {
        unsigned int pos = i * num_voxels;
        unsigned short *image_buffer = new unsigned short[num_voxels];

        for (unsigned int iz = 0; iz < image_dim[2]; iz++)
        {
            for (unsigned int iy =0; iy < image_dim[1]; iy++)
            {
                for (unsigned int ix =0; ix < image_dim[0]; ix++)
                {
                    unsigned int nifti_pos = pos + ((iz * image_dim[1] + iy) * image_dim[0] + ix);
                    unsigned int image_pos = iz * image_dim[1] * image_dim[0] + iy * image_dim[0] + ix;

                    if(image[nifti_pos] < 0)
                        image_buffer[image_pos] = 0;
                    else
                        image_buffer[image_pos] = (unsigned short)std::floor(image[nifti_pos]);

                    unsigned short value = image_buffer[image_pos];

                    if(diff_acq->getUmax() < value) diff_acq->setUmax(value);
                    if(diff_acq->getUmin() > value) diff_acq->setUmin(value);

                    if(diff_acq->getUmax(i) < value) diff_acq->setUmax(i, value);
                    if(diff_acq->getUmin(i) > value) diff_acq->setUmin(i, value);
                }
            }
        }

        diff_acq->setImageBuffer(i, reinterpret_cast<char*>(image_buffer));
    }

    diff_acq->setImageBufferLength(num_voxels * 2); // in bytes

    reorientVolumes(diff_acq);

    std::cout << "\nafter reorient" << std::endl;
    std::cout << diff_acq->getDirCos(0, 0) << " " << diff_acq->getDirCos(0, 1) << " " << diff_acq->getDirCos(0, 2) << std::endl;
    std::cout << diff_acq->getDirCos(1, 0) << " " << diff_acq->getDirCos(1, 1) << " " << diff_acq->getDirCos(1, 2) << std::endl;
    std::cout << diff_acq->getDirCos(2, 0) << " " << diff_acq->getDirCos(2, 1) << " " << diff_acq->getDirCos(2, 2) << std::endl;

    float new_image_orientation[9];
    new_image_orientation[0] = diff_acq->getDirCos(0, 0);
    new_image_orientation[1] = diff_acq->getDirCos(0, 1);
    new_image_orientation[2] = diff_acq->getDirCos(0, 2);

    new_image_orientation[3] = diff_acq->getDirCos(1, 0);
    new_image_orientation[4] = diff_acq->getDirCos(1, 1);
    new_image_orientation[5] = diff_acq->getDirCos(1, 2);

    new_image_orientation[6] = diff_acq->getDirCos(2, 0);
    new_image_orientation[7] = diff_acq->getDirCos(2, 1);
    new_image_orientation[8] = diff_acq->getDirCos(2, 2);

    recalculateGradients(diff_acq, new_image_orientation);

    return diff_acq;
}

bool NiftiFileLoaderDiffusionAcquisition::loadBvalsAndBvecs(vmtkDiffAcquisition *diff_acq, std::string file_path)
{
    unsigned short str_size = file_path.size();
    std::ifstream bvecs_file, bvals_file;
    std::string bvecs_file_path, bvals_file_path;

    if(file_path.compare(str_size - 4, 4, ".nii") == 0)
    {
        bvecs_file_path = file_path.substr(0, str_size - 3).append("bvec");
        bvals_file_path = file_path.substr(0, str_size - 3).append("bval");

    } else {

        bvecs_file_path = std::string(file_path).append(".bvec");
        bvals_file_path = std::string(file_path).append(".bval");
    }

    bvecs_file.open(bvecs_file_path.c_str());
    bvals_file.open(bvals_file_path.c_str());

    if(!bvals_file.is_open())
    {
        std::cerr << "NiftiFileLoaderDiffusionAcquisition --> It was not possible to open the file " << bvals_file_path << std::endl;
        return false;
    }

    if(!bvecs_file.is_open())
    {
        std::cerr << "NiftiFileLoaderDiffusionAcquisition --> It was not possible to open the file " << bvecs_file_path << "!" << std::endl;
        return false;
    }

    unsigned short num_volumes = diff_acq->getDimensions(3);
    float bvals[num_volumes];
    float bvecs[num_volumes][3];

    for(unsigned short i = 0; i < num_volumes; i++)
        bvals_file >> bvals[i];

    for(unsigned short j = 0; j < 3; j++)
        for(unsigned short i = 0; i < num_volumes; i++)
            bvecs_file >> bvecs[i][j];

    int num_b0_img = 0, num_gradient_img = 0;
    for(unsigned short i = 0; i < num_volumes; i++)
    {
        diff_acq->setGradientOrientation(i, bvecs[i][0], bvecs[i][1], bvecs[i][2]);
        diff_acq->setBValue(i, bvals[i]);

        if(int(bvecs[i][0]) == 0 && int(bvecs[i][1]) == 0 && int(bvecs[i][2]) == 0 && int(bvals[i]) == 0)
        {
            diff_acq->setIsB0(i, true);
            num_b0_img++;

        } else {

            diff_acq->setIsDWI(i, true);
            num_gradient_img++;
        }
    }

    diff_acq->setNumB0Vol(num_b0_img);
    diff_acq->setNumGradientVol(num_gradient_img);

    return true;
}

void NiftiFileLoaderDiffusionAcquisition::reorientVolumes(vmtkDiffAcquisition *diff_acq)
{
    vmtkAcquisition *acq = new vmtkAcquisition;

    for(unsigned short i = 0; i < diff_acq->getDimensions(3); i++)
    {
        acq->setDimensions(0, diff_acq->getDimensions(0));
        acq->setDimensions(1, diff_acq->getDimensions(1));
        acq->setDimensions(2, diff_acq->getDimensions(2));

        acq->setDirCos(0, 0, diff_acq->getDirCos(0, 0)); acq->setDirCos(0, 1, diff_acq->getDirCos(0, 1)); acq->setDirCos(0, 2, diff_acq->getDirCos(0, 2));
        acq->setDirCos(1, 0, diff_acq->getDirCos(1, 0)); acq->setDirCos(1, 1, diff_acq->getDirCos(1, 1)); acq->setDirCos(1, 2, diff_acq->getDirCos(1, 2));
        acq->setDirCos(2, 0, diff_acq->getDirCos(2, 0)); acq->setDirCos(2, 1, diff_acq->getDirCos(2, 1)); acq->setDirCos(2, 2, diff_acq->getDirCos(2, 2));

        acq->setBitsAllocated(diff_acq->getBitsAllocated( ));

        acq->setPixelSpacing(0, diff_acq->getPixelSpacing(0));
        acq->setPixelSpacing(1, diff_acq->getPixelSpacing(1));
        acq->setPixelSpacing(2, diff_acq->getPixelSpacing(2));

        acq->setImageBuffer(diff_acq->getImageBuffer(i));
        acq->setImageBufferLength(diff_acq->getImageBufferLength( ));

        vmtkReorientImageVolumeToLPS::reorient( acq );
    }

    diff_acq->setDimensions(0, acq->getDimensions(0));
    diff_acq->setDimensions(1, acq->getDimensions(1));
    diff_acq->setDimensions(2, acq->getDimensions(2));

    diff_acq->setDirCos(0, 0, acq->getDirCos(0, 0)); diff_acq->setDirCos(0, 1, acq->getDirCos(0, 1)); diff_acq->setDirCos(0, 2, acq->getDirCos(0, 2));
    diff_acq->setDirCos(1, 0, acq->getDirCos(1, 0)); diff_acq->setDirCos(1, 1, acq->getDirCos(1, 1)); diff_acq->setDirCos(1, 2, acq->getDirCos(1, 2));
    diff_acq->setDirCos(2, 0, acq->getDirCos(2, 0)); diff_acq->setDirCos(2, 1, acq->getDirCos(2, 1)); diff_acq->setDirCos(2, 2, acq->getDirCos(2, 2));

    diff_acq->setPixelSpacing(0, acq->getPixelSpacing(0));
    diff_acq->setPixelSpacing(1, acq->getPixelSpacing(1));
    diff_acq->setPixelSpacing(2, acq->getPixelSpacing(2));

}

void NiftiFileLoaderDiffusionAcquisition::recalculateGradients(vmtkDiffAcquisition *diff_acq, float matrix[9])
{
    float grad_x = 0, grad_y = 0, grad_z = 0;
    float new_grad_x = 0, new_grad_y = 0, new_grad_z = 0;

    for(int i = 0; i < diff_acq->getDimensions(3); i++)
    {
        diff_acq->getGradientOrientation(i, grad_x, grad_y, grad_z);

        if( diff_acq->getBValue(i) > 0 && (grad_x != 0 || grad_y != 0 || grad_z != 0))
        {
            new_grad_x = static_cast<float>(matrix[0] * grad_x + matrix[1] * grad_y + matrix[2] * static_cast<double>(grad_z));
            new_grad_y = static_cast<float>(matrix[3] * grad_x + matrix[4] * grad_y + matrix[5] * static_cast<double>(grad_z));
            new_grad_z = static_cast<float>(matrix[6] * grad_x + matrix[7] * grad_y + matrix[8] * static_cast<double>(grad_z));

            std::cout << "grad " << new_grad_x << " " << new_grad_y << " " << new_grad_z << std::endl;

            diff_acq->setGradientOrientation(i, new_grad_x, new_grad_y, new_grad_z);
        }
    }
}
