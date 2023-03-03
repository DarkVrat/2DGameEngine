#pragma once

#include "Button.h"

#define UI_VERTICAL_SLIDER true
#define UI_HORIZONTAL_SLIDER false

namespace UserInterface {
	class Slider {
	public:
		Slider(glm::vec3 position, glm::vec2 size, bool view, glm::vec2 minMax, float startValue);
		Slider();
		~Slider();

		void create(glm::vec3 position, glm::vec2 size, bool view, glm::vec2 minMax, float startValue);

		void render();
		void checkClick();
		void updatePositionSlider();

		void setCallBack(std::function<void(float value)> callBack);

		float getValue() { return m_value; }

	private:
		std::shared_ptr<Renderer::Sprite> m_spriteBackGroung;
		std::shared_ptr<Renderer::Sprite> m_spriteSliderOn;
		std::shared_ptr<Renderer::Sprite> m_spriteSliderOff;
		Button m_buttonLess;
		Button m_buttonMore;
		glm::vec2 m_MinMax;
		glm::vec2 m_position;
		glm::vec2 m_size;
		glm::vec2 m_positionSlider;
		glm::vec2 m_sizeSlider;
		glm::vec4 m_area;
		float m_step;
		float m_value;
		float m_layer;
		bool m_view;
		bool m_click;
		std::function<void(float value)> m_callback;
	};
}
