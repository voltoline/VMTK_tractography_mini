#include "RaycastingVolumeRenderer3D.h"

#include "utilities/QManagementResources.h"
#include "utilities/functions/vmtkGeneralFunctions.h"
#include "RaycastingPreRenderShader.h"
#include "RaycastingShader.h"

RaycastingVolumeRenderer3D::RaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[], OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource):
    BaseRaycastingVolumeRenderer3D(volume_dim, scale_factors)
{
    m_openGLVolumeRenderingResource = opengl_volume_rendering_resource;
}

BaseRaycastingShader *RaycastingVolumeRenderer3D::createRaycastingShader()
{
    RaycastingShader *raycasting_shader = new RaycastingShader();
    raycasting_shader->setVolumeDim(m_volumeDimension);
    raycasting_shader->setScaleFactors(m_scaleFactors);
    raycasting_shader->setOpenGLVolumeRenderingResource(m_openGLVolumeRenderingResource);

    return raycasting_shader;
}

BaseRaycastingShader *RaycastingVolumeRenderer3D::createBackRaycastingShader()
{
    RaycastingShader *back_raycasting_shader = new RaycastingShader(static_cast<RaycastingShader*>(getRaycastingShader()));
    back_raycasting_shader->setFragmentShaderPath(":/depth_map_back_frag");

    return back_raycasting_shader;
}

void RaycastingVolumeRenderer3D::changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader)
{
    RaycastingShader *raycasting_shader = dynamic_cast<RaycastingShader*>(new_raycasting_shader);

    if(raycasting_shader)
    {
        raycasting_shader->copyData(static_cast<RaycastingShader*>(getRaycastingShader()));
        BaseRaycastingVolumeRenderer3D::changeRaycastingShader(raycasting_shader);

    } else
        throw std::runtime_error("RaycastingVolumeRenderer3D::changeRaycastingShader -->> Object is not of the RaycastingShader class!");
}

void RaycastingVolumeRenderer3D::restoreRaycastingShader()
{
    if(getPreviousRaycastingShader())
    {
        static_cast<RaycastingShader*>(getPreviousRaycastingShader())->copyData(static_cast<RaycastingShader*>(getRaycastingShader()));
        BaseRaycastingVolumeRenderer3D::restoreRaycastingShader();

    } else
        std::cerr << "RaycastingVolumeRenderer3D::restoreRaycastingShader -->> Previous raycasting shader is NULL!" << std::endl;
}

void RaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen(QString name, BaseRaycastingShader *rcs, bool enable){
    RaycastingShader *raycasting_shader = dynamic_cast<RaycastingShader*>(rcs);

    if(raycasting_shader)
    {
        raycasting_shader->copyData(static_cast<RaycastingShader*>(getRaycastingShader()));
        BaseRaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen(name,rcs, enable);

    } else
        throw std::runtime_error("RaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen -->> Object is not of the RaycastingShader class!");
}

void RaycastingVolumeRenderer3D::addFromPreviustRCS(QString name, bool enable){
    if( getRCSOffScreen(name) == nullptr ){
        RaycastingShader *prevRCS = new RaycastingShader(static_cast<RaycastingShader*>(getPreviousRaycastingShader()));
        if(prevRCS){
            prevRCS->initialize();
            BaseRaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen(name,prevRCS, enable);
        }
    }
}
void RaycastingVolumeRenderer3D::addFromOriginalRCS(QString name, bool enable){
    if( getRCSOffScreen(name) == nullptr ){
        RaycastingShader *prevRCS = new RaycastingShader(static_cast<RaycastingShader*>( getRaycastingShader() ));
        if(prevRCS){
            prevRCS->initialize();
            BaseRaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen(name,prevRCS, enable);
        }
    }
}
