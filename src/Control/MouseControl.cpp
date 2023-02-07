#include "MouseControl.h"

std::array<Control::MouseControl::E_BUTTON_ACTION, 8> Control::MouseControl::m_keys;
GLFWwindow* Control::MouseControl::m_PWindow;
double Control::MouseControl::m_height;
glm::vec2 Control::MouseControl::m_mousePosition;
glm::vec2 Control::MouseControl::m_scroll;

namespace Control {
	void MouseControl::updatePosition(){
		double x, y;
		glfwGetCursorPos(m_PWindow, &x, &y);
		m_mousePosition = glm::vec2(x, m_height -y ); 
	}
	void MouseControl::updateButton() {
		m_scroll = glm::vec2(0, 0);
		for (int i = 0; i < m_keys.size(); i++) {
			if (m_keys[i] == E_BUTTON_ACTION::Pressed)
				m_keys[i] = E_BUTTON_ACTION::Clamped;
			if (m_keys[i] == E_BUTTON_ACTION::Released)
				m_keys[i] = E_BUTTON_ACTION::NotClamped;
		}
	}

	void MouseControl::setWindow(GLFWwindow* pWindow){
		m_PWindow = pWindow;
	}

	void MouseControl::setHeight(double height){
		m_height = height - 1;
	}

	void MouseControl::setButton(GLFWwindow* pWindow, int button, int action, int mods){
		if (button < 0 || button>7) {
			return;
		}
		if (action == GLFW_PRESS) {
			m_keys[button] = E_BUTTON_ACTION::Pressed;
		}
		else {
			m_keys[button] = E_BUTTON_ACTION::Released;
		}
	}

	bool MouseControl::ifPressed(const int key){
		if (m_keys[key] == E_BUTTON_ACTION::Pressed)
			return true;
		return false;
	}
	bool MouseControl::ifReleased(const int key){
		if (m_keys[key] == E_BUTTON_ACTION::Released)
			return true;
		return false;
	}
	bool MouseControl::ifClamped(const int key){
		if (m_keys[key] == E_BUTTON_ACTION::Pressed || m_keys[key] == E_BUTTON_ACTION::Clamped)
			return true;
		return false;
	}

	bool MouseControl::ifNotClamped(const int key){
		if (m_keys[key] == E_BUTTON_ACTION::Released || m_keys[key] == E_BUTTON_ACTION::NotClamped)
			return true;
		return false;
	}

	void MouseControl::setScroll(GLFWwindow* окно, double x, double y){
		m_scroll = glm::vec2(x,y);
	}

	glm::vec2 MouseControl::getScroll(){
		return m_scroll;
	}

	glm::vec2 MouseControl::getPosition(){
		return m_mousePosition;
	}

	MouseControl::MouseControl() {
		m_PWindow = nullptr;
		m_height = 0;
		m_mousePosition = glm::vec2(0, 0);
		m_scroll = glm::vec2(0, 0);
		m_keys.fill(E_BUTTON_ACTION::NotClamped);
	}
}