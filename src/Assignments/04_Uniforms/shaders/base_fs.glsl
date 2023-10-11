#version 420

layout(location=0) out vec4 vFragColor;
layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3  mix_color; 
};


in vec4 vertex_color;

void main() {
    vFragColor = vertex_color;
}
