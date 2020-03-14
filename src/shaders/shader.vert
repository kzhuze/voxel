#version 330 core
layout (location = 0) in vec3 local_Position;
layout (location = 1) in vec3 my_texcoord;

out vec3 texcoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(local_Position, 1.0);
    texcoord = my_texcoord;
}
