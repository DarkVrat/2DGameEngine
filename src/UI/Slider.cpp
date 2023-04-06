#include "Slider.h"

#include <cmath>
#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

namespace UserInterface{ 
	Slider::Slider(const glm::vec3& position, const glm::vec2& size, const bool& view, const glm::vec2& minMax, const float& startValue, const glm::vec2& origin) {
		create(position, size, view, minMax, startValue, origin);
	}

	Slider::Slider() {
		m_spriteBackGroung = nullptr;
		m_spriteSliderOn = nullptr;
		m_spriteSliderOff = nullptr;
		m_position = glm::vec3(0, 0, 0);
		m_size = glm::vec2(0, 0);
		m_positionSlider = glm::vec2(0,0);
		m_MinMax = glm::vec2(0, 0);
		m_value = 0.f;
		m_view = false;
		m_step = 0.f;
	}

	Slider::~Slider(){}

	void Slider::create(const glm::vec3& position, const glm::vec2& size, const bool& view, const glm::vec2& minMax, const float& startValue, const glm::vec2& origin){
		m_view = view;
		m_MinMax = minMax;
		m_value = startValue;
		m_step = (minMax.y - minMax.x) / 20;
		m_position = position;
		m_size = size;
		m_position = position;
		m_origin = origin;

		m_spriteSliderOn = RESOURCE_MANAGER::getSprite("Slider_On");
		m_spriteSliderOff = RESOURCE_MANAGER::getSprite("Slider_Off");

		if (m_view) {
			m_spriteBackGroung = RESOURCE_MANAGER::getSprite("Slider_Background_Vertical");
			m_buttonLess.create(glm::vec3(0, 0, 0), glm::vec2(0, 0), 90, E_ARROW);
			m_buttonMore.create(glm::vec3(0, 0, 0), glm::vec2(0, 0), -90, E_ARROW);
		}
		else {
			m_spriteBackGroung = RESOURCE_MANAGER::getSprite("Slider_Background_Horizontal");
			m_buttonLess.create(glm::vec3(0, 0, 0), glm::vec2(0, 0), 180, E_ARROW);
			m_buttonMore.create(glm::vec3(0, 0, 0), glm::vec2(0, 0), 0, E_ARROW);
		}

		update();

		m_buttonLess.setCallBack([&]() {
				m_value -= m_step;
				if (m_value < m_MinMax.x)
					m_value = m_MinMax.x;
				updatePositionSlider();
			});
		m_buttonMore.setCallBack([&]() {
				m_value += m_step;
				if (m_value > m_MinMax.y)
					m_value = m_MinMax.y;
				updatePositionSlider();
			});
	}

	void Slider::render(){
		m_buttonLess.render();
		m_buttonMore.render();
		m_spriteBackGroung->renderUI(m_positionSprite, m_sizeSprite, 0.f, m_origin);
		if (m_click) {
			m_spriteSliderOn->renderUI(glm::vec3(m_positionSlider, m_position.z + 0.1f), m_sizeSlider, 0.f);
		}
		else {
			m_spriteSliderOff->renderUI(glm::vec3(m_positionSlider, m_position.z + 0.1f), m_sizeSlider, 0.f);;
		}
	}

