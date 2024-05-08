#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "LightCube.h"
#include "WoodenTable.h"
#include "Skybox.h"
#include "PointShadow.h"
#include "DirectionalShadow.h"

// callback function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// other function prototypes
void processInput(GLFWwindow* window, ImGuiIO& io);
void renderUI(WoodenTable& woodenTable);

// window size parameters
const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;

// variables for calculating the duration of the current frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// variables for mouse input
double lastX, lastY;
bool firstMouse = true;

// view parameters
const float NEAR = 0.1f;
const float FAR = 100.0f;

// lighting parameters
std::vector<glm::vec3> lightCubePositions = 
{
    glm::vec3(3.0f, 2.0f, -1.0f),
    glm::vec3(-2.0f, 3.0f, 5.0f),
    glm::vec3(-2.0f, -2.0f, 4.0f)
};
glm::vec3 dirLightDirection = glm::vec3(-5.0f, -5.0f, -5.0f);

// create the camera object
const glm::vec3 cameraPos = glm::vec3(-3.0f, 2.0f, 10.0f);
Camera camera(cameraPos);

int main()
{
    // initialize GLFW, set version to 4.6, set to core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Engine", NULL, NULL);
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
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // register callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
	//glfwSetCursorPosCallback(window, cursor_position_callback);

    // flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	// the skybox object
    Skybox skybox;

    // the objects to be drawn into the scene
    WoodenTable woodenTable(lightCubePositions, cameraPos, dirLightDirection);
    LightCube lightCube(lightCubePositions);

    // the object to render the shadows
	PointShadow pointShadow(lightCubePositions);
    DirectionalShadow directionalShadow(dirLightDirection);

	// set the light space matrices
	woodenTable.setLightSpaceMatrix(directionalShadow.lightSpaceMatrix);

    // initialize the view and projection matrices
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    // main loop
    while (!glfwWindowShouldClose(window)) 
    {
        // calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // check keyboard input
        processInput(window, io);

        // update the model, view and projection matrices
		woodenTable.updateModelMatrix(deltaTime);
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / WINDOW_HEIGHT, NEAR, FAR);

        // rendering commands
		// render to the depth cubemap
        glViewport(0, 0, POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT);
        switch (woodenTable.lightingType)
        {
		    case POINT_LIGHT:
			    for (int idx = 0; idx < lightCubePositions.size(); idx++)
			    {
				    pointShadow.configureShader(idx);
				    woodenTable.drawShadow(pointShadow.getShader());
			    }
			    break;
			case DIRECTIONAL_LIGHT:
				directionalShadow.configureShader();
				woodenTable.drawShadow(directionalShadow.getShader());
                break;
        }
		

        // prepare state for ordinary drawing
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // start new frame in ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // draw the objects
        switch (woodenTable.lightingType)
        {
            case POINT_LIGHT:
                woodenTable.setPointShadowMaps(pointShadow.getDepthCubemaps());
                lightCube.draw(view, projection);
                break;
			case DIRECTIONAL_LIGHT:
				woodenTable.setDirShadowMap(directionalShadow.getDepthMap());
                break;
        }
        woodenTable.draw(view, projection);

		// draw the skybox last, when the depth buffer is full
		skybox.draw(view, projection);

        // configure the Imgui window
		renderUI(woodenTable);

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

void renderUI(WoodenTable& woodenTable) 
{
    ImGui::Begin("Window");
    // set the width of the sliders
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float padding = ImGui::GetStyle().WindowPadding.x;
    float width = (ImGui::GetContentRegionAvail().x - 2 * spacing - 2 * padding) / 6;
    float width3 = 3 * width + 2 * spacing;
    // the header for configuring the model settings
    if (ImGui::TreeNode("Model"))
    {
        ImGui::PushItemWidth(width3);
        ImGui::SliderFloat("Rotation speed", &(woodenTable.rotSpeed), 0.0f, 60.0f);

        ImGui::PushItemWidth(width);
        ImGui::SliderFloat("##x_axis", &(woodenTable.axis.x), 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("##y_axis", &(woodenTable.axis.y), 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("##z_axis", &(woodenTable.axis.z), 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::Text("Rotation axis");

        ImGui::PushItemWidth(width);
        ImGui::SliderFloat("##x_pos", &(woodenTable.position.x), -25.0f, 25.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("##y_pos", &(woodenTable.position.y), -25.0f, 25.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("##z_pos", &(woodenTable.position.z), -25.0f, 25.0f);
        ImGui::SameLine();
        ImGui::Text("Position");

        ImGui::TreePop();
    }
    // the header for configuring the material settings
    if (ImGui::TreeNode("Material")) {
        static int selectedItem = 0;
        const char* items[] = { "Wood", "Metal", "Rock" };

        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            if (ImGui::RadioButton(items[n], selectedItem == n))
            {
                selectedItem = n;
                woodenTable.setMaterial((MaterialType)n);
            }
        }

        ImGui::TreePop();
    }
    // the header for configuring the lighting settings
    if (ImGui::TreeNode("Lighting"))
    {
        // radio button list for selecting the lighting type
        if (ImGui::TreeNode("Type"))
        {
            static int selectedItem = 0;
            const char* items[] = { "Point", "Directional" };

            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                if (ImGui::RadioButton(items[n], selectedItem == n))
                {
                    selectedItem = n;
                    woodenTable.setLightingType((LightingType)n);
                }
            }
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

// callback function for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// callback function for scroll input
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scrollInput(window, -yoffset);
}

// function for checking keyboard inputs and for setting callbacks that conflict with ImGui
void processInput(GLFWwindow* window, ImGuiIO& io) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (!io.WantCaptureMouse)
    {
        cursor_position_callback(window, io.MousePos.x, io.MousePos.y);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // Y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

	    // Use xoffset and yoffset to rotate the camera the opposite direction of the cursor movement
	    camera.mouseInput(window, -xoffset, -yoffset);
	}
    else 
    {
		firstMouse = true;
    }
}