#version 330
/**
  * @file cursor3D_depth.frag
  * @brief A depth map of the visible volume is written in the current FBO.
  * @author Wu Shin Ting
  * @date June 2015
  * @namespace GLSL
  * @class cursor3D_depthFS
  */
uniform mat4 mvp_matrix;            /**< projection*modelview matrix */
uniform mat4 viewMatrixInv;         /**< modelview inverse matrix */
uniform vec4 scaleFactors;          /**< scaling factors in native space */
uniform vec4 fparam;                /**< step length, noise */
uniform sampler3D volumetexture;    /**< raw volume data */
uniform vec4 clipping_plane;        /**< MPR clipping plane */
in vec4 vColor;                     /**< texture coordinates */
out vec4 color;                     /**< depth value */

/**
* @fn void main(void)
* @brief Main funtion
*/
void main(void){
 bool flag; 

 mat4 mvi = viewMatrixInv;
 vec4 invScaleFactors = 1./scaleFactors;
 float noise = fparam[1];

 // starting point in the normalized texture coordinates

 vec4 initPos = vColor;

 // transforming the viewing direction
 vec4 geomDir = normalize(viewMatrixInv * vec4(0.0,0.0,-1.0,0.0)); 

 // tracing
 vec4 geomPos, texpos;
 float scalar, depth, t;
 t = 0;
 do {
    texpos = initPos + geomDir*t;          //next texture sample
    geomPos = texpos * 2.0 - vec4(1.0);    // sample coordinates in normalized native space 
    geomPos = mvp_matrix*(geomPos*scaleFactors);          // transformed coordinates
//    if (clipping_plane.x*geomPos.x +
//                        clipping_plane.y*geomPos.y +
//                        clipping_plane.z*geomPos.z +
//                        clipping_plane.w > 0.0) continue;
    scalar = texture(volumetexture, texpos.xyz).r;
    if (scalar > noise) {
	depth = (geomPos.z+1)/2.0;        // viewport
	color = vec4(depth, depth, depth, 1.0);
	 return;
	}
    t += fparam[0]; 
 } while (t < 2.0); 
 
 color = vec4(1.0, 1.0, 1.0, 1.0);  // backplane
}
