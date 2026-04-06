cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

######################################################################################

# OpenGL

# CMake comes with the ability to "go find" opengl on most systems out of the box.
# That is what is happening here.
# Assuming it is found we add it to the link libraries for this project immediately
# The "target_link_libraries" command will APPEND to the list of libraries to link 
# to for a given target, not overwrite.

find_package(OpenGL REQUIRED)

######################################################################################

# Vendor libraries

########
# GLFW #
########

# Disable everything we don't need
# We have to do this here because the CMakeLists that is gonna be in the vendor/glfw
# directory is going to be the one that gets pulled down from github. Editing that
# manually would be silly, so we set these variable up-front here.

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Keep in mind, this assumes we've already pulled all the git submodules down.
# If we haven't then this is just an empty directory with no CMakeLists in it.

add_subdirectory(vendor/glfw)

#######
# GLM #
#######

# From what I understand this is kind of just a hack.
# Since GLM is a header only library, we can just say "add_subdirectory" and a target
# will get created called "glm" and when you add it to link libraries with
# "target_link_libraries(<project> <visibility> glm)" it really is just going to expose
# all of those headers.

# Might want to clean this up into something more discrete later on.

add_subdirectory(vendor/glm)

########
# glad #
########

# Remember, glad is just a little online tool that generates a glad.c and glad.h 
# (and possibly other headers) and all you do i just compile and link with it
# it's sole purpose is to just give you all the modern opengl function pointers

add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

#########
# ImGui #
#########

# Kinda same deal as glad. Even the DearImGui github repo itself just says
# "compile and link against what you need" that is literally it
# more files than glad, but as it says on the github repo, you just cmpile and
# link with what you use.

add_library(imgui STATIC
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    vendor/imgui
    vendor/imgui/backends
)

#the reason we need imgui to link with glfw is because if you look closely above, 
# we compile "imgui_imp_glfw.cpp". This is just one of the cpp files dearimgui
# comes with out of the box that allows it to work seamlessly with glfw-based
# applications. Remember though that imgui is linking PRIVATELY with glfw.
# meaning our "imgui" library depends on glfw in order to build itself.
# but users of the "imgui" library do not NECESSARILY need to link with glfw.
# (although they probably will on their own)
target_link_libraries(imgui PRIVATE glfw)

######################################################################################

# My project now

# Define SOURCES as every .cpp file under ./src
# GLOB_RECURSE will do this find recursively for every subdirectory of ./src

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# Remember, "include" here is a relative path.
# Meaning "${PROJECT_NAME}"s include directories are: "./include"
# As in, OUR include files we write, not vendor includes.

target_include_directories(${PROJECT_NAME} PRIVATE include)

# target ${PROJECT_NAME} links with the libraries we defined above, plus OpenGL
# The "OpenGL::GL" syntax is some modern CMake business I'm not sure I
# entirely understand yet. To my understanding, whenever we do a "find_package"
# what gets found is referred to by "library_name::target_name"
# kind of like a c++ operator where library_name == namespace
# and target_name == class definition.
target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    glm
    imgui
    OpenGL::GL
)

# copy shaders and assets to be next to the executable

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
