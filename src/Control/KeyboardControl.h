#pragma once

#include <array>
#include <iostream>
#include <GLFW/glfw3.h>

#define KEYBOARD Control::KeyboardControl 

 //(RUS) Класс для управления с  клавиатуры, отслеживание состояний кнопок, и ввода текста 
//(ENG) Class for keyboard control, tracking button states, and text input

namespace Control {
	class KeyboardControl {
	public:
		static void setKey(GLFWwindow* pWindow, int key, int scancode, int action, int mode);
		static bool ifPressed(const int& key);
		static bool ifReleased(const int& key);
		static bool ifClamped(const int& key);

		static void startWritingText();
		static void endWritingText();
		static void addCharInBuffer(GLFWwindow* окно, unsigned codepoint);
		static bool getWritingText();
		static char getBuffer();
	private:
		KeyboardControl();

		enum E_BUTTON_ACTION : int8_t {
			NOT_CLAMPED,
			PRESSED,
			CLAMPED,
			RELEASED
		};
		static std::array<E_BUTTON_ACTION, 349> m_keys;
		static char m_buffer;
		static bool m_writeText;
	};
}