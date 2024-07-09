#ifndef TEXT_RENDERER_2D_VIEW_H
#define TEXT_RENDERER_2D_VIEW_H

#include "vmtkTextRenderer.h"
#include <renderer/SubRendererBase.h>

class TextRenderer2DView: public SubRendererBase
{
public:
    TextRenderer2DView();

    /**
   * @enum TVoxelOrder
   * @brief voxel order on the screen
   */
    enum TVoxelOrder
    {
        VMTK_LAS = 0,    /**< LeftPosteriorSuperior (Radiological) */
        VMTK_RAS = 1     /**< RightAnteriorSuperior (Neurological) */
    };

    /**
   * @enum TAxis
   * @brief types of axis
   */
    enum TAxis
    {
        VMTK_Z_AXIS = 0,    /**< z-axis */
        VMTK_X_AXIS = 1,    /**< x-axis */
        VMTK_Y_AXIS = 2     /**< y-axis */
    };

    TAxis	    m_tAxis;	    /**< which axis should be sliced? */

    void initialize();

    /**
     * @brief renders the texts on the screen
     */
    void render();
    void resize(int w, int h);

    void pixelRatioChanged(float pixelRatio);

    void setAxis(const TAxis &tAxis);
    void setCurrentSlice(unsigned int slice);
    void setVoxelOrder(const TVoxelOrder tVoxelOrder);

    void setMaxSlice(unsigned int maxSlice);
    void setPixelSpacing(float pixelSpacing);

    void setStudyDescription(const std::string &studyDescription);
    void setPatientBirthDate(const std::string &patientBirthDate);
    void setPatientID(const std::string &patientID);
    void setSerieDescription(const std::string &serieDescription);
    void setManufacturesModel(const std::string &manufacturesModel);
    void setInstitutionName(const std::string &institutionName);

private:
    /**
     * @brief sets the texts to be rendered
     */
    void setText();

    /**
     * @brief updates text informations
     */
    void updateText();

    TVoxelOrder m_tVoxelOrder; /**< which voxel order on the screen? */

    int m_width;
    int m_height;

    unsigned int m_nSlice;       /**< which slice should be rendered? */
    unsigned int m_maxSlice;
    float m_pixelSpacing;

    std::string m_studyDescription;
    std::string m_patientBirthDate;
    std::string m_patientID;
    std::string m_serieDescription;
    std::string m_manufacturesModel;
    std::string m_institutionName;

    vmtkTextRenderer *m_text;  /**< render text on the screen */

    bool m_initialized;

    float m_pixelRatio;
};

#endif // TEXT_RENDERER_2D_VIEW_H
