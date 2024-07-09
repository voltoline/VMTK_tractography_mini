#version 330
/**
  * @file simple.vert
  * @brief The texture coordinates of the vertex is passed to the next stage.
  * @author José Angel Ivan Rubianes Silva and Wu Shin-Ting
  * @date January 2015
  * @namespace GLSL
  * @class SimpleVS
  */

in vec3 texCoord;   /**< Texture coordinates (R,G,B,A) that are mapped to the vertex. */
in vec4 position;
out vec3 TexCoord;
uniform mat4 mvp_matrix;    /**< modelview and projection transformation */
//uniform vec4 scaleFactors;  /**< scale factors for compensating the texture volume distortion */

/**
* @fn void main (void)
* @brief Main function
*/
void main (void)
{
  // gl_Position = position;
    gl_Position = mvp_matrix * (position);  /**< vertex coordinates in the scene reference */
    TexCoord = texCoord; /**< texture coordinates */
}
