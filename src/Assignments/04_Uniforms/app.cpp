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

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        3, 4, 5,
        5, 4, 6
    };

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    OGL_CALL(glGenBuffers(1, &v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    // This sets up a Vertex Array Object (VAO) that encapsulates
    // the state of all vertex buffers needed for rendering.
    // The vao_ variable is a member of the SimpleShapeApplication class and is defined in src/Application/app.h.
    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));

    // Generating the buffer and loading the indices data into it.

    GLuint i_buffer_handle;
    OGL_CALL(glGenBuffers(1, &i_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));
    OGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));

    // Generating uniform buffer

    GLuint u_buffer_handle;
    OGL_CALL(glGenBuffers(1, &u_buffer_handle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    float strength = 0.5;
    float mix_color[3] = {0.0, 0.0, 1.0};

    // Loading strength and mix_color data into uniform buffer

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_buffer_handle));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), mix_color));

    // Generating second uniform buffer

    GLuint u2_buffer_handle;
    OGL_CALL(glGenBuffers(1, &u2_buffer_handle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u2_buffer_handle));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 12 * sizeof(float), NULL, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    float theta = glm::pi<float>() / 6.0f; //30 degrees
    float cs = std::cos(theta);
    float ss = std::sin(theta);
    glm::mat2 rot{cs,ss,-ss,cs};
    glm::vec2 trans{0.0, -0.25};
    glm::vec2 scale{0.5, 0.5};

    // Loading scale, translation and rotation data into uniform buffer

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u2_buffer_handle));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(float), &scale));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(float), 2 * sizeof(float), &trans));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 2 * sizeof(float), &rot[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(float), 2 * sizeof(float), &rot[1]));


    // This specifies that the data for attribute 0 should be read from a vertex buffer
    OGL_CALL(glEnableVertexAttribArray(0));
    // and this specifies the vertices data layout in the buffer.
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 
                                   reinterpret_cast<GLvoid *>(0)));

    // This specifies that the data for attribute 1 should be read from a vertex buffer
    OGL_CALL(glEnableVertexAttribArray(1));
    
    // and this specifies the color data layout in the buffer.
    OGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 
                                   reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)) ));                               

    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));
    //end of vao "recording"

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will set up all the required vertex attribute arrays.
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, nullptr));
    OGL_CALL(glBindVertexArray(0));
}
