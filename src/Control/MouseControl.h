#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

#define MOUSE Control::MouseControl::Get()

namespace Control {
	class MouseControl {
	public:
		static MouseControl* Get();

		void UpdatePosition();
		void UpdateButton();

		void SetWindow(GLFWwindow* pWindow);
		void SetHeight(double height);
		void SetScroll(glm::vec2 scroll);
		void SetKey(const int key, const int action);

		bool IfPressed(const int key);
		bool IfReleased(const int key);
		bool IfClamped(const int key);
		bool IfNotClamped(const int key);
		glm::vec2 GetScroll();
		glm::vec2 GetPosition();

	private:
		MouseControl();
		~MouseControl() {};

		enum class BUTTON_ACTION {
			NotClamped,
			Pressed,
			Clamped,
			Released
		};
		std::array<BUTTON_ACTION, 8> m_keys;
		GLFWwindow* m_PWindow;
		double m_height;
		glm::vec2 m_mousePosition;
		glm::vec2 m_scroll;
	};
}