#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // Test key input for changing state
    else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        // change state
        // Renderer's toolState.toolState.UpdateCurrentState(ToolStateEnum::...);
        //Renderer::toolState.UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION);
    }
}

void LightmapBaker::Renderer::Renderer::Initialize()
{
    GLFWInitialize();
    ImGuiInitialize();
}

void LightmapBaker::Renderer::Renderer::Update()
{
    GLFWUpdate();
    ImGuiUpdate();
}

void LightmapBaker::Renderer::Renderer::Render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLFWRender();
    ImGuiRender();

    glfwSwapBuffers(glfwWindow);
}

bool LightmapBaker::Renderer::Renderer::WindowShouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void LightmapBaker::Renderer::Renderer::Exit()
{
    GLFWExit();
    ImGuiExit();
}

void LightmapBaker::Renderer::Renderer::GLFWInitialize()
{
    glfwInit();
    glfwWindow = glfwCreateWindow(screenWidth, screenHeight, "Lightmap Baker", 0, 0);

    quadricObj = gluNewQuadric();

    glfwMakeContextCurrent(glfwWindow);

    glfwSetKeyCallback(glfwWindow, KeyCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

void LightmapBaker::Renderer::Renderer::GLFWUpdate()
{
    glfwPollEvents();
}

void LightmapBaker::Renderer::Renderer::GLFWRender()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect_ratio = ((float)screenWidth) / screenHeight;
    gluPerspective(80, (1.f / aspect_ratio), 0.0f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    static float angle = 0.0f;
    angle += 0.5f;
    cameraAngleX += 0.5f;

    //glRotatef(angle, 0, 0, 1);

    float camX = cameraDistance * -sinf(cameraAngleX * (M_PI / 180)) * cosf((cameraAngleY) * (M_PI / 180));
    float camY = cameraDistance * -sinf((cameraAngleY) * (M_PI / 180));
    float camZ = -cameraDistance * cosf((cameraAngleX) * (M_PI / 180)) * cosf((cameraAngleY) * (M_PI / 180));
    gluLookAt(camX, camY, camZ,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glColor3f(1, 1, 0);
    gluQuadricDrawStyle(quadricObj, GLU_LINE);
    gluCylinder(quadricObj, 10, 10, 30, 10, 10);
}

void LightmapBaker::Renderer::Renderer::GLFWExit()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void LightmapBaker::Renderer::Renderer::ImGuiInitialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    toolState.UpdateCurrentState(ToolStateEnum::AFTER_LIGHTMAP_BAKE);
}

void LightmapBaker::Renderer::Renderer::ImGuiUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    toolState.RenderCurrentUI();
}

void LightmapBaker::Renderer::Renderer::ImGuiRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LightmapBaker::Renderer::Renderer::ImGuiExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
