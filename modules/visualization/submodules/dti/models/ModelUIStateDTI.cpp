#include "ModelUIStateDTI.h"

ModelUIStateDTI::ModelUIStateDTI()
{
    m_isDTIComputed = false;
    m_isFiberRenderingActivated = false;

    m_isGlyphRenderingActivated = false;
    m_isSuperquadricRenderingActivated = true;
    m_isLineRenderingActivated = false;

    m_isGlyphBlendingActivated = false;

    m_glyphFilterFA = 0.f;
}

bool ModelUIStateDTI::isFiberRenderingActivated() const
{
    return m_isFiberRenderingActivated;
}

ModelUIStateDTIEvent ModelUIStateDTI::setFiberRenderingActivated(bool isFiberRenderingActivated)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setFiberRenderingActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isDTIComputed && (m_isFiberRenderingActivated != isFiberRenderingActivated))
    {
        m_isFiberRenderingActivated = isFiberRenderingActivated;
        return ModelUIStateDTIEvent::FIBER_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

bool ModelUIStateDTI::isSuperquadricRenderingActivated() const
{
    return m_isSuperquadricRenderingActivated;
}

ModelUIStateDTIEvent ModelUIStateDTI::setGlyphRenderingActivated(bool isGlyphRenderingActivated)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setGlyphRenderingActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isDTIComputed && (m_isGlyphRenderingActivated != isGlyphRenderingActivated))
    {
        m_isGlyphRenderingActivated = isGlyphRenderingActivated;
        return ModelUIStateDTIEvent::GLYPH_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

bool ModelUIStateDTI::isLineRenderingActivated() const
{
    return m_isLineRenderingActivated;
}

ModelUIStateDTIEvent ModelUIStateDTI::setLineRenderingActivated(bool isLineRenderingActivated)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setLineRenderingActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isGlyphRenderingActivated && (m_isLineRenderingActivated != isLineRenderingActivated))
    {
        m_isLineRenderingActivated = isLineRenderingActivated;
        return ModelUIStateDTIEvent::GLYPH_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

ModelUIStateDTIEvent ModelUIStateDTI::setSuperquadricRenderingActivated(bool isSuperquadricRenderingActivated)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setSuperquadricRenderingActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isGlyphRenderingActivated && (m_isSuperquadricRenderingActivated != isSuperquadricRenderingActivated))
    {
        m_isSuperquadricRenderingActivated = isSuperquadricRenderingActivated;
        return ModelUIStateDTIEvent::GLYPH_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

bool ModelUIStateDTI::isGlyphBlendingActivated() const
{
    return m_isGlyphBlendingActivated;
}

ModelUIStateDTIEvent ModelUIStateDTI::setGlyphBlendingActivated(bool isSuperquadricBlendingActivated)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setSuperquadricBlendingActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isGlyphRenderingActivated)
    {
        m_isGlyphBlendingActivated = isSuperquadricBlendingActivated;
        return ModelUIStateDTIEvent::GLYPH_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

float ModelUIStateDTI::getGlyphFilterFA() const
{
    return m_glyphFilterFA;
}

ModelUIStateDTIEvent ModelUIStateDTI::setGlyphFilterFA(float superquadricFilterFA)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setSuperquadricFilterFA -->> A notification is in progress!" << std::endl;
    }

    if(m_isGlyphRenderingActivated)
    {
        m_glyphFilterFA = superquadricFilterFA;
        return ModelUIStateDTIEvent::GLYPH_RENDERING_STATE_CHANGED;
    }

    return ModelUIStateDTIEvent::NONE;
}

void ModelUIStateDTI::setIsDTIComputed(bool isDTIComputed)
{
    m_isDTIComputed = isDTIComputed;
}

bool ModelUIStateDTI::isGlyphRenderingActivated() const
{
    return m_isGlyphRenderingActivated;
}
