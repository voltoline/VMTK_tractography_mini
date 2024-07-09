#ifndef MODEL_UI_STATE_DTI_H
#define MODEL_UI_STATE_DTI_H

#include <model/ModelSubject.h>

enum class ModelUIStateDTIEvent
{
    NONE,
    FIBER_RENDERING_STATE_CHANGED,
    GLYPH_RENDERING_STATE_CHANGED,
};

class ModelUIStateDTI: public ModelSubject<ModelUIStateDTI, ModelUIStateDTIEvent>
{
public:
    ModelUIStateDTI();

    bool isFiberRenderingActivated() const;
    ModelUIStateDTIEvent setFiberRenderingActivated(bool isFiberRenderingActivated);

    bool isSuperquadricRenderingActivated() const;
    ModelUIStateDTIEvent setSuperquadricRenderingActivated(bool isSuperquadricRenderingActivated);

    bool isGlyphBlendingActivated() const;
    ModelUIStateDTIEvent setGlyphBlendingActivated(bool isGlyphBlendingActivated);

    float getGlyphFilterFA() const;
    ModelUIStateDTIEvent setGlyphFilterFA(float superquadricFilterFA);

    void setIsDTIComputed(bool isDTIComputed);

    bool isGlyphRenderingActivated() const;
    ModelUIStateDTIEvent setGlyphRenderingActivated(bool isGlyphRenderingActivated);

    bool isLineRenderingActivated() const;
    ModelUIStateDTIEvent setLineRenderingActivated(bool isLineRenderingActivated);

private:
    bool m_isDTIComputed;

    bool m_isGlyphRenderingActivated;
    bool m_isSuperquadricRenderingActivated;
    bool m_isLineRenderingActivated;

    bool m_isFiberRenderingActivated;
    bool m_isGlyphBlendingActivated;

    float m_glyphFilterFA;
};

#endif // MODEL_UI_STATE_DTI_H
