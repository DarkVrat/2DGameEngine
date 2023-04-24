#pragma once

#include "Button.h"

namespace UserInterface {
	class SwitchBool:public IElementUI {
	public:
		SwitchBool(const glm::vec3& position, const glm::vec2& size, const bool& startFlag, const glm::vec2& origin = glm::vec2(0.5, 0.5));
		SwitchBool();
		~SwitchBool();

		void create(const glm::vec3& position, const glm::vec2& size, const bool& startFlag, const glm::vec2& origin = glm::vec2(0.5, 0.5));

		void render()override;
		void update()override;
		bool checkClick()override;

		void setCallBack(std::function<void(bool flag)> callBack);

	private:
		Button m_button;
		bool m_flag;
		std::function<void(bool flag)> m_callback;
	};
}