#version 330
/**
  * @file primitiveLineP2P.vert
  * @brief Rendering of primitive geometries using different parameters for visualization.
  * @author José Angel Ivan Rubianes Silva
  * @date March 2020
  * @namespace GLSL
  * @class Primitive object
  */

layout ( location = 0 ) in vec3 verCoord;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;
out vec2 vTexCoord;
out vec3 vNormal;
uniform mat4 ProjectionMatrix; // Projection matrix
uniform mat4 ModelViewMatrix; // Projection matrix

void main(void) {
vec4 position;
vec4 pos = vec4(verCoord ,1.0);
position = ProjectionMatrix * ( ModelViewMatrix * pos );
vTexCoord = texCoord;
vNormal = normal;
gl_Position = position;
}
