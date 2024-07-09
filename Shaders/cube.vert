#version 330
/**
  * @file cube.vert
  * @brief The white color of the cube vertex is passed to the next stage.
  * @author  José Angel Ivan Rubianes Silva
  * @date January 2015
  * @namespace GLSL
  * @class CubeVS
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
    Color=color;                         /**< white color */
}
