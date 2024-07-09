#ifndef MODEL_2D_SLICE_H
#define MODEL_2D_SLICE_H

#include "model/ModelSubject.h"
#include <glm/glm.hpp>

enum class Model2DSliceEvent
{
    NONE,
    SLICING_CHANGED,
    VOXEL_ORDER_CHANGED,
};

class Model2DSlice: public ModelSubject<Model2DSlice, Model2DSliceEvent>
{
public:
    enum class VoxelOrder
    {
        LAS = 0,    /**< LeftPosteriorSuperior (Radiological) --> LeftAnteriorSuperior (Ting, 10/12/2019) */
        RAS = 1,     /**< RightAnteriorSuperior (Neurological) */
        LPS,
        CURRENT
    };

    Model2DSlice();
    ~Model2DSlice();

    void setAxialInterval(unsigned short min, unsigned short max);
    void setSagittalInterval(unsigned short min, unsigned short max);
    void setCoronalInterval(unsigned short min, unsigned short max);

    void getAxialInterval(unsigned short &min, unsigned short &max);
    void getSagittalInterval(unsigned short &min, unsigned short &max);
    void getCoronalInterval(unsigned short &min, unsigned short &max);

    Model2DSliceEvent setAxialSlice(unsigned short value);
    Model2DSliceEvent setCoronalSlice(unsigned short value);
    Model2DSliceEvent setSagittalSlice(unsigned short value);

    unsigned short getAxialSlice(VoxelOrder order = VoxelOrder::CURRENT) const;
    unsigned short getCoronalSlice(VoxelOrder order = VoxelOrder::CURRENT) const;
    unsigned short getSagittalSlice(VoxelOrder order = VoxelOrder::CURRENT) const;

    Model2DSliceEvent setRadiological();
    Model2DSliceEvent setNeurological();
    bool isRadiological() const;
    bool isNeurological() const;

    Model2DSliceEvent resetSlices();

    void getAxialTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);
    void getCoronalTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);
    void getSagittalTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);

private:

    typedef struct
    {
        unsigned short mAxial;
        unsigned short mCoronal;
        unsigned short mSagittal;

        unsigned short mAxialMin;
        unsigned short mAxialMax;
        unsigned short mCoronalMin;
        unsigned short mCoronalMax;
        unsigned short mSagittalMin;
        unsigned short mSagittalMax;

    } SliceData;

    SliceData m_sliceData;

    VoxelOrder m_voxelOrder;
};

#endif // MODEL_2D_SLICE_H
