#ifndef FIBER_TRACKING_SEED_CREATOR_H
#define FIBER_TRACKING_SEED_CREATOR_H

#include <iostream>
#include <vector>
#include <memory>

class DTI;

class FiberTrackingSeedCreator
{
public:
    FiberTrackingSeedCreator();

    typedef struct _seed_buffer
    {
        typedef std::shared_ptr<struct _seed_buffer> sPtr;

        float *mSeedBuffer;
        unsigned int mNumSeeds;

        _seed_buffer()
        {
            mSeedBuffer = nullptr;
            mNumSeeds = 0;
        }

        ~_seed_buffer()
        {
            if(mSeedBuffer == nullptr)
                delete[] mSeedBuffer;
        }

        _seed_buffer(float *seed_buffer, unsigned int num_seed)
        {
            mSeedBuffer = seed_buffer;
            mNumSeeds = num_seed;
        }

    } SeedBuffer;

    SeedBuffer::sPtr createSeedsFromVoxelIndices(const DTI *dti, float fractional_anisotropy_threshold, std::vector<int> voxel_indices);
    SeedBuffer createSeedsFromWholeBrain(const DTI *dti, float fractional_anisotropy_threshold);
    SeedBuffer loadSeedsFromNIfTIFiles(std::string file_paths[], unsigned short num_files);

    void setBaseFolderPath(const std::string &baseFolderPath);

private:
    std::string m_baseFolderPath;
};

#endif // FIBER_TRACKING_SEED_CREATOR_H
