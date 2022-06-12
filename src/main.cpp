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
static const int WINDOW_WIDTH = 640;
// Standard window height
static const int WINDOW_HEIGHT = 480;

static float zNear = 0.1f;
static float zFar = 100.0f;

static cg::ShaderManager shaderManager;

static cg::Scene scene;

static std::shared_ptr<cg::Object> origin;

static std::shared_ptr<cg::Object> sphere;
static std::shared_ptr<cg::Object> planet;
static std::shared_ptr<cg::Object> moon1;
static std::shared_ptr<cg::Object> moon2;
static std::shared_ptr<cg::Object> moonsRotationAnchor;

static std::shared_ptr<cg::Object> axisSun;
static std::shared_ptr<cg::Object> axisPlanet;

static cg::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);

static float rotationSpeed = 0.2f;
static float planetSpeedMod = 1.0f;
static bool planetStopped = false;

static std::shared_ptr<cg::Object> createSphereObj(uint8_t sd, float r, const glm::vec3& c, const std::string& shader, const std::string& dbgName = "")
{
    cg::MeshData mesh;
    auto obj = std::make_shared<cg::Object>(dbgName);

    cg::GeometryUtil::generateSphereModel(&mesh, sd, r, c);
    obj->setMesh(mesh);
    obj->setShader(shaderManager.getShader(shader));

    return obj;
}

static std::shared_ptr<cg::Object> createLineObj(float len, const glm::vec3& c, const std::string& shader, const std::string& dbgName = "")
{
    cg::MeshData mesh;
    auto obj = std::make_shared<cg::Object>(dbgName);

    cg::GeometryUtil::generateLineModel(&mesh, len, { 0.0f, 1.0f, 0.0f }, c);
    obj->setMesh(mesh);
    obj->setShader(shaderManager.getShader(shader));

    return obj;
}

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
    sphere = createSphereObj(12, 0.75f, { 1.0f, 1.0f, 0.0f }, "default", "Sun");


    // Planet model
    planet = createSphereObj(8, 0.4f, { 0.8f, 0.2f, 0.2f }, "default", "Planet");
    planet->position.x = 2.5f;


    // Moons
    moon1 = createSphereObj(6, 0.25f, { 0.2f, 0.2f, 0.8f }, "default", "Moon 1");
    moon1->position.y = 1.0f;

    moon2 = createSphereObj(6, 0.25f, { 0.2f, 0.2f, 0.8f }, "default", "Moon 2");
    moon2->position.y = -1.0f;

    moonsRotationAnchor = std::make_shared<cg::Object>();


    // Sun Axis model
    axisSun = createLineObj(100.0f, { 1.0f, 0.0f, 0.0f }, "default", "Sun Axis");

    // Planet Axis model
    axisPlanet = createLineObj(100.0f, { 1.0f, 1.0f, 0.0f }, "default", "Planet Axis");


    // Add to scene, do not add child objects to scene!
    scene.addObject(origin);
    scene.addObject(sphere);

    // Make hierarchy
    sphere->addChild(planet);
    planet->addChild(moonsRotationAnchor);
    moonsRotationAnchor->addChild(moon1);
    moonsRotationAnchor->addChild(moon2);

    sphere->addChild(axisSun);
    planet->addChild(axisPlanet);

    scene.getCamera().setPosition(glm::vec3(0.0f, 1.0f, 6.0f));

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
    switch (keycode)
    {
    case 'g': planetStopped = !planetStopped; break;
    }
}

/**
 * Called every frame in main loop.
 * Contains the logic to update every object.
 */
static void updateLogic()
{
    planet->rotateAroundOrigin(rotationSpeed * planetSpeedMod, { 0, 1, 0 });
    planet->rotation.y += rotationSpeed * planetSpeedMod;

    moon1->rotateAroundOrigin(rotationSpeed * 2.0f, { 1, 0, 0 });
    moon2->rotateAroundOrigin(rotationSpeed * 2.0f, { 1, 0, 0 });

    // Calculate angle of vector between planet and sun
    glm::vec3& pos = planet->position;
    auto angle = std::atan2(glm::sqrt(pos.z * pos.z + pos.x * pos.x), pos.y) - glm::pi<float>() * 0.5f;

    moonsRotationAnchor->rotation.z = -glm::degrees(angle);

    // Smooth planet acceleration / decceleration
    planetSpeedMod += planetStopped ? -0.01f : 0.01f;
    planetSpeedMod = glm::clamp(planetSpeedMod, 0.0f, 1.0f);

    // Smooth acceleration of planet and moons rotation
    rotationSpeed += window.isKeyDown(GLFW_KEY_F) ? 0.002f : (window.isKeyDown(GLFW_KEY_D) ? -0.002f : 0.0f);
    rotationSpeed = glm::clamp(rotationSpeed, 0.0f, 2.0f);

    // Smooth rotation of solar system
    sphere->rotation.z += window.isKeyDown(GLFW_KEY_Q) ? 0.2f : (window.isKeyDown(GLFW_KEY_W) ? -0.2f : 0.0f);
    sphere->rotation.z = glm::clamp(sphere->rotation.z, 0.0f, 360.0f);

    // Smooth planet up/down movement
    planet->position.y += window.isKeyDown(GLFW_KEY_U) ? 0.015f : (window.isKeyDown(GLFW_KEY_I) ? -0.015f : 0.0f);
    planet->position.y = glm::clamp(planet->position.y, -10.0f, 10.0f);
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
        glfwPollEvents();
        
        updateLogic();

        scene.renderScene();
        window.swapBuffers();
    }

    return 0;
}
