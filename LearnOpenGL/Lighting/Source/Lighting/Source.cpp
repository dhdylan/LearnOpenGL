#define DIFFUSE_UNIT 0
#define SPECULAR_UNIT 1
#define EMISSION_UNIT 2

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
#include <dearimgui/imgui_impl_glfw.h>
#include <dearimgui/imgui_impl_opengl3.h>
#include <dearimgui/imgui.h>
#include <dearimgui/imconfig.h>
#include <material.h>
#include <light.h>
#include <world_object.h>
#include <geometry.h>
#include <world.h>
#include <texture.h>
#include <filesystem>

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
        {"down", engine::Button(GLFW_KEY_DOWN)},
        {"tab", engine::Button(GLFW_KEY_TAB)}
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
    input_manager.set_cursor_mode(GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    #pragma endregion

    #pragma region set up ImGui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    #pragma endregion
 
    #pragma region set up glfw callbacks
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, input_manager.static_key_callback);
    glfwSetCursorPosCallback(window, input_manager.static_mouse_callback);
    glfwSetScrollCallback(window, input_manager.static_scroll_callback);
    #pragma endregion
    
    #pragma region set up shader
    engine::Shader standard_shader("./Source/Lighting/standard_lit.vert", "./Source/Lighting/standard_lit.frag");
    engine::Shader light_cube_shader("./Source/Lighting/light.vert", "./Source/Lighting/light.frag");
    engine::Shader axes_shader("./Source/Lighting/axes.vert", "./Source/Lighting/axes.frag");
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
    engine::World world;
    engine::Point_Light point_light(light_cube_shader);
    point_light.vao = light_cube_VAO;
    world.dir_light.color = glm::vec4(0.1f);
    world.dir_light.ambient = 0.25f;
    point_light.color = glm::vec4(1.0f, 0.3f, 0.15f, 1.0f);
    point_light.position = glm::vec4(2.0f, 1.0f, 0.0f, 1.0);
    world.point_lights.push_back(point_light);
    point_light.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    point_light.position = glm::vec4(-2.5f, -1.8f, 0.8f, 1.0f);
    world.point_lights.push_back(point_light);
    engine::Texture crate_diffuse = engine::Texture((std::filesystem::current_path().string() + "\\container2.png").c_str());
    engine::Texture crate_specular = engine::Texture((std::filesystem::current_path().string() + "\\container2_specular.png").c_str());

    float light1[] = {
        world.point_lights[0].color.x,
        world.point_lights[0].color.y,
        world.point_lights[0].color.z,
        world.point_lights[0].color.w,
    };
    float light2[] = {
        world.point_lights[1].color.x,
        world.point_lights[1].color.y,
        world.point_lights[1].color.z,
        world.point_lights[1].color.w,
    };
    float light1_pos[] = { world.point_lights[0].position.x, world.point_lights[0].position.y, world.point_lights[0].position.z };
    float light2_pos[] = { world.point_lights[1].position.x, world.point_lights[1].position.y, world.point_lights[1].position.z };


    //make a bunch of cubes
    for (int i = 0; i < 10; i++)
    {
        world.world_objects.push_back(engine::Cube_Object(standard_shader, VAO, crate_diffuse.texture_id, crate_specular.texture_id));
        world.world_objects[i].position = glm::vec3(
            10 * ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 0.5f),
            10 * ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 0.5f),
            10 * ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 0.5f));
    }

    //dear ImGui bool
    bool my_tool_active = true;
    
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
        //turn the cursor on and off (for purposes of fucking with UI)
        if (input_manager.buttons.at("tab").down)
        {
            bool cursor_state = input_manager.getptr()->get_mouse_enabled_status();

            if (cursor_state == true)
            {
                input_manager.getptr()->set_cursor_mode(GLFW_CURSOR_DISABLED);
            }
            else
            {
                input_manager.getptr()->set_cursor_mode(GLFW_CURSOR_NORMAL);
            }
        }

        //if cursor is on, dont let the camera move
        if (!input_manager.getptr()->get_mouse_enabled_status())
        {
            //movement input
            glm::vec3 delta_move(0.0f, 0.0f, 0.0f);
            //forward
            if (input_manager.buttons.at("w").held)
            {
                delta_move += move_speed * (float)delta_time * world.user_camera->local_forward;
            }
            //back
            if (input_manager.buttons.at("s").held)
            {
                delta_move -= move_speed * (float)delta_time * world.user_camera->local_forward;
            }
            //left
            if (input_manager.buttons.at("a").held)
            {
                delta_move += move_speed * (float)delta_time * world.user_camera->local_right;
            }
            //right
            if (input_manager.buttons.at("d").held)
            {
                delta_move -= move_speed * (float)delta_time * world.user_camera->local_right;
            }
            //camera up
            if (input_manager.buttons.at("e").held)
            {
                delta_move += move_speed * delta_time * world.user_camera->local_up;
            }
            //camera down
            if (input_manager.buttons.at("q").held)
            {
                delta_move -= move_speed * delta_time * world.user_camera->local_up;
            }
            world.user_camera->position += delta_move;

            //first person look input and calcs
            glm::vec2 new_rotation(0.0f);
            glm::vec2& mouse_offset = input_manager.mouse_offset;
            new_rotation.y = world.user_camera->rotation.y - (mouse_offset.x * mouse_sensitivity);
            new_rotation.x = world.user_camera->rotation.x - (mouse_offset.y * mouse_sensitivity);
            if (new_rotation.x > 89.0f)
                new_rotation.x = 89.0f;
            if (new_rotation.x < -89.0f)
                new_rotation.x = -89.0f;
            world.user_camera->set_rotation(new_rotation);
            input_manager.mouse_offset = glm::vec2(0.0f, 0.0f); // mouse offset has to be reset each frame since the callback is only called when there is mouse input.

            //zoom input and calcs
            //--------------------
            world.user_camera->fov -= input_manager.scroll_offset.y * zoom_speed;
            if (world.user_camera->fov < world.user_camera->min_fov)
            {
                world.user_camera->fov = world.user_camera->min_fov;
            }
            if (world.user_camera->fov > world.user_camera->max_fov)
            {
                world.user_camera->fov = world.user_camera->max_fov;
            }
            input_manager.scroll_offset = glm::vec2(0.0f, 0.0f); // scroll offset has to be reset each frame since the callback is only called when there is scrollinput.  
        }
        #pragma endregion

        #pragma region matrices
        glm::mat4 camera_view = world.user_camera->get_view_matrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(world.user_camera->fov),
            world.user_camera->aspect_ratio.y / world.user_camera->aspect_ratio.x,
            world.user_camera->near_plane,
            world.user_camera->far_plane
        );
        #pragma endregion

        #pragma region drawing
        //draw calls
        
        //make background black and clear the buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        world.point_lights[0].color = glm::vec4(light1[0], light1[1], light1[2], light1[3]);
        world.point_lights[1].color = glm::vec4(light2[0], light2[1], light2[2], light2[3]);

        world.point_lights[0].position = glm::vec3(light1_pos[0], light1_pos[1], light1_pos[2]);
        world.point_lights[1].position = glm::vec3(light2_pos[0], light2_pos[1], light2_pos[2]);

        world.draw_world();

        #pragma region drawing ImGui
        // render your GUI
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        //Edit a color (stored as ~4 floats)
        ImGui::ColorEdit4("Color", light1);
        ImGui::ColorEdit4("Color2", light2);
        ImGui::ShowDemoWindow();
        ImGui::End();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    #pragma endregion

    return 0;
}