	bool Slider::checkClick() {
		if (m_buttonLess.checkClick()|| m_buttonMore.checkClick()) {
			m_callback(m_value);
			return true;
		}
		else if (MOUSE::ifInArea(m_areaMin)||MOUSE::ifInArea(m_area)|| MOUSE::ifInArea(m_areaMax)) {
			if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = true;
			}

			if (MOUSE::ifClamped(GLFW_MOUSE_BUTTON_LEFT)&&m_click){

				if (MOUSE::ifInArea(m_areaMin)) {
					m_value = m_MinMax.x;
				}
				else if (MOUSE::ifInArea(m_areaMax)) {
					m_value = m_MinMax.y;
				}
				else if (MOUSE::ifInArea(m_area)) {
					float procent;
					if (m_view) {
						float s = m_area.z - m_area.x;
						procent = (MOUSE::getPosition().x - m_area.x) / s;
					}
					else {
						float s = m_area.w - m_area.y;
						procent = (MOUSE::getPosition().y - m_area.y) / s;
					}
					m_value = (m_MinMax.y - m_MinMax.x) * procent + m_MinMax.x;
				}

				updatePositionSlider();
				m_callback(m_value);
			}
			

			if (MOUSE::ifReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = false;
				return true;
			}
			
			if (std::abs(MOUSE::getScroll().y) > 0) {
				m_value += MOUSE::getScroll().y * m_step * 0.1;
				if (m_value < m_MinMax.x) {
					m_value = m_MinMax.x;
				}
				else if (m_value > m_MinMax.y) {
					m_value = m_MinMax.y;
				}
				updatePositionSlider();
				m_callback(m_value);
				return true;
			}
		}
		else {
			if (m_click) {
				m_callback(m_value);
				updatePositionSlider();
				m_click = false;
				return true;
			}
		}
		return false;
	}

	void Slider::updatePositionSlider(){
		float procent = (m_value-m_MinMax.x) / (m_MinMax.y - m_MinMax.x);
		if (m_view) {
			float s = m_area.z - m_area.x;
			m_positionSlider = glm::vec2(m_area.x + s * procent, m_position.y + m_sizeSprite.y * (0.5 - m_origin.y));
		}
		else {
			float s = m_area.w - m_area.y;
			m_positionSlider = glm::vec2(m_position.x + m_sizeSprite.x * (0.5 - m_origin.x), m_area.y + s * procent);
		}
	}

	void Slider::update(){
		glm::vec2 size = m_size; 

		if (m_view) {
			if (std::abs(size.x) < 0.000001f) {
				size.x = size.y * (ResourceManager::getSprite("Slider_Background_Vertical")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
			}
			else if (std::abs(size.y) < 0.000001f) {
				size.y = size.x / (ResourceManager::getSprite("Slider_Background_Vertical")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
			}

			m_buttonLess.setSize(glm::vec2(0, size.y));
			m_buttonMore.setSize(glm::vec2(0, size.y));

			m_buttonLess.setPosition(glm::vec3( m_position.x - size.x * m_origin.x + m_buttonLess.getSize().x / 2,
												m_position.y + (0.5 - m_origin.y) * size.y,
												m_position.z));
			m_buttonMore.setPosition(glm::vec3(	m_position.x + size.x * (1 - m_origin.x) - m_buttonMore.getSize().x / 2,
												m_position.y + (0.5 - m_origin.y) * size.y,
												m_position.z));

			m_sizeSprite = glm::vec2(size.x - m_buttonLess.getSize().x * 2, size.y);
			m_positionSprite = glm::vec3(	m_position.x + (0.5 - m_origin.x) * m_buttonLess.getSize().x * 2,
											m_position.y,
											m_position.z);
			

			m_area = glm::vec4(	m_positionSprite.x - m_sizeSprite.x * m_origin.x + m_buttonLess.getSize().x / 2,
								m_positionSprite.y - m_sizeSprite.y * m_origin.y,
								m_positionSprite.x + m_sizeSprite.x * (1 - m_origin.x) - m_buttonMore.getSize().x / 2,
								m_positionSprite.y + m_sizeSprite.y * (1 - m_origin.y));
			m_areaMin = glm::vec4(m_area.x - m_buttonLess.getSize().x / 2, m_area.y, m_area.x, m_area.w);
			m_areaMax = glm::vec4(m_area.z, m_area.y, m_area.z + m_buttonLess.getSize().x / 2, m_area.w);
		}
		else {
			if (std::abs(size.x) < 0.000001f) {
				size.x = size.y / (ResourceManager::getSprite("Slider_Background_Vertical")->getInversRatio() + ResourceManager::getSprite("Arrow_Off")->getInversRatio() * 2);
			}
			else if (std::abs(size.y) < 0.000001f) {
				size.y = size.x * (ResourceManager::getSprite("Slider_Background_Vertical")->getInversRatio() + ResourceManager::getSprite("Arrow_Off")->getInversRatio() * 2);
			}

			m_buttonLess.setSize(glm::vec2(size.x, 0));
			m_buttonMore.setSize(glm::vec2(size.x, 0));

			m_buttonLess.setPosition(glm::vec3(	m_position.x + (0.5 - m_origin.x) * size.x,
												m_position.y - size.y * m_origin.y + m_buttonMore.getSize().y / 2,
												m_position.z));
			m_buttonMore.setPosition(glm::vec3(	m_position.x + (0.5 - m_origin.x) * size.x,
												m_position.y + size.y * (1-m_origin.y) - m_buttonMore.getSize().y / 2,
												m_position.z));

			m_sizeSprite = glm::vec2(size.x, size.y - m_buttonLess.getSize().y * 2);
			m_positionSprite = glm::vec3(	m_position.x,
											m_position.y + (0.5 - m_origin.y) * m_buttonLess.getSize().y * 2,
											m_position.z);


			m_area = glm::vec4(	m_positionSprite.x - m_sizeSprite.x * m_origin.x,
								m_positionSprite.y - m_sizeSprite.y * m_origin.y + m_buttonLess.getSize().y / 2,
								m_positionSprite.x + m_sizeSprite.x * (1 - m_origin.x),
								m_positionSprite.y + m_sizeSprite.y * (1 - m_origin.y) - m_buttonMore.getSize().y / 2);
			m_areaMin = glm::vec4(m_area.x, m_area.y - m_buttonLess.getSize().y / 2, m_area.z, m_area.y);
			m_areaMax = glm::vec4(m_area.x, m_area.w, m_area.z, m_area.w + m_buttonLess.getSize().y / 2);
		}

		m_sizeSlider = m_buttonLess.getSize();

		updatePositionSlider();
	}

	void Slider::setCallBack(std::function<void(float value)> callBack){
		m_callback = callBack;
	}


}
