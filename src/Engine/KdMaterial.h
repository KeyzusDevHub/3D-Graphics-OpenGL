#pragma once

#include "AbstractMaterial.h"

namespace xe {
    class KdMaterial : public AbstractMaterial<KdMaterial> {
        public:

            static GLint map_Kd_location;

            KdMaterial(const glm::vec4 &Kd) : Kd_(Kd), use_vertex_colors_(false) {}

            static void init() {
                KdMaterial::create_material_uniform_buffer(sizeof(glm::vec4) + 2 * sizeof(int));
                create_program_in_project({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                                           {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
                map_Kd_location = glGetUniformLocation(program(),"map_Kd");
                if (map_Kd_location == -1) {
                        SPDLOG_WARN("Cannot find map_Kd uniform");
                }
            };

            void bind() const override;

            void unbind() const override;
            

        private:
            const glm::vec4 Kd_;

            bool use_vertex_colors_;


    };
 }