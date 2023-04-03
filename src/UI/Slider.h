#pragma once

#include "Button.h"

#define UI_VERTICAL_SLIDER true
#define UI_HORIZONTAL_SLIDER false

namespace UserInterface{
	class Slider : public UIElement{
	public:
		Slider(const glm::vec3& position, const glm::vec2& size, const bool& view, const glm::vec2& minMax, const float& startValue, const glm::vec2& origin=glm::vec2(0.5, 0.5));
		Slider();
		~Slider();

		void create(const glm::vec3& position, const glm::vec2& size, const bool& view, const glm::vec2& minMax, const float& startValue, const glm::vec2& origin = glm::vec2(0.5, 0.5));

		void render()override;
		void update()override;
		bool checkClick()override;

		void updatePositionSlider();
		void setCallBack(std::function<void(float value)> callBack);

		float getValue() { return m_value; }

	private:
		Button m_buttonLess;
		Button m_buttonMore;
		glm::vec3 m_position;
		glm::vec2 m_size;
		glm::vec2 m_origin;

		std::shared_ptr<Renderer::Sprite> m_spriteBackGroung;
		glm::vec3 m_positionSprite;
		glm::vec2 m_sizeSprite;
		glm::vec4 m_area;
		glm::vec4 m_areaMin;
		glm::vec4 m_areaMax;

		std::shared_ptr<Renderer::Sprite> m_spriteSliderOn;
		std::shared_ptr<Renderer::Sprite> m_spriteSliderOff;
		glm::vec2 m_positionSlider;
		glm::vec2 m_sizeSlider;
		
		glm::vec2 m_MinMax;
		float m_step;
		float m_value;

		bool m_view;
		bool m_click;
		std::function<void(float value)> m_callback;
	};
}
