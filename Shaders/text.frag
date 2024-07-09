#version 330

out vec4 FragColor;
in vec2 gTexCoord;

uniform sampler2D Sampler;
uniform vec4 TextColor;

void main()
{
    float A = texture(Sampler, gTexCoord).r;
    FragColor = vec4(TextColor.rgb, A * TextColor.a);

//    FragColor = texture(Sampler, gTexCoord);
}
