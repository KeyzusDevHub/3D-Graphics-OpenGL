#pragma once

#include "AbstractMaterial.h"

#include "Engine/utils.h"

#include "Engine/mesh_loader.h"

#include "ObjectReader/sMesh.h"

#include "texture.h"

namespace xe {
    class KdMaterial : public AbstractMaterial<KdMaterial> {
        public:

            static GLint map_Kd_location;

            KdMaterial(const glm::vec4 &Kd) : Kd_(Kd), use_vertex_colors_(false), texture_(0) {}

            KdMaterial(const glm::vec4 &Kd, int uvc, GLuint &texture) : Kd_(Kd), use_vertex_colors_(uvc), texture_(texture) {}

            static void init();

            static Material* create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);

            void set_texture(GLuint texture) {texture_ = texture;};

            void bind() const override;

            void unbind() const override;
            

        private:
            const glm::vec4 Kd_;

            int use_vertex_colors_;

            GLuint texture_;


    };
 }