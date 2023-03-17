#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#define MOUSE Control::MouseControl

 //(RUS) Класс для управления с  мыши, отслеживание состояний кнопок и позиции мыши
//(ENG) Class for mouse control, tracking button states and mouse position

namespace Control {
	class MouseControl {
	public:
		static void updatePositionAndScroll();

		static void setWindow(GLFWwindow* pWindow);
		static void setWindowSize(const glm::vec2& windowSize);
		static void setScroll(GLFWwindow* окно, double x, double y);
		static void setButton(GLFWwindow* pWindow, int button, int action, int mods);

		static bool ifPressed(const int& key);
		static bool ifReleased(const int& key);
		static bool ifClamped(const int& key);

		static bool ifInArea(glm::vec4 area);

		static glm::vec2 getScroll();
		static glm::vec2 getPosition();

	private:
		MouseControl();

		enum E_BUTTON_ACTION : int8_t {
			NOT_CLAMPED,
			PRESSED,
			CLAMPED,
			RELEASED
		};
		static std::array<E_BUTTON_ACTION, 8> m_keys;
		static GLFWwindow* m_PWindow;
		static glm::vec2 m_windowSize;
		static glm::vec2 m_mousePosition;
		static glm::vec2 m_scroll;
	};
}