#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <reactphysics3d.h>

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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents ();



        glfwSwapBuffers (window);
    }

    std::cout << "Hello, world!\n";
}
