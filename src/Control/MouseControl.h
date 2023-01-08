#pragma once
#include<glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <array>

namespace Control {
	class MouseControl {
	public:
		static MouseControl* Get();

		void Update();
		void SetWindow(GLFWwindow* pWindow) { m_PWindow = pWindow; }
		void SetHeight(double height) { m_height = height; }

		void SetKey(const int key, const int action);
		bool IfPressed(const int key);
		bool IfReleased(const int key);
		bool IfClamped(const int key);
		bool IfNotClamped(const int key);


		glm::vec2 GetPosition() { return m_mousePosition; }
	private:
		MouseControl();
		~MouseControl() {};

		GLFWwindow* m_PWindow;
		double m_height;
		glm::vec2 m_mousePosition;

		enum class BUTTON_ACTION {
			NotClamped,
			Pressed,
			Clamped,
			Released
		};
		std::array<BUTTON_ACTION, 8> m_keys;
	};
}