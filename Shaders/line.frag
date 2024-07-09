#version 330
/**
  * @file line.frag
  * @brief The color is drawn on the pixel.
  * @author José Angel Ivan Rubianes Silva
  * @date June 2015
  * @namespace GLSL
  * @class LineFS
  */
in vec4 Color;
out vec4 fColor;
void main(void)
{
    fColor = Color;
}
