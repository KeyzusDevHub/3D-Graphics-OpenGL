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
    vec4 Ks;
    float Ns;
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
    vec4 color = vec4(1,1,1,1);
    // Ambient color
    color.rgb = Ka.rgb * ambient;
    
    vec4 color_Kd = Kd;
    
    if (use_vertex_color){
        color_Kd *= vertex_color;
    }

    if (use_map_Kd){
        vec4 texture_color = texture(map_Kd, vertex_texture);
        color_Kd *= texture_color;
        color_Kd.rgb = srgb_gamma_correction(color.rgb);
    }

    vec3 normal = normalize(vertex_normal_vs);

    if (!gl_FrontFacing) {
        normal = -normal;
    }


    for (int i = 0; i < n_lights; i++){
        vec3 light_vector = normalize(lights[i].position - vertex_position_vs);
        float light_distance = length(lights[i].position - vertex_position_vs);
        float diffuse = max(0.0, dot(normal, light_vector));
        float r = max(lights[i].radius, light_distance);
        float attenuation = 1.0 / (r * r);

        vec3 view_vector = normalize(-vertex_position_vs);
        vec3 half_vector = normalize(light_vector + view_vector);
        vec3 specular = ((Ns + 8) / (8 * radians(180))) * pow(max(dot(normal, half_vector), 0.0), Ns) * Ks.rgb * attenuation * lights[i].color * lights[i].intensity;
        
        color.rgb += specular;
        color.rgb += 1.0 / radians(180) * color_Kd.rgb * lights[i].color * lights[i].intensity * diffuse * attenuation;
    }
    vFragColor = color;
}

