#include "Model2DSlice.h"

Model2DSlice::Model2DSlice()
{
    m_sliceData.mAxialMin = 0;
    m_sliceData.mAxialMax = 0;
    m_sliceData.mCoronalMin = 0;
    m_sliceData.mCoronalMax = 0;
    m_sliceData.mSagittalMin = 0;
    m_sliceData.mSagittalMax = 0;

    m_voxelOrder = VoxelOrder::LAS;

    resetSlices();
}

Model2DSlice::~Model2DSlice()
{

}

Model2DSliceEvent Model2DSlice::resetSlices()
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::resetSlices -->> A notification is in progress!" << std::endl;
    }

    m_sliceData.mAxial = m_sliceData.mAxialMax / 2;
    m_sliceData.mCoronal = m_sliceData.mCoronalMax / 2;
    m_sliceData.mSagittal = m_sliceData.mSagittalMax / 2;

    return Model2DSliceEvent::SLICING_CHANGED;
}

void Model2DSlice::setAxialInterval(unsigned short min, unsigned short max)
{
    m_sliceData.mAxialMin = min;
    m_sliceData.mAxialMax = max;
}

void Model2DSlice::setSagittalInterval(unsigned short min, unsigned short max)
{
    m_sliceData.mSagittalMin = min;
    m_sliceData.mSagittalMax = max;
}

void Model2DSlice::setCoronalInterval(unsigned short min, unsigned short max)
{
    m_sliceData.mCoronalMin = min;
    m_sliceData.mCoronalMax = max;
}

void Model2DSlice::getAxialInterval(unsigned short &min, unsigned short &max)
{
    min = m_sliceData.mAxialMin;
    max = m_sliceData.mAxialMax;
}

void Model2DSlice::getSagittalInterval(unsigned short &min, unsigned short &max)
{
    min = m_sliceData.mSagittalMin;
    max = m_sliceData.mSagittalMax;
}

void Model2DSlice::getCoronalInterval(unsigned short &min, unsigned short &max)
{
    min = m_sliceData.mCoronalMin;
    max = m_sliceData.mCoronalMax;
}

Model2DSliceEvent Model2DSlice::setAxialSlice(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::setAxialSlice -->> A notification is in progress!" << std::endl;
    }

    if(m_sliceData.mAxialMin > value)
        value = m_sliceData.mAxialMin;

    else if(m_sliceData.mAxialMax < value)
        value = m_sliceData.mAxialMax;

    if(m_sliceData.mAxial == value)
        return Model2DSliceEvent::NONE;

    m_sliceData.mAxial = value;

    return Model2DSliceEvent::SLICING_CHANGED;
}

Model2DSliceEvent Model2DSlice::setCoronalSlice(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::setCoronalSlice -->> A notification is in progress!" << std::endl;
    }

    if(m_sliceData.mCoronalMin > value)
        value = m_sliceData.mCoronalMin;

    else if(m_sliceData.mCoronalMax < value)
        value = m_sliceData.mCoronalMax;

    if(m_sliceData.mCoronal == value)
        return Model2DSliceEvent::NONE;

    m_sliceData.mCoronal = value;

    return Model2DSliceEvent::SLICING_CHANGED;

}

Model2DSliceEvent Model2DSlice::setSagittalSlice(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::setSagittalSlice -->> A notification is in progress!" << std::endl;
    }

    if(m_sliceData.mSagittalMin > value)
        value = m_sliceData.mSagittalMin;

    else if(m_sliceData.mSagittalMax < value)
        value = m_sliceData.mSagittalMax;

    if(m_sliceData.mSagittal == value)
        return Model2DSliceEvent::NONE;

    m_sliceData.mSagittal = value;

    return Model2DSliceEvent::SLICING_CHANGED;

}

unsigned short Model2DSlice::getAxialSlice(VoxelOrder /*order*/) const
{
    return m_sliceData.mAxial;
}

unsigned short Model2DSlice::getCoronalSlice(VoxelOrder order) const
{
    if(order == VoxelOrder::LPS)
        return m_sliceData.mCoronalMax - m_sliceData.mCoronal + 1;

    else
        return m_sliceData.mCoronal;
}

