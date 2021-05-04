#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

#include "utils.hpp"
#include "shader.h"
#include "model.h"
#include "data.h"
#include "camera.h"
#include "binding.h"
#include "global_state.h"
#include "loader.h"

namespace Engine
{
// global states
std::shared_ptr<Global::State> globalState;

void onMouseButton(GLFWwindow* window, int button, int action, int mod)
{
    Global::Mouse& mouse = globalState->GetMouse();
    mouse.action = action;
    mouse.button = button;
    mouse.modifier = mod;
}

void onMouseMove(GLFWwindow* window, double x, double y)
{
    Global::Mouse& mouse = globalState->GetMouse();
    mouse.dx = x - mouse.xPos;
    mouse.dy = y - mouse.yPos;
    mouse.xPos = x;
    mouse.yPos = y;
}

void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    Global::Mouse& mouse = globalState->GetMouse();
    mouse.wheel.enabled = true;
    mouse.wheel.dy = yoffset;
}

int main(int argc, char* argv[])
{
    // Init GLFW
    glfwInit();

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 800;
    const int height = 600;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    GLFWwindow* window = glfwCreateWindow(width, height, "stunning-succotash", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, NULL);

    // GLFW input callbacks
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);
    glfwSetScrollCallback(window, onMouseScroll);

    // Load all OpenGL functions using the glfw loader function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // shaders
    const std::string filePath(argv[0]);
    const std::string absPath = extractPath(filePath);
    const std::string absPathVS = absPath + "shaders/triangle.vert";
    const std::string absPathFS = absPath + "shaders/triangle.frag";

    std::vector<GL::ShaderProgram> shaders;
    shaders.push_back(GL::ShaderProgram(absPathVS, GL_VERTEX_SHADER));
    shaders.push_back(GL::ShaderProgram(absPathFS, GL_FRAGMENT_SHADER));

    GL::ProgramPipeline programPipeline(shaders);

    if (glGetError() != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error " << std::endl;
        return EXIT_FAILURE;
    }

    // load our image
    std::shared_ptr<Loader::Image> image(new Loader::Image("../../odf.nii.gz"));

    // create our model
    GL::Model model(image);

    // initalize global state for tracking mouse/keyboard inputs
    globalState.reset(new Global::State());

    // create our camera
    GL::SphericalCoordinates position;
    position.r = 10.0;
    position.theta = M_PI / 2.0;
    position.phi = 0.0;
    GL::Camera camera(position,
                      glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::radians(60.0f),
                      aspectRatio,
                      0.5f,
                      500.0f,
                      globalState);

    // OpenGL parameters
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Handle events
        glfwPollEvents();

        // Draw scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        programPipeline.Bind();
        camera.Refresh();
        model.Draw();

        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return EXIT_SUCCESS;
}
}

int main(int argc, char** argv)
{
    return Engine::main(argc, argv);
}