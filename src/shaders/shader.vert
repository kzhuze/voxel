#version 330 core
layout (location = 0) in vec3 local_Position;
layout (location = 1) in vec3 face_Color;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(local_Position, 1.0);
    Color = face_Color;
}
