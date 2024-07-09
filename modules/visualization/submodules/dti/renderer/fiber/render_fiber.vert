#version 330

in vec4 position;
in vec4 direction;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 reorient_matrix;

uniform ivec2 step_range;
uniform bool is_paint_activated;

//uniform sampler3D mainDirTex;
//uniform sampler3D faTex;

out vec4 color;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * reorient_matrix * vec4(position.xyz, 1.f);

    if(is_paint_activated)
    {
        if(position.w < 0 && position.w <= step_range[0])
            color = vec4(1.f);

        else if(position.w >= 0 && position.w >= step_range[1])
            color = vec4(0.0, 0.0, 1.0, 1.0);

        else
            color = vec4(abs(direction.xyz), 1.f);

    } else
        color = vec4(abs(direction.xyz), 1.f);

}

