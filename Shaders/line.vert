#version 330
/**
  * @file line.vert
  * @brief The color of the line vertex is passed to the next stage.
  * @author  José Angel Ivan Rubianes Silva
  * @date June 2015
  * @namespace GLSL
  * @class LineVS
  */
in vec4 position;
uniform mat4 mvp_matrix;
uniform vec4 color;
out vec4 Color;

/**
* @fn void main (void)
* @brief Main function
*/
void main()
{
    gl_Position = mvp_matrix * position; /**< vertex coordinates in the scene reference */
    Color=color;                         /**< a different color for each slice. */
}
