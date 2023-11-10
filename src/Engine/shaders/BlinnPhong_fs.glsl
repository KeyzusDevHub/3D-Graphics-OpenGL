#version 420

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
};

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform KdMaterial {
    vec4 Ka;
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd;
};

const int MAX_POINT_LIGHTS = 16;

layout (std140, binding=2) uniform Lights {
    vec3 ambient;
    int n_lights;
    PointLight lights[MAX_POINT_LIGHTS];
};


vec3 srgb_gamma_correction(vec3 color) {
   color = clamp(color, 0.0, 1.0);
   color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
   return color;
}

in vec4 vertex_color;

in vec2 vertex_texture;

in vec3 vertex_normal_vs;

in vec3 vertex_position_vs;

uniform sampler2D map_Kd;


void main() {
    if (use_vertex_color){
        vFragColor = vertex_color;
    }
    else if (use_map_Kd){
        vec4 texture_color = texture(map_Kd, vertex_texture);
        vFragColor = Kd * texture_color;
        vFragColor.rgb = srgb_gamma_correction(vFragColor.rgb);
    }
    else{
        vFragColor = Kd;
    }
    vFragColor.rgb = Ka.rgb * ambient;
}
