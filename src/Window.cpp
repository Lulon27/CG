#include "CG/Window.h"

#include <glad/glad.h>

namespace cg
{
	Window::Window(unsigned int width, unsigned int height)
	{
        if (!glfwInit())
        {
            m_error = true;
            return;
        }

        //We only want OpenGL 3.2+
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        m_windowHandle = glfwCreateWindow(width, height, "Title", nullptr, nullptr);
        if (!m_windowHandle)
        {
            glfwTerminate();
            m_error = true;
            return;
        }

        glfwMakeContextCurrent(m_windowHandle);
        if (!gladLoadGL())
        {
            glfwTerminate();
            m_error = true;
            return;
        }

        glfwSwapInterval(1);
        glfwSetWindowUserPointer(m_windowHandle, this);

        glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow* window, int width, int height)
        {
            Window* myWind = (Window*)glfwGetWindowUserPointer(window);
            if (myWind->onWindowResized)
            {
                myWind->onWindowResized(width, height);
            }
        });

        glfwSetCharCallback(m_windowHandle, [](GLFWwindow* window, unsigned int codepoint)
        {
            Window* myWind = (Window*)glfwGetWindowUserPointer(window);
            if (myWind->onCharTyped)
            {
                myWind->onCharTyped(codepoint);
            }
        });
	}

	Window::~Window()
	{
        if (!m_error)
        {
            glfwTerminate();
        }
	}

    bool Window::getShouldClose() const
    {
        return glfwWindowShouldClose(m_windowHandle);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    unsigned int Window::getWidth() const
    {
        int width, height;
        glfwGetWindowSize(m_windowHandle, &width, &height);
        return width;
    }
    unsigned int Window::getHeight() const
    {
        glfwSwapBuffers(m_windowHandle);
        int width, height;
        glfwGetWindowSize(m_windowHandle, &width, &height);
        return height;
    }

    bool Window::isKeyDown(int key) const
    {
        int state = glfwGetKey(m_windowHandle, key);
        return state == GLFW_PRESS;
    }
}