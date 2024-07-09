#include "TextRenderer3DView.h"

#include "vmtkTextRenderer.h"

TextRenderer3DView::TextRenderer3DView()
{
    m_text = nullptr;

    m_pixelRatio = 1.f;
}

void TextRenderer3DView::initialize()
{
    if(m_text == nullptr)
        m_text = new vmtkTextRenderer();

    m_text->initialize();

    setText();
}

void TextRenderer3DView::render()
{
    glEnable(GL_BLEND);

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    glViewport(0, 0, m_width, m_height);

    m_text->render();

    glDisable(GL_BLEND);
}

void TextRenderer3DView::resize(int w, int h)
{
    m_width = w;
    m_height = h;

    m_text->setGrid(w, h);
    updateText();
}

void TextRenderer3DView::setText()
{
    m_text->setDefaultSize(15 * m_pixelRatio, 6 * m_pixelRatio);

    m_text->addText("3D view");

    m_text->setTextOrigin(5 * m_pixelRatio, 5 * m_pixelRatio);
    m_text->rightAligneText(-1, 5 * m_pixelRatio);

    char new_text[30];
    sprintf(new_text, "w: %i h: %i", m_width, m_height);

    m_text->addText(new_text);
    m_text->setTextOrigin(5 * m_pixelRatio, 5 * m_pixelRatio);
}

void TextRenderer3DView::pixelRatioChanged(float pixelRatio)
{
    m_pixelRatio = pixelRatio;
}

void TextRenderer3DView::updateText()
{
    char new_text[30];
    sprintf(new_text, "w: %i h: %i", m_width, m_height);

    m_text->changeText(new_text, 1);
}
