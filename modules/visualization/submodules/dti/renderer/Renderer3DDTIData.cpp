#include "Renderer3DDTIData.h"

#include <utilities/QCheckingOpenGL.h>

#include <modules/visualization/submodules/volume_renderer/3d_renderer/RaycastingVolumeRenderer3D.h>
#include <modules/visualization/submodules/volume_renderer/3d_renderer/RaycastingShader.h>
#include <modules/visualization/submodules/volume_renderer/DepthRenderer.h>
#include "fiber/FiberRenderer.h"

RendererDTIData::RendererDTIData()
{
    m_renderSuperquadric = false;
    m_renderLines = false;
    m_renderTracts = false;

    m_faThreshold = 0.f;
    m_isBlendingActivated = false;

    m_depthRenderer = nullptr;
    m_fiberRender = nullptr;

    m_dti = nullptr;

    m_dtiOpenGLResource = nullptr;
    m_raycastingVolumeRenderer3D = nullptr;
    m_diff_acq = nullptr;

    m_height = -1;
    m_width = -1;
}

void RendererDTIData::initialize()
{

}

void RendererDTIData::render()
{

    if(m_renderSuperquadric)
    {
        if (m_width < m_height) {
            glViewport (0, (m_height-m_width)/2, m_width, m_width);
        } else {
            glViewport ((m_width-m_height)/2, 0, m_height, m_height);
        }
    }

    if(m_renderTracts)
    {
        glViewport(0, 0, m_width, m_height);

        /* Onboard Intel Graphics draws trash in the color buffer. To overcome
           this problem, we blend the incoming fragment with the older one. (Ting, 30/10/2019)
        */
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_ONE);
        glEnable(GL_DEPTH_TEST);
        {
            if(m_depthRenderer == nullptr)
            {
                m_depthRenderer = new DepthRenderer();
                m_depthRenderer->initialize();
            }

            m_depthRenderer->execute(m_raycastingVolumeRenderer3D->getRaycastingShader()->getRayTracedFBO().getDepthTexture());
        }
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        if (m_width < m_height) {
            glViewport(0, (m_height-m_width)/2, m_width, m_width);
        } else {
            glViewport((m_width-m_height)/2, 0, m_height, m_height);
        }

        if(m_fiberRender == nullptr)
        {
            m_fiberRender = new FiberRenderer(m_dti, m_fScaleFactors);
            m_fiberRender->initialize();
            m_fiberRender->setFiberClusters(m_fiberClusterSet);
        }

        m_fiberRender->setModelViewMatrix(m_modelViewMatrix);
        m_fiberRender->setProjectionMatrix(m_projectionMatrix);
        m_fiberRender->render();
    }

}

void RendererDTIData::resize(int width, int height)
{
    m_height = height;
    m_width = width;
}

void RendererDTIData::enableTractographyRendering(bool render_tracts)
{
    m_renderTracts = render_tracts;
}

void RendererDTIData::setScaleFactors(float scale_factors[])
{
    m_fScaleFactors[0] = scale_factors[0];
    m_fScaleFactors[1] = scale_factors[1];
    m_fScaleFactors[2] = scale_factors[2];
    m_fScaleFactors[3] = scale_factors[3];
}

void RendererDTIData::setFiberClusters(std::vector<FiberClusterSet::sPtr> fiber_cluster_set)
{
    m_fiberClusterSet = fiber_cluster_set;

    if(m_fiberRender != nullptr)
        m_fiberRender->setFiberClusters(m_fiberClusterSet);
}

void RendererDTIData::setFiberStepRange(int min, int max)
{
    m_fiberRender->setFiberStepRange(min, max);
}

void RendererDTIData::setFiberPaintingActivated(bool state)
{
    m_fiberRender->setFiberPaintingActivated(state);
}

void RendererDTIData::setDTI(const DTI *dti)
{
    m_dti = dti;
}

void RendererDTIData::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D)
{
    m_raycastingVolumeRenderer3D = raycastingVolumeRenderer3D;
}

void RendererDTIData::setModelViewMatrix(const glm::mat4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}

void RendererDTIData::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

void RendererDTIData::setDtiOpenGLResource(DTIOpenGLResource *dtiOpenGLResource)
{
    m_dtiOpenGLResource = dtiOpenGLResource;
}

void RendererDTIData::setDiff_acq(vmtkDiffAcquisition *diff_acq)
{
    m_diff_acq = diff_acq;
}
