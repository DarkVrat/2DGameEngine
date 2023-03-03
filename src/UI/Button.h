#pragma once

#include <memory>
#include <functional>
#include "../Renderer/Sprite.h"
#include "../Renderer/PrintText.h"

#define E_STANDART UserInterface::Button::E_BUTTON_TYPE::STANDART
#define E_ARROW UserInterface::Button::E_BUTTON_TYPE::ARROW
#define E_SLIDER UserInterface::Button::E_BUTTON_TYPE::SLIDER
#define E_FALSE UserInterface::Button::E_BUTTON_TYPE::FALSE
#define E_TRUE UserInterface::Button::E_BUTTON_TYPE::TRUE
#define E_NONE UserInterface::Button::E_BUTTON_TYPE::NONE

namespace UserInterface {
	class Button {
	public:

		enum E_BUTTON_TYPE : uint8_t {
			STANDART,
			ARROW,
			SLIDER,
			FALSE,
			TRUE,
			NONE
		};

		Button(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text = "", GLint scale = 0, glm::vec3 color = glm::vec3(1, 1, 1));
		Button(glm::vec3 position, glm::vec2 size, float rotation, E_BUTTON_TYPE type);
		Button();
		~Button();

		void create(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text = "", GLint scale = 0, glm::vec3 color = glm::vec3(1, 1, 1));
		void create(glm::vec3 position, glm::vec2 size, float rotation, E_BUTTON_TYPE type);

		void render();
		bool checkClick();

		void setCallBack(std::function<void()> callBack);
		void setType(E_BUTTON_TYPE type);

	private:
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOn;
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOff;
		glm::vec2 m_position;
		glm::vec2 m_size;
		glm::vec4 m_area;
		float m_rotation;
		GLfloat m_layer;
		PRINT_TEXT::Text m_textButton;
		E_BUTTON_TYPE m_typeButton;

		bool m_click;

		std::function<void()> m_callback;
	};
}