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
    unsigned int VBO, VAO, lightVAO;
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
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
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
        input_manager.scroll_offset = glm::vec2(0.0f, 0.0f); // scroll offset has to be reset each frame since the callback is only called when there is scroll input.  
#pragma endregion

#pragma region matrices
        glm::mat4 camera_view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), camera.aspect_ratio.y / camera.aspect_ratio.x, camera.near_plane, camera.far_plane);
        glm::mat4 cube_model(1.0f);
        cube_model = glm::translate(cube_model, glm::vec3(-2.0f, 0, 1.0f));
        cube_model = glm::rotate(cube_model, current_time / 8, glm::vec3(0.2f, 0.5f, 0.9f));
        glm::mat4 light_model(1.0f);
        glm::vec3 light_pos(1.0f, 1.0f, 1.0f);
        light_model = glm::translate(light_model, light_pos);
        glm::mat4 light_rotation_matrix(1.0f);

        light_rotation_matrix = glm::rotate(
            light_rotation_matrix,
            (input_manager.buttons.at("right").held - input_manager.buttons.at("left").held) * move_speed * delta_time,
            glm::vec3(0.0f, 0.0f, 1.0f));
        light_rotation_matrix = glm::rotate(
            light_rotation_matrix,
            (input_manager.buttons.at("up").held - input_manager.buttons.at("down").held) * move_speed * delta_time,
            glm::vec3(0.0f, 1.0f, 0.0f));
        light_direction = glm::vec3(light_rotation_matrix * glm::vec4(light_direction, 1.0f));
        glm::mat4 axes_model(1.0f);

        
#pragma endregion

#pragma region draw calls
        //draw calls
        //----------
        //make background black and clear the buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //cube drawing
        //------------
        standard_shader.use();
        standard_shader.setMat4("u_model", cube_model);
        standard_shader.setMat4("u_view", camera_view);
        standard_shader.setMat4("u_projection", projection);
        standard_shader.setVec3("u_cameraPos", camera.get_position());

        standard_shader.setInt("material.diffuse_map", 0);
        standard_shader.setInt("material.specular_map", 1);
        standard_shader.setInt("material.emission_map", 2);
        standard_shader.setFloat("material.shininess", 32.0f);
        standard_shader.setVec3("light.ambient", glm::vec3(0.1f));
        standard_shader.setVec3("light.diffuse", glm::vec3(1.0f));
        standard_shader.setVec3("light.specular", glm::vec3(1.0f));
        standard_shader.setVec3("light.direction", light_direction);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //draw a couple more
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            standard_shader.setMat4("u_model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //axis drawing
        //------------
        axes_shader.use();
        axes_shader.setMat4("u_model", cube_model);
        axes_shader.setMat4("u_view", camera_view);
        axes_shader.setMat4("u_projection", projection);
        axes_shader.setFloat("scale", 10.0f);
        glBindVertexArray(axesVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axesEBO);
        glLineWidth(2.0f);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

        axes_shader.setMat4("u_model", glm::mat4(1.0f));
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

        //draw a couple more
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            axes_shader.setMat4("u_model", model);
            glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        }

        //light_cube_shader.use();
        //light_cube_shader.setMat4("u_model", light_model);
        //light_cube_shader.setMat4("u_view", camera_view);
        //light_cube_shader.setMat4("u_projection", projection);
        //glBindVertexArray(lightVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //light_cube_shader.unuse();
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
