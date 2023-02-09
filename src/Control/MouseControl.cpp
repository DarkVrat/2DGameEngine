#include "MouseControl.h"

std::array<Control::MouseControl::E_BUTTON_ACTION, 8> Control::MouseControl::m_keys;
GLFWwindow* Control::MouseControl::m_PWindow;
double Control::MouseControl::m_height;
glm::vec2 Control::MouseControl::m_mousePosition;
glm::vec2 Control::MouseControl::m_scroll;

namespace Control {
	 //(RUS) ���������� ������� ����, � ��������
	//(ENG) updating the position of the mouse, and the wheel
	void MouseControl::updatePositionAndScroll(){
		double x, y;
		glfwGetCursorPos(m_PWindow, &x, &y);
		m_mousePosition.x = x;
		m_mousePosition.y = m_height - y;
		m_scroll.x = 0;
		m_scroll.y = 0;
	}

	 //(RUS) ��������� GFLWwindow � ������ ����
	//(ENG) setting GFLWwindow and window height
	void MouseControl::setWindow(GLFWwindow* pWindow) { m_PWindow = pWindow; }
	void MouseControl::setHeight(double height) { m_height = height - 1; }

	 //(RUS) ��������� ������ ������� ������
	//(ENG) setting the event of the pressed button
	void MouseControl::setButton(GLFWwindow* pWindow, int button, int action, int mods){
		if (action == GLFW_REPEAT) { 
			m_keys[button] = E_BUTTON_ACTION::Clamped; 
			return; 
		}
		if (action == GLFW_PRESS) {
			m_keys[button] = E_BUTTON_ACTION::Pressed;
			return;
		}
		if (action == GLFW_RELEASE) {
			m_keys[button] = E_BUTTON_ACTION::Released;
			return;
		}
	}

	 //(RUS) �������� ������ ������
	//(ENG) button event check
	bool MouseControl::ifPressed(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Pressed) {
			m_keys[key] = E_BUTTON_ACTION::Clamped;
			return true;
		}
		return false;
	}
	bool MouseControl::ifClamped(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Pressed || m_keys[key] == E_BUTTON_ACTION::Clamped)
			return true;
		return false;
	}
	bool MouseControl::ifReleased(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Released) {
			m_keys[key] = E_BUTTON_ACTION::NotClamped;
			return true;
		}
		return false;
	}
	
	 //(RUS) ��������� � ��������� ��������� ������, ��������� ��������� ����
	//(ENG) Setting and getting the scroll wheel, getting the mouse position
	void MouseControl::setScroll(GLFWwindow* ����, double x, double y) { m_scroll = glm::vec2(x,y); }
	glm::vec2 MouseControl::getScroll() { return m_scroll; }
	glm::vec2 MouseControl::getPosition() {	return m_mousePosition; }

	 //(RUS) ������������� � ���������� �������� �����������
	//(ENG) Initialization and padding with zero parameters
	MouseControl::MouseControl() {
		m_PWindow = nullptr;
		m_height = 0;
		m_mousePosition = glm::vec2(0, 0);
		m_scroll = glm::vec2(0, 0);
		m_keys.fill(E_BUTTON_ACTION::NotClamped);
	}
}