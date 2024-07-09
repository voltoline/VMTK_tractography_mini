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
uniform sampler3D ref_volumetexture;      /**< volume data */
uniform sampler1D ref_transferfunction;   /**< transfer function data */
//uniform sampler1D ref_equalized_map;
//uniform float ref_eq_map_scale_factor;

uniform sampler3D float_volumetexture;      /**< volume data */
uniform sampler1D float_transferfunction;   /**< transfer function data */
//uniform sampler1D float_equalized_map;
//uniform float float_eq_map_scale_factor;

uniform  vec4 ref_phyDimensions;  /**< scale factors for compensating the reference texture volume distortion */
uniform  vec4 float_phyDimensions;  /**< scale factors for compensating the floating texture volume distortion */

uniform mat4 inv_registration_matrix;    /**< inverse registration matrix */

uniform  float blending_factor;      /**< linear interpolation weight for reference */

out vec4 color;

/**
* @fn void main(void)
* @brief Mani funtion
*/
void main()
{
//    vec3 color = vec3(0.0);
    vec4 fusion = vec4(0.0);
    float ref_density = 0.0;
    float ref_eq_density = 0.0;
    float float_density = 0.0;
    float float_eq_density = 0.0;

    vec4 inv = vec4(1. / float_phyDimensions.x, 1. / float_phyDimensions.y,
                    1. / float_phyDimensions.z, 1.0);

    ref_density = texture(ref_volumetexture, TexCoord.xyz).r;// * ref_eq_map_scale_factor; // get the density from the volume data

    ref_eq_density = ref_density;
//    ref_eq_density = texture(ref_equalized_map, ref_density).r;
    vec4 ref_tf_color = vec4(vec3(texture(ref_transferfunction, ref_eq_density).xyz), 1.0);          // get the color from the transfer fucntion

    vec4 pt = vec4(ref_phyDimensions.xyz * TexCoord.xyz, 1.0);
    vec4 corresp = inv * (inv_registration_matrix * pt);

    float_density = texture(float_volumetexture, corresp.xyz).r;// * float_eq_map_scale_factor;
    // Ting: obter equalized_density
//     float_eq_density = texture(float_equalized_map,float_density).r;
                float_eq_density = float_density;
    vec4 float_tf_color = texture(float_transferfunction, float_eq_density);  //float voxel color

//    vec4 float_tf_color = vec4(texture(float_volumetexture, corresp.xyz).rgb, 1.f);

    color = vec4(mix(float_tf_color, ref_tf_color, blending_factor).xyz, 1.0);            // pixel´s color


}

