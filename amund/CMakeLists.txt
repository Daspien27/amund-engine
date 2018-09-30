add_subdirectory(third_party)

set(SOURCE_FILES "src/main.cpp")

add_executable(amund ${SOURCE_FILES})

target_include_directories(amund 
    PUBLIC "include"
    PRIVATE "src")