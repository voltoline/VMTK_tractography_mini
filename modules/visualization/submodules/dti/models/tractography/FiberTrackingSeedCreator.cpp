#include "FiberTrackingSeedCreator.h"

#include "modules/file_reader/nifti/NiftiFileLoader3DAcquisition.h"
#include "modules/visualization/submodules/dti/models/tensor_fitter/DTI.h"
#include "modules/file_reader/dicom/vmtkAcquisition.h"

FiberTrackingSeedCreator::FiberTrackingSeedCreator()
{
    m_baseFolderPath = "";
}

FiberTrackingSeedCreator::SeedBuffer FiberTrackingSeedCreator::createSeedsFromWholeBrain(const DTI *dti, float fractional_anisotropy_threshold)
{
    GrayscaleImage3DFloat::sPtr fa_buffer = dti->getFractionalAnisotropy();

    float voxel_dim[3];
    voxel_dim[0] = 1.f / dti->getDimensions(0);
    voxel_dim[1] = 1.f / dti->getDimensions(1);
    voxel_dim[2] = 1.f / dti->getDimensions(2);

    float voxel_center[3];
    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    const unsigned short num_seed_offsets = 27;
    unsigned short num_seed_per_voxels = num_seed_offsets * 2;
    float seed_spacing[3] = {voxel_center[0] * (2.f / 3.f), voxel_center[1] * (2.f / 3.f), voxel_center[2] * (2.f / 3.f)};
    float seed_offsets[num_seed_offsets][3] = {

        { seed_spacing[0],  seed_spacing[1],  seed_spacing[2]},
        { seed_spacing[0],              0.f,  seed_spacing[2]},
        { seed_spacing[0], -seed_spacing[1],  seed_spacing[2]},
        {             0.f,  seed_spacing[1],  seed_spacing[2]},
        {             0.f,              0.f,  seed_spacing[2]},
        {             0.f, -seed_spacing[1],  seed_spacing[2]},
        {-seed_spacing[0],  seed_spacing[1],  seed_spacing[2]},
        {-seed_spacing[0],              0.f,  seed_spacing[2]},
        {-seed_spacing[0], -seed_spacing[1],  seed_spacing[2]},

        { seed_spacing[0],  seed_spacing[1],              0.f},
        { seed_spacing[0],              0.f,              0.f},
        { seed_spacing[0], -seed_spacing[1],              0.f},
        {             0.f,  seed_spacing[1],              0.f},
        {             0.f,              0.f,              0.f},
        {             0.f, -seed_spacing[1],              0.f},
        {-seed_spacing[0],  seed_spacing[1],              0.f},
        {-seed_spacing[0],              0.f,              0.f},
        {-seed_spacing[0], -seed_spacing[1],              0.f},

        { seed_spacing[0],  seed_spacing[1], -seed_spacing[2]},
        { seed_spacing[0],              0.f, -seed_spacing[2]},
        { seed_spacing[0], -seed_spacing[1], -seed_spacing[2]},
        {             0.f,  seed_spacing[1], -seed_spacing[2]},
        {             0.f,              0.f, -seed_spacing[2]},
        {             0.f, -seed_spacing[1], -seed_spacing[2]},
        {-seed_spacing[0],  seed_spacing[1], -seed_spacing[2]},
        {-seed_spacing[0],              0.f, -seed_spacing[2]},
        {-seed_spacing[0], -seed_spacing[1], -seed_spacing[2]}
    };

    unsigned int num_valid_voxels = 0;
    unsigned int num_voxels = dti->getDimensions(0) * dti->getDimensions(1) * dti->getDimensions(2);
    for(unsigned int i = 0; i < num_voxels; i++)
        if(fa_buffer->at(i) > fractional_anisotropy_threshold)
            num_valid_voxels++;

    unsigned int num_seeds = num_valid_voxels * num_seed_per_voxels;
    std::cout << "num_seeds " << num_seeds << std::endl;

    float *seed_positions = new float[num_seeds * 4];
    unsigned int voxel_count = 0, seed_positions_ind = 0;
    for(unsigned short k = 0; k < dti->getDimensions(2); k++)
        for(unsigned short j = 0; j < dti->getDimensions(1); j++)
            for(unsigned short i = 0; i < dti->getDimensions(0); i++)
            {
                if(fa_buffer->at(voxel_count) > fractional_anisotropy_threshold)
                {
                    for(unsigned short s = 0; s < num_seed_offsets; s++)
                    {
                        seed_positions[seed_positions_ind * 4] = static_cast<float>(i) * voxel_dim[0] + voxel_center[0] + seed_offsets[s][0];
                        seed_positions[seed_positions_ind * 4 + 1] = static_cast<float>(j) * voxel_dim[1] + voxel_center[1] + seed_offsets[s][1];
                        seed_positions[seed_positions_ind * 4 + 2] = static_cast<float>(k) * voxel_dim[2] + voxel_center[2] + seed_offsets[s][2];

                        seed_positions[(seed_positions_ind + 1) * 4] = seed_positions[seed_positions_ind * 4];
                        seed_positions[(seed_positions_ind + 1) * 4 + 1] = seed_positions[seed_positions_ind * 4 + 1];
                        seed_positions[(seed_positions_ind + 1) * 4 + 2] = seed_positions[seed_positions_ind * 4 + 2];

                        seed_positions[seed_positions_ind * 4 + 3] = 1.f;
                        seed_positions[(seed_positions_ind + 1) * 4 + 3] = -1.f;

                        seed_positions_ind += 2;
                    }

                }

                voxel_count++;
            }

    std::cout << "mNumSeed " << num_seeds << std::endl;

    return {seed_positions, num_seeds};
}

