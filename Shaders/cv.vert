#version 330
/**
  * @file line.vert
  * @brief The color of the line vertex is passed to the next stage.
  * @author  José Angel Ivan Rubianes Silva
  * @date June 2015
  * @namespace GLSL
  * @class LineVS
  */
layout (location=0) in vec2 position;

uniform mat4 mvp_matrix;
/**
* @fn void main (void)
* @brief Main function
*/
void main()
{
     vec4 pos = vec4(position, 1, 1);
     gl_Position = mvp_matrix * pos; /**< vertex coordinates in the scene reference */
}
