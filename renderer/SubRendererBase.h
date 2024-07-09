#ifndef SUB_RENDERER_BASE_H
#define SUB_RENDERER_BASE_H


class SubRendererBase
{
public:
    SubRendererBase();
    virtual ~SubRendererBase() { }

    /**
     * @brief
     *
     */
    virtual void initialize() = 0;

    /**
     * @brief
     *
     */
    virtual void render() = 0;

    /**
     * @brief
     *
     * @param int
     * @param int
     */
    virtual void resize(int, int) = 0;

    virtual void pixelRatioChanged(float) { }
    virtual void viewportChanged(int /*x*/, int /*y*/, int /*width*/, int /*height*/) { }
};

#endif // SUBRENDERER_H
