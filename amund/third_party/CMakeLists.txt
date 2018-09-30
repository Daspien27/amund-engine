cmake_minimum_required(VERSION 3.8.0)

add_subdirectory(glad)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)

option(RP3D_COMPILE_TESTBED OFF)
option(RP3D_COMPILE_TESTS OFF)
option(RP3D_PROFILING_ENABLED OFF)
option(RP3D_LOGS_ENABLED OFF)
option(RP3D_DOUBLE_PRECISION_ENABLED OFF)
add_subdirectory("reactphysics3d")

option(BUILD_TESTING "Enable creation of Eigen tests." OFF)
add_subdirectory("Eigen")