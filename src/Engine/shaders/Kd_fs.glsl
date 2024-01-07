#version 420

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd;
};

vec3 srgb_gamma_correction(vec3 color) {
   color = clamp(color, 0.0, 1.0);
   color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
   return color;
}

in vec4 vertex_color;

in vec2 vertex_texture;

uniform sampler2D map_Kd;


void main() {
    vec4 color = vec4(1,1,1,1);
    if (use_vertex_color){
       color *= vertex_color;
    }
    color *= Kd;
    if (use_map_Kd){
        vec4 texture_color = texture(map_Kd, vertex_texture);
        color *= texture_color;
        color.rgb = srgb_gamma_correction(color.rgb);
    }
    vFragColor = color;
}
