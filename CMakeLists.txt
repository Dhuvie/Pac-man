cmake_minimum_required(VERSION 3.10)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Set include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

# Add source files
set(SOURCES
    main.cpp
    Game.cpp
    Player.cpp
    Ghost.cpp
    Maze.cpp
    Renderer.cpp
    ParticleSystem.cpp
    Shader.cpp
    ResourceManager.cpp
)

# Add headers
set(HEADERS
    Game.h
    Player.h
    Ghost.h
    Maze.h
    Renderer.h
    ParticleSystem.h
    Shader.h
    ResourceManager.h
)

# Create executable
add_executable(Pacman ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(Pacman 
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
    glm::glm
)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Set output directory
set_target_properties(Pacman PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# For Windows, copy DLLs if needed
if(WIN32)
    # Add any additional Windows-specific configurations here
endif()
