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

    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    GLuint gl_textures;
    
    OGL_CALL(glGenTextures(1, &gl_textures));

    glBindTexture(GL_TEXTURE_2D, gl_textures); 

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    
    glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(img);

    /*
     * A utility function that reads the shaders' source files, compiles them and creates the program object,
     * as everything in OpenGL we reference the program by an integer "handle".
     * The input to this function is a map that associates a shader type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER) with
     * its source file. The source files are located in the PROJECT_DIR/shaders directory, where  PROJECT_DIR is the
     * current assignment directory (e.g., src/Assignments/Triangle).
     */

    // A vector containing the x,y,z vertex coordinates for the triangle.
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

    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));



    auto pyramid = new xe::Mesh(5 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                             indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);

    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());

    pyramid->add_attribute(xe::POSITION, 3, GL_FLOAT, 0);

    pyramid->add_attribute(xe::TEXCOORD_0, 2, GL_FLOAT, 3 * sizeof(GLfloat));

    pyramid->load_indices(0, indices.size() * sizeof(GLubyte), indices.data());

    pyramid->add_primitive(0, 18, new xe::KdMaterial({1.f, 1.f, 1.0f, 1.0f}, false, gl_textures));

    add_mesh(pyramid);

    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    

    auto [w, h] = frame_buffer_size();

    M_ = glm::mat4(1.0f);

    camera()->look_at(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));
    for (auto m: meshes_)
        m->draw();
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
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