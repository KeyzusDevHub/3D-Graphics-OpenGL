cmake_minimum_required(VERSION 3.15)
project(Triangle)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        )
        
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)


