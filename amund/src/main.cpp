#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <reactphysics3d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "amund/shader.h"

int main(int, char**) {

try
{
glfwInit ();

 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    std::cout << "GlViewport" << std::endl;

    glEnable(GL_DEPTH_TEST);

    std::cout << "GL_DEPTHTEST" << std::endl;

    //Shader Program Data
    auto vs_loc = std::filesystem::current_path() / "amund" / "assets" / "shaders" / "shader_vert.spv";
    auto fs_loc = std::filesystem::current_path() / "amund" / "assets" / "shaders" / "shader_frag.spv";

    std::cout << vs_loc.string() << std::endl;
    std::cout << fs_loc.string() << std::endl;

    auto gp = amund::graphics::GraphicsShaderProgram (vs_loc, fs_loc);

    std::cout << "Shader program successfully compiled" << std::endl;
    //Uniform Data
    gp.use ();
    std::cout << "Shader program successfully used" << std::endl;
    
    struct UniformBufferObject
    {
        glm::mat4 proj;
        glm::mat4 view;
        glm::mat4 model;

        void BindBufferData(unsigned int buffer)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, buffer);

            glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferObject), this, GL_DYNAMIC_DRAW);

            //glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformBufferObject, proj), sizeof(glm::mat4), glm::value_ptr(proj));
            //glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformBufferObject, view), sizeof(glm::mat4), glm::value_ptr(view));
            //glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformBufferObject, model), sizeof(glm::mat4), glm::value_ptr(model));
        
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    } MyUniform;

    auto proj = glm::perspective(glm::radians(45.0f), (float)800/(float)800, 0.1f, 100.0f);
    auto view = glm::lookAt(glm::vec3(0.0f,0.0f, 6.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));

    MyUniform.proj = proj;
    MyUniform.view = view;
    MyUniform.model = glm::mat4(1.0f);
    unsigned int ubo = 0;
    glGenBuffers(1, &ubo);
    MyUniform.BindBufferData(ubo);

    unsigned int binding = 0;
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);

    auto uniform = glGetUniformBlockIndex (gp.get_id(), "UniformBufferObject");
    assert(uniform != GL_INVALID_INDEX);
    glUniformBlockBinding(gp.get_id(), uniform, binding);

    std::cout << "ubo successfully made." << std::endl;
    //Light ubo

    unsigned int light_ubo = 0;
    glGenBuffers(1, &light_ubo);

    struct LightUBO
    {
        glm::vec4 Pos;
        glm::vec4 Color;

        void BindBufferData(unsigned int buffer)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, buffer);

            glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUBO), this, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    } MyLightUniform;

    MyLightUniform.Pos = glm::vec4(2.0, 2.0, 3.0, 1.0);
    MyLightUniform.Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    MyLightUniform.BindBufferData(light_ubo);

    unsigned int light_binding = 1;

    auto light_uniform = glGetUniformBlockIndex (gp.get_id(), "Light");

    assert(light_uniform != GL_INVALID_INDEX);

    glUniformBlockBinding(gp.get_id(), light_uniform, light_binding);
    
    glBindBufferBase(GL_UNIFORM_BUFFER, light_binding, light_ubo);

    std::cout << "lights successfully made." << std::endl;

    //Vertex Data
    std::vector<float> vertices = {
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);

    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


    std::cout << "vao/vbo successfully made." << std::endl;

    auto prev = std::chrono::high_resolution_clock::now();
    auto lag = 0ll;

    while (!glfwWindowShouldClose(window))
    {
        auto curr = std::chrono::high_resolution_clock::now();
        auto dur = curr - prev;
        prev = curr;
        lag += std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        glfwPollEvents ();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        auto model = glm::mat4(1.0);
        model = glm::rotate(model, glm::radians(static_cast<float>(lag))/20, glm::vec3(0.5,1.0,1.0));
        

        glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(model));
        glBindBuffer(GL_UNIFORM, 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers (window);
    }
}
catch (std::exception& e)
{
    std::cerr << e.what () << std::endl;
}
    
}
