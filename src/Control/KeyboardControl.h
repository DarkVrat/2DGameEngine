#pragma once
#include <array>
#include <iostream>
#include <glm/vec2.hpp>

#define KEYBOARD Control::KeyboardControl::Get()

namespace Control {
	class KeyboardControl {
	public:
		static KeyboardControl* Get();

		void UpdateButton();

		void SetKey(const int key, const int action);
		bool IfPressed(const int key);
		bool IfReleased(const int key);
		bool IfClamped(const int key);
		bool IfNotClamped(const int key);

		void startWritingText() { WriteText = true; }
		void addCharInBuffer(unsigned int codepoint);
		void deleteLastCharInBuffer();
		bool GetWriteText() { return WriteText; }
		std::string GetBuffer() { return buffer; }
		std::string GetBufferAndRemove();

	private:
		KeyboardControl() { m_keys.fill(BUTTON_ACTION::NotClamped); buffer = ""; WriteText = false; };
		~KeyboardControl() {};

		enum class BUTTON_ACTION {
			NotClamped,
			Pressed,
			Clamped,
			Released
		};
		std::array<BUTTON_ACTION, 349> m_keys;
		std::string buffer;
		bool WriteText;
	};
}