#pragma once

#include "Button.h"

namespace UserInterface {
	class SwitchBool {
	public:
		SwitchBool(glm::vec3 position, glm::vec2 size, bool startFlag);
		SwitchBool();
		~SwitchBool();

		void create(glm::vec3 position, glm::vec2 size, bool startFlag);

		void render();
		void checkClick();

		void setCallBack(std::function<void(bool flag)> callBack);

	private:
		Button m_button;
		bool m_flag;
		std::function<void(bool flag)> m_callback;
	};
}