#version 330
/**
  * @file registration_depth.vert
  * @brief The volume data are ray-casted and the depth of the first valid visible voxel is computed.
  * @author Wu Shin-Ting
  * @date April 2015
  * @namespace GLSL
  * @class RegistrationDepthVS
  */
                                    //Definidos pelo cliente:
uniform mat4 viewMatrix, projMatrix;//matrizes MODELVIEW e PROJECTION
in vec3 position;                   //coordenadas espaciais do vertice
in vec3 texCoord0;                  //coordenadas de textura do vertice
out vec3 texCoord;

void main (void)
{
   vec4 tmp;
   tmp = vec4(position, 1.0); // converter para coord.homog.
   gl_Position = projMatrix * viewMatrix * tmp; // projecao do ponto
   texCoord = texCoord0 ; // transferir coord. de textura
}

