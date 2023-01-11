#pragma once
#include <array>

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

	private:
		KeyboardControl() { m_keys.fill(BUTTON_ACTION::NotClamped); };
		~KeyboardControl() {};

		enum class BUTTON_ACTION {
			NotClamped,
			Pressed,
			Clamped,
			Released
		};
		std::array<BUTTON_ACTION, 349> m_keys;
	};
}