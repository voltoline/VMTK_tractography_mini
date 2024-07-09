#ifndef MODEL_2D_TRANSFORMATION_H
#define MODEL_2D_TRANSFORMATION_H

#include "model/ModelSubject.h"
#include <glm/glm.hpp>

enum class Model2DTransformationEvent
{
    NONE,
    SAGITTAL_TRANSFORMATIONS_CHANGED,
    AXIAL_TRANSFORMATIONS_CHANGED,
    CORONAL_TRANSFORMATIONS_CHANGED,
    MOUSE_MODE_CHANGED
};

class Model2DTransformation: public ModelSubject<Model2DTransformation, Model2DTransformationEvent>
{
public:

    enum class MouseMode
    {
        ROTATION,
        MOVE,
        ZOOM
    };

    class TransformationData
    {
    public:
        TransformationData(Model2DTransformationEvent event, Model2DTransformation *model_2d);

        glm::mat4 getRotationMatrix() const;
        Model2DTransformationEvent setRotationMatrix(const glm::mat4 &setRotationMatrix);

        glm::mat4 getScalingMatrix() const;
        Model2DTransformationEvent setScalingMatrix(const glm::mat4 &setScalingMatrix);

        glm::mat4 getTranslationMatrix() const;
        Model2DTransformationEvent setTranslationMatrix(const glm::mat4 &getTranslationMatrix);

        glm::mat4 getProjectionMatrix() const;
        Model2DTransformationEvent setProjectionMatrix(const glm::mat4 &getProjectionMatrix);

        glm::mat4 getMVPMatrix() const;
        glm::mat4 getModelViewMatrix() const;

        Model2DTransformationEvent resetTransformations();

    private:

        void updateProjection();

        glm::mat4 m_rotationMatrix;         /**< rotationMatrix transformation matrix */
        glm::mat4 m_scalingMatrix;          /**< scalationMatrix transformation matrix */
        glm::mat4 m_translationMatrix;      /**< traslationMatrix transformation matrix */
        glm::mat4 m_projectionMatrix;       /**< projectionMatrix transformation matrix */

        Model2DTransformationEvent m_event;
        Model2DTransformation *m_model2d;
    };

    Model2DTransformation();
    ~Model2DTransformation();

    MouseMode getMouseMode() const;
    Model2DTransformationEvent setMouseMode(const MouseMode _2D_mouse_mode);

    TransformationData& getAxialData();
    TransformationData& getCoronalData();
    TransformationData& getSagittalData();

private:

    TransformationData m_axialData;
    TransformationData m_coronalData;
    TransformationData m_sagittalData;

    MouseMode m_mouseMode;
};

#endif // MODEL_2D_TRANSFORMATION_H
