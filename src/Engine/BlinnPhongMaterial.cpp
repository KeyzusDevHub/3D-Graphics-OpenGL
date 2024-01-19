#include "BlinnPhongMaterial.h"

GLint xe::BlinnPhongMaterial::map_Kd_location;

glm::vec4 xe::BlinnPhongMaterial::Ka_;
glm::vec4 xe::BlinnPhongMaterial::Ks_;
float xe::BlinnPhongMaterial::Ns_;

xe::Material *xe::BlinnPhongMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
     glm::vec4 color = get_color(mat.diffuse);
     xe::BlinnPhongMaterial::Ka_ = get_color(mat.ambient);
     xe::BlinnPhongMaterial::Ks_ = get_color(mat.specular);
     xe::BlinnPhongMaterial::Ns_ = mat.shininess;
     SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
     auto material = new xe::BlinnPhongMaterial(color);
     if (!mat.diffuse_texname.empty()) {
         auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
         SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
         if (texture > 0) {
             material->set_texture(texture);
         }
     }

     return material;
}

void xe::BlinnPhongMaterial::init() {
    BlinnPhongMaterial::create_material_uniform_buffer(5 * sizeof(glm::vec4));
    create_program_in_project({{GL_VERTEX_SHADER, "BlinnPhong_vs.glsl"},
                                {GL_FRAGMENT_SHADER, "BlinnPhong_fs.glsl"}});

    map_Kd_location = glGetUniformLocation(program(), "map_Kd");
    if (map_Kd_location == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
    }
    add_mat_function("BlinnPhongMaterial", BlinnPhongMaterial::create_from_mtl);
};

void xe::BlinnPhongMaterial::bind() const{
    glUseProgram(program());
    int use_map_kd = 0;
    if (texture_ > 0){
        use_map_kd = 1;
    }
    
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &xe::BlinnPhongMaterial::Ka_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &xe::BlinnPhongMaterial::Ks_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(GLfloat), &xe::BlinnPhongMaterial::Ns_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(GLfloat), sizeof(int), &use_vertex_colors_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(GLfloat) + sizeof(int), sizeof(int), &use_map_kd));
    
    if (texture_ > 0){
        OGL_CALL(glActiveTexture(GL_TEXTURE0));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    }
};

void xe::BlinnPhongMaterial::unbind() const{
    if (texture_ > 0){
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
    
}