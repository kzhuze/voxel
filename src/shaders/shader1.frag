#version 330 core
in vec2 texcoord;

uniform sampler2D my_texture;

out vec4 FragColor;

void main() {
    FragColor = texture(my_texture, texcoord);
}
