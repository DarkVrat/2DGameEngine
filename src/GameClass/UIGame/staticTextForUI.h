#pragma once

#include "../../Renderer/PrintText.h"
#include "../../Renderer/Sprite.h"

class staticTextForUI {
public:
	staticTextForUI(const TEXT& text, float size, bool center, std::shared_ptr<Renderer::Sprite> sprite = nullptr, float shift = 0);

	void render();

private:
	TEXT m_text;
	float m_sizeText;
	bool m_centrText;
	std::shared_ptr<Renderer::Sprite> m_spriteBackLine;
	float m_shift;
};