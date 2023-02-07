#include "KeyboardControl.h"

std::array<Control::KeyboardControl::E_BUTTON_ACTION, 349> Control::KeyboardControl::m_keys;
std::string Control::KeyboardControl::m_buffer;
bool Control::KeyboardControl::m_writeText;

namespace Control {
	void KeyboardControl::updateButton(){
		for (int i = 0; i < m_keys.size(); i++) {
			if (m_keys[i] == E_BUTTON_ACTION::Pressed)
				m_keys[i] = E_BUTTON_ACTION::Clamped;
			if (m_keys[i] == E_BUTTON_ACTION::Released)
				m_keys[i] = E_BUTTON_ACTION::NotClamped;
		}
	}

	void KeyboardControl::setKey(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(pWindow, GL_TRUE);
		}
		if (key < 0 || key>348) {
			return; 
		}
		if (action == GLFW_PRESS) {
			m_keys[key] = E_BUTTON_ACTION::Pressed;
		}
		else {
			m_keys[key] = E_BUTTON_ACTION::Released;
		}
	}

	bool KeyboardControl::ifPressed(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Pressed)
			return true;
		return false;
	}
	bool KeyboardControl::ifReleased(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Released)
			return true;
		return false;
	}
	bool KeyboardControl::ifClamped(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Pressed || m_keys[key] == E_BUTTON_ACTION::Clamped)
			return true;
		return false;
	}

	bool KeyboardControl::ifNotClamped(const int key) {
		if (m_keys[key] == E_BUTTON_ACTION::Released || m_keys[key] == E_BUTTON_ACTION::NotClamped)
			return true;
		return false;
	}

	void KeyboardControl::startWritingText(){
		m_writeText = true;
	}

	void KeyboardControl::addCharInBuffer(GLFWwindow* окно, unsigned int codepoint) {
		if (m_writeText) {
			if (codepoint > 128) {codepoint -= 848;}
			if (codepoint == 7622) { codepoint = 185; }
			if (codepoint == 257) { codepoint = 184; }
			if (codepoint == 177) { codepoint = 168; }
			m_buffer += (char)codepoint;
		} 
	}

	void KeyboardControl::deleteLastCharInBuffer(){
		if (!m_buffer.empty()) {
			m_buffer.pop_back();
		}
	}

	bool KeyboardControl::getWriteText(){
		return m_writeText;
	}

	std::string KeyboardControl::getBuffer(){
		return m_buffer;
	}

	std::string KeyboardControl::getBufferAndRemove() {
		m_writeText = false;
		std::string s = m_buffer;
		m_buffer = "";
		return s;
	}

	KeyboardControl::KeyboardControl() { 
		m_keys.fill(E_BUTTON_ACTION::NotClamped); m_buffer = ""; m_writeText = false; 
	}
}