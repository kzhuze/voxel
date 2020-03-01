#version 330 core
layout (location = 0) in vec3 local_Position;
layout (location = 1) in vec2 my_texcoord;
layout (location = 2) in vec3 block_Position;
out vec2 texcoord;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    mat4 model = mat4(0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    block_Position.x, block_Position.y, block_Position.z, 1.0);
    gl_Position = projection * view * model * vec4(local_Position, 1.0);
    texcoord = my_texcoord;
}
