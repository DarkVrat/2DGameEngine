#pragma once

#include <vector>
#include "Button.h"
#include "../Renderer/PrintText.h"

namespace UserInterface {
	template<class T>
	class ListParameter {
	public:
		ListParameter(glm::vec3 position, glm::vec2 size, int scaleText, std::vector<T> vectorParam);
		ListParameter();
		~ListParameter();

		void create(glm::vec3 position, glm::vec2 size, int scaleText, std::vector<T> vectorParam);

		void render();
		void checkClick();

		void setTypeToString(std::function<std::string(T value)> typeToStringFunction);

		void updateText();

		T getParametr();

	private:
		Button m_buttonLeft;
		Button m_buttonRight;

		std::shared_ptr<Renderer::Sprite> m_spriteBackGroung;
		glm::vec3 m_position;
		glm::vec2 m_size;

		PRINT_TEXT::Text m_text;
		int m_index;
		std::vector<T> m_vectorParametrs;
		std::function<std::string(T value)> m_typeToString;
	};

	template<class T>
	inline ListParameter<T>::ListParameter(glm::vec3 position, glm::vec2 size, int scaleText, std::vector<T> vectorParam) {
		create(position, size, scaleText, vectorParam);
	}

	template<class T>
	inline ListParameter<T>::ListParameter(){
		m_spriteBackGroung=nullptr;
		m_position = glm::vec3(0, 0, 0);
		m_size = glm::vec2(0, 0);
		m_text;
		m_index=0;
	}

	template<class T>
	inline ListParameter<T>::~ListParameter() {
		m_spriteBackGroung.~shared_ptr();
	}

	template<class T>
	inline void ListParameter<T>::create(glm::vec3 position, glm::vec2 size, int scaleText, std::vector<T> vectorParam) {
		m_index = 0;
		m_position = position;
		m_size = glm::vec2(size.x - size.y * 2, size.y);
		m_spriteBackGroung = RESOURCE_MANAGER::getSprite("Button_Off");
		m_text.ms_scale = scaleText;
		m_text.ms_color = glm::vec3(0, 0, 0);
		m_vectorParametrs = vectorParam;

		m_buttonLeft.create(glm::vec3(position.x - (size.x / 2 - size.y / 2), position.y, position.z), glm::vec2(size.y, size.y), 90.f, E_ARROW);
		m_buttonRight.create(glm::vec3(position.x + (size.x / 2 - size.y / 2), position.y, position.z), glm::vec2(size.y, size.y), -90.f, E_ARROW);

		m_buttonLeft.setCallBack([&]() {
			m_index--;
			if (m_index < 0)
				m_index = 0;
			updateText();
			});
		m_buttonRight.setCallBack([&]() {
			m_index++;
			if (m_index >= m_vectorParametrs.size())
				m_index = m_vectorParametrs.size() - 1;
			updateText();
			});
	}

	template<class T>
	inline void ListParameter<T>::render() {
		m_buttonLeft.render();
		m_buttonRight.render();
		m_spriteBackGroung->render(m_position, m_size, 0.f);
		PRINT_TEXT::printText(m_text);
	}

	template<class T>
	inline void ListParameter<T>::checkClick() {
		m_buttonLeft.checkClick();
		m_buttonRight.checkClick();
	}

	template<class T>
	inline void ListParameter<T>::setTypeToString(std::function<std::string(T value)> typeToStringFunction) {
		m_typeToString = typeToStringFunction;
		updateText();
	}

	template<class T>
	inline void ListParameter<T>::updateText() {
		m_text.ms_text = m_typeToString(m_vectorParametrs[m_index]);

		glm::vec3 posText(0, 0, 0);
		posText.z = m_position.z + 0.1f;
		posText.y = m_position.y - m_text.ms_scale / 2.0;
		posText.x = m_position.x - Renderer::PrintText::sizeText(m_text.ms_text, m_text.ms_scale) / 2.0;
		m_text.ms_position = posText;
	}

	template<class T>
	inline T ListParameter<T>::getParametr() {
		return m_vectorParametrs[m_index];
	}
}
