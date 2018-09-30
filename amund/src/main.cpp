#include <iostream>

#include <glfw/glfw3.h>

int main(int, char**) {

    glfwInit ();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);

    auto window = glfwCreateWindow(800,800,"Hello World!", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Error creating GLFW window!" << std::endl;

        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents ();



        glfwSwapBuffers (window);
    }

    std::cout << "Hello, world!\n";
}
