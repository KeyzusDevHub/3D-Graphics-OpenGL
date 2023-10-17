#include "KdMaterial.h"

GLint xe::KdMaterial::map_Kd_location;

void xe::KdMaterial::bind() const{
    glUseProgram(program());
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), (void*) 0));
};

void xe::KdMaterial::unbind() const{
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
}