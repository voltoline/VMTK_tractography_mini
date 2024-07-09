#ifndef MODEL_2D_RENDERING_H
#define MODEL_2D_RENDERING_H

#include "model/ModelSubject.h"
#include <glm/glm.hpp>

enum class Model2DRenderingEvent
{
    NONE,
    AXIAL_SLICE_CHANGED,
    CORONAL_SLICE_CHANGED,
    SAGITTAL_SLICE_CHANGED,
    SLICE_PLANE_CHANGED,
    AXIAL_TRANSFORMATION_CHANGED,
    CORONAL_TRANSFORMATION_CHANGED,
    SAGITTAL_TRANSFORMATION_CHANGED,
};

class Model2DRendering: public ModelSubject<Model2DRendering, Model2DRenderingEvent>
{
public:
    class SliceData
    {
    public:
        SliceData(float scale_factors[], Model2DRenderingEvent plane_event, Model2DRenderingEvent transform_event, Model2DRendering *model_2d);

        Model2DRenderingEvent setPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);
        void getPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);

        Model2DRenderingEvent setPlaneTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);
        void getPlaneTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);

        Model2DRenderingEvent setModelViewMatrix(const glm::mat4 &modelViewMatrix);
        Model2DRenderingEvent setProjectionMatrix(const glm::mat4 &projectionMatrix);

        glm::mat4 getModelViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

    private:

        typedef struct
        {
            glm::vec3 m_v1;           /**< coordinates of v1 */
            glm::vec3 m_v2;           /**< coordinates of v2 */
            glm::vec3 m_v3;           /**< coordinates of v3 */
            glm::vec3 m_v4;           /**< coordinates of v4 */

        } SliceCoord;

        SliceCoord m_planePos;
        SliceCoord m_planeTex;

        glm::mat4 m_modelViewMatrix;
        glm::mat4 m_projectionMatrix;

        Model2DRenderingEvent m_planeEvent;
        Model2DRenderingEvent m_transformEvent;
        Model2DRendering *m_model2D;

        float m_scaleFactors[4];
    };

    Model2DRendering(float scale_factors[4]);

    void getScaleFactors(float scale_factors[4]);

    SliceData& getAxialData();
    SliceData& getCoronalData();
    SliceData& getSagittalData();

private:

    SliceData m_axialData;
    SliceData m_coronalData;
    SliceData m_sagittalData;

    float m_scaleFactors[4];
};

#endif // MODEL_2D_RENDERING_H
