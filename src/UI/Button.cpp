#include "Button.h"

#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

namespace UserInterface {
	Button::Button(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text, GLint scale, glm::vec3 color){
		if (type == STANDART) {
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_button_On");
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_button_Off");
		}
		m_typeButton = type;

		m_position = glm::vec2(position.x, position.y);
		m_layer = position.z;
		m_size = size;

		glm::vec3 posText(0, 0, 0);
		posText.z = position.z + 0.1f;
		posText.y = position.y + size.y / 2.0 - scale / 2.0;
		posText.x = position.x + (size.x - Renderer::PrintText::sizeText(text, scale)) / 2;
		m_textButton = PRINT_TEXT::Text(text, posText, scale, color);

		m_click = false;
	}

	Button::Button(){
		m_SpriteButtonOn = nullptr;
		m_SpriteButtonOff = nullptr;
		m_position = glm::vec2(0, 0);
		m_layer = 0.f;
		m_size = glm::vec2(1, 1);
		m_click = false;
	}

	Button::~Button(){
		m_SpriteButtonOn.~shared_ptr();
		m_SpriteButtonOff.~shared_ptr();
	}

	void Button::create(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text, GLint scale, glm::vec3 color) {
		if (type == STANDART) {
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_button_On");
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_button_Off");
		}
		m_typeButton = type;

		m_position = glm::vec2(position.x, position.y);
		m_layer = position.z;
		m_size = size;
		m_area = glm::vec4(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2, m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);

		glm::vec3 posText(0, 0, 0);
		posText.z = position.z + 0.1f;
		posText.y = position.y - scale / 2.0;
		posText.x = position.x - Renderer::PrintText::sizeText(text, scale) / 2.0;
		m_textButton = PRINT_TEXT::Text(text, posText, scale, color);

		m_click = false;
	}

	void Button::render(){
		if (m_click) {
			m_SpriteButtonOn->render(m_position, m_size, 0.f, m_layer);
		}
		else {
			m_SpriteButtonOff->render(m_position, m_size, 0.f, m_layer);
		}

		if (m_typeButton == STANDART) {
			PRINT_TEXT::printText(m_textButton);
		}
	}

	void Button::checkClick(){
		if (MOUSE::ifInArea(m_area)) {
			if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = true;
			}
			if (m_click && MOUSE::ifReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = false;
				m_callback();
			}
		}
		else {
			m_click = false;
		}
	}

	void Button::setCallBack(std::function<void()> callBack){
		m_callback = callBack;
	}
}