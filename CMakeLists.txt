cmake_minimum_required(VERSION 3.14)

#LANGUAGES can be: C , CXX (i.e. C++)
project(posaix VERSION 1.0
DESCRIPTION "texture shader using mathematical functions"
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# To prevent Apple frameworks from being included before brew installed packages
include_directories(BEFORE /usr/local/include)
set(CMAKE_FIND_FRAMEWORK LAST) 

# SDL
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2 REQUIRED)

# GLM
find_package(glm REQUIRED)

# Create the library
add_library(${PROJECT_NAME})

# submodules
add_subdirectory(extern) 

# source -- source for the library
add_subdirectory(source) 

# staging -- these are test executables to run
option(BUILD_STAGING "Build staging" ON)
if(BUILD_STAGING)
    add_subdirectory(staging) 
endif()

# examples -- fully fletched out examples
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples) 
endif()

# testing
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    #googletest
    find_package(GTest CONFIG REQUIRED) 
    # Enable testing for the project.
    enable_testing()
    # Include your tests subdirectory.
    add_subdirectory(tests)
endif()

# Link SDL to the library
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# Enable imgui default math operator
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

# Copy assets to the build directory
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)