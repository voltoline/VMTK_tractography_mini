#version 330
/**
  * @file registration_depth.frag
  * @brief The volume data are ray-casted and the depth of the first valid visible voxel is computed.
  * @author Wu Shin-Ting
  * @date April 2015
  * @namespace GLSL
  * @class RegistrationDepthFS
  */

uniform mat4 viewMatrix, projMatrix;    //matriz de MODELVIEW
uniform mat4 viewMatrixInv; //matriz inversa de MODELVIEW
uniform vec2 fparam;      //passo de percurso, noise
uniform vec4 scaleFactors;//fatores de escala do espaco de textura em relacao
                            // ao espaco do volume normalizado
uniform sampler3D VOLUME;   //densidades da neuroimagem 3D
in vec3 texCoord;           //coordenadas de textura
out vec4 FragColor;         //cor do fragmento

void main(void){
 vec4 texpos, initPos, geomDir, geomPos;
 float t, scalar, noise; 

 mat4 mvi = viewMatrixInv;
 vec4 invScaleFactors = 1./scaleFactors;
 noise = fparam[1];

 // ponto de entrada do volume
 initPos = vec4(texCoord, 1.0)*scaleFactors;

 // direcao do raio paralelo ao eixo z 
 geomDir = normalize(mvi * vec4(0.0,0.0,-1.0,0.0)); 

 // inicia com initPos
 t = 0;
 do {
    geomPos = initPos + geomDir*t;   //proxima amostra ao longo do raio
    texpos = geomPos * invScaleFactors;    // coordenadas de textura
    if (texpos.x >= 0.0 && texpos.x <= 1.0 && texpos.y >= 0.0 && texpos.y <= 1.0 
        && texpos.z >= 0.0 && texpos.z <= 1.0) {
      scalar = texture(VOLUME, texpos.xyz).r;
      if (scalar > noise) {
        geomPos = projMatrix * viewMatrix * geomPos;
        geomPos.z = (geomPos.z+1)/2.0;      
        FragColor = vec4(geomPos.z, geomPos.z, geomPos.z, 1.0);
        return; 
      }
    }
    t += fparam[0]; 
 } while (t <= 1.0); 
 
 FragColor = vec4(1.0, 1.0, 1.0, 1.0); 
}
