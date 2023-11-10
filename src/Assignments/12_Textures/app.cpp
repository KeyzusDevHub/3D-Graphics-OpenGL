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

#include "stb/stb_image.h"

#include "Engine/KdMaterial.h"

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    xe::KdMaterial::init();

    // Load texture image
    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    // Creating gl_texture and setting its parameters
    GLuint gl_textures;
    
    OGL_CALL(glGenTextures(1, &gl_textures));

    glBindTexture(GL_TEXTURE_2D, gl_textures); 

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    
    glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(img);

    // A vector containing the x,y,z,x_tex,y_tex vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.5f, 0.0f, 0.1910f, 0.5f,
            0.5f, 0.5f, 0.0f, 0.5f, 0.8090f,
            0.5f, -0.5f, 0.0f, 0.8090f, 0.5f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.1910f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        2, 3, 5,
        0, 1, 6,
        1, 2, 7
    };

    // Create and set new Camera object
    set_camera(new xe::Camera);
    // Create and set new CameraController object
    set_controler(new xe::CameraController(camera()));

    // Create new mesh object
    auto pyramid = new xe::Mesh(5 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                             indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);

    // Load vertices to mesh object
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());

    // Adding position attribute to mesh
    pyramid->add_attribute(xe::POSITION, 3, GL_FLOAT, 0);

    // Adding texture position attribute to mesh
    pyramid->add_attribute(xe::TEXCOORD_0, 2, GL_FLOAT, 3 * sizeof(GLfloat));

    // Load indices to mesh object
    pyramid->load_indices(0, indices.size() * sizeof(GLubyte), indices.data());

    // Creating texture Material and adding it to primitive
    pyramid->add_primitive(0, 18, new xe::KdMaterial({1.f, 1.f, 1.0f, 1.0f}, false, gl_textures));

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