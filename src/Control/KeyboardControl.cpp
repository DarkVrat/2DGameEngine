#include "KeyboardControl.h"

#include <thread>

std::array<Control::KeyboardControl::E_BUTTON_ACTION, 349> Control::KeyboardControl::m_keys;
std::string Control::KeyboardControl::m_buffer;
bool Control::KeyboardControl::m_writeText;

namespace Control {
	 //(RUS) установка ивента нажатой клавиши, проверка key
	//(ENG) setting the event of the pressed key, checking the key
	void KeyboardControl::setKey(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(pWindow, GL_TRUE); }

		if (key == -1) return;

		if (action == GLFW_REPEAT) {
			m_keys[key] = E_BUTTON_ACTION::CLAMPED;
			return;
		}
		if (action == GLFW_PRESS){
			m_keys[key] = E_BUTTON_ACTION::PRESSED;
			return;
		}
		if (action == GLFW_RELEASE) {
			m_keys[key] = E_BUTTON_ACTION::PRESSED;
			return;
		}
	}

	 //(RUS) проверка ивента кнопок
	//(ENG) button event check
	bool KeyboardControl::ifPressed(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED) {
			m_keys[key] = E_BUTTON_ACTION::CLAMPED;
			return true;
		}
		return false;
	}
	bool KeyboardControl::ifClamped(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED || m_keys[key] == E_BUTTON_ACTION::CLAMPED) 
			return true;
		return false;
	}
	bool KeyboardControl::ifReleased(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED) {
			m_keys[key] = E_BUTTON_ACTION::NOT_CLAMPED;
			return true;
		}
		return false;
	}

	 //(RUS) Установка WritingText на true для режима ввода текста, получение WritingText и буфера с текстом
	//(ENG) Setting WritingText to true for text input mode, getting WritingText and text buffer
	void KeyboardControl::startWritingText() { m_writeText = true; }
	bool KeyboardControl::getWriteText() { return m_writeText; }
	std::string KeyboardControl::getBuffer() { return m_buffer; }

	 //(RUS) Добавление символа в буфер. изменения значения для соответствия символа кириллице
	//(ENG) Adding a character to the buffer. changing the value to match the Cyrillic character
	void KeyboardControl::addCharInBuffer(GLFWwindow* окно, unsigned codepoint) {
		if (m_writeText) {
			if (codepoint > 128)  {codepoint-= 848;}
			if (codepoint == 7622){codepoint = 185;}
			if (codepoint == 257) {codepoint = 184;}
			if (codepoint == 177) {codepoint = 168;}
			m_buffer += (char)codepoint;
		} 
	}

	 //(RUS) удаление последнего символа в буфере
	//(ENG) deleting the last character in the buffer
	void KeyboardControl::deleteLastCharInBuffer(){
		if (!m_buffer.empty()) 
			m_buffer.pop_back();
	}

	 //(RUS) получение текста из буфера и его очистка
	//(ENG) getting text from buffer and clearing it
	std::string KeyboardControl::getBufferAndRemove() {
		m_writeText = false;
		std::string s = m_buffer;
		m_buffer = "";
		return s;
	}

	 //(RUS) создание и заполнение переменных начальными параметрами
	//(ENG) creating and filling variables with initial parameters
	KeyboardControl::KeyboardControl() { 
		m_keys.fill(E_BUTTON_ACTION::NOT_CLAMPED); m_buffer = ""; m_writeText = false; 
	}
}