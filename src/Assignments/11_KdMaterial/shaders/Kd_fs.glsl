#version 420

layout(location=0) out vec4 vFragColor;
layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
};

in vec4 vertex_color;

void main() {
    if (use_vertex_color){
        vFragColor = vertex_color;
    }
    else{
        vFragColor = Kd;
    }
}
