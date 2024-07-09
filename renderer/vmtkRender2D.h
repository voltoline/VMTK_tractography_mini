/**
  * @file vmtkRender2D.h
  * @brief renders a slice of the volume data with ray-casting procedure.
  * @author JosÃ© Angel Ivan Rubianes Silva, Raphael Voltoline, Wallaxce Souza Loos, and Wu Shin-Ting
  * @date March 2015
  * @note This code is adapted from the code available at http://www.cg.tuwien.ac.at/courses/Visualisierung/2010-2011/Beispiel1/Moellinger_Ludwig/.
  */
#ifndef VMTK_RENDER_2D_H
#define VMTK_RENDER_2D_H

#include "vmtkAbstractRender.h"

/**
 * @brief The vmtkRender2D class
 * @details This class is for rendering slices and is executed when receives signals form the @ref vmtkDicomReader class
 */

class vmtkRender2D : public vmtkAbstractRender
{
public:

    /**
   * @brief vmtkRender2D
   * @param [in] data volume data to be sliced and rendered
   */
    vmtkRender2D( );


    /**
   * @brief initializes the slice renderer by compiling and linking its shader.
   */
    void initialize();

    /**
   * @brief renders the slices according to the specified SlicedRenderer::TMode.
   */
    void render();

    /**
   * @brief resizes the dimensions of the display
   * @param[in] w width of image
   * @param[in] h height of image
   */
    void resize(int, int);

private:
    void computeViewport();

    int m_w;                        /**< viewport width */
    int m_h;                        /**< viewport height */

    int m_viewport[4]; /**< viewport for rendered slice */
};

#endif // VMTK_RENDER_2D_H
