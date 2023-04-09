#include "staticTextForUI.h"

staticTextForUI::staticTextForUI(const TEXT& text, float size, bool center, std::shared_ptr<Renderer::Sprite> sprite, float shift)
	:m_text(text), m_sizeText(size), m_centrText(center), m_spriteBackLine(sprite), m_shift(shift) {}

void staticTextForUI::render(){
	if (m_spriteBackLine) {
		m_spriteBackLine->renderUI(glm::vec3(m_text.ms_position.x - m_shift, m_text.ms_position.y, m_text.ms_position.z - 0.1f), glm::vec2(m_sizeText + m_shift, m_text.ms_scale), 0, glm::vec2(0, 0));
	}
	PRINT_TEXT::printTextReduction(m_text, m_sizeText, m_centrText);
}