#version 330
out vec4 FragColor;
in vec2 gTexCoord;
uniform bool StateDepthTex;
uniform sampler2D DepthTex;
uniform sampler2D Sampler;
uniform vec4 TextColor;

uniform vec3 DepthScaleFactor;
in Data
{
    in vec3 pos;
}Fdatain;

void main()
{
    float alpha = TextColor.a;
    float depth=1.0;

    float normalized_pos_z = gl_FragCoord.z;
    if(StateDepthTex){
        depth = texelFetch(DepthTex, ivec2(gl_FragCoord.xy), 0).r; // depth texture of volume
        float eps = 0.000001;
//        if (depth < Fdatain.pos.z-eps){ alpha = 0.0; }
        if (depth < normalized_pos_z-eps){ alpha = 0.0; }
        else { alpha = TextColor.a; }
    }
    float A = texture(Sampler, gTexCoord).r;
    FragColor = vec4(TextColor.rgb, A*alpha);
}
