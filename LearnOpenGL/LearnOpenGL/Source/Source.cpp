#include < glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#include <filesystem>
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <input.h>
#include <functional>
#include <camera.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
#pragma region setting up input
    //set up a list of buttons since i dont have any kind of config file type shit set up rn
    std::map<std::string, engine::Button> buttons = {
        {"esc", engine::Button(GLFW_KEY_ESCAPE)},
        {"w", engine::Button(GLFW_KEY_W)},
        {"a", engine::Button(GLFW_KEY_A)},
        {"s", engine::Button(GLFW_KEY_S)},
        {"d", engine::Button(GLFW_KEY_D)}
    };
    //buttons and window members must be set with setter functions after construction
    engine::InputManager& input_manager = *engine::InputManager::getptr();
    input_manager.buttons = buttons;
#pragma endregion

#pragma region glfw initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //create a new GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 650, "Hello Triangle!", NULL, NULL);
    if (window == NULL) { // error check
        std::cout << "Window error" << std::endl;
        glfwTerminate();
        return -1;
    }
    input_manager.setWindow(window);
    glfwMakeContextCurrent(window); //make this window the "current" window
    // init glad so we can use openGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // tell openGL what the viewport size is
    glViewport(0, 0, 800, 650);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
#pragma endregion

#pragma region set up glfw callbacks
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, input_manager.static_key_callback);
    glfwSetCursorPosCallback(window, input_manager.static_mouse_callback);
    glfwSetScrollCallback(window, input_manager.static_scroll_callback);
#pragma endregion

#pragma region set up shader
    Shader standard_shader("./Source/shader.vert", "./Source/shader.frag");
    standard_shader.use();
#pragma endregion

#pragma region set up texture
    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glGenTextures(1, &texture2);

    //first texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //2nd texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image and generate texture
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        //note that this image is a png which has an alpha channel so we specify that with the glenum "GL_RGBA"
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    //tell shader to use the textures
    standard_shader.setInt("_texture", 0);
    standard_shader.setInt("_texture2", 1);
#pragma endregion

#pragma region vertex data
    float vertices2[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float vertices[] = {
        // positions          // colors           // texture coords
    /*0*/ 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // front top right
    /*1*/ 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // front bottom right
    /*2*/-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // front bottom left
    /*3*/-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // front top left 
    /*4*/ 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // back top right
    /*5*/ 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // back bottom right
    /*6*/-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // back bottom left
    /*7*/-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // back top left 
    };

    int indices[] = {
        0, 1, 2, //front
        2, 3, 0,

        3, 7, 6, // left
        6, 2, 3,

        0, 1, 5, //right
        5, 4, 0,

        1, 5, 6, //bottom
        6, 2, 1,

        0, 4, 7, //top
        7, 3, 0,

        4, 7, 6, //back
        6, 5, 4
    };

#pragma endregion

#pragma region set up vertex buffers
    //generate VBO and VAO objects on the GPU
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //bind vertex array object so all subsequent VBO calls will be stored with this VAO

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind this buffer as the current GL_ARRAY_BUFFER object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW); //give that buffer it's data

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set up vertex attributes
    //position
    glEnableVertexAttribArray(0); //tell opengl we are using attribute position 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //tell openGL how the vertex data is supposed to be read

    //color
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    //texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //unbind
    glBindVertexArray(0);
#pragma endregion
   
#pragma region misc update variables
    float last_time = (float)glfwGetTime();
    float current_time = (float)glfwGetTime();
    float delta_time = 0;
    float move_speed = 2.5;
    float mouse_sensitivity = 0.5f;
    float zoom_speed = 15.0f;
    engine::Camera camera;
    
#pragma endregion

#pragma region cube positions
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
#pragma endregion

#pragma region update
    while (!glfwWindowShouldClose(window))
    {
        //delta time
        current_time = (float)glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        //check if we want to close window and end program.
        glfwSetWindowShouldClose(window, input_manager.buttons.at("esc").down);

        //movement input
        //--------------
        glm::vec3 delta_move(0.0f, 0.0f, 0.0f);
        //forward
        if (input_manager.buttons.at("w").down)
        {
            delta_move += move_speed * (float)delta_time * camera.get_forward();
        }
        //back
        if (input_manager.buttons.at("s").down)
        {
            delta_move -= move_speed * (float)delta_time * camera.get_forward();
        }
        //left
        if (input_manager.buttons.at("a").down)
        {
            delta_move += move_speed * (float)delta_time * camera.get_right();
        }
        //right
        if (input_manager.buttons.at("d").down)
        {
            delta_move -= move_speed * (float)delta_time * camera.get_right();
        }        
        camera.set_position(delta_move + camera.get_position());


        //first person look input and calcs
        //---------------------------------
        glm::vec2 new_rotation;
        glm::vec2& mouse_offset = input_manager.mouse_offset;
        new_rotation.y = camera.get_rotation().y - (mouse_offset.x * mouse_sensitivity);
        new_rotation.x = camera.get_rotation().x - (mouse_offset.y * mouse_sensitivity);
        if (new_rotation.x > 89.0f)
            new_rotation.x = 89.0f;
        if (new_rotation.x < -89.0f)
            new_rotation.x = -89.0f;
        camera.set_rotation(new_rotation);
        input_manager.mouse_offset = glm::vec2(0.0f, 0.0f); // mouse offset has to be reset each frame since the callback is only called when there is mouse input.


        //zoom input and calcs
        //--------------------
        camera.fov -= input_manager.scroll_offset.y * zoom_speed;
        if (camera.fov < camera.min_fov)
        {
            camera.fov = camera.min_fov;
        }
        if (camera.fov > camera.max_fov)
        {
            camera.fov = camera.max_fov;
        }
        input_manager.scroll_offset = glm::vec2(0.0f, 0.0f); // scroll offset has to be reset each frame since the callback is only called when there is scroll input.

        //rendering stuff
        //---------------
        glm::mat4 view = glm::lookAt(camera.get_position(), camera.get_position() + camera.get_forward(), camera.get_up());
        standard_shader.setMat4("view", view);
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.fov), camera.aspect_ratio.y / camera.aspect_ratio.x , camera.near_plane, camera.far_plane);
        standard_shader.setMat4("projection", projection);

        //make a background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //now render actual triangle
        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20 * i;
            model = glm::rotate(model, (float)glm::radians(angle) + (float)current_time, glm::vec3(1.0f, 0.3, 0.5f));
            standard_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma endregion

#pragma region termination 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
#pragma endregion

    return 0;
}
