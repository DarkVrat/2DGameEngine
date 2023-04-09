#include "Button.h"

#include <cmath>
#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

std::shared_ptr<Audio::SoundEffectsPlayer> UserInterface::Button::m_soundClick;

namespace UserInterface {
	Button::Button(const glm::vec3& position, const glm::vec2& size, const std::string& text, const GLfloat& scale, const glm::vec3& color, const glm::vec2& origin){
		create(position, size, text, scale, color, origin);
	}

	Button::Button(const glm::vec3& position, const glm::vec2& size, float rotation, const E_BUTTON_TYPE& type, const glm::vec2& origin){
		create(position, size, rotation, type, origin);
	}

	Button::Button(){
		setType(NONE);
		m_position = glm::vec3(0, 0, 0);
		m_sizeStandart = glm::vec2(0, 0);
		m_size = glm::vec2(0, 0);
		m_area=glm::vec4(0,0,0,0);
		m_origin=glm::vec2(0,0);
		m_rotation=0.f;
		m_scaleText=0.f;
		m_textButton;
		m_click=false;
	}

	Button::~Button(){}
	 
	void Button::create(const glm::vec3& position, const glm::vec2& size, const std::string& text, const GLfloat& scale, const glm::vec3& color, const glm::vec2& origin) {
		setType(STANDART);

		m_position = position;
		m_sizeStandart = size;
		m_rotation = 0.f;
		m_click = false;
		m_scaleText = scale;
		m_textButton = TEXT(text, m_position, scale, color);
		m_origin = origin;

		update();
	}

	void Button::create(const glm::vec3& position, const glm::vec2& size, float rotation, const E_BUTTON_TYPE& type, const glm::vec2& origin){
		setType(type);

		m_position = position;
		m_sizeStandart = size;
		m_rotation = rotation;
		m_origin = origin;
		m_click = false;

		update();
	}

	void Button::render(){
		if (m_click) {
			m_SpriteButtonOn->renderUI(m_position, m_size, m_rotation, m_origin);
		}
		else {
			m_SpriteButtonOff->renderUI(m_position, m_size, m_rotation, m_origin);
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

		if (std::abs(m_size.x) < 0.000001f) {
			m_size.x = m_size.y * m_SpriteButtonOff->getRatio();
		}
		else if (std::abs(m_size.y) < 0.000001f) {
			m_size.y = m_size.x / m_SpriteButtonOff->getRatio();
		}

		m_area = glm::vec4(m_position.x - m_size.x*m_origin.x, m_position.y - m_size.y * m_origin.y, m_position.x + m_size.x * (1-m_origin.x), m_position.y + m_size.y * (1-m_origin.y));

		if (m_typeButton == E_BUTTON_TYPE::STANDART) {
			setText(m_textButton.ms_text);
		}
	}

	bool Button::checkClick(){
		if (MOUSE::ifInArea(m_area)) {
			if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = true;
			}
			else if (m_click && MOUSE::ifReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = false;
				if (m_soundClick == nullptr) {
					m_soundClick = MAKE_SOUND_PLAYER("UIButton", "UI");
				}
				m_soundClick->play();
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

	void Button::setType(const E_BUTTON_TYPE& type){
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

	void Button::setPosition(const glm::vec3& position){
		m_position = position;
		update();
	}

	void Button::setSize(const glm::vec2& size){
		m_sizeStandart = size;
		update();
	}

	void Button::setText(const std::string& text){
		m_textButton.ms_text = text;

		m_textButton.ms_scale = m_size.y * m_scaleText;

		if (Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale) > m_size.x * 0.9) {
			m_textButton.ms_scale = (m_size.x * 0.9 * m_textButton.ms_scale) / Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale);
		}

		glm::vec3 posText(0, 0, 0);
		posText.z = m_position.z + 0.1f;
		posText.y = m_position.y + (0.5 - m_origin.y) * m_size.y - m_textButton.ms_scale / 2.0;
		posText.x = m_position.x + (0.5 - m_origin.x) * m_size.x - Renderer::PrintText::sizeText(m_textButton.ms_text, m_textButton.ms_scale) / 2.0;
		m_textButton.ms_position = posText;
	}

	glm::vec2 Button::getSize()
	{
		return m_size;
	}
}