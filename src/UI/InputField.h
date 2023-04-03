#pragma once

#include "Button.h"

namespace UserInterface {
	class InputField : public UIElement {
	public:
		InputField(const glm::vec3& position, const glm::vec2& size, const std::string& text="", const GLfloat& scale=0.9, const glm::vec3& color=glm::vec3(0,0,0), const glm::vec2& origin=glm::vec2(0.5,0.5));
		InputField();
		~InputField();

		void create(const glm::vec3& position, const glm::vec2& size, const std::string& text = "", const GLfloat& scale = 0.9, const glm::vec3& color = glm::vec3(0,0,0), const glm::vec2& origin = glm::vec2(0.5, 0.5));

		void render()override;
		void update()override;
		bool checkClick()override;
		
		std::string stringForPaint();

		void setCallBack(std::function<void(std::string value)> callBack);

	private:
		Button m_field;
		bool m_writeText;
		unsigned m_index;
		std::string m_bufferString;
		std::function<void(std::string value)> m_callBack;
	};
}