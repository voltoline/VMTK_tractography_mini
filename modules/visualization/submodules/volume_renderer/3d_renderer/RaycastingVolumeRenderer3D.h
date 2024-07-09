#ifndef RAYCASTING_VOLUME_RENDERER_3D_H
#define RAYCASTING_VOLUME_RENDERER_3D_H

#include "utilities/QCheckingOpenGL.h"
#include "BaseRaycastingVolumeRenderer3D.h"

#include <glm/glm.hpp>
#include <modules/visualization/submodules/volume_renderer/OpenGLVolumeRenderingResource.h>

class RaycastingShader;
class RaycastingPreRenderShader;

/*!
 * \brief Volume raycasting, consisting in three steps: pre-render the casting range stored in a front and back planes
 * (RaycastingPreRenderShader class), raycast the medical volume in the reange of the front and back planes (RaycastingShader class)
 * and render the result on a plane (RaycastingResultSDhader class)
 */
class RaycastingVolumeRenderer3D: public BaseRaycastingVolumeRenderer3D
{
public:
    RaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[], OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource);
    virtual ~RaycastingVolumeRenderer3D() { }

    void changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader) override;
    void restoreRaycastingShader() override;

    void addRayCastingShaderToOffScreen(QString name, BaseRaycastingShader *rcs, bool enable = false) override;

    void addFromPreviustRCS(QString name, bool enable = false);

    void addFromOriginalRCS(QString name, bool enable = false);
protected:
    OpenGLVolumeRenderingResource::sPtr m_openGLVolumeRenderingResource;

private:
    BaseRaycastingShader* createRaycastingShader() override;
    BaseRaycastingShader* createBackRaycastingShader() override;
};

#endif // RAYCASTINGVOLUMERENDERER3D_H
