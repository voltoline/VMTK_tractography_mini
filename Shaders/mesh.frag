#version 330

uniform int type;

out vec4 color;

void main(void)
{
    if (type == -6)
        color = vec4(1.0,0.647,0.0,1.0);//laranja
    else if (type == -5)
         color = vec4(1.0,1.0,1.0,1.0);//branco
    else if (type == -4)
         color = vec4(0.0,0.0,0.0,1.0);//preto
    else if (type == -3)
        color = vec4(0.0,1.0,1.0,1.0); //ciano
    else if(type == -2)
        color = vec4(1.0,0.0,1.0,1.0);//magenta
    else if(type == -1)
        color = vec4(0.0,0.0,1.0,1.0);//azul
    else if (type == 0)
        color = vec4(0.0,1.0,0.0,1.0);//verde
    else
        color = vec4(1.0,0.0,0.0,1.0);//vermelho
}
