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

#include "Engine/texture.h"

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    xe::KdMaterial::init();

    // Create and set new Camera object
    set_camera(new xe::Camera);
    // Create and set new CameraController object
    set_controler(new xe::CameraController(camera()));

    // Create new mesh object
    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj", std::string(ROOT_DIR) + "/Models");
    
    // Add mesh to list
    add_mesh(pyramid);

    // Generating empty uniform buffer
    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    

    auto [w, h] = frame_buffer_size();

    M_ = glm::mat4(1.0f);

    // Setting camera position and view angle
    camera()->look_at(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera()->perspective(glm::radians(45.0f), (GLfloat) w / (GLfloat) h, 0.1f, 20.0f);

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    // This set up an OpenGL viewport of the size of the whole rendering window.
    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_DEPTH_TEST);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    // Creating PVM matrix from camera data and model matrix
    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    // Sending PVM matrix to uniform buffer
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));
    // Draw all meshes in mesh list
    for (auto m: meshes_)
        m->draw();
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
}

// Resize function for OpenGL window
void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    glViewport(0, 0, w, h);
    Application::framebuffer_resize_callback(w, h);
    camera()->set_aspect((GLfloat) w / (GLfloat) h);
}

// Scroll handling function for OpenGL window
void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);   
    camera()->zoom(yoffset / -20.0f);
}

// Mouse click handling function for OpenGL window
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

// Cursor movement handling function for OpenGL window
void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controller_) {
        controller_->mouse_moved(x, y);
    }
}