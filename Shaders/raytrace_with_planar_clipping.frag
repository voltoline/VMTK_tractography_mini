#version 330
/**
  * @file raytrace.frag
  * @brief The volume data are ray-casted and its colors blended to get the pixel´s color.
  * @author Wu Shin-Ting, José Angel Ivan Rubianes Silva and Raphael Voltoline
  * @date March 2015
  * @namespace GLSL
  * @class RaytraceFS
  */
in vec2 TexCoord;   /**< interpolated texture coordinates (R,G,B,A) from raytrace.vs */
uniform sampler2D backface_fbo; /**< texture unit storing backface plane in the viewing direction */
uniform sampler2D frontface_fbo; /**< texture unit storing frontplane plane in the viewing direction */
uniform sampler3D volumetexture; /**< texture unit storing volume data */
uniform sampler1D transferfunction;  /**< texture unit storing transfer function */
//uniform sampler1D equalized_map;
//uniform float eq_map_scale_factor;
uniform int height;           /**< height of the volume data */
uniform int width;            /**< width of the volume data */
uniform int depth;            /**< depth of the volume data */
uniform int steps_mode;       /**< sampling resolution: high (1) or low (otherwise)  */
uniform float noise_threshold; /**< threshold for removing noise */
uniform bool state_noise_threshold; /**< threshold state for removing noise */
uniform bool state_cursor3D; /**< cursor 3D state*/
out vec4 fColor;
uniform vec4 clipping_plane;  /**< clipping plane */

//CR
//uniform vec4 planeAxialCR;
//uniform bool enableFastCR;

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

void main(void)
{
    float accum_alpha;
    vec3 start_position = texture(frontface_fbo, TexCoord).xyz;
    vec3 end_position = texture(backface_fbo, TexCoord).xyz;

    if (true)
    {
        if (start_position == end_position) { // if two points are coincident
//            if (state_noise_threshold){
                fColor = vec4(0.5, 0.5, 0.5,1.0);
//            }
//            else{
//                fColor = vec4(0.0, 0.0, 0.0,1.0);
//            }      // color of pixel is black
        }
        else                                 // compute the samples
        {
            vec4 color = vec4(0.0);
            float density = 0.0;
            float eq_density = 0.0;
            float alpha = 0.0;
            bool first=false, second=false, third=false, fourth=false;

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
            accum_alpha=0.0;
            float ii=0.0;
            for (float i=0.0; i < steps; i+=1.0)
            {
                vec3 current_position = start_position + i*step;
				
	        // discard all fragments on the side of the normal vector
                if (clipping_plane.x*current_position.x +
				clipping_plane.y*current_position.y + 
				clipping_plane.z*current_position.z + 
                                clipping_plane.w > 0.0) continue;

                density = texture(volumetexture, current_position).r;// * eq_map_scale_factor;

                //                if (density > 0.0) {
                //                    fColor = vec4(1.0,1.0,1.0,1.0);
                //                    return;
                //                }

                if (!state_noise_threshold)
                    first = true;

                else if (!first && density >= noise_threshold)
                    first = true;

                /* Ting: just for validating peeling for skull stripping (3/10/2015) */
//                if (!state_noise_threshold) {
//                    /* threshold disabled */
//                    first = second = third = fourth = true;
//                }
//                else if (!first && density >= noise_threshold) {
//                    /* reach the scalp */
//                    first = true;
//                    ii = i;
//                }
//                else if (first &&  !second && density >= 1.5*noise_threshold) { // adjusted to Aline Mirella Magalhães
//                    /* reach the skull */
//                    /* peeling procedure does not consider the head's shape!!! */
//                    second = true;
//                    ii = i;
//                }
//                else if (first && second && !third && (density < 0.7*noise_threshold)) {
//                    /* reach the subarachnoid space */
//                    third = true;
//                    ii = i;
//                }
//                else if (first && second && third && !fourth && (density >= 2.*noise_threshold)) {
//                    /* reach the cerebral cortex */
//                    fourth = true;
//                    ii = i;
//                }

//                if (fourth) {
//                  if (third) {
//                  if (second) {
                    /* end validating peeling (Ting) */
                  if (first) {
                    //                    if (density > 0.5) {
                    //                        fColor = vec4(1.0,1.0,1.0,1.0);
                    //                        return;
                    //                    }

                    //                    eq_density = texture(equalized_map, density).r;
                    // Ting: obter equalized_density
                    // eq_density = texture(equalizetexture,density);
                    eq_density = density;

                    vec4 tf_color = texture(transferfunction, eq_density);

                    if (clipping_plane.x != 0.0 ||
                            clipping_plane.y != 0.0 ||
                            clipping_plane.z != 0.0 ||
                            clipping_plane.w != 0.0)
                    {
                        fColor = vec4(tf_color.rgb, 1.0);
                        return;

                    } else if (alpha < 0.99) {
//                        color.rgb = mix((tf_color.rgb*tf_color.a), color.rgb, alpha);
                        color.rgb = (1-alpha)*color.rgb + tf_color.rgb;
                        color.a = mix(tf_color.a, 1.0, color.a); // tf_color.a*(1.-color.a)+1.*color.a
                        accum_alpha=alpha;
//                        alpha += (1-alpha)*tf_color.a;
                        alpha = color.a;

//                        //CR--------------------

//                        if (enableFastCR && planeAxialCR.x*current_position.x + planeAxialCR.y*current_position.y + planeAxialCR.z*current_position.z + planeAxialCR.w > 0.0 &&
//                                planeAxialCR.x*current_position.x + planeAxialCR.y*current_position.y + planeAxialCR.z*current_position.z + planeAxialCR.w <0.01){
//                            fColor = vec4(vec3(0.7, 0.0, 0.0),1.0);
//                            return;
//                        }
//                        //----------------------


                    } else {
                        fColor = vec4(color.rgb, 1.0);
                        return;
                    }
                }
            }
//            if (state_noise_threshold || accum_alpha == 0.0)
            if (accum_alpha == 0.0)  /* Bug fixed:
                              Same visual effect for threshold enabled (threshold=0)
                              and disabled (Ting - 3/10/2015) */
            {
                fColor = vec4(0.5,0.5,0.5, 1.0);
            }
            else
            {
                fColor = vec4(color.rgb, 1.0);
            }

        }
    }
    else
    {
         fColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
} // end main
