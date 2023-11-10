#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=3) in vec2 vTexture;
layout(location=5) in vec4 vFragColor2;


layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

out vec4 vertex_color;

out vec2 vertex_texture;

void main() {
    gl_Position = PVM * a_vertex_position;
    vertex_color = vFragColor2;
    vertex_texture = vTexture;
}
