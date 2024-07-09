#version 330
/**
  * @file cube.frag
  * @brief The white color is drawn on the pixel.
  * @author José Angel Ivan Rubianes Silva
  * @date January 2015
  * @namespace GLSL
  * @class CubeFS
  */
in vec4 Color;
out vec4 fColor;
void main(void)
{
    fColor = Color;
}
