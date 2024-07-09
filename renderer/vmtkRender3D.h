/**
  * @file vmtkRender3D.h
  * @brief manages the raycasting 3D render of volume data.
  * @author José Angel Ivan Rubianes Silva, Raphael Voltoline and Wu Shin-Ting
  * @date March 2015
  * @note This code is adapted from the code available at http://www.cg.tuwien.ac.at/courses/Visualisierung/2010-2011/Beispiel1/Moellinger_Ludwig/.
  */
#ifndef VMTKRENDER3D_H
#define VMTKRENDER3D_H

#include "renderer/vmtkAbstractRender.h"
#include <glm/vec4.hpp>
#include <functional>

/**
 * @brief The vmtkRender3D class
 * @details raycasted rendering comprising of four steps: 1) renders front depth map;
 * 2) renders back depth map; 3) renders the raycasted volume; 4) renders to on-screen frame buffer;
 */
class vmtkRender3D : public vmtkAbstractRender
{
public:
    /**
     * @brief vmtkRenderer3D constructor
     * @param[in] acq volume data to be raycasted.
     */
    vmtkRender3D( );

    /**
     * @brief initializes 3D volume rendering
     */
    void initialize();

    /**
     * @brief renders the data with the specified redering mode.
     */
    /**
     * @brief render the texture coordinates for the visible volume
     * @param[in] mvp modelview transformation matrix
     */
    void render();

    /**
     * @brief resizes the dimensions of the display
     * @param[in] w width of image
     * @param[in] h height of image
     */
    void resize(int, int height);

    void registerInitializes(std::function<void()> initializes, size_t &id);
    void registerRenders(std::function<void()> renders, size_t &id);
    void registerResizes(std::function<void(int,int)> resizes, size_t &id);

    void updateInitializes(std::function<void ()> initializes, size_t id);
    void updateRenders(std::function<void ()> renders, size_t id);
    void updateResizes(std::function<void (int, int)> resizes, size_t id);

    void unregisterInitializes(size_t id);
    void unregisterRenders(size_t id);
    void unregisterResizes(size_t id);

private:

    void computeViewport();

    int m_viewport[4]; /**< viewport for rendered slice */

    int m_w;                     /**< viewport width */
    int m_h;                     /**< viewport height */

    std::vector<std::function<void ()>> m_initializes;
    std::vector<std::function<void ()>> m_renders;
    std::vector<std::function<void (int, int)>> m_resizes;

    void executeInitializes();
    void executeRenders();
    void executeResizes(int w, int h);
};

#endif // VMTKRENDER3D_H
