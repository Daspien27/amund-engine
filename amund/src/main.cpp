#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <reactphysics3d.h>

#include <glm/glm.hpp>

#include "amund/shader.h"

int main(int, char**) {

    glfwInit ();

 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    auto window = glfwCreateWindow(800,800,"Hello World!", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Error creating GLFW window!" << std::endl;

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Properly initialized gl bindings." << std::endl;

    auto vs_loc = std::filesystem::current_path() / "build" / "assets" / "shaders" / "shader_vert.spv";
    auto fs_loc = std::filesystem::current_path() / "build" / "assets" / "shaders" / "shader_frag.spv";

    auto gp = amund::graphics::GraphicsShaderProgram (vs_loc, fs_loc);

    gp.use ();

    glViewport (0, 0, 800, 800);

    std::vector<float> triangle = {-0.5f, -0.5f, 0.0f,
                                    0.5f, -0.5f, 0.0f,
                                    0.0f,  0.5f, 0.0f};

    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);

    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, triangle.size()/3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, triangle.size() * sizeof(float), triangle.data(), GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents ();

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers (window);
    }

    std::cout << "Hello, world!\n";
}
