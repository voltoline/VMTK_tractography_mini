#version 330
/**
  * @file slicing.frag
  * @brief The fragment´s color is retrieved from the transfer function and the volumedata density.
  * @author Wu Shin-Ting, José Angel Ivan Rubianes Silva and Raphael Voltoline
  * @date January 2015
  * @namespace GLSL
  * @class SlicingFS
  */

uniform sampler2D depthmap;
out vec4 color;

/**
* @fn void main(void)
* @brief Mani funtion
*/
void main()
{
    vec4 pos;
    float alpha;
    pos.z = texelFetch(depthmap, ivec2(gl_FragCoord.xy), 0).x; //volume depth texture

    if (pos.z > gl_FragCoord.z)
        color = vec4(1.0, 0.0, 0.0, 1.0);
    else {
        alpha = gl_FragCoord.z-pos.z;
        color = vec4(1.0,0.0,0.0, 1.0-alpha);
    }

}

