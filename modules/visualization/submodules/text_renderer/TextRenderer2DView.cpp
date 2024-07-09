#include "TextRenderer2DView.h"

TextRenderer2DView::TextRenderer2DView()
{
    m_initialized = false;
    m_text = new vmtkTextRenderer();

    m_pixelRatio = 1.f;
}

void TextRenderer2DView::initialize()
{
    if(m_initialized)
        return;

    m_text->initialize();

    setText();

    m_initialized = true;
}

void TextRenderer2DView::render()
{
    if(!m_initialized)
        initialize();

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    glViewport(0, 0, m_width, m_height);

    m_text->render();

    if (m_width < m_height) {
        glViewport (0, (m_height-m_width)/2, m_width, m_width);
    } else {
        glViewport ((m_width-m_height)/2, 0, m_height, m_height);
    }

    glDisable(GL_BLEND);
}

void TextRenderer2DView::resize(int w, int h)
{
    if(!m_initialized)
        initialize();

    m_width = w;
    m_height = h;

    m_text->setGrid(w, h);
    updateText();
}

void TextRenderer2DView::setCurrentSlice(unsigned int slice)
{
    m_nSlice = slice;
    updateText();
}

void TextRenderer2DView::setVoxelOrder(const TVoxelOrder tVoxelOrder)
{
    m_tVoxelOrder = tVoxelOrder;
    updateText();
}

void TextRenderer2DView::updateText()
{
    char new_text[50];

    if(m_tAxis == VMTK_X_AXIS)
        sprintf(new_text, "Im: %i/%i SAG", m_nSlice, m_maxSlice);

    else if(m_tAxis == VMTK_Y_AXIS)
        sprintf(new_text, "Im: %i/%i COR", m_nSlice, m_maxSlice);

    else
        sprintf(new_text, "Im: %i/%i AXI", m_nSlice, m_maxSlice);

    m_text->changeText(new_text, 0);

    if(m_tAxis == VMTK_X_AXIS)
    {
        if(m_tVoxelOrder == VMTK_LAS)
        {
            m_text->changeText("A", 1);
            m_text->changeText("P", 2);

        }else{
            m_text->changeText("P", 1);
            m_text->changeText("A", 2);
        }
    }
    else
    {
        if(m_tVoxelOrder == VMTK_LAS)
        {
            m_text->changeText("R", 1);
            m_text->changeText("L", 2);

        }else{
            m_text->changeText("L", 1);
            m_text->changeText("R", 2);
        }
    }

    unsigned short center = (m_height / 2)  * m_pixelRatio;
    m_text->setTextOrigin(center, 5 * m_pixelRatio, 1);
    m_text->setTextOrigin(center, (m_width - 10) * m_pixelRatio, 2);

    m_text->setTextOrigin((m_height - 20) * m_pixelRatio, 5 * m_pixelRatio, 3);

    m_text->setTextOrigin((m_height - 35) * m_pixelRatio, 5 * m_pixelRatio, 7);
    m_text->rightAligneText(7 * m_pixelRatio, 5 * m_pixelRatio);

    m_text->setTextOrigin((m_height - 20) * m_pixelRatio, 5 * m_pixelRatio, 8);
    m_text->rightAligneText(8 * m_pixelRatio, 5 * m_pixelRatio);
}

void TextRenderer2DView::pixelRatioChanged(float pixelRatio)
{
    m_pixelRatio = pixelRatio;
}

void TextRenderer2DView::setAxis(const TAxis &tAxis)
{
    m_tAxis = tAxis;
}

void TextRenderer2DView::setText()
{
    m_text->setDefaultSize(15 * m_pixelRatio, 6 * m_pixelRatio);

    m_text->addText("Im: 0/0");
    m_text->setTextOrigin(20 * m_pixelRatio, 5 * m_pixelRatio);
    m_text->setTextColor(0.f, 1.f, 0.f, 1.0f);

    char left[2] = "0", right[2] = "0";

    if(m_tAxis == VMTK_X_AXIS)
    {
        /* According to Vanessa, the sagital view should be A->P in LAS (radiologial view) - 29/08/2015 */
        if(m_tVoxelOrder == VMTK_LAS)
        {
            left[0]  = 'A';
            right[0]  = 'P';
        } else {
            left[0]  = 'P';
            right[0]  = 'A';
        }
    }

    else {
        if(m_tVoxelOrder == VMTK_LAS)
        {
            left[0]  = 'R';
            right[0]  = 'L';

        }else{
            left[0]  = 'L';
            right[0]  = 'R';
        }
    }

    m_text->addText(left);
    m_text->setTextColor(1.f, 1.f, 0.f, 1.0f);

    m_text->addText(right);
    m_text->setTextColor(1.f, 1.f, 0.f, 1.0f);

    char new_text[30];

    sprintf(new_text, "Slab: %.3fmm", m_pixelSpacing);

    m_text->addText(new_text);

    m_text->addText(m_institutionName.c_str());
    m_text->setTextOrigin(20 * m_pixelRatio, 5 * m_pixelRatio);
    m_text->rightAligneText();

    m_text->addText(m_manufacturesModel.c_str());
    m_text->setTextOrigin(35 * m_pixelRatio, 5 * m_pixelRatio);
    m_text->rightAligneText(5 * m_pixelRatio, 5 * m_pixelRatio);

    m_text->addText(m_serieDescription.c_str());
    m_text->setTextOrigin(35 * m_pixelRatio, 5 * m_pixelRatio);

    m_text->addText(("ID: " + m_patientID).c_str());

    m_text->addText(("Birth: " + m_patientBirthDate).c_str());

    m_text->addText(("Study: " + m_studyDescription).c_str());
    m_text->setTextOrigin(5 * m_pixelRatio, 5 * m_pixelRatio);

    if(m_tAxis == VMTK_X_AXIS)
        m_text->addText("Sagittal view");

    else if(m_tAxis == VMTK_Y_AXIS)
        m_text->addText("Coronal view");

    else
        m_text->addText("Axial view");

    m_text->setTextOrigin(5 * m_pixelRatio, 5 * m_pixelRatio);
    m_text->rightAligneText(-1, 5 * m_pixelRatio);
}

void TextRenderer2DView::setInstitutionName(const std::string &institutionName)
{
    m_institutionName = institutionName;
}

void TextRenderer2DView::setManufacturesModel(const std::string &manufacturesModel)
{
    m_manufacturesModel = manufacturesModel;
}

void TextRenderer2DView::setSerieDescription(const std::string &serieDescription)
{
    m_serieDescription = serieDescription;
}

void TextRenderer2DView::setPatientID(const std::string &patientID)
{
    m_patientID = patientID;
}

void TextRenderer2DView::setPatientBirthDate(const std::string &patientBirthDate)
{
    m_patientBirthDate = patientBirthDate;
}

void TextRenderer2DView::setStudyDescription(const std::string &studyDescription)
{
    m_studyDescription = studyDescription;
}

void TextRenderer2DView::setPixelSpacing(float pixelSpacing)
{
    m_pixelSpacing = pixelSpacing;
}

void TextRenderer2DView::setMaxSlice(unsigned int maxSlice)
{
    m_maxSlice = maxSlice;
}
