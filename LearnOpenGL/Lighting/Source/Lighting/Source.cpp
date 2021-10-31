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
        {"d", engine::Button(GLFW_KEY_D)},
        {"q", engine::Button(GLFW_KEY_Q)},
        {"e", engine::Button(GLFW_KEY_E)},
        {"right", engine::Button(GLFW_KEY_RIGHT)},
        {"left", engine::Button(GLFW_KEY_LEFT)},
        {"up", engine::Button(GLFW_KEY_UP)},
        {"down", engine::Button(GLFW_KEY_DOWN)}
    };
    //buttons and window members must be set with setter functions after construction
    engine::InputManager& input_manager = *engine::InputManager::getptr();
    input_manager.first_frame = true;
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
    Shader standard_shader("./Source/Lighting/shader.vert", "./Source/Lighting/shader.frag");
    Shader light_cube_shader("./Source/Lighting/light.vert", "./Source/Lighting/light.frag");
    Shader axes_shader("./Source/Lighting/axes.vert", "./Source/Lighting/axes.frag");
    #pragma endregion
    
    #pragma region set up texture
    unsigned int crate_texture, crate_texture_specular, matrix;
    glGenTextures(1, &crate_texture);
    glGenTextures(1, &crate_texture_specular);
    glGenTextures(1, &matrix);

    //first texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crate_texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // `second texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crate_texture_specular);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // `third texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, matrix);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("matrix.jpg", &width, &height, &nrChannels, 0);
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
    #pragma endregion
    
    #pragma region vertex data
    float cube[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    float axes[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    unsigned int axes_indices[] = {
        0, 1,
        0, 2,
        0, 3
    };
    #pragma endregion
    
    #pragma region set up vertex buffers
    //generate VBO and VAO objects on the GPU
    unsigned int VBO, VAO, light_cube_VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //bind vertex array object so all subsequent VBO calls will be stored with this VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind this buffer as the current GL_ARRAY_BUFFER object
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW); //give that buffer it's data
    //set up vertex attributes
    //position
    glEnableVertexAttribArray(0); //tell opengl we are using attribute position 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //tell openGL how the vertex data is supposed to be read
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    //for light source cube
    glGenVertexArrays(1, &light_cube_VAO);
    glBindVertexArray(light_cube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    //position
    glEnableVertexAttribArray(0); //tell opengl we are using attribute position 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //tell openGL how the vertex data is supposed to be read

    //3d axes objects
    unsigned int axesVAO, axesVBO, axesEBO;
    glGenVertexArrays(1, &axesVAO);
    glBindVertexArray(axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
    glGenBuffers(1, &axesEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axes_indices), axes_indices, GL_STATIC_DRAW);

    //vertex attrib for both color and position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

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
    glm::vec3 light_direction(1.0f);

    standard_shader.use();

    // positions all containers
    glm::vec3 cube_positions[] = {
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

    //point light Positions
    glm::vec3 light_positions[] =
    {
        glm::vec3(3.0f, 0.3f, 3.0f),
        glm::vec3(-3.0f, 1.3f, 3.0f),
        glm::vec3(3.0f, 0.3f, -3.0f),
        glm::vec3(0.5f, 1.2f, -.3f)
    };

    standard_shader.setInt("u_material.diffuse_map", 0);
    standard_shader.setInt("u_material.specular_map", 1);
    standard_shader.setInt("u_material.emission_map", 2);
    standard_shader.setFloat("u_material.shininess", 32.0f);
    
    #pragma endregion
    
    #pragma region update
    while (!glfwWindowShouldClose(window))
    {
        //check if we want to close window and end program.
        glfwSetWindowShouldClose(window, input_manager.buttons.at("esc").down);
        //delta time
        current_time = (float)glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        #pragma region input processing
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
        //camera up
        if (input_manager.buttons.at("e").down)
        {
            delta_move += move_speed * delta_time * camera.get_up();
        }
        //camera down
        if (input_manager.buttons.at("q").down)
        {
            delta_move -= move_speed * delta_time * camera.get_up();
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
        input_manager.scroll_offset = glm::vec2(0.0f, 0.0f); // scroll offset has to be reset each frame since the callback is only called when there is scrollinput.  
        #pragma endregion

        #pragma region matrices
        glm::mat4 camera_view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), camera.aspect_ratio.y / camera.aspect_ratio.x, camera.near_plane, camera.far_plane);

        glm::mat4 cube_model(1.0f);
        cube_model = glm::translate(cube_model, glm::vec3(-2.0f, 0, 1.0f));
        cube_model = glm::rotate(cube_model, current_time / 8, glm::vec3(0.2f, 0.5f, 0.9f));

        glm::mat4 axes_model(1.0f);
        #pragma endregion

        #pragma region drawing
        //draw calls
        //----------
        //make background black and clear the buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #pragma region setting shaders
        standard_shader.use();
        standard_shader.setMat4("u_model", cube_model);
        standard_shader.setMat4("u_view", camera_view);
        standard_shader.setMat4("u_projection", projection);
        standard_shader.setVec3("u_viewPos", camera.get_position());

        //directional light
        standard_shader.setVec3("u_dirLight.direction", glm::vec3(0.2f, 0.7f, 0.1f));
        standard_shader.setVec3("u_dirLight.ambient", glm::vec3(0.05f));
        standard_shader.setVec3("u_dirLight.diffuse", glm::vec3(0.05f));
        standard_shader.setVec3("u_dirLight.specular", glm::vec3(0.0f));

        //point lights
        for (int i = 0; i < 4; i++)
        {
            standard_shader.setVec3("u_pointLight[" + std::to_string(i) + "].position", light_positions[i]);
            standard_shader.setVec3("u_pointLight[" + std::to_string(i) + "].diffuse", glm::vec3(0.25f, 0.9f, 0.1f));
            standard_shader.setVec3("u_pointLight[" + std::to_string(i) + "].specular", glm::vec3(0.6f, 1.0f, 0.3f));
            standard_shader.setFloat("u_pointLight[" + std::to_string(i) + "].constant", 1.0f);
            standard_shader.setFloat("u_pointLight[" + std::to_string(i) + "].linear", 0.6f);
            standard_shader.setFloat("u_pointLight[" + std::to_string(i) + "].quadratic", 0.45f);
        }

        //spot light
        standard_shader.setVec3("u_spotLight.direction", camera.get_forward());
        standard_shader.setVec3("u_spotLight.position", camera.get_position());
        standard_shader.setVec3("u_spotLight.diffuse", glm::vec3(0.5f, 0.4f, 0.3f));
        standard_shader.setVec3("u_spotLight.specular", glm::vec3(0.9f, 0.7f, 0.5f));
        standard_shader.setFloat("u_spotLight.innerCutoff", glm::cos(glm::radians(9.0f)));
        standard_shader.setFloat("u_spotLight.outerCutoff", glm::cos(glm::radians(19.0f)));
        standard_shader.setFloat("u_spotLight.constant", 1.0f);
        standard_shader.setFloat("u_spotLight.linear", 0.14f);
        standard_shader.setFloat("u_spotLight.quadratic", 0.07f);
        #pragma endregion

        #pragma region drawing objects
        //draw cubes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            standard_shader.setMat4("u_model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //draw light cubes for point lights
        glBindVertexArray(light_cube_VAO);
        light_cube_shader.use();
        light_cube_shader.setMat4("u_projection", projection);
        light_cube_shader.setMat4("u_view", camera_view);
        light_cube_shader.setVec3("u_lightColor", glm::vec3(0.25f, 0.9f, 0.1f));
        glm::mat4 light_model(1.0f);
        for (int i = 0; i<4; i++)
        {
            light_model = glm::mat4(1.0f);
            light_model = glm::translate(light_model, light_positions[i]);
            light_model = glm::scale(light_model, glm::vec3(0.2f));
            light_cube_shader.setMat4("u_model", light_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //and another one for the spot light. I'l make it scaled down to show the difference
        //light_model = glm::mat4(1.0f);
        //light_model = glm::translate(light_model, glm::vec3(0.0f, 3.0f, 0.0f)); //position is just coped from above here
        //light_model = glm::scale(light_model, glm::vec3(0.3f));
        //light_cube_shader.setMat4("u_model", light_model);
        //light_cube_shader.setVec3("u_lightColor", glm::vec3(1.0f));
        //glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion

        #pragma endregion

        //swap buffers and check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    #pragma endregion

    #pragma region termination 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    #pragma endregion

    return 0;
}
