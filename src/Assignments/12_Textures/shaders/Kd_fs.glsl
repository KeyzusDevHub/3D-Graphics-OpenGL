#version 420

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd;
};

in vec4 vertex_color;

in vec2 vertex_texture;

uniform sampler2D map_Kd;


void main() {
    if (use_vertex_color){
        vFragColor.rg = vertex_texture.rg;
    }
    else if (use_map_Kd){
        vec4 texture_color = texture(map_Kd, vertex_texture); 
    }
    else{
        vFragColor = Kd;
    }
}
