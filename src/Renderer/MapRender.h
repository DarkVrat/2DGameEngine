#pragma once

#include "Texture2D.h"

#define MAP Renderer::MapRenderer

namespace Renderer {
	class MapRenderer {
	public:
		static void init(const std::shared_ptr<Texture2D>& texture, const int& sizeCellTexture);
		static void setLayer(const float& layer);

		static void render();

		static void setMap(const std::string& mapPath, const float& sizeCellMap);
	private:
		static glm::vec4 pixelToTextureCoords(const uint8_t& red, const uint8_t& green);

		static std::shared_ptr<Texture2D> m_texture;
		static int m_sizeCellTexture;

		static std::map<uint32_t, glm::vec4> m_coordCells;
		static float m_sizeCellMap;
		static float m_layer;

		static uint32_t* m_mapPixels;
		static glm::ivec2 m_sizeMap; 
	};
}