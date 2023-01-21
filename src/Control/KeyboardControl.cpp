#include <GLFW/glfw3.h>
#include "KeyboardControl.h"



static Control::KeyboardControl* keyboardControl = nullptr;

namespace Control {
	KeyboardControl* KeyboardControl::Get(){
		if (keyboardControl == nullptr)
			keyboardControl = new KeyboardControl();
		return keyboardControl;
	}

	void KeyboardControl::UpdateButton(){
		for (int i = 0; i < m_keys.size(); i++) {
			if (m_keys[i] == BUTTON_ACTION::Pressed)
				m_keys[i] = BUTTON_ACTION::Clamped;
			if (m_keys[i] == BUTTON_ACTION::Released)
				m_keys[i] = BUTTON_ACTION::NotClamped;
		}
	}

	void KeyboardControl::SetKey(const int key, const int action) {
		if (action == GLFW_PRESS)
			m_keys[key] = BUTTON_ACTION::Pressed;
		else
			m_keys[key] = BUTTON_ACTION::Released;
	}

	bool KeyboardControl::IfPressed(const int key) {
		if (m_keys[key] == BUTTON_ACTION::Pressed)
			return true;
		return false;
	}
	bool KeyboardControl::IfReleased(const int key) {
		if (m_keys[key] == BUTTON_ACTION::Released)
			return true;
		return false;
	}
	bool KeyboardControl::IfClamped(const int key) {
		if (m_keys[key] == BUTTON_ACTION::Pressed || m_keys[key] == BUTTON_ACTION::Clamped)
			return true;
		return false;
	}

	bool KeyboardControl::IfNotClamped(const int key) {
		if (m_keys[key] == BUTTON_ACTION::Released || m_keys[key] == BUTTON_ACTION::NotClamped)
			return true;
		return false;
	}

	void KeyboardControl::addCharInBuffer(unsigned int codepoint) {
		if (WriteText) {
			if (codepoint > 128) {
				codepoint -= 848;
			}
			buffer += (char)codepoint;
		} 
	}

	std::string KeyboardControl::GetBufferAndRemove() {
		WriteText = false;
		std::string s = buffer;
		buffer = "";
		return s;
	}
}