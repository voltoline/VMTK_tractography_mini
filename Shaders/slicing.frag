#version 330
/**
  * @file slicing.frag
  * @brief The fragment´s color is retrieved from the transfer function and the volumedata density.
  * @author Wu Shin-Ting, José Angel Ivan Rubianes Silva and Raphael Voltoline
  * @date January 2015
  * @namespace GLSL
  * @class SlicingFS
  */

in vec3 TexCoord;           /**< interpolated textur coordinates from the textures of the vertices */
uniform sampler3D volumetexture;      /**< volume data */
uniform sampler1D transferfunction;   /**< transfer function data */
//uniform sampler1D equalized_map;
//uniform float eq_map_scale_factor;
out vec4 color;

/**
* @fn void main(void)
* @brief Mani funtion
*/
void main()
{
    float density = texture(volumetexture, TexCoord.xyz).r;// * eq_map_scale_factor; // get the density from the volume data
//    float eq_density = texture(equalized_map, density).r;
    float eq_density = density;
    vec4 tf_color = vec4(vec3(texture(transferfunction, eq_density).xyz), 1.0);          // get the color from the transfer fucntion
    color = tf_color;            // pixel´s color
}

