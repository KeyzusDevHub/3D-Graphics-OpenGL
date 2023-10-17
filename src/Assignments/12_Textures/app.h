//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "glad/gl.h"

#include "Engine/camera.h"

#include "Engine/camera_controller.h"

#include "Engine/Mesh.h"

#include "Engine/KdMaterial.h"

#include "glm/glm.hpp"

#include "Application/application.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
                                                                                               debug), camera_(nullptr) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void set_camera(xe::Camera *camera) { camera_ = camera; }

    void set_controler(xe::CameraController *controller) { controller_ = controller; }

    xe::Camera* camera() const {
        assert(camera_);
        return camera_;
    }

    void add_mesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }

private:
    GLuint vao_;
    GLuint u_trans_buffer_handle_;
    
    xe::Camera *camera_;
    xe::CameraController *controller_;

    glm::mat4 M_;

    std::vector<xe::Mesh*> meshes_; 
};