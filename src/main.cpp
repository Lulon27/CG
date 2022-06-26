#include <iostream>
#include <tuple>

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

static cg::Scene scene;

static std::shared_ptr<cg::Object> origin;

static std::shared_ptr<cg::Object> sphere;
static std::shared_ptr<cg::Object> planet;
static std::shared_ptr<cg::Object> moon1;
static std::shared_ptr<cg::Object> moon2;
static std::shared_ptr<cg::Object> moonsRotationAnchor;

static std::shared_ptr<cg::Object> axisSun;
static std::shared_ptr<cg::Object> axisPlanet;

static std::shared_ptr<cg::Object> normalsSphere;
static std::shared_ptr<cg::Object> normalsPlanet;
static std::shared_ptr<cg::Object> normalsMoon1;
static std::shared_ptr<cg::Object> normalsMoon2;

static cg::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);

static float rotationSpeed = 0.2f;
static float planetSpeedMod = 1.0f;
static bool planetStopped = false;
static float camDistance = 10.0f;
static const char* shaderSwitch[] = {"default", "phong", "gouraud"};
static const unsigned int shaderSwitchAmount = 3;
static unsigned int currentShaderIndex = 0;

static std::tuple<std::shared_ptr<cg::Object>, std::shared_ptr<cg::Object>> createSphereObj(uint8_t sd, float r, const glm::vec3& c, const std::string& shader, const std::string& dbgName = "")
{
    cg::MeshData mesh;
    auto obj = std::make_shared<cg::Object>(dbgName);

    cg::GeometryUtil::generateSphereModel(&mesh, sd, r, c);
    obj->setMesh(mesh);
    obj->setShader(cg::ShaderManager::getShader(shader));
    obj->setColor(c);

    cg::MeshData meshNormalObj;
    cg::GeometryUtil::generateNormalDisplayObj(&meshNormalObj, &mesh);


    auto normalsObj = std::make_shared<cg::Object>(dbgName);
    normalsObj->setMesh(meshNormalObj);
    normalsObj->setShader(cg::ShaderManager::getShader("default"));

    return { obj, normalsObj };
}

static std::shared_ptr<cg::Object> createLineObj(float len, const glm::vec3& c, const std::string& shader, const std::string& dbgName = "")
{
    cg::MeshData mesh;
    auto obj = std::make_shared<cg::Object>(dbgName);

    cg::GeometryUtil::generateLineModel(&mesh, len, { 0.0f, 1.0f, 0.0f }, c);
    obj->setMesh(mesh);
    obj->setShader(cg::ShaderManager::getShader(shader));

    return obj;
}

/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool createScene()
{
    if(!cg::ShaderManager::loadShader("default",
    {
        { "shader/simple.vert", cg::GLSLShader::GLSLShaderType::VERTEX },
        { "shader/simple.frag", cg::GLSLShader::GLSLShaderType::FRAGMENT }
    })) return false;

    if (!cg::ShaderManager::loadShader("shaded",
    {
        { "shader/shaded.vert", cg::GLSLShader::GLSLShaderType::VERTEX },
        { "shader/shaded.frag", cg::GLSLShader::GLSLShaderType::FRAGMENT }
    })) return false;

    if (!cg::ShaderManager::loadShader("phong",
    {
        { "shader/shadedPhong.vert", cg::GLSLShader::GLSLShaderType::VERTEX },
        { "shader/shadedPhong.frag", cg::GLSLShader::GLSLShaderType::FRAGMENT }
    })) return false;

    if (!cg::ShaderManager::loadShader("gouraud",
    {
        { "shader/shadedGouraud.vert", cg::GLSLShader::GLSLShaderType::VERTEX },
        { "shader/shadedGouraud.frag", cg::GLSLShader::GLSLShaderType::FRAGMENT }
    })) return false;

    // Origin symbol
    cg::MeshData mesh;
    cg::GeometryUtil::generateOriginModel(&mesh);

    origin = std::make_shared<cg::Object>("Origin");
    origin->setMesh(mesh);
    origin->setShader(cg::ShaderManager::getShader("default"));


    // Sphere model
    std::tie(sphere, normalsSphere) = createSphereObj(12, 0.75f, {1.0f, 1.0f, 0.0f}, "phong", "Sun");


    // Planet model
    std::tie(planet, normalsPlanet) = createSphereObj(8, 0.4f, { 0.8f, 0.2f, 0.2f }, "phong", "Planet");
    planet->position.x = 2.5f;


    // Moons
    std::tie(moon1, normalsMoon1) = createSphereObj(6, 0.25f, { 0.2f, 0.2f, 0.8f }, "phong", "Moon 1");
    moon1->position.y = 1.0f;

    std::tie(moon2, normalsMoon2) = createSphereObj(6, 0.25f, { 0.2f, 0.2f, 0.8f }, "phong", "Moon 2");
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

static void toggleNormalsAll()
{
    static bool normalsOn = false;

    normalsOn = !normalsOn;

    if (normalsOn)
    {
        sphere->addChild(normalsSphere);
        planet->addChild(normalsPlanet);
        moon1->addChild(normalsMoon1);
        moon2->addChild(normalsMoon2);
    }
    else
    {
        sphere->removeChild(normalsSphere);
        planet->removeChild(normalsPlanet);
        moon1->removeChild(normalsMoon1);
        moon2->removeChild(normalsMoon2);
    }
}

static void toggleWireframe()
{
    static bool wireframe = false;

    wireframe = !wireframe;

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

}

static void switchNextShader()
{
    currentShaderIndex = (currentShaderIndex + 1) % shaderSwitchAmount;

    sphere->setShader(cg::ShaderManager::getShader(shaderSwitch[currentShaderIndex]));
    planet->setShader(cg::ShaderManager::getShader(shaderSwitch[currentShaderIndex]));
    moon1->setShader(cg::ShaderManager::getShader(shaderSwitch[currentShaderIndex]));
    moon2->setShader(cg::ShaderManager::getShader(shaderSwitch[currentShaderIndex]));
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
    case 'n': toggleNormalsAll(); break;
    case 'z': toggleWireframe(); break;
    case 'l': scene.setUseViewLight(!scene.getUseViewLight()); break;
    case 'h': switchNextShader(); break;
    }
}

template <typename T>
static void inputSlideVal(T* var, const T& min, const T& max, const T& step, int keyMin, int keyMax)
{
    *var += window.isKeyDown(keyMax) ? step : (window.isKeyDown(keyMin) ? -step : 0.0f);
    *var = glm::clamp(*var, min, max);
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
    inputSlideVal(&rotationSpeed, 0.0f, 2.0f, 0.002f, GLFW_KEY_D, GLFW_KEY_F);

    // Smooth rotation of solar system
    inputSlideVal(&sphere->rotation.z, 0.0f, 360.0f, 0.2f, GLFW_KEY_W, GLFW_KEY_Q);

    // Smooth planet up/down movement
    inputSlideVal(&planet->position.y, -10.0f, 10.0f, 0.015f, GLFW_KEY_I, GLFW_KEY_U);

    // Smooth camera zoom
    inputSlideVal(&camDistance, 3.5f, 15.0f, 0.05f, GLFW_KEY_A, GLFW_KEY_S);

    auto camPos = scene.getCamera().getPosition();
    camPos /= glm::length(camPos);
    camPos *= camDistance;
    scene.getCamera().setPosition(camPos);
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
