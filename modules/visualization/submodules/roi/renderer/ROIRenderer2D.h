#ifndef ROI_RENDERER_2D_H
#define ROI_RENDERER_2D_H

#include <renderer/SubRendererBase.h>
#include <glm/glm.hpp>

class FreeDrawCursor;

class ROIRenderer2D: public SubRendererBase
{
public:
    ROIRenderer2D();

    void initialize() override;
    void render() override;
    void resize(int w, int h) override;

    void setROIDrawingActivated(bool state);
    bool isROIDrawingActivated() const;
    void setROISlice(int id, unsigned int slice);
    void setROIColor(int id, glm::vec4 color);
    void addPoint(glm::vec3 point);
    void setCurrentActiveROI(int id);
    void removeROI(int id);
    void setCurrentSlice(unsigned short slice);
    void addNewROI(int id, int roid_id, glm::vec4 color);
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);
    void setModelViewMatrix(const glm::mat4 &modelViewMatrix);

    void viewportChanged(int x, int y, int width, int height) override;
    int* getViewport();

    void setCurrentTractID(int currentTractID);

private:
    FreeDrawCursor *m_freeDrawCursor;
    bool m_isROIDrawingActivated;
    bool m_isInitialized;
    unsigned short m_slice;

    glm::mat4 m_projectionMatrix;     /**< projection matrix */
    glm::mat4 m_modelViewMatrix;  /**< modelview transformation matrix */

    int m_viewport[4]; /**< viewport for rendered slice */

    int m_currentTractID;
};

#endif // ROI_RENDERER_2D_H
