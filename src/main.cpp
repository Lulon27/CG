#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "CG/GLTools.h"
#include "CG/ShaderManager.h"
#include "CG/Object.h"
#include "CG/Scene.h"
#include "CG/GeometryUtil.h"
#include "CG/Window.h"

// Standard window width
const int WINDOW_WIDTH = 640;
// Standard window height
const int WINDOW_HEIGHT = 480;

float zNear = 0.1f;
float zFar = 100.0f;

static cg::ShaderManager shaderManager;

cg::Scene scene;

std::shared_ptr<cg::Object> origin;

std::shared_ptr<cg::Object> sphere;
std::shared_ptr<cg::Object> planet;
std::shared_ptr<cg::Object> moon1;
std::shared_ptr<cg::Object> moon2;

std::shared_ptr<cg::Object> axisSun;
std::shared_ptr<cg::Object> axisPlanet;

cg::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);

/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool createScene()
{
    bool shaderLoaded = shaderManager.loadShader("default",
    {
        { "shader/simple.vert", cg::GLSLShader::GLSLShaderType::VERTEX },
        { "shader/simple.frag", cg::GLSLShader::GLSLShaderType::FRAGMENT }
    });

    if (!shaderLoaded)
    {
        return false;
    }

    // Origin symbol
    cg::MeshData mesh;
    cg::GeometryUtil::generateOriginModel(&mesh);

    origin = std::make_shared<cg::Object>("Origin");
    origin->setMesh(mesh);
    origin->setShader(shaderManager.getShader("default"));


    // Sphere model
    sphere = std::make_shared<cg::Object>("Sun");
    cg::GeometryUtil::generateSphereModel(&mesh, 12, 0.75f);
    sphere->setMesh(mesh);
    sphere->setShader(shaderManager.getShader("default"));


    // Planet model
    planet = std::make_shared<cg::Object>("Planet");
    cg::GeometryUtil::generateSphereModel(&mesh, 8, 0.4f, { 0.8f, 0.2f, 0.2f });
    planet->setMesh(mesh);
    planet->setShader(shaderManager.getShader("default"));
    planet->position.x = 2.5f;


    // Moons
    moon1 = std::make_shared<cg::Object>("Moon 1");
    cg::GeometryUtil::generateSphereModel(&mesh, 6, 0.25f, { 0.2f, 0.2f, 0.8f });
    moon1->setMesh(mesh);
    moon1->setShader(shaderManager.getShader("default"));
    moon1->position.x = 1.0f;

    moon2 = std::make_shared<cg::Object>("Moon 2");
    cg::GeometryUtil::generateSphereModel(&mesh, 6, 0.25f, { 0.2f, 0.2f, 0.8f });
    moon2->setMesh(mesh);
    moon2->setShader(shaderManager.getShader("default"));
    moon2->position.x = -1.0f;


    // Sun Axis model
    axisSun = std::make_shared<cg::Object>("Sun Axis");
    cg::GeometryUtil::generateLineModel(&mesh, 5.0f, { 0, 1, 0 }, { 1, 0, 0 });
    axisSun->setMesh(mesh);
    axisSun->setShader(shaderManager.getShader("default"));

    // Planet Axis model
    axisPlanet = std::make_shared<cg::Object>("Planet Axis");
    cg::GeometryUtil::generateLineModel(&mesh, 5.0f, { 0, 1, 0 }, { 1, 1, 0 });
    axisPlanet->setMesh(mesh);
    axisPlanet->setShader(shaderManager.getShader("default"));


    // Add to scene, do not add child objects to scene!
    scene.addObject(origin);
    scene.addObject(sphere);

    // Relationships
    sphere->addChild(planet);
    planet->addChild(moon1);
    planet->addChild(moon2);

    sphere->addChild(axisSun);
    planet->addChild(axisPlanet);

    scene.getCamera().setPosition(glm::vec3(0.0f, 1.0f, 4.0f));

    return true;
}

/*
 Resize callback.
 */
void updateViewport(unsigned int width, unsigned int height)
{
    // Division by zero is bad...
    height = height < 1 ? 1 : height;
    glViewport(0, 0, width, height);

    // Construct projection matrix.
    scene.getCamera().calculateProjection(60.0f, (float)width / height, zNear, zFar);
}

/*
 Callback for char input.
 */
void charCallback(unsigned int keycode)
{
    std::cout << "Pressed key \"" << (char)keycode << "\"\n";
}

int main(int argc, char** argv)
{
    if (window.getError())
    {
        std::cerr << "Failed to initialize window\n";
        return -1;
    }

#if _DEBUG
    if (glDebugMessageCallback)
    {
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(cg::glErrorVerboseCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            nullptr,
            true); // get all debug messages
    }
    else
    {
        std::cout << "glDebugMessageCallback not available" << std::endl;
    }
#endif


    bool result = createScene();
    if (!result)
    {
        return -2;
    }

    window.setCharTypedCallback(charCallback);
    window.setWindowResizedCallback(updateViewport);

    updateViewport(window.getWidth(), window.getHeight());

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while (!window.getShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sphere->rotation.y += 0.01f;
        planet->rotation.y += 0.01f;
        sphere->position.y = glm::sin(glfwGetTime());
        sphere->position.x = glm::cos(glfwGetTime());

        glfwPollEvents();
        scene.renderScene();
        window.swapBuffers();
    }

    return 0;
}
