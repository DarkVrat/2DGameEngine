#pragma once

#include "Texture2D.h"
#include "VertexArray.h"

#define MAP Renderer::MapRenderer

namespace Renderer {
	class MapRenderer {
	public:
		static void init(const std::shared_ptr<Texture2D>& texture, const int& sizeCellTexture);
		static void setLayer(const float& layer);

		static void render();

		static void setMap(const std::string& mapPath, const float& sizeCellMap);
	private:
		static std::pair<glm::vec4, float> pixelToTextureCoords(const uint8_t& red, const uint8_t& green, const uint8_t& blue);

		static glm::ivec4 m_bufferPixelCoords;
		static uint32_t m_bufferCountPixels;

		static std::shared_ptr<Texture2D> m_texture;
		static int m_sizeCellTexture;

		static std::map<uint32_t, std::pair<glm::vec4, float>> m_coordCells;
		static float m_sizeCellMap;
		static float m_layer;

		static uint32_t* m_mapPixels;
		static glm::ivec2 m_sizeMap; 

		static std::shared_ptr<VertexArray> m_VAO;
		static VertexBuffer m_PositionVBO;
		static VertexBuffer m_TextureVBO;
		static VertexBuffer m_RotateVBO;
	};
}