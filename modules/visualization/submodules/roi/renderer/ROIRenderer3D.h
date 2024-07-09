#ifndef ROI_RENDERER_3D_H
#define ROI_RENDERER_3D_H

#include <glm/glm.hpp>

class FreeDrawCursor;

class ROIRenderer3D
{
public:
    ROIRenderer3D();

    void initialize();
    void render();
    void resize(int width, int height);

    void setEnableFreeDraw(bool enable);
    void addNewPointFreeDraw3D(glm::vec3 point);
    void addNewFreeDraw3D(int id, int tract_id);
    void removeFreeDraw3D(int index);
    void setCurrentActiveFreeDraw3D(int index);
    void setModelViewMatrix(glm::mat4 modelViewMatrix);

    void setCurrentTractID(int currentTractID);

private:
    void makeFreeDraw3D();
    void drawFreeDraw3Dfrom3D();

    int m_currentTractID;

    FreeDrawCursor * m_freeLineDraw;
    bool m_enabeFreeDraw;
    glm::mat4 m_modelViewMatrix;
    int m_w;                     /**< viewport width */
    int m_h;
};

#endif // ROI_RENDERER_3D_H
