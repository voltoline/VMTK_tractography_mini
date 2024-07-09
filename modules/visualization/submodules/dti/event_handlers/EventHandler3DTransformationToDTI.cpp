#include "EventHandler3DTransformationToDTI.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "../renderer/Renderer3DDTIData.h"

#include <modules/visualization/submodules/dti/models/ModelUIStateDTI.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/QCheckingOpenGL.h"

EventHandler3DTransformationToDTI::EventHandler3DTransformationToDTI()
{
    matrix_applied = false;

    m_model3DTransformation = nullptr;
    m_renderingSurfaceSet = nullptr;
    m_rendererDTIData = nullptr;
    m_modelUIStateDTI = nullptr;
}

void EventHandler3DTransformationToDTI::afterRendering()
{

}

void EventHandler3DTransformationToDTI::update(Model3DTransformation *, Model3DTransformationEvent event_type)
{
    switch(event_type)
    {

    case Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();
        m_rendererDTIData->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix());
        m_rendererDTIData->setProjectionMatrix(m_model3DTransformation->getProjectionMatrix(true));

        break;
    }

    default:
        break;

    }
}

void EventHandler3DTransformationToDTI::setModel3DTransformation(Model3DTransformation *model3DTransformation)
{
    m_model3DTransformation = model3DTransformation;
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void EventHandler3DTransformationToDTI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandler3DTransformationToDTI::setRendererDTIData(RendererDTIData *rendererDTIData)
{
    m_rendererDTIData = rendererDTIData;
}

void EventHandler3DTransformationToDTI::setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI)
{
    m_modelUIStateDTI = modelUIStateDTI;
}