FiberTrackingSeedCreator::SeedBuffer::sPtr FiberTrackingSeedCreator::createSeedsFromVoxelIndices(const DTI *dti, float fractional_anisotropy_threshold, std::vector<int> voxel_indices)
{
    unsigned int num_voxels = voxel_indices.size()/3;

    GrayscaleImage3DFloat::sPtr fa_buffer = dti->getFractionalAnisotropy();

    float voxel_dim[3];
    voxel_dim[0] = 1.f / dti->getDimensions(0);
    voxel_dim[1] = 1.f / dti->getDimensions(1);
    voxel_dim[2] = 1.f / dti->getDimensions(2);

    float voxel_center[3];
    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    const unsigned short num_seed_offsets = 45;
    unsigned short num_seed_per_voxels = num_seed_offsets * 2;
    float seed_spacing[3] = {voxel_center[0] * (2.f / 3.f), voxel_center[1] * (2.f / 3.f), voxel_center[2] * (2.f / 3.f)};
    float seed_spacing_2[3] = {voxel_center[0] * (1.f / 3.f), voxel_center[1] * (1.f / 3.f), voxel_center[2] * (1.f / 3.f)};
    float seed_offsets[num_seed_offsets][3] = {

        { seed_spacing[0],  seed_spacing[1],  seed_spacing[2]},
        { seed_spacing[0],              0.f,  seed_spacing[2]},
        { seed_spacing[0], -seed_spacing[1],  seed_spacing[2]},
        {             0.f,  seed_spacing[1],  seed_spacing[2]},
        {             0.f,              0.f,  seed_spacing[2]},
        {             0.f, -seed_spacing[1],  seed_spacing[2]},
        {-seed_spacing[0],  seed_spacing[1],  seed_spacing[2]},
        {-seed_spacing[0],              0.f,  seed_spacing[2]},
        {-seed_spacing[0], -seed_spacing[1],  seed_spacing[2]},

        { seed_spacing[0],  seed_spacing[1],  seed_spacing_2[2]},
        { seed_spacing[0],              0.f,  seed_spacing_2[2]},
        { seed_spacing[0], -seed_spacing[1],  seed_spacing_2[2]},
        {             0.f,  seed_spacing[1],  seed_spacing_2[2]},
        {             0.f,              0.f,  seed_spacing_2[2]},
        {             0.f, -seed_spacing[1],  seed_spacing_2[2]},
        {-seed_spacing[0],  seed_spacing[1],  seed_spacing_2[2]},
        {-seed_spacing[0],              0.f,  seed_spacing_2[2]},
        {-seed_spacing[0], -seed_spacing[1],  seed_spacing_2[2]},

        { seed_spacing[0],  seed_spacing[1],              0.f},
        { seed_spacing[0],              0.f,              0.f},
        { seed_spacing[0], -seed_spacing[1],              0.f},
        {             0.f,  seed_spacing[1],              0.f},
        {             0.f,              0.f,              0.f},
        {             0.f, -seed_spacing[1],              0.f},
        {-seed_spacing[0],  seed_spacing[1],              0.f},
        {-seed_spacing[0],              0.f,              0.f},
        {-seed_spacing[0], -seed_spacing[1],              0.f},

        { seed_spacing[0],  seed_spacing[1], -seed_spacing_2[2]},
        { seed_spacing[0],              0.f, -seed_spacing_2[2]},
        { seed_spacing[0], -seed_spacing[1], -seed_spacing_2[2]},
        {             0.f,  seed_spacing[1], -seed_spacing_2[2]},
        {             0.f,              0.f, -seed_spacing_2[2]},
        {             0.f, -seed_spacing[1], -seed_spacing_2[2]},
        {-seed_spacing[0],  seed_spacing[1], -seed_spacing_2[2]},
        {-seed_spacing[0],              0.f, -seed_spacing_2[2]},
        {-seed_spacing[0], -seed_spacing[1], -seed_spacing_2[2]},

        { seed_spacing[0],  seed_spacing[1], -seed_spacing[2]},
        { seed_spacing[0],              0.f, -seed_spacing[2]},
        { seed_spacing[0], -seed_spacing[1], -seed_spacing[2]},
        {             0.f,  seed_spacing[1], -seed_spacing[2]},
        {             0.f,              0.f, -seed_spacing[2]},
        {             0.f, -seed_spacing[1], -seed_spacing[2]},
        {-seed_spacing[0],  seed_spacing[1], -seed_spacing[2]},
        {-seed_spacing[0],              0.f, -seed_spacing[2]},
        {-seed_spacing[0], -seed_spacing[1], -seed_spacing[2]}
    };

    unsigned int num_seeds = num_voxels * num_seed_per_voxels;
    std::cout << "num_seeds " << num_seeds << std::endl;

    float current_roi_plane_id = 1.0;

    float *seed_positions = new float[num_seeds * 4];
    unsigned int voxel_count = 0, seed_positions_ind = 0;
    unsigned int created_seeds = 0;
            for(unsigned short array_index = 0; array_index < num_voxels; array_index++)
            {
                int i = voxel_indices[array_index * 3];
                int j = voxel_indices[array_index * 3 + 1];
                int k = voxel_indices[array_index * 3 + 2];
                unsigned int voxel_index = i + j * dti->getDimensions(0) + k * dti->getDimensions(0) * dti->getDimensions(1);

                if(i < -0.5 && j < -0.5 && k < -0.5)
                {
                    current_roi_plane_id = i; // retrieve the plane ID
                    continue;
                }

                if(current_roi_plane_id > 0)
                    std::cerr << "FiberTrackingSeedCreator::createSeedsFromVoxelIndices -->> ROI plane id not found!" << std::endl;

                if(fa_buffer->at(voxel_index) > fractional_anisotropy_threshold)
                {
                    for(unsigned short s = 0; s < num_seed_offsets; s++)
                    {
                        seed_positions[seed_positions_ind * 4] = static_cast<float>(i) * voxel_dim[0] + voxel_center[0] + seed_offsets[s][0];
                        seed_positions[seed_positions_ind * 4 + 1] = static_cast<float>(j) * voxel_dim[1] + voxel_center[1] + seed_offsets[s][1];
                        seed_positions[seed_positions_ind * 4 + 2] = static_cast<float>(k) * voxel_dim[2] + voxel_center[2] + seed_offsets[s][2];

                        seed_positions[(seed_positions_ind + 1) * 4] = seed_positions[seed_positions_ind * 4];
                        seed_positions[(seed_positions_ind + 1) * 4 + 1] = seed_positions[seed_positions_ind * 4 + 1];
                        seed_positions[(seed_positions_ind + 1) * 4 + 2] = seed_positions[seed_positions_ind * 4 + 2];

                        seed_positions[seed_positions_ind * 4 + 3] = 1.f * (-current_roi_plane_id); // add plane ID info into the seed point
                        seed_positions[(seed_positions_ind + 1) * 4 + 3] = -1.f * (-current_roi_plane_id); // add plane ID info into de seed point

                        seed_positions_ind += 2;

                        created_seeds += 2;
                    }

                }

                voxel_count++;
            }

    std::cout << "mNumSeed " << num_seeds << std::endl;
    std::cout << "created_seeds " << created_seeds << std::endl;
    std::cout << "seed_positions_ind " << seed_positions_ind << std::endl;

    return std::make_shared<FiberTrackingSeedCreator::SeedBuffer>(seed_positions, created_seeds);
}

FiberTrackingSeedCreator::SeedBuffer FiberTrackingSeedCreator::loadSeedsFromNIfTIFiles(std::string file_paths[], unsigned short num_files)
{
    NiftiFileLoader3DAcquisition nifti_loader;
    std::cout << "num_files masks " << num_files << std::endl;
    vmtkAcquisition *seed_volumes[num_files];

    for(unsigned short i = 0; i < num_files; i++)
    {
        std::string file_path = m_baseFolderPath + file_paths[i];
        seed_volumes[i] = static_cast<vmtkAcquisition*>(nifti_loader.loadImage(file_path));
    }

    float voxel_dim[3];
    voxel_dim[0] = 1.f / seed_volumes[0]->getDimensions(0);
    voxel_dim[1] = 1.f / seed_volumes[0]->getDimensions(1);
    voxel_dim[2] = 1.f / seed_volumes[0]->getDimensions(2);

    float voxel_center[3];
    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    const unsigned short num_seed_offsets = 7;
    unsigned short num_seed_per_voxels = num_seed_offsets * 2;
    float seed_offsets[num_seed_offsets][3] = {
        {0.f, 0.f, 0.f},
        {(voxel_center[0] / 2), 0.f, 0.f},
        {-(voxel_center[0] / 2), 0.f, 0.f},
        {0.f, (voxel_center[1] / 2), 0.f},
        {0.f, -(voxel_center[1] / 2), 0.f},
        {0.f, 0.f, (voxel_center[2] / 2)},
        {0.f, 0.f, -(voxel_center[2] / 2)}
    };

    unsigned int num_valid_voxels = 0;
    unsigned int num_voxels = seed_volumes[0]->getDimensions(0) * seed_volumes[0]->getDimensions(1) * seed_volumes[0]->getDimensions(2);

    unsigned short *seed_volume_buffer;
    for(unsigned short f = 0; f < num_files; f++)
    {
        seed_volume_buffer = reinterpret_cast<unsigned short*>(seed_volumes[f]->getImageBuffer());
        for(unsigned int i = 0; i < num_voxels; i++)
            if(seed_volume_buffer[i] > 0)
                num_valid_voxels++;
    }

    num_valid_voxels = num_valid_voxels * num_seed_per_voxels;
    std::cout << "num_valid_voxels " << num_valid_voxels << std::endl;

    float *seed_positions = new float[num_valid_voxels * 4];
    unsigned int seed_positions_ind = 0;

    for(unsigned short f = 0; f < num_files; f++)
    {
        unsigned int voxel_count = 0;
        seed_volume_buffer = reinterpret_cast<unsigned short*>(seed_volumes[f]->getImageBuffer());
        for(unsigned short k = 0; k < seed_volumes[f]->getDimensions(2); k++)
            for(unsigned short j = 0; j < seed_volumes[f]->getDimensions(1); j++)
                for(unsigned short i = 0; i < seed_volumes[f]->getDimensions(0); i++)
                {
                    if(seed_volume_buffer[voxel_count] > 0)
                    {
                        for(unsigned short s = 0; s < num_seed_offsets; s++)
                        {
                            seed_positions[seed_positions_ind * 4] = static_cast<float>(i) * voxel_dim[0] + voxel_center[0] + seed_offsets[s][0];
                            seed_positions[seed_positions_ind * 4 + 1] = static_cast<float>(j) * voxel_dim[1] + voxel_center[1] + seed_offsets[s][1];
                            seed_positions[seed_positions_ind * 4 + 2] = static_cast<float>(k) * voxel_dim[2] + voxel_center[2] + seed_offsets[s][2];

                            seed_positions[(seed_positions_ind + 1) * 4] = seed_positions[seed_positions_ind * 4];
                            seed_positions[(seed_positions_ind + 1) * 4 + 1] = seed_positions[seed_positions_ind * 4 + 1];
                            seed_positions[(seed_positions_ind + 1) * 4 + 2] = seed_positions[seed_positions_ind * 4 + 2];

                            seed_positions[seed_positions_ind * 4 + 3] = 1.f;
                            seed_positions[(seed_positions_ind + 1) * 4 + 3] = -1.f;

                            seed_positions_ind += 2;
                        }

                    }

                    voxel_count++;
                }
    }

    for(unsigned short n = 0; n < num_files; n++)
        delete seed_volumes[n];

    return {seed_positions, num_valid_voxels};
}

void FiberTrackingSeedCreator::setBaseFolderPath(const std::string &baseFolderPath)
{
    m_baseFolderPath = baseFolderPath;
}
