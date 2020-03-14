#version 330 core
in vec3 texcoord;

uniform sampler2D top_texture;
uniform sampler2D side_texture;

out vec4 FragColor;

void main() {
    if (texcoord.z==1) {
        FragColor = texture(top_texture, texcoord.xy);
    } else {
        FragColor = texture(side_texture, texcoord.xy);
    }
}
