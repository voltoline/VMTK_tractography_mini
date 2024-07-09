#version 330
//http://www.opengl.org/wiki/GLSL_:_common_mistakes
/*  
 *  dvr.frag: shader de fragmento para ray-casting rendering
 *
 *  Copyright (C) 2013  Wu Shin-Ting, FEEC, Unicamp
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

                                    //Definidos pelo cliente:
uniform mat4 viewMatrix, projMatrix;//matrizes MODELVIEW e PROJECTION
in vec3 position;                   //coordenadas espaciais do vértice
in vec3 texCoord0;                  //coordenadas de textura do vértice
out vec3 texCoord;

void main (void)
{
   vec4 tmp;
   tmp = vec4(position, 1.0); // converter para coord.homog.
   gl_Position = projMatrix * viewMatrix * tmp; // projeção do ponto
   texCoord = texCoord0 ; // transferir coord. de textura
}