unsigned short Model2DSlice::getSagittalSlice(VoxelOrder order) const
{
    if(order == VoxelOrder::LPS)
    {
        if(isRadiological())
            return m_sliceData.mSagittal;
        else
            return m_sliceData.mSagittalMax - m_sliceData.mSagittal + 1;

    } else
        return m_sliceData.mSagittal;
}

Model2DSliceEvent Model2DSlice::setRadiological()
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::setRadiologicalVoxelOrder -->> A notification is in progress!" << std::endl;
    }

    m_voxelOrder = VoxelOrder::LAS;

    return Model2DSliceEvent::VOXEL_ORDER_CHANGED;
}

Model2DSliceEvent Model2DSlice::setNeurological()
{
    if(isNotifying())
    {
        std::cerr << "Model2DSlice::setNeurologicalVoxelOrder -->> A notification is in progress!" << std::endl;
    }

    m_voxelOrder = VoxelOrder::RAS;

    return Model2DSliceEvent::VOXEL_ORDER_CHANGED;
}

bool Model2DSlice::isRadiological() const
{
    return (m_voxelOrder == VoxelOrder::LAS);
}

bool Model2DSlice::isNeurological() const
{
    return (m_voxelOrder == VoxelOrder::RAS);
}

void Model2DSlice::getAxialTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    float slice_tex = (m_sliceData.mAxial / float(m_sliceData.mAxialMax)) - (1 / float(m_sliceData.mAxialMax * 2));

    if (m_voxelOrder == VoxelOrder::LAS)
    {
        v1 = glm::vec3(1.0f, 0.0f, slice_tex);
        v2 = glm::vec3(0.0f, 0.0f, slice_tex);
        v3 = glm::vec3(0.0f, 1.0f, slice_tex);
        v4 = glm::vec3(1.0f, 1.0f, slice_tex);

    } else {     // VMTK_RAS

        v1 = glm::vec3(0.0f, 0.0f, slice_tex);
        v2 = glm::vec3(1.0f, 0.0f, slice_tex);
        v3 = glm::vec3(1.0f, 1.0f, slice_tex);
        v4 = glm::vec3(0.0f, 1.0f, slice_tex);
    }
}

void Model2DSlice::getCoronalTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    float slice_tex = (m_sliceData.mCoronal / float(m_sliceData.mCoronalMax)) - (1 / float(m_sliceData.mCoronalMax * 2));

    if (m_voxelOrder == VoxelOrder::LAS)
    {
        v1 = glm::vec3(1.0f, (1.0 - slice_tex), 1.0f);
        v2 = glm::vec3(0.0f, (1.0 - slice_tex), 1.0f);
        v3 = glm::vec3(0.0f, (1.0 - slice_tex), 0.0f);
        v4 = glm::vec3(1.0f, (1.0 - slice_tex), 0.0f);

    } else {     // VMTK_RAS

        v1 = glm::vec3(0.0f, (1.0 - slice_tex), 1.0f);
        v2 = glm::vec3(1.0f, (1.0 - slice_tex), 1.0f);
        v3 = glm::vec3(1.0f, (1.0 - slice_tex), 0.0f);
        v4 = glm::vec3(0.0f, (1.0 - slice_tex), 0.0f);
    }
}

void Model2DSlice::getSagittalTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    float slice_tex = (m_sliceData.mSagittal / float(m_sliceData.mSagittalMax)) - (1 / float(m_sliceData.mSagittalMax * 2));

    if(m_voxelOrder == VoxelOrder::LAS)
    {
        /* A->P is the usual view for radiologists (29/08/2015) */
        v1 = glm::vec3(slice_tex, 1.0f, 1.0f);
        v2 = glm::vec3(slice_tex, 0.0f, 1.0f);
        v3 = glm::vec3(slice_tex, 0.0f, 0.0f);
        v4 = glm::vec3(slice_tex, 1.0f, 0.0f);

    } else {     // VMTK_RAS

        v1 = glm::vec3((1.0 - slice_tex), 0.0f, 1.0f);
        v2 = glm::vec3((1.0 - slice_tex), 1.0f, 1.0f);
        v3 = glm::vec3((1.0 - slice_tex), 1.0f, 0.0f);
        v4 = glm::vec3((1.0 - slice_tex), 0.0f, 0.0f);
    }
}
