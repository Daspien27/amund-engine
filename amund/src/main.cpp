#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <reactphysics3d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    glViewport (0, 0, 800, 800);

    //Shader Program Data
    auto vs_loc = std::filesystem::current_path() / "build" / "assets" / "shaders" / "shader_vert.spv";
    auto fs_loc = std::filesystem::current_path() / "build" / "assets" / "shaders" / "shader_frag.spv";

    auto gp = amund::graphics::GraphicsShaderProgram (vs_loc, fs_loc);

    //Uniform Data
    gp.use ();
    auto proj = glm::ortho(-4, 4, -4, 4, 1, 100);
    auto view = glm::lookAt(glm::vec3(0.0,0.0, -5.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));

    auto uniform = glGetUniformBlockIndex (1, "UniformBufferObject");

    assert(uniform != GL_INVALID_INDEX);

    unsigned int binding = 0;
    glUniformBlockBinding(1, uniform, binding);

    unsigned int ubo = 0;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
 
    struct UniformBufferObject
    {
        glm::mat4 proj;
        glm::mat4 view;
        glm::mat4 model;

        void BindBufferData(unsigned int binding, unsigned int buffer)
        {
            float uniform [16*3];

            auto p_proj = glm::value_ptr(proj);
            for (int i  = 0; i < 16; ++i)
            {
                uniform[i] = p_proj[i];
            }

            auto p_view = glm::value_ptr(view);
            for (int i  = 0; i < 16; ++i)
            {
                uniform[i+16] = p_view[i];
            }

            auto p_model = glm::value_ptr(model);
            for (int i  = 0; i < 16; ++i)
            {
                uniform[i+32] = p_model[i];
            }

            glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform), uniform, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
        }
    } MyUniform;

    MyUniform.proj = proj;
    MyUniform.view = view;

    MyUniform.BindBufferData(binding, ubo);


    //Vertex Data
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

    auto prev = std::chrono::high_resolution_clock::now();
    auto lag = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        auto curr = std::chrono::high_resolution_clock::now();
        auto dur = curr - prev;
        prev = curr;
        lag += std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        glfwPollEvents ();

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        auto model = glm::mat4(1.0);
        //model = glm::rotate(model, static_cast<float>(lag), glm::vec3(0.0,0.0,1.0));
        
        glBufferSubData(GL_UNIFORM, offsetof(UniformBufferObject, model), 16, glm::value_ptr(model));


        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers (window);
    }

    std::cout << "Hello, world!\n";
}
