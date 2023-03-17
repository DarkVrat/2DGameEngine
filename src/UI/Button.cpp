#include "Button.h"

#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

namespace UserInterface {
	Button::Button(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text, GLfloat scale, glm::vec3 color){
		create(position, size, type, text, scale, color);
	}

	Button::Button(glm::vec3 position, glm::vec2 size, float rotation, E_BUTTON_TYPE type){
		create(position, size, rotation, type);
	}

	Button::Button(){
		setType(NONE);
		m_position = glm::vec3(0, 0, 0);
		m_size = glm::vec2(1, 1);
		m_click = false;
	}

	Button::~Button(){
		m_SpriteButtonOn.~shared_ptr();
		m_SpriteButtonOff.~shared_ptr();
	}

	void Button::create(glm::vec3 position, glm::vec2 size, E_BUTTON_TYPE type, std::string text, GLfloat scale, glm::vec3 color) {
		setType(type);

		m_position = position;
		m_sizeStandart = size;
		m_rotation = 0.f;
		m_click = false;
		m_scaleText = scale;
		m_textButton = PRINT_TEXT::Text(text, m_position, scale, color);

		update();
	}

	void Button::create(glm::vec3 position, glm::vec2 size, float rotation, E_BUTTON_TYPE type){
		setType(type);

		m_position = position;
		m_sizeStandart = size;
		m_rotation = rotation;
		m_click = false;

		update();
	}

	void Button::render(){
		if (m_click) {
			m_SpriteButtonOn->render(m_position, m_size, m_rotation);
		}
		else {
			m_SpriteButtonOff->render(m_position, m_size, m_rotation);
		}

		if (m_typeButton == STANDART) {
			PRINT_TEXT::printText(m_textButton);
		}
	}

	void Button::update(){
		if (m_SpriteButtonOff == nullptr) {
			return;
		}

		m_size = m_sizeStandart;

		if (m_size.x < 0.000001f && m_size.x>-0.000001f) {
			m_size.x = m_size.y * m_SpriteButtonOff->getRatio();
		}
		if (m_size.y < 0.000001f && m_size.y>-0.000001f) {
			m_size.y = m_size.x / m_SpriteButtonOff->getRatio();
		}

		m_area = glm::vec4(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2, m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);

		if (m_typeButton != E_BUTTON_TYPE::STANDART) {
			return;
		}

		m_textButton.ms_scale = m_size.y * m_scaleText;

		if (Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale) > m_size.x * 0.9) {
			m_textButton.ms_scale = (m_size.x * 0.9 * m_textButton.ms_scale) / Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale);
		}

		glm::vec3 posText(0, 0, 0);
		posText.z = m_position.z + 0.1f;
		posText.y = m_position.y - m_textButton.ms_scale / 2.0;
		posText.x = m_position.x - Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale) / 2.0;
		m_textButton.ms_position = posText;

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
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("Button_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("Button_On");
			break;
		case UserInterface::Button::ARROW:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("Arrow_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("Arrow_On");
			break;
		case UserInterface::Button::FALSE:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("False_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("False_On");
			break;
		case UserInterface::Button::TRUE:
			m_SpriteButtonOff = RESOURCE_MANAGER::getSprite("True_Off");
			m_SpriteButtonOn = RESOURCE_MANAGER::getSprite("True_On");
			break;
		default:
			m_SpriteButtonOn = nullptr;
			m_SpriteButtonOff = nullptr;
			break;
		}
		
	}
}