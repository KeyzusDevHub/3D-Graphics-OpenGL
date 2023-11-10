//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include <numeric>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "glm/glm.hpp"

#include "glm/gtc/constants.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "Engine/KdMaterial.h"

#include "Engine/BlinnPhongMaterial.h"

#include "Engine/texture.h"

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    xe::BlinnPhongMaterial::init();

    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));

    lights_.push_back(xe::PointLight(glm::vec3({0.0f, 0.0f, 1.0f}), glm::vec3({1.0f, 1.0f, 1.0f}), 1.0f, 0.1f));

    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(pyramid);

    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::mat4x3), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    OGL_CALL(glGenBuffers(1, &u_light_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, xe::MAX_POINT_LIGHTS * sizeof(xe::PointLight) + sizeof(glm::vec3) + sizeof(int), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    

    auto [w, h] = frame_buffer_size();

    M_ = glm::mat4(1.0f);

    camera()->look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera()->perspective(glm::radians(45.0f), (GLfloat) w / (GLfloat) h, 0.1f, 20.0f);

    // Setting the background color of the rendering window,
    // I suggest not using white or black for better debugging.
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    // This set up an OpenGL viewport of the size of the whole rendering window.
    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_DEPTH_TEST);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;

    glm::mat4 VM = camera()->view() * M_;

    auto R = glm::mat3(VM);
    auto VM_Normal = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    auto ambient = glm::vec3({0.25f, 0.25f, 0.25f});
    auto l_size = lights_.size();

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &VM_Normal[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &VM_Normal[1]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &VM_Normal[2]));

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &ambient));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(int), &l_size));
    for(int i = 0; i < l_size; i++){
        lights_[i].transform(camera());
        void* ptr = glMapBufferRange(GL_UNIFORM_BUFFER, i * sizeof(xe::PointLight) + sizeof(glm::vec3) + sizeof(int), sizeof(glm::vec3), GL_MAP_WRITE_BIT);
        assert(ptr);
        memcpy(ptr, &lights_[i], sizeof(glm::vec3));
        OGL_CALL(glUnmapBuffer(GL_UNIFORM_BUFFER));
    }
    for (auto m: meshes_)
        m->draw();
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, 0));
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    glViewport(0, 0, w, h);
    Application::framebuffer_resize_callback(w, h);
    camera()->set_aspect((GLfloat) w / (GLfloat) h);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);   
    camera()->zoom(yoffset / -20.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controller_) {
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        controller_->LMB_pressed(x, y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        controller_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controller_) {
        controller_->mouse_moved(x, y);
    }
}