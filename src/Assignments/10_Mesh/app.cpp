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

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    // A vector containing the x,y,z,r,g,b vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
            };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };

    // Create and set new Camera object
    set_camera(new xe::Camera);
    // Create and set new CameraController object
    set_controler(new xe::CameraController(camera()));

    // Create new mesh object
    auto pyramid = new xe::Mesh(6 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                             indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);

    // Load vertices to mesh object
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());

    // Adding position attribute to mesh
    pyramid->add_attribute(xe::POSITION, 3, GL_FLOAT, 0);

    // Adding color attribute to mesh
    pyramid->add_attribute(xe::COLOR_0, 3, GL_FLOAT, 3 * sizeof(GLfloat));

    // Load indices to mesh object
    pyramid->load_indices(0, indices.size() * sizeof(GLubyte), indices.data());

    // Add primitive for all triangles in mesh
    pyramid->add_primitive(0, 18);

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

    OGL_CALL(glUseProgram(program));
    glEnable(GL_DEPTH_TEST);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    // Creating PVM matrix from camera data and model matrix
    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    // Draw all meshes in mesh list
    for (auto m: meshes_)
        m->draw();
    // Sending PVM matrix to uniform buffer
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM[0]));
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