#ifndef MODEL_TRANSFER_FUNCTION_H
#define MODEL_TRANSFER_FUNCTION_H

#include "model/ModelSubject.h"

enum class ModelTransferFunctionEvent
{
    WINDOWING_CHANGED,
    TF_FILE_LOADED, // novo enum para o evento do carregamento - José Angel Ivan Rubianes Silva
    TF_FILE_SAVED, // novo enum para o evento do salvamento- José Angel Ivan Rubianes Silva
    EQUALIZE_CHANGED // novo enum para o evento de mudança de estado da equalização- José Angel Ivan Rubianes Silva
};

class ModelTransferFunction: public ModelSubject<ModelTransferFunction, ModelTransferFunctionEvent>
{
public:
    ModelTransferFunction();

    ModelTransferFunctionEvent setWindowingDelta(int deltaX, int deltaY);
    void getWindowingDelta(int &deltaX, int &deltaY);

    void setAcquisitionMinMax(int min, int max);
    void getAcquisitionMinMax(int &min, int &max);  // Added by Ting (08/01/2022)

    /**
      * @brief Set step sizes in windowing level (brightness) and width (contrast) corresponding to mouse motions in y- and x- directions
      * @param[in] stepLevel level step size
      * @param[in] stepWidth width step size
      */
    void setWindowingSteps(int stepLevel, int stepWidth);

    /**
      * @brief Get step sizes in windowing level (brightness) and width (contrast) corresponding to mouse motions in y- and x- directions
      * @param[out] stepLevel level step size
      * @param[out] stepWidth width step size
      */
    void getWindowingSteps(int &stepLevel, int &stepWidth);

    /**
      * @brief Set the domain of the windowing function
      * @param[in] tfMin minimum value
      * @param[in] tfMax maximum value
      */
    void initiateWindowingFunction(int tfMin, int tfMax);

    /**
      * @brief Update windowing function level (brightness) and width (contrast)
      * @param[in] win_center windowing center
      * @param[in] win_width windowing width
      */
    void updateWindowingFunction(int win_center, int win_width);

    /**
      * @brief Get windowing function level (brightness) and width (contrast)
      * @param[out] win_center windowing center
      * @param[out] win_width windowing width
      */
    void getWindowingFunction(int &win_center, int &win_width);

    /**
     * @brief Estimate step sizes in windowing edition
     * @param[in] width of the windowing function (contrast)
     * @param[in] height/center of the windowing function (brightness)
     */


    /*!
     * \brief estimateStepSize
     * \param width
     * \param height
     */
    void estimateStepSize(int width, int height);

    /*!
     * \brief filePathTFLoaded
     * \return
     */
    const std::string &filePathTFLoaded() const;

    /*!
     * \brief filePathTFSaved
     * \return
     */
    const std::string &filePathTFSaved() const;

    /*!
     * \brief saveFileTF
     * \param filePath: caminho do arquivo
     * \return evento do salvamento do arquivo
     * \author José Angel Iván Rubianes Silva
     */
    ModelTransferFunctionEvent saveFileTF(std::string filePath);

    /*!
     * \brief loadFileTF
     * \param filePath: caminho do arquivo
     * \return evento do carregamento do arquivo
     * \author José Angel Iván Rubianes Silva
     */
    ModelTransferFunctionEvent loadFileTF(std::string filePath);

    /*!
     * \brief setFilePathTFLoaded
     * \param newFilePathTFLoaded, caminho do arquivo carregado.
     */
    void setFilePathTFLoaded(const std::string &newFilePathTFLoaded);

    /*!
     * \brief setFilePathTFSaved
     * \param newFilePathTFSaved, caminho do arquivo salvo.
     * \author José Angel Iván Rubianes Silva
     */
    void setFilePathTFSaved(const std::string &newFilePathTFSaved);


    /*!
     * \brief equalizated
     * \return o estado da equalização
     * \author José Angel Iván Rubianes Silva
     */
    bool equalizated() const;

    /*!
     * \brief setEqualizated
     * \param newEqualizated insere o estado da equalização
     * \return
     * \author José Angel Iván Rubianes Silva
     */
    ModelTransferFunctionEvent setEqualizated(bool newEqualizated);

private:
    float m_deltaX;
    float m_deltaY;

    int m_acqUmin;
    int m_acqUmax;

    int m_stepLevel;  // brightness (center) of volume image
    int m_stepWidth;  // contrast (width) of volume image (Ting, 08/01/2022)
    int m_win_center; // center of windowing function;
    int m_win_width;  // width of windowing function;


    std::string m_filePathTFLoaded; // nova variável member - José Angel Ivan Rubianes Silva

    std::string m_filePathTFSaved; // nova variável member - José Angel Ivan Rubianes Silva

    bool m_equalizated; // nova variável member - José Angel Ivan Rubianes Silva
};

#endif // MODEL_TRANSFER_FUNCTION_H
