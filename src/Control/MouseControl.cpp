#include "MouseControl.h"

static Control::MouseControl* mouseControl = nullptr;

namespace Control {
	MouseControl* MouseControl::Get(){
		if (mouseControl == nullptr)
			mouseControl = new MouseControl();
		return mouseControl;
	}

	void MouseControl::UpdatePosition(){
		double x, y;
		glfwGetCursorPos(m_PWindow, &x, &y);
		m_mousePosition = glm::vec2(x, m_height -y);
	}
	void MouseControl::UpdateButton() {
		m_scroll = glm::vec2(0, 0);
		for (int i = 0; i < m_keys.size(); i++) {
			if (m_keys[i] == BUTTON_ACTION::Pressed)
				m_keys[i] = BUTTON_ACTION::Clamped;
			if (m_keys[i] == BUTTON_ACTION::Released)
				m_keys[i] = BUTTON_ACTION::NotClamped;
		}
	}

	void MouseControl::SetKey(const int key, const int action){
		if (action == GLFW_PRESS)
			m_keys[key] = BUTTON_ACTION::Pressed;
		else
			m_keys[key] = BUTTON_ACTION::Released;
	}

	bool MouseControl::IfPressed(const int key){
		if (m_keys[key] == BUTTON_ACTION::Pressed)
			return true;
		return false;
	}
	bool MouseControl::IfReleased(const int key){
		if (m_keys[key] == BUTTON_ACTION::Released)
			return true;
		return false;
	}
	bool MouseControl::IfClamped(const int key){
		if (m_keys[key] == BUTTON_ACTION::Pressed || m_keys[key] == BUTTON_ACTION::Clamped)
			return true;
		return false;
	}

	bool MouseControl::IfNotClamped(const int key){
		if (m_keys[key] == BUTTON_ACTION::Released || m_keys[key] == BUTTON_ACTION::NotClamped)
			return true;
		return false;
	}

	MouseControl::MouseControl() {
		m_PWindow = nullptr;
		m_height = 0;
		m_mousePosition = glm::vec2(0, 0);
		m_scroll = glm::vec2(0, 0);
		m_keys.fill(BUTTON_ACTION::NotClamped);
	}
}