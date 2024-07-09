#version 330
/**
  * @file castregister.frag
  * @brief The volume data are ray-casted and its colors blended to get the pixel´s color.
  * @author Wu Shin-Ting and Raphael Voltoline
  * @date April 2015
  * @namespace GLSL
  * @class CastRegisterFS
  */
in vec2 TexCoord;   /**< interpolated texture coordinates (R,G,B,A) from raytrace.vs */
uniform  sampler2D backface_fbo; /**< texture unit storing backface plane in the viewing direction */
uniform  sampler2D frontface_fbo; /**< texture unit storing frontplane plane in the viewing direction */
uniform  sampler3D ref_volumetexture; /**< texture unit storing reference volume data */
uniform  sampler3D float_volumetexture; /**< texture unit storing floating volume data */
uniform  sampler1D ref_transferfunction;  /**< texture unit storing transfer function of reference volume data*/
uniform  sampler1D float_transferfunction;  /**< texture unit storing transfer function of floating volume data */
uniform  int height;           /**< height of the volume data */
uniform  int width;            /**< width of the volume data */
uniform  int depth;            /**< depth of the volume data */
uniform  int steps_mode;       /**< sampling resolution: high (1) or low (otherwise)  */
uniform  float noise_threshold; /**< threshold for removing noise */
uniform  vec4 ref_phyDimensions;  /**< scale factors for compensating the reference texture volume distortion */
uniform  vec4 float_phyDimensions;  /**< scale factors for compensating the floating texture volume distortion */
uniform mat4 inv_registration_matrix;    /**< inverse registration matrix */
uniform  float blending_factor;      /**< linear interpolation weight for reference */
uniform vec4 clipping_plane;         /**< MPR clipping plane */
uniform bool state_noise_threshold; /**< threshold state for removing noise */

uniform vec3 highlighted_planes;        /**< highlighted plane coordinates (x = sagittal, y = coronal, z = axial) */
uniform bool highlight_plane_activated;    /**< enable or disable the highlighted planes >*/

uniform vec4 scaleFactors;
uniform mat4 mvp_matrix;            /**< projection*modelview matrix */

out vec4 fColor;
/**
* @fn void main(void)
* @brief Main function for volume ray-casting.
* @details Color values of several samples along the ray between the entry point stored in
* frontface_fbo (texture unit of RaytracedRenderer::m_pFrontSidesFBO) and the exit point
* stored in backface_fbo (texture unit of RaytracedRenderer::m_pBackSidesFBO) are computed.
* There are four modes to compose them: (0) only the first valid hit is considered; (2) only
* the sample with the highest intensity value is considered; (2) the colors are summed from
* front to back; (3) the colors are summed from back to front. In addition, the colors may
* be get only from the transfer function (shading disabled) or influenced by an illumination
* model (shading enabled).
*/
float coordvert2Tex(float cv);
float coordText2Vert(float ct);
vec3 coordText2Vert(vec3 ct);
float computeDepth(vec3 current_position);

void main(void)
{

    vec3 start_position = texture(frontface_fbo, TexCoord).xyz;
    vec3 end_position = texture(backface_fbo, TexCoord).xyz;

    gl_FragDepth = 1.0;

    if (start_position == end_position)  // if two points are coincident
        fColor = vec4(0.5, 0.5, 0.5,1.0);        // color of pixel is black
    else                                 // compute the samples
    {
        vec4 color = vec4(0.0);
        vec4 fusion = vec4(0.0);
        float ref_density = 0.0;
        float ref_eq_density = 0.0;
        float float_density = 0.0;
        float float_eq_density = 0.0;
        float alpha = 0.0;
        float accum_alpha = 0.0;
        bool first=false;
        bool depthFirst = false;
        vec4 inv = vec4(1. / float_phyDimensions.x, 1. / float_phyDimensions.y,
                        1. / float_phyDimensions.z, 1.0);

        float length_of_ray_x = (end_position.x - start_position.x) * float(width);
        float length_of_ray_y = (end_position.y - start_position.y) * float(height);
        float length_of_ray_z = (end_position.z - start_position.z) * float(depth);
        float length_of_ray = sqrt(length_of_ray_x * length_of_ray_x + length_of_ray_y * length_of_ray_y + length_of_ray_z * length_of_ray_z);
        float steps;
        float factor_for_tf_alpha;

        if (steps_mode == 0)            // sample resolution is 2xlength
        {
            steps = length_of_ray * 2.0;
            factor_for_tf_alpha = 2.0;
        }
        else
        {                               // sample resolution is fixed on a pre-defined value
            steps = float(steps_mode);
            factor_for_tf_alpha = steps / length_of_ray;
        }

        vec3 step = (end_position - start_position) / steps; // sample step

        for (float i=0.0; i < steps; i+=1.0)
        {
            vec3 current_position = start_position + i*step;

            // discard all fragments on the side of the normal vector
            if (clipping_plane.x*current_position.x +
                            clipping_plane.y*current_position.y +
                            clipping_plane.z*current_position.z +
                            clipping_plane.w > 0.0) continue;

            ref_density = texture(ref_volumetexture, current_position).r;

            if(!state_noise_threshold)
                first = true;
            else if (!first && ref_density >= noise_threshold || highlight_plane_activated) {
                first = true;
            }

            if (first) {
                // Ting: obter equalized_density
//                 ref_eq_density = texture(ref_equalized_map,ref_density).r;
                ref_eq_density = ref_density;

                vec4 ref_tf_color = texture(ref_transferfunction, ref_eq_density); // reference voxel color
//                ref_tf_color.rgb *= ref_tf_color.a;

                // get the correspondence in floating volume texture
                vec4 pt = vec4(ref_phyDimensions.xyz * (current_position), 1.0);
                vec4 corresp = inv * (inv_registration_matrix * pt);

                float_density = texture(float_volumetexture, corresp.xyz).r;
                // Ting: obter equalized_density
//                 float_eq_density = texture(float_equalized_map,float_density).r;
                float_eq_density = float_density;
                vec4 float_tf_color = texture(float_transferfunction, float_eq_density);  //float voxel color
//                float_tf_color.rgb *= float_tf_color.a;

                // blend two voxel colors
//                fusion = mix(float_tf_color, ref_tf_color, blending_factor); // (1-blending_factor)*float_tf_color+blending_factor*ref_tf_color
//                fusion.rgb = mix(float_tf_color.rgb, ref_tf_color.rgb, blending_factor); // (1-blending_factor)*float_tf_color+blending_factor*ref_tf_color
                fusion = mix(float_tf_color, ref_tf_color, blending_factor);
//                fusion.a = min(ref_tf_color.a,float_tf_color.a);

                if(highlight_plane_activated) // process highlighted planes
                {
                    bvec3 result_less_than = lessThan(abs(highlighted_planes - current_position), vec3(0.005));
                    if(any(result_less_than))
                    {
//                            in_slice = true;
                        fColor = vec4(fusion.rgb, 1.0);
                        gl_FragDepth  = computeDepth(current_position);
                        return;

                    }
                    continue;
                } else
//                fusion = mix(float_tf_color, ref_tf_color, 0.3); // (1-blending_factor)*float_tf_color+blending_factor*ref_tf_color
                    if (clipping_plane.x != 0.0 ||
                        clipping_plane.y != 0.0 ||
                        clipping_plane.z != 0.0 ||
                        clipping_plane.w != 0.0) {

                    vec4 posCubeScaled= vec4(coordText2Vert(current_position),1)*scaleFactors;
                    vec4 fPos = mvp_matrix*posCubeScaled;
                    float depth_dst = coordvert2Tex(fPos.z);
                    gl_FragDepth  = depth_dst;

                    fColor = vec4(fusion.rgb, 1.0);
                    return;
                } else if (alpha < 0.99) {
//                    color.rgb = mix(fusion.rgb, color.rgb, alpha);
//                    alpha += (1-alpha)*fusion.a;
//                    color.rgb = (1-alpha)*color.rgb + fusion.rgb;
                    color.rgb = (alpha) * color.rgb + (1.0 - alpha) * fusion.rgb;
                    color.a = mix(fusion.a, 1.0, color.a);
                    accum_alpha=alpha;
                    alpha = color.a;

                    if(!depthFirst){
                        vec4 posCubeScaled= vec4(coordText2Vert(current_position),1)*scaleFactors;
                        vec4 fPos = mvp_matrix*posCubeScaled;
                        float depth_dst = coordvert2Tex(fPos.z);
//                            gl_FragDepth  = vec4(depth_dst, depth_dst, depth_dst, 1.0);
                        gl_FragDepth  = depth_dst;
                        depthFirst = true;
                    }

                } else {
                    vec4 posCubeScaled= vec4(coordText2Vert(current_position),1)*scaleFactors;
                    vec4 fPos = mvp_matrix*posCubeScaled;
                    float depth_dst = coordvert2Tex(fPos.z);
//                        gl_FragDepth  = vec4(depth_dst, depth_dst, depth_dst, 1.0);
                    gl_FragDepth  = depth_dst;

                    fColor = vec4(color.rgb, 1.0);
                    return;
                }
            }
        }
        if ( accum_alpha == 0.0)
        {
            fColor = vec4(0.5,0.5,0.5, 1.0);
            gl_FragDepth = 1.0;
        }
        else
        {
            fColor = vec4(color.rgb, 1.0);
        }
    }
} // end main

float computeDepth(vec3 current_position)
{
    vec4 posCubeScaled= vec4(coordText2Vert(current_position), 1) * scaleFactors;
    vec4 fPos = mvp_matrix * posCubeScaled;
    return coordvert2Tex(fPos.z);
}

float coordvert2Tex(float cv)
{
    return (cv + 1.0) / 2;
}

float coordText2Vert(float ct)
{
    return ((ct * 2) - 1);
}

//vec3 coordText2Vert(vec3 ct){ return vec3(coordText2Vert(ct.x),coordText2Vert(ct.y),coordText2Vert(ct.z)); }
vec3 coordText2Vert(vec3 ct)
{
    return ct * 2 - vec3(1.0);
}
