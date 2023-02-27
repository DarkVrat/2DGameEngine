#include "Button.h"

#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

namespace UserInterface {
	Button::Button(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text, GLint scale, glm::vec3 color){
		create(position, size, type, text, scale, color);
	}

	Button::Button(){
		setType(NONE);
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
		setType(type);

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

	bool Button::checkClick(){
		if (MOUSE::ifInArea(m_area)) {
			if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = true;
				
			}
			if (m_click && MOUSE::ifReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = false;
				m_callback();
				return true;
			}
		}
		else {
			m_click = false;
		}
		return false;
	}

	void Button::setCallBack(std::function<void()> callBack){
		m_callback = callBack;
	}

	void Button::setType(E_BUTTON_TYPE type){
		m_typeButton = type;
		switch (type){
		case UserInterface::Button::STANDART:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_button_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_button_On");
			break;
		case UserInterface::Button::ARROW:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_arrow_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_arrow_On");
			break;
		case UserInterface::Button::SLIDER:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_slider_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_slider_On");
			break;
		case UserInterface::Button::FALSE:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_false_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_false_On");
			break;
		case UserInterface::Button::TRUE:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_true_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_true_On");
			break;
		case UserInterface::Button::LIST:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("S_list_Option");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("S_list_Option");
			break;
		default:
			m_SpriteButtonOn = nullptr;
			m_SpriteButtonOff = nullptr;
			break;
		}
		
	}
}