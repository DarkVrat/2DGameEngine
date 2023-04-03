#pragma once

#include <memory>
#include <functional>
#include "UIElement.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/PrintText.h"
#include "../Audio/SoundEffectsPlayer.h"

#define E_STANDART UserInterface::Button::E_BUTTON_TYPE::STANDART
#define E_ARROW UserInterface::Button::E_BUTTON_TYPE::ARROW
#define E_FALSE UserInterface::Button::E_BUTTON_TYPE::FALSE
#define E_TRUE UserInterface::Button::E_BUTTON_TYPE::TRUE
#define E_NONE UserInterface::Button::E_BUTTON_TYPE::NONE
  
namespace UserInterface {
	class Button : public UIElement{
	public:

		enum E_BUTTON_TYPE : uint8_t {
			STANDART,
			ARROW,
			FALSE,
			TRUE,
			NONE
		};

		Button(const glm::vec3& position, const glm::vec2& size, const std::string& text = "", const GLfloat& scale = 0, const glm::vec3& color = glm::vec3(0,0,0), const glm::vec2& origin = glm::vec2(0.5, 0.5));
		Button(const glm::vec3& position, const glm::vec2& size, float rotation, const E_BUTTON_TYPE& type, const glm::vec2& origin = glm::vec2(0.5, 0.5));
		Button();
		~Button();

		void create(const glm::vec3& position, const glm::vec2& size, const std::string& text = "", const GLfloat& scale = 0, const glm::vec3& color = glm::vec3(0, 0, 0), const glm::vec2& origin = glm::vec2(0.5, 0.5));
		void create(const glm::vec3& position, const glm::vec2& size, float rotation, const E_BUTTON_TYPE& type, const glm::vec2& origin = glm::vec2(0.5, 0.5));

		void render() override;
		void update() override;
		bool checkClick() override;

		void setCallBack(std::function<void()> callBack);
		void setType(const E_BUTTON_TYPE& type);
		void setPosition(const glm::vec3& position);
		void setSize(const glm::vec2& size);
		void setText(const std::string& text);

		glm::vec2 getSize();

	private:
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOn;
		std::shared_ptr<Renderer::Sprite> m_SpriteButtonOff;
		glm::vec3 m_position;
		glm::vec2 m_sizeStandart;
		glm::vec2 m_size;
		glm::vec4 m_area;
		glm::vec2 m_origin;
		float m_rotation;
		float m_scaleText;
		PRINT_TEXT::Text m_textButton;
		E_BUTTON_TYPE m_typeButton;

		bool m_click;
		std::shared_ptr<Audio::SoundEffectsPlayer> m_soundClick;
		std::function<void()> m_callback;
	};
}