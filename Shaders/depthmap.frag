#version 330
/**
  * @file depthmap.frag
  * @brief The volume data are ray-casted and the depth map of the surface of relevant voxels is generated.
  * @author José Angel Iván Rubianes Silva, Wu Shin-Ting and Wallace Souza Loos
  * @date february 2018
  * @namespace GLSL
  * @class depthmapFS
  */
in vec2 TexCoord;   /**< interpolated texture coordinates (R,G,B,A) from raytrace.vs */
uniform sampler3D volumetexture; /**< texture unit storing volume data */
uniform  sampler2D backface_fbo; /**< texture unit storing backface plane in the viewing direction */
uniform  sampler2D frontface_fbo; /**< texture unit storing frontplane plane in the viewing direction */
uniform  int height;           /**< height of the volume data */
uniform  int width;            /**< width of the volume data */
uniform  int depth;            /**< depth of the volume data */
uniform  int steps_mode;       /**< sampling resolution: high (1) or low (otherwise)  */
uniform  float noise_threshold; /**< threshold for removing noise */
uniform bool high_background; /**< background signal is high ? (Ting: 3/12/2016) */
uniform mat4 mvp_matrix;            /**< projection*modelview matrix */
uniform vec4 scaleFactors;
uniform vec4 clipping_plane;  /**< clipping plane */

out vec4 fColor;

float coordvert2Tex(float cv);
float coordText2Vert(float ct);
vec3 coordText2Vert(vec3 ct);
/**
* @fn void main(void)
* @brief Main function for volume ray-casting.
* @details Color values of several samples along the ray between the entry point stored in
* frontface_fbo (texture unit of RaytracedRenderer::m_pFrontSidesFBO) and the exit point
* stored in backface_fbo (texture unit of RaytracedRenderer::m_pBackSidesFBO) are computed.
* Only the first valid hit is consideredand its depth is output.
*/

void main(void)
{
    vec3 start_position = texture(frontface_fbo, TexCoord).xyz;
    vec3 end_position = texture(backface_fbo, TexCoord).xyz;
    if (start_position == end_position)  // if two points are coincident
        fColor = vec4(1.0, 1.0, 1.0,1.0);        // color of pixel is white (depth = 1.0)
    else                                 // compute the samples
    {
        float density = 0.0;

        float length_of_ray_x = (end_position.x - start_position.x) * float(width);
        float length_of_ray_y = (end_position.y - start_position.y) * float(height);
        float length_of_ray_z = (end_position.z - start_position.z) * float(depth);
        float length_of_ray = sqrt(length_of_ray_x * length_of_ray_x + length_of_ray_y * length_of_ray_y + length_of_ray_z * length_of_ray_z);
        float steps;

        if (steps_mode == 0)            // sample resolution is 2xlength
        {
            steps = length_of_ray * 2.0;
        }
        else
        {                               // sample resolution is fixed on a pre-defined value
            steps = float(steps_mode);
        }

        vec3 step = (end_position - start_position) / steps; // sample step

        for (float i=0.0; i < steps; i+=1.0)
        {
            vec3 current_position = start_position + i*step;

            if (clipping_plane.x*current_position.x +
                    clipping_plane.y*current_position.y +
                    clipping_plane.z*current_position.z +
                    clipping_plane.w > 0.0) continue;

            density = texture(volumetexture, current_position).r;
            if ( density >= noise_threshold) {
                vec4 posCubeScaled= vec4(coordText2Vert(current_position),1)*scaleFactors;
                vec4 fPos = mvp_matrix*posCubeScaled;
                float depth_dst = coordvert2Tex(fPos.z);
                fColor = vec4(depth_dst, depth_dst, depth_dst, 1.0);
                return;
            }
        }
       fColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
} // end main

float coordvert2Tex(float cv){ return (cv+1.0)/2; }
float coordText2Vert(float ct){ return ((ct*2)-1); }
vec3 coordText2Vert(vec3 ct){ return vec3(coordText2Vert(ct.x),coordText2Vert(ct.y),coordText2Vert(ct.z)); }
