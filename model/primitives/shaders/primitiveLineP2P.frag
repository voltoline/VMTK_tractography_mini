#version 330
/**
  * @file primitiveLineP2P.frag
  * @brief Rendering of primitive geometries using different parameters for visualization.
  * @author José Angel Ivan Rubianes Silva
  * @date March 2020
  * @namespace GLSL
  * @class Primitive object
  */




uniform bool StateTexImage;
uniform sampler2D TextureImage;
uniform sampler2D DepthTex;
uniform bool StateDepthTex;
uniform sampler2D DepthTexBack;
uniform bool StateDepthTexBack;
uniform bool fixedAlpha;
uniform vec4 Color;
uniform bool EnableView;

in vec3 vNormal;
in vec2 vTexCoord;
out vec4 fcolor;



void main(void) {


    vec3 tcolor = Color.xyz;
    float alpha = Color.a;
    vec3 normal = vNormal;
    if(StateDepthTex && !StateDepthTexBack){
        float normalized_pos_z = gl_FragCoord.z;
        float depth = texelFetch(DepthTex, ivec2(gl_FragCoord.xy), 0).r; // depth texture of volume
        float eps = 0.000001;
        if (depth < normalized_pos_z-eps){
            alpha = 0.0;
        }
    }
    if(StateDepthTex && StateDepthTexBack){
        float normalized_pos_z = gl_FragCoord.z;
        float depth = texelFetch(DepthTex, ivec2(gl_FragCoord.xy), 0).r; // depth texture of volume
        float depthBack = texelFetch(DepthTexBack, ivec2(gl_FragCoord.xy), 0).r; // depth back texture of volume
        float eps = 0.00001;
        float d = depthBack - depth;
        if (depthBack < normalized_pos_z - eps){
            alpha = 0.1;
            tcolor = vec3 (0.0, 0.0, 1.0);
        } else if (depth < normalized_pos_z + eps) {
                alpha = 1.0 / (2*(10*(normalized_pos_z - depth)/d));
                tcolor = vec3 (0.0, 1.0, 0.0);
        } else {
            alpha = 1.0;
        }
    }
    if(!EnableView){
        alpha = 0.0;
    }
    if(fixedAlpha){
        alpha = 1.0;
    }
    vec4 outColor = vec4(tcolor, alpha );


    fcolor = outColor;
}
