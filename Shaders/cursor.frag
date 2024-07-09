#version 330
/**
  * @file slicing.frag
  * @brief The fragment´s color is retrieved from the transfer function and the volumedata density.
  * @author Wu Shin-Ting, José Angel Ivan Rubianes Silva and Raphael Voltoline
  * @date January 2015
  * @namespace GLSL
  * @class SlicingFS
  */
in vec4 Color;
out vec4 fcolor;

/**
* @fn void main(void)
* @brief Mani funtion
*/
void main()
{
    fcolor = Color;//vec4(1, 0, 0, 1);            // pixel´s color
}

