#pragma once

#include <memory>
#include <functional>
#include "../Renderer/Sprite.h"
#include "../Renderer/PrintText.h"

namespace UserInterface {
	class Button {
	public:
		Button(glm::vec3 position, glm::vec2 size, std::string text, GLint scale, glm::vec3 color);
		Button();
		~Button();

		void create(glm::vec3 position, glm::vec2 size, std::string text, GLint scale, glm::vec3 color);

		void render();
		void checkClick();

		void setCallBack(std::function<void()> callBack);

	private:
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOn;
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOff;
		glm::vec2 m_position;
		glm::vec2 m_size;
		GLfloat m_layer;
		PRINT_TEXT::Text m_textButton;

		bool m_click;

		std::function<void()> m_callback;
	};
}