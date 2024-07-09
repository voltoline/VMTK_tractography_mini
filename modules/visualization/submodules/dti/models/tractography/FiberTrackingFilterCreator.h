#ifndef FIBERTRACKINGFILTERCREATOR_H
#define FIBERTRACKINGFILTERCREATOR_H

#include <memory>

#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"

class FiberTrackingFilterCreator
{
public:
    FiberTrackingFilterCreator();

    typedef struct _filter_buffer
    {
        typedef std::shared_ptr<struct _filter_buffer> sPtr;

        int *mFilterBuffer;
        int *mFilterIndex;
        unsigned int mSizeFilter;
        unsigned int mNumFilter;

        _filter_buffer()
        {
            mFilterBuffer = nullptr;
            mFilterIndex = nullptr;
            mSizeFilter = 0;
            mNumFilter = 0;
        }

        ~_filter_buffer()
        {
            if(mFilterBuffer)
                delete[] mFilterBuffer;

            if(mFilterIndex)
                delete[] mFilterIndex;
        }

        _filter_buffer(int *filter_buffer, unsigned int size, int* filter_index, unsigned int num_filter)
        {
            mFilterBuffer = filter_buffer;
            mSizeFilter = size;
            mFilterIndex = filter_index;
            mNumFilter = num_filter;
        }

    } FilterBuffer;

    FilterBuffer createFilterFromVoxelIndices(vmtkDiffAcquisition &diffusion_acq, std::vector<int> voxel_indices, unsigned int num_filter);
    FilterBuffer::sPtr createFilterFromVoxelIndices(vmtkDiffAcquisition &diffusion_acq, std::vector<int> voxel_indices, std::vector<int> num_filter);

    void setBaseFolderPath(const std::string &baseFolderPath);

private:
    std::string m_baseFolderPath;

};

#endif // FIBERTRACKINGFILTERCREATOR_H
