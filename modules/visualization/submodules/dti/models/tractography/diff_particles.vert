#version 330

in vec4 seed_position;
in vec4 direction;

out vec4 dir;

void main()
{
    gl_Position = seed_position;
    dir = direction;
}

