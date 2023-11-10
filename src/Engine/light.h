#pragma once
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "stb/stb_image.h"
#include "Application/utils.h"
#include "camera.h"

namespace xe {
    
    const GLuint MAX_POINT_LIGHTS = 16;

    struct PointLight {
        PointLight() = default;
        PointLight(const glm::vec3 &pos, const glm::vec3 &color, float intensity, float radius)
                : position_in_vs(pos),
                  color(color), intensity(intensity), radius(radius) {}

        alignas(16) glm::vec3 position_in_vs;
        float radius;
        alignas(16) glm::vec3 color;
        float intensity;
      
        void transform(Camera* camera){

            glm::vec4 tmp = glm::vec4(position_in_vs, 1.0f) * camera->view();
            position_in_vs.x = tmp.x;
            position_in_vs.y = tmp.y;
            position_in_vs.z = tmp.z;
        }
    };
}