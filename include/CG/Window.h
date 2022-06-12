#pragma once

#include <GLFW/glfw3.h>

namespace cg
{
	class Window
	{
		using WindowResizedCallback = void (*)(unsigned int width, unsigned int height);
		using CharTypedCallback = void (*)(unsigned int codepoint);

	public:
		Window(unsigned int width, unsigned int height);
		~Window();

		bool getError() const { return m_error; }
		GLFWwindow* getHandle() const { return m_windowHandle; }
		bool getShouldClose() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		bool isKeyDown(int key) const;

		void swapBuffers();

		void setWindowResizedCallback(WindowResizedCallback cb) { onWindowResized = cb; }
		void setCharTypedCallback(CharTypedCallback cb) { onCharTyped = cb; }

	private:
		Window(const Window&) = delete;
		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

	private:
		GLFWwindow* m_windowHandle = nullptr;
		bool m_error = false;

		// Window callbacks
		WindowResizedCallback onWindowResized = nullptr;
		CharTypedCallback onCharTyped = nullptr;
	};
}
