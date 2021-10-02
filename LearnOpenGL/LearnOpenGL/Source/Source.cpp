#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>

const char* VERTEX_SHADER_SOURCE =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = aColor;\n"
"}\0";

const char* FRAGMENT_SHADER_SOURCE = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    
    //initialization
    //--------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //create a new GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 650, "Hello Triangle!", NULL, NULL);
    if (window == NULL) { // error check
        std::cout << "Window error" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //make this window the "current" window
    // init glad so we can use openGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // tell openGL what the viewport size is
    glViewport(0, 0, 800, 650);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //tell glfw to call this function for the "SetFrameBufferSizeCallback" callback



    //setting up shaders and shader program
    //-------------------------------------
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER); //create GPU object
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL); //give that GPU object it's source code
    glCompileShader(vertex_shader); //compile source code
    //check for shader cource code compilation errors
    int  successV;
    char infoLogV[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &successV);
    if (!successV)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLogV);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogV << std::endl;
    }
    //set up frag shader
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
    glCompileShader(fragment_shader);
    //check for shader cource code compilation errors
    int  successF;
    char infoLogF[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &successF);
    if (!successF)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLogF);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogF << std::endl;
    }

    //create shader program that links our shaders together
    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // error check shader_program linking
    int successP;
    char infoLogP[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &successP); //get status
    if (!successP)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLogP); //if failed, tell us what happened
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLogF << std::endl;
    }

    glDeleteShader(vertex_shader); //now get rid of the shader objects, we're done w them
    glDeleteShader(fragment_shader);


    //set up vertex data and buffers
    //------------------------------
    float vertices[] = {
        // positions            // colors
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f    // top 
    };

    int indices[] = {
        0, 1, 2
    };
    //generate VBO and VAO objects on the GPU
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //bind vertex array object so all subsequent VBO calls will be stored with this VAO

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind this buffer as the current GL_ARRAY_BUFFER object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //give that buffer it's data

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set up vertex attributes
    //position
    glEnableVertexAttribArray(0); //tell opengl we are using attribute position 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //tell openGL how the vertex data is supposed to be read
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //unbind
    glBindVertexArray(0);


    //main tick loop
    //--------------
    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering stuff
        
        //make a background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //now render actual triangle
        glUseProgram(shader_program); // tell openGL that we are using this shader program
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
