cmake_minimum_required(VERSION 3.8.0)

add_subdirectory(third_party)

list(APPEND PROJECT_SOURCES 
    "src/main.cpp"
    )

list(APPEND PROJECT_HEADERS
    )

list(APPEND PROJECT_SHADERS
    )

list(APPEND PROJECT_MODELS
    )

source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Shader Files" FILES ${PROJECT_SHADERS})
source_group("Model Files" FILES ${PROJECT_MODELS})


add_executable(amund 
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_SHADERS}
    ${PROJECT_MODELS})

target_include_directories(amund 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_features(amund PRIVATE cxx_std_17)
    
target_link_libraries(amund
    PRIVATE glfw
    PRIVATE glad
    PRIVATE reactphysics3d
    PRIVATE glm
    ) 

file(COPY "assets" DESTINATION "${CMAKE_BINARY_DIR}")