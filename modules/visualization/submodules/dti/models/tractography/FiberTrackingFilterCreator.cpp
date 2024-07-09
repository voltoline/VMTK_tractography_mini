#include "FiberTrackingFilterCreator.h"

FiberTrackingFilterCreator::FiberTrackingFilterCreator()
{
    m_baseFolderPath = "";
}

FiberTrackingFilterCreator::FilterBuffer FiberTrackingFilterCreator::createFilterFromVoxelIndices(vmtkDiffAcquisition &diffusion_acq, std::vector<int> voxel_indices, unsigned int num_filter)
{
    unsigned int num_voxels = voxel_indices.size()/3;

    float voxel_dim[3];
    voxel_dim[0] = 1.f / diffusion_acq.getDimensions(0);
    voxel_dim[1] = 1.f / diffusion_acq.getDimensions(1);
    voxel_dim[2] = 1.f / diffusion_acq.getDimensions(2);

    float voxel_center[3];
    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    unsigned int size_filters = num_voxels;
    std::cout << "num_filter_points " << size_filters << std::endl;

    int *filter_positions = new int[size_filters * 3];
    unsigned int voxel_count = 0, filter_positions_ind = 0;

    for(unsigned short array_index = 0; array_index < size_filters; array_index++)
    {
        unsigned short i = voxel_indices[array_index * 3];
        unsigned short j = voxel_indices[array_index * 3 + 1];
        unsigned short k = voxel_indices[array_index * 3 + 2];

        filter_positions[filter_positions_ind * 3] = static_cast<float>(i); // * voxel_dim[0]; // + voxel_center[0];
        filter_positions[filter_positions_ind * 3 + 1] = static_cast<float>(j); // * voxel_dim[1]; // + voxel_center[1];
        filter_positions[filter_positions_ind * 3 + 2] = static_cast<float>(k); // * voxel_dim[2];  // + voxel_center[2];

        filter_positions_ind += 1;

        voxel_count++;
    }

    return {filter_positions, size_filters, nullptr, num_filter};
}

FiberTrackingFilterCreator::FilterBuffer::sPtr FiberTrackingFilterCreator::createFilterFromVoxelIndices(vmtkDiffAcquisition &diffusion_acq, std::vector<int> voxel_indices, std::vector<int> num_filter)
{
    unsigned int num_voxels = voxel_indices.size()/3;

    float voxel_dim[3];
    voxel_dim[0] = 1.f / diffusion_acq.getDimensions(0);
    voxel_dim[1] = 1.f / diffusion_acq.getDimensions(1);
    voxel_dim[2] = 1.f / diffusion_acq.getDimensions(2);

    float voxel_center[3];
    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    unsigned int size_filters = num_voxels;
    std::cout << "num_filter_points " << size_filters << std::endl;

    int *filter_positions = new int[size_filters * 3];
    unsigned int voxel_count = 0, filter_positions_ind = 0;

    for(unsigned short array_index = 0; array_index < size_filters; array_index++)
    {
        unsigned short i = voxel_indices[array_index * 3];
        unsigned short j = voxel_indices[array_index * 3 + 1];
        unsigned short k = voxel_indices[array_index * 3 + 2];

        filter_positions[filter_positions_ind * 3] = static_cast<float>(i); // * voxel_dim[0]; // + voxel_center[0];
        filter_positions[filter_positions_ind * 3 + 1] = static_cast<float>(j); // * voxel_dim[1]; // + voxel_center[1];
        filter_positions[filter_positions_ind * 3 + 2] = static_cast<float>(k); // * voxel_dim[2];  // + voxel_center[2];

        filter_positions_ind += 1;

        voxel_count++;
    }

    int* filter_number_index = new int[num_filter.size()];
    for (unsigned int i = 0; i < num_filter.size(); i++)
    {
        filter_number_index[i] = num_filter[i];
    }

    return std::make_shared<FiberTrackingFilterCreator::FilterBuffer>(filter_positions, size_filters, filter_number_index, num_filter.size());
}

void FiberTrackingFilterCreator::setBaseFolderPath(const std::string &baseFolderPath)
{
    m_baseFolderPath = baseFolderPath;
}
