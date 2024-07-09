#ifndef TEXT_RENDERER_3D_VIEW_H
#define TEXT_RENDERER_3D_VIEW_H

#include <renderer/SubRendererBase.h>

class vmtkTextRenderer;

class TextRenderer3DView: public SubRendererBase
{
public:
    TextRenderer3DView();

    void initialize();

    /**
     * @brief renders the texts on the screen
     */
    void render();
    void resize(int w, int h);

    vmtkTextRenderer *m_text;  /**< render text on the screen */

    void pixelRatioChanged(float pixelRatio);

private:
    void updateText();

    /**
     * @brief sets the texts to be rendered
     */
    void setText();

    float m_pixelRatio;

    int m_width;
    int m_height;
};

#endif // TEXT_RENDERER_3D_VIEW_H
