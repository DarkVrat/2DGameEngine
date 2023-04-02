#include "InputField.h"

#include "../Control/KeyboardControl.h"

namespace UserInterface {
	InputField::InputField(const glm::vec3& position, const glm::vec2& size, const std::string& text, const GLfloat& scale, const glm::vec3& color, const glm::vec2& origin){
		create(position, size, text, scale, color, origin);
	}

	InputField::InputField(){
		m_writeText = false;
		m_bufferString = "";
		m_index = 0;
	}

	InputField::~InputField(){
		m_field.~Button();
	}

	void InputField::create(const glm::vec3& position, const glm::vec2& size, const std::string& text, const GLfloat& scale, const glm::vec3& color, const glm::vec2& origin){
		m_field.create(position, size, E_STANDART, text, scale, color, origin);
		m_field.setCallBack([]() {});
		m_writeText = false;
		m_bufferString = text;
		m_index = text.length();
	}

	void InputField::render(){
		m_field.render();
	}

	void InputField::update(){
		m_field.update();
	}

	void InputField::checkClick(){
		bool click = m_field.checkClick();

		if (click && !m_writeText && !KEYBOARD::getWritingText()) {
			KEYBOARD::startWritingText();
			m_writeText = true;
			click = false;
		}

		if ((click||KEYBOARD::ifPressed(GLFW_KEY_ENTER)) && m_writeText && KEYBOARD::getWritingText()) {
			KEYBOARD::endWritingText();
			m_writeText = false;
			m_callBack(m_bufferString);
			m_field.setText(m_bufferString);
		}

		if (m_writeText) {
			char c= KEYBOARD::getBuffer();
			if (c != 0) {
				m_bufferString.insert(m_index, 1, c);
				m_index++;
			}

			if (KEYBOARD::ifPressed(GLFW_KEY_LEFT) && m_index > 0) {
				m_index--;
			}

			if (KEYBOARD::ifPressed(GLFW_KEY_RIGHT) && m_index < m_bufferString.length()) {
				m_index++;
			}

			if (KEYBOARD::ifPressed(GLFW_KEY_BACKSPACE) && m_bufferString.length() > 0) {
				m_index--;
				m_bufferString.erase(m_index, 1); 
			}

			m_field.setText(stringForPaint());
		}
	}

	std::string InputField::stringForPaint(){
		std::string buf = m_bufferString;
		if (static_cast<int>(glfwGetTime())%2==0) {
			buf.insert(m_index, 1, '|');
		}
		return buf;
	}

	void InputField::setCallBack(std::function<void(std::string value)> callBack){
		m_callBack = callBack;
	}

}
