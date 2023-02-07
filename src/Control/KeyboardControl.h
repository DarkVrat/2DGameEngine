#pragma once

#include <array>
#include <iostream>
#include <GLFW/glfw3.h>

#define KEYBOARD Control::KeyboardControl 

namespace Control {
	class KeyboardControl {
	public:

		static void updateButton();

		static void setKey(GLFWwindow* pWindow, int key, int scancode, int action, int mode);
		static bool ifPressed(const int key);
		static bool ifReleased(const int key);
		static bool ifClamped(const int key);
		static bool ifNotClamped(const int key);

		static void startWritingText();
		static bool getWriteText();
		static void addCharInBuffer(GLFWwindow* окно, unsigned int codepoint);
		static void deleteLastCharInBuffer();
		static std::string getBuffer();
		static std::string getBufferAndRemove();

	private:
		KeyboardControl();

		enum class E_BUTTON_ACTION {
			NotClamped,
			Pressed,
			Clamped,
			Released
		};
		static std::array<E_BUTTON_ACTION, 349> m_keys;
		static std::string m_buffer;
		static bool m_writeText;
	};
}