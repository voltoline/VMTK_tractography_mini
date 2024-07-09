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

uniform sampler3D float_volumetexture;      /**< volume data */
uniform sampler1D float_transferfunction;   /**< transfer function data */

uniform  vec4 ref_phyDimensions;  /**< scale factors for compensating the reference texture volume distortion */
uniform  vec4 float_phyDimensions;  /**< scale factors for compensating the floating texture volume distortion */

uniform mat4 inv_registration_matrix;    /**< inverse registration matrix */

uniform  float blending_factor;      /**< linear interpolation weight for reference */

uniform bool save_tex_coord = false;
uniform bool is_grayscale = true;
uniform bool use_transfer_function = true;

out vec4 color;

vec4 getRefColor(vec3 tex_coord)
{
    if(!is_grayscale)
        return vec4(texture(ref_volumetexture, tex_coord.xyz).rgb, 1.f);          // get the color from the transfer fucntion

    float ref_density = texture(ref_volumetexture, tex_coord.xyz).r;// * ref_eq_map_scale_factor; // get the density from the volume data

    if(use_transfer_function)
        return vec4(vec3(texture(ref_transferfunction, ref_density).xyz), 1.f);          // get the color from the transfer fucntion
    else
        return vec4(ref_density, ref_density, ref_density, 1.f);
}

vec4 getFloatColor(vec3 tex_coord)
{
    float float_density = texture(float_volumetexture, tex_coord).r;// * float_eq_map_scale_factor;
    return texture(float_transferfunction, float_density);  //float voxel color
}

/**
* @fn void main(void)
* @brief Mani funtion
*/
void main()
{
    vec4 inv = vec4(1.f / float_phyDimensions.x, 1.f / float_phyDimensions.y, 1.f / float_phyDimensions.z, 1.f);

    if(save_tex_coord)
    {
        color = vec4(TexCoord, 0.f);
        return;
    }

    vec4 ref_tf_color = getRefColor(TexCoord);

    vec4 pt = vec4(ref_phyDimensions.xyz * TexCoord.xyz, 1.f);
    vec4 corresp = inv * (inv_registration_matrix * pt);

    vec4 float_tf_color = getFloatColor(corresp.xyz);

    color = vec4(mix(float_tf_color, ref_tf_color, blending_factor).xyz, 1.f);            // pixel´s color
//    color = vec4(mix(float_tf_color, vec4(0.f, 0.f, 0.f, 1.f), blending_factor).xyz, 1.0);            // pixel´s color

}

