#include "Slider.h"

#include "../Managers/ResourceManager.h"
#include "../Control/MouseControl.h"

namespace UserInterface{ 
	Slider::Slider(glm::vec3 position, glm::vec2 size, bool view, glm::vec2 minMax, float startValue) {
		create(position, size, view, minMax, startValue);
	}

	Slider::Slider() {
		m_spriteBackGroung = nullptr;
		m_spriteSliderOn = nullptr;
		m_spriteSliderOff = nullptr;
		m_position = glm::vec2(0, 0);
		m_size = glm::vec2(0, 0);
		m_positionSlider = glm::vec2(0,0);
		m_MinMax = glm::vec2(0, 0);
		m_value = 0.f;
		m_layer = 0.f;
		m_view = false;
		m_step = 0.f;
	}

	Slider::~Slider(){
		m_spriteBackGroung.~shared_ptr();
		m_spriteSliderOff.~shared_ptr();
		m_spriteSliderOn.~shared_ptr();
	}

	void Slider::create(glm::vec3 position, glm::vec2 size, bool view, glm::vec2 minMax, float startValue){
		m_view = view;
		m_MinMax = minMax;
		m_value = startValue;
		m_step = (minMax.y - minMax.x) / 20;
		m_position = glm::vec2(position.x, position.y);
		m_layer = position.z;


		m_spriteBackGroung = RESOURCE_MANAGER::getSprite("S_slider_Background");
		m_spriteSliderOn = RESOURCE_MANAGER::getSprite("S_slider_On");
		m_spriteSliderOff = RESOURCE_MANAGER::getSprite("S_slider_Off");

		if (view) {
			m_buttonLess.create(glm::vec3(position.x - (size.x / 2 - size.y / 2), position.y, position.z), glm::vec2(size.y, size.y), 90.f, E_ARROW);
			m_buttonMore.create(glm::vec3(position.x + (size.x / 2 - size.y / 2), position.y, position.z), glm::vec2(size.y, size.y), -90.f, E_ARROW);
			m_sizeSlider = glm::vec2(size.y, size.y);
			m_size = glm::vec2(size.x - size.y * 2, size.y);
			m_area = glm::vec4(m_position.x - m_size.x / 2 + m_size.y / 2, m_position.y - m_size.y / 2, m_position.x + m_size.x / 2 - m_size.y / 2, m_position.y + m_size.y / 2);
		}
		else {
			m_buttonLess.create(glm::vec3(position.x , position.y + (size.x / 2 - size.y / 2), position.z), glm::vec2(size.x, size.x), 180.f, E_ARROW);
			m_buttonMore.create(glm::vec3(position.x , position.y - (size.x / 2 - size.y / 2), position.z), glm::vec2(size.x, size.x), 0.f, E_ARROW);
			m_sizeSlider = glm::vec2(size.x, size.x);
			m_size = glm::vec2(size.x, size.y - size.x * 2);
			m_area = glm::vec4(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2 + m_size.x / 2, m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 - m_size.x / 2);
		}

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

		
		updatePositionSlider();
	}

	void Slider::render(){
		m_buttonLess.render();
		m_buttonMore.render();
		m_spriteBackGroung->render(m_position, m_size, 0.f, m_layer);
		if (m_click) {
			m_spriteSliderOn->render(m_positionSlider, m_sizeSlider, 0.f, m_layer+0.1f);
		}
		else {
			m_spriteSliderOff->render(m_positionSlider, m_sizeSlider, 0.f, m_layer + 0.1f);;
		}
	}

	void Slider::checkClick() {
		if (m_buttonLess.checkClick()) {
			m_callback(m_value);
		}
		if (m_buttonMore.checkClick()) {
			m_callback(m_value);
		}
		if (MOUSE::ifInArea(m_area)) {
			if (MOUSE::ifClamped(GLFW_MOUSE_BUTTON_LEFT)){
				m_click = true;
				float procent;
				if (m_view) {
					m_positionSlider.x = MOUSE::getPosition().x;
					float s = m_size.x - m_size.y;
					procent = (m_positionSlider.x - m_position.x + s / 2) / s ;
				}
				else {
					m_positionSlider.y = MOUSE::getPosition().y;
					float s = m_size.y - m_size.x;
					procent = (m_positionSlider.y - m_position.y + s / 2) / s;
  				}
				m_value = (m_MinMax.y - m_MinMax.x) * procent + m_MinMax.x;
			}
			if (MOUSE::ifReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				m_click = false;
				m_callback(m_value);
			}
		}
		else {
			if(m_click)
				m_callback(m_value);
			m_click = false;
		}
	}

	void Slider::updatePositionSlider(){
		float procent = m_value / (m_MinMax.y - m_MinMax.x);
		if (m_view) {
			float s = m_size.x - m_size.y;
			m_positionSlider = glm::vec2(m_position.x-s/2+s*procent, m_position.y);
		}
		else {
			float s = m_size.y - m_size.x;
			m_positionSlider = glm::vec2(m_position.x, m_position.y - s / 2 + s * procent);
		}
	}

	void Slider::setCallBack(std::function<void(float value)> callBack){
		m_callback = callBack;
	}


}
