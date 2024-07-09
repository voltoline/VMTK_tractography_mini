#version 330
/**
  * @file drawresult.frag
  * @brief The fragment´s color is retrieved from the texture object with an interpolated texture coordinate.
  * @author José Angel Ivan Rubianes Silva
  * @date February 2015
  * @note This code is based on http://www.cg.tuwien.ac.at/courses/Visualisierung/2010-2011/Beispiel1/Moellinger_Ludwig/.
  * @namespace GLSL
  * @class DrawresultFS
  */

uniform sampler2D depth_tex; /**< input image as a texture object */
in vec2 TexCoord; /**< interpolated texture coordinates computed from the values of the vertex. */
//in vec4 vColor;   /**< dummy variable */
out vec4 Color;   /**< fragment color */

/**
*  @fn void main(void)
*  @brief attributes the RGBA color in the position gl_TexCoord in DrawresultFS::raycast_fbo to the current pixel.
*/
void main(void)
{
    gl_FragDepth = texture(depth_tex, TexCoord).r;
}
