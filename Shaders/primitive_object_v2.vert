#version 330
/**
  * @file primitive_object_v2.vert
  * @brief Rendering of primitive geometries using different parameters for visualization.
  * @author José Angel Ivan Rubianes Silva
  * @date october 2019
  * @namespace GLSL
  * @class Primitive object
  */

layout ( location = 0 ) in vec3 verCoord;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;
out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vColor;
out vec4 vEnableView;
uniform mat4 ProjectionMatrix; // Projection matrix
uniform samplerBuffer modelViewMatrixTexture; // modelview matrices in buffer.
uniform samplerBuffer colorTexture; // color vectors in buffer.
uniform samplerBuffer enableViewTexture; // enable view vectors in buffer.

void main(void) {
vec4 position;
vColor = vec4(0.0,1.0,1.0,0.5);
vEnableView = vec4(1);

// As matrices en formato de colunas" //
vec4 col1 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4);
vec4 col2 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 1);
vec4 col3 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 2);
vec4 col4 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 3);

// montamos a matrix e obtemos a posição dos vértices da primitiva.
mat4 model_matrix = mat4(col1, col2, col3, col4);
vec4 pos = vec4(verCoord ,1.0);
position = ProjectionMatrix * ( model_matrix * pos );

// Cor independente para cada primitiva.
vColor = texelFetch(colorTexture, gl_InstanceID);

// Cor independente para cada primitiva.
vEnableView = texelFetch(enableViewTexture, gl_InstanceID);

// a cordenada de textura e as normais estão normalizadas.
vTexCoord = texCoord;
vNormal = normal;
gl_Position = position;
}
