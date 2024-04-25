#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"

// callback function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// other function prototypes
void processInput(GLFWwindow* window);

// window size parameters
const float windowWidth = 1080.0f;
const float windowHeight = 720.0f;

// view parameters
const float fov = 45.0f;
const float near = 0.1f;
const float far = 100.0f;

// rotation parameters
glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
float rotSpeed = 15.0f;

// variables for calculating the duration of the current frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// create the camera object
const glm::vec3 cameraPos = glm::vec3(-1.0f, 2.0f, 10.0f);
Camera camera(cameraPos);


int main()
{
    // initialize GLFW, set version to 4.6, set to core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Maze", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // set the viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // register callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // model
    Model woodenTable = Model("res/models/wooden_table/Wooden Table.dae");

    // initialize the view and projection matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f)); // shrink down the table 10x
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(fov, windowWidth / windowHeight, near, far);

    // shaders
    Shader basicShader = Shader("res/shaders/vertex/basic.vert", "res/shaders/fragment/basic.frag");
    basicShader.use();
    basicShader.setMat4("u_model", model);
    basicShader.setMat4("u_projection", projection);
    basicShader.setInt("u_material.texture_albedo", 0);
    basicShader.addTexture("res/models/wooden_table/Albedo.jpg");

    // main loop
    while (!glfwWindowShouldClose(window)) 
    {
        // calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // check keyboard input
        processInput(window);

        // set the uniforms
        basicShader.use();
        view = camera.getViewMatrix();
        basicShader.setMat4("u_view", view);
        // model rotation
        if (glm::length(axis) != 0) { // if axis == (0, 0, 0), remain still
            model = glm::rotate(model, glm::radians(deltaTime) * rotSpeed, axis);
            basicShader.setMat4("u_model", model);
        }

        // rendering commands
        // clear the screen
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // start new frame in ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // draw the model
        woodenTable.Draw(basicShader);

        // configure the Imgui window
        ImGui::Begin("Window");
        ImGui::Text("Hello there, adventurer!");
        ImGui::SliderFloat("Rotation speed", &rotSpeed, 0.0f, 60.0f);
        ImGui::SliderFloat("X axis", &axis.x, 0.0f, 1.0f);
        ImGui::SliderFloat("Y axis", &axis.y, 0.0f, 1.0f);
        ImGui::SliderFloat("Z axis", &axis.z, 0.0f, 1.0f);
        ImGui::End();

        // render Imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events, then swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // terminate GLFW
    glfwTerminate();
	return 0;
}

// callback function for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scrollInput(window, xoffset);
}

// function for checking keyboard inputs
void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}