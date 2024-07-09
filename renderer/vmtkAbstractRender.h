#ifndef VMTK_ABSTRACT_RENDER_H
#define VMTK_ABSTRACT_RENDER_H

#include <vector>
#include <memory>
#include "SubRendererBase.h"

/**
 * @brief
 *
 */
class vmtkAbstractRender {

public:
    typedef std::shared_ptr<vmtkAbstractRender> sPtr;

    /**
     * @brief
     *
     */
    vmtkAbstractRender();

    /**
     * @brief
     *
     */
    virtual void initialize();

    /**
     * @brief
     *
     */
    virtual void render();

    /**
     * @brief
     *
     * @param int
     * @param int
     */
    virtual void resize(int width, int height);

    virtual void updatePixelRatio(float value);

    virtual void updateViewport(int x, int y, int width, int height);

    void addSubRenderer(SubRendererBase * sub_renderer);
    void removeSubRenderer(SubRendererBase *sub_renderer);

private:

    std::vector<SubRendererBase*> m_subRenderers;
};

#endif // VMTK_ABSTRACT_RENDER_H
