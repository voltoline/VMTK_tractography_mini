#version 330
/**
  * @file simple.vert
  * @brief The texture coordinates of the vertex is passed to the next stage.
  * @author José Angel Ivan Rubianes Silva and Wu Shin-Ting
  * @date January 2015
  * @namespace GLSL
  * @class SimpleVS
  */

in vec4 position;
uniform vec4 color;
out vec2 texCoord;
out vec3 texCoord3D;
out vec4 Color;
/**
* @fn void main (void)
* @brief Main function
*/
void main (void)
{
  texCoord = vec2((position+1)/2.0);
  texCoord3D = vec3((position+1)/2.0);
  gl_Position = position;
  Color = color;
}
