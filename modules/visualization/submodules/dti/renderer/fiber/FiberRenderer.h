#ifndef FIBER_RENDERER_H
#define FIBER_RENDERER_H

#include "utilities/QCheckingOpenGL.h"
#include "../../models/tractography/FiberClusterSet.h"
#include <iostream>
#include "utilities/vmath.h"
#include <glm/glm.hpp>
#include <memory>

class DTI;
class vmtkManagementShader;

class FiberRenderer
{
public:

    FiberRenderer(const DTI *dti, const float *scaleFactors);
    ~FiberRenderer();

    virtual void initialize();
    virtual void resize(int w, int h);
    void setModelViewMatrix(glm::mat4 modelView);
    void setProjectionMatrix(glm::mat4 projection);

    void render();

    void setFiberClusters(std::vector<FiberClusterSet::sPtr> fiber_cluster_set);

    void setFiberStepRange(int min, int max);

    void setFiberPaintingActivated(bool state);

private:

    typedef struct multi_draw_array_info
    {
        GLint *first;
        GLint *count;
        GLsizei primcount;

    } MultiDrawArrayInfo;

    typedef struct _fiber_rendering_data
    {
        unsigned short m_numFiberGroups;
        GLuint *m_VBOFiberVertices;
        GLuint *m_VBOFiberDirections;
        std::vector<MultiDrawArrayInfo> m_vecMultiDrawArrayInfo;

        _fiber_rendering_data()
        {
            m_numFiberGroups = 0;
            m_VBOFiberVertices = nullptr;
            m_VBOFiberDirections = nullptr;
        }

    } FiberRenderingData;

    std::vector<FiberRenderingData> m_fiberRenderingData;

    void createMainDirectionTex();

    void renderFibers();
    void set3DViewMatrix();

    void setSagittalViewLAS();
    void setAxialViewLAS();
    void setCoronalViewLAS();

    void setSagittalViewRAS();
    void setAxialViewRAS();
    void setCoronalViewRAS();

    template<typename T>
    void deleteBuffer(T **buffer);
    void reset();

    GLuint m_vao;

    vmath::Matrix4f m_model_view_matrix;
    vmath::Matrix4f m_reorient_point_matrix;
    vmath::Matrix4f m_reorient_point_matrix_scale;
    vmath::Matrix4f m_reorient_point_matrix_rotation;
    vmath::Matrix4f m_reorient_point_matrix_translation;

    GLshort seed_position_attr_loc;

    GLshort fiber_point_positions_loc;
    GLshort fiber_point_direction_loc;

    vmtkManagementShader* mFiberRenderingProgram;

    float m_scale_factors[3];

    const DTI *m_dti;
    bool m_initialized;

    glm::mat4 m_projectionMatrix;  /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;  /**< modelview transformation matrix */

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
    double acc_time = 0;
    double acc_query = 0;
    int count_acc = 0;
#endif
};

#endif // FIBER_RENDERER_H
