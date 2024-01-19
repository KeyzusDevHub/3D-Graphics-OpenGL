#version 420

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec3 a_vertex_normal;
layout(location=3) in vec2 vTexture;
layout(location=5) in vec4 vFragColor2;


layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
    mat4 VM;
    mat3 VM_normal;
    vec3 viewPos;
};


out vec4 vertex_color;

out vec2 vertex_texture;

out vec3 vertex_normal_vs;

out vec3 vertex_position_vs;

void main() {
    gl_Position = PVM * a_vertex_position;
    vertex_color = vFragColor2;
    vertex_texture = vTexture;
    vertex_normal_vs = normalize(VM_normal * a_vertex_normal);

    vec4 a_vertex_position_vs = VM * a_vertex_position;
    vertex_position_vs = a_vertex_position_vs.xyz/a_vertex_position_vs.w;
}
