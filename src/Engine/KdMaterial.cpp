#include "KdMaterial.h"

GLint xe::KdMaterial::map_Kd_location;

void xe::KdMaterial::bind() const{
    glUseProgram(program());
    int use_map_kd = 0;
    if (texture_ > 0){
        use_map_kd = 1;
    }

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_kd));
    
    if (texture_ > 0){
        OGL_CALL(glActiveTexture(GL_TEXTURE0));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    }
};

void xe::KdMaterial::unbind() const{
    if (texture_ > 0){
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
}