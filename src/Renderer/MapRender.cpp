#include "MapRender.h"

#include "../Managers/ResourceManager.h"
#include "../Managers/stb_image.h"
#include "../GameEngine/Objects/Camera.h"
#include "RenderEngine.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

std::shared_ptr<Renderer::Texture2D> Renderer::MapRenderer::m_texture;
int Renderer::MapRenderer::m_sizeCellTexture;

glm::ivec4 Renderer::MapRenderer::m_bufferPixelCoords;
uint32_t Renderer::MapRenderer::m_bufferCountPixels;

std::map<uint32_t, std::pair<glm::vec4, float>> Renderer::MapRenderer::m_coordCells;
float Renderer::MapRenderer::m_sizeCellMap;
float Renderer::MapRenderer::m_layer;

uint32_t* Renderer::MapRenderer::m_mapPixels;
glm::ivec2 Renderer::MapRenderer::m_sizeMap;

std::shared_ptr<Renderer::VertexArray> Renderer::MapRenderer::m_VAO;
Renderer::VertexBuffer Renderer::MapRenderer::m_PositionVBO;
Renderer::VertexBuffer Renderer::MapRenderer::m_TextureVBO;
Renderer::VertexBuffer Renderer::MapRenderer::m_RotateVBO;
    
namespace Renderer {
	void MapRenderer::init(const std::shared_ptr<Texture2D>& texture, const int& sizeCellTexture){
		m_texture = texture;
		m_sizeCellTexture = sizeCellTexture;
		m_bufferPixelCoords = glm::ivec4(0, 0, 0, 0);
		float vertex[] = {
			0.f, 0.f,   // 0    1--2
			0.f, 1.f,   // 1    | /|
			1.f, 1.f,   // 2    |/ |
			1.f, 0.f,   // 3    0--3  
		};

		m_VAO = std::make_shared<VertexArray>();
		m_VAO->bind();

		VertexBuffer vertexVBO;
		vertexVBO.init(vertex, sizeof(vertex));
		m_VAO->addBuffer(vertexVBO, 0, 2);

		m_PositionVBO.init(NULL, NULL);
		m_VAO->addBuffer(m_PositionVBO, 1, 4, GL_FLOAT, true);

		m_TextureVBO.init(NULL, NULL);
		m_VAO->addBuffer(m_TextureVBO, 2, 4, GL_FLOAT, true);

		m_RotateVBO.init(NULL, NULL);
		m_VAO->addBuffer(m_RotateVBO, 3, 1, GL_FLOAT, true);

		m_TextureVBO.unbind();
		m_VAO->unbind();
	}

	void MapRenderer::setLayer(const float& layer){
		m_layer = layer;
	}
	 
	void MapRenderer::render() {
		glm::vec2 camPos = CAMERA::getCoords();
		glm::vec2 camSiz = CAMERA::getSize();

		glm::ivec2 leftButtonBorderCamera = glm::ivec2(	std::round((camPos.x - (camSiz.x + m_sizeCellMap) / 2) / m_sizeCellMap),
														std::round((camPos.y - (camSiz.y + m_sizeCellMap) / 2) / m_sizeCellMap));
		glm::ivec2 rightTopBorderCamera = glm::ivec2(	std::round((camPos.x + (camSiz.x + m_sizeCellMap) / 2) / m_sizeCellMap),
														std::round((camPos.y + (camSiz.y + m_sizeCellMap) / 2) / m_sizeCellMap));

		if (leftButtonBorderCamera.x < 0) { leftButtonBorderCamera.x = 0; }
		if (leftButtonBorderCamera.y < 0) { leftButtonBorderCamera.y = 0; }
		if (rightTopBorderCamera.x > m_sizeMap.x) { rightTopBorderCamera.x = m_sizeMap.x; }
		if (rightTopBorderCamera.y > m_sizeMap.y) { rightTopBorderCamera.y = m_sizeMap.y; }


		if (m_bufferPixelCoords.x > leftButtonBorderCamera.x || m_bufferPixelCoords.y > leftButtonBorderCamera.y || m_bufferPixelCoords.z < rightTopBorderCamera.x || m_bufferPixelCoords.w < rightTopBorderCamera.y) {

			leftButtonBorderCamera = glm::ivec2((camPos.x - camSiz.x) / m_sizeCellMap, (camPos.y - camSiz.y) / m_sizeCellMap);
			rightTopBorderCamera = glm::ivec2((camPos.y + camSiz.x) / m_sizeCellMap, (camPos.y + camSiz.y) / m_sizeCellMap);
			if (leftButtonBorderCamera.x < 0) { leftButtonBorderCamera.x = 0; }
			if (leftButtonBorderCamera.y < 0) { leftButtonBorderCamera.y = 0; }
			if (rightTopBorderCamera.x > m_sizeMap.x) { rightTopBorderCamera.x = m_sizeMap.x; }
			if (rightTopBorderCamera.y > m_sizeMap.y) { rightTopBorderCamera.y = m_sizeMap.y; }

			m_bufferPixelCoords = glm::ivec4(leftButtonBorderCamera, rightTopBorderCamera);

			if (leftButtonBorderCamera.y >= rightTopBorderCamera.y || leftButtonBorderCamera.x >= rightTopBorderCamera.x) {
				return;
			}

			m_bufferCountPixels = (rightTopBorderCamera.x - leftButtonBorderCamera.x) * (rightTopBorderCamera.y - leftButtonBorderCamera.y);

			float startXCellPosition = leftButtonBorderCamera.x * m_sizeCellMap;
			float offsetXCell = startXCellPosition;
			float offsetYCell = leftButtonBorderCamera.y * m_sizeCellMap;

			std::vector<glm::vec4> Position;
			std::vector<glm::vec4> Texture;
			std::vector<float> Rotate;
			Position.reserve(m_bufferCountPixels);
			Texture.reserve(m_bufferCountPixels);
			Rotate.reserve(m_bufferCountPixels);

			for (int h = leftButtonBorderCamera.y; h < rightTopBorderCamera.y; h++) {
				for (int w = leftButtonBorderCamera.x; w < rightTopBorderCamera.x; w++) {
					auto& pixelData = m_coordCells.at(m_mapPixels[h * m_sizeMap.x + w]);

					Position.push_back(glm::vec4(offsetXCell, offsetYCell, m_layer, m_sizeCellMap));
					Texture.push_back(pixelData.first);
					Rotate.push_back(pixelData.second);

					offsetXCell += m_sizeCellMap;
				}
				offsetXCell = startXCellPosition;
				offsetYCell += m_sizeCellMap;
			}
			m_PositionVBO.update(&Position[0], Position.size() * sizeof(glm::vec4));
			m_TextureVBO.update(&Texture[0], Texture.size() * sizeof(glm::vec4));
			m_RotateVBO.update(&Rotate[0], Rotate.size() * sizeof(float));

			Position.clear();
			Texture.clear();
			Rotate.clear();
		}

		m_texture->bind();

		RENDER_ENGINE::drawInstanced(*m_VAO, m_bufferCountPixels);

		m_VAO->unbind();
		m_TextureVBO.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void MapRenderer::setMap(const std::string& mapPath, const float& sizeCellMap){
		m_sizeCellMap = sizeCellMap;
		if (m_mapPixels) {
			delete[] m_mapPixels;
			m_coordCells.clear();
		}

		int channels;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* Pixels = stbi_load(std::string(RESOURCE_MANAGER::getExecutablePath() + "/" + mapPath).c_str(), &m_sizeMap.x, &m_sizeMap.y, &channels, 0);
		
		if (!Pixels) {
			std::cerr << "(!) ERROR MAP LOAD" << mapPath << std::endl;
			return;
		}

		m_mapPixels = new uint32_t[m_sizeMap.x*m_sizeMap.y];

		for (int h = 0; h < m_sizeMap.y; h++) {
			for (int w = 0; w < m_sizeMap.x; w++) {
				int pixelIndex = h * m_sizeMap.x + w;

				uint8_t red = Pixels[pixelIndex * channels];
				uint8_t green = Pixels[pixelIndex * channels + 1];
				uint8_t blue = Pixels[pixelIndex * channels + 2];

				m_mapPixels[pixelIndex] = (red << 16) | (green << 8) | blue;

				if (m_coordCells.count(m_mapPixels[pixelIndex]) < 1) {
					m_coordCells.emplace(m_mapPixels[pixelIndex], pixelToTextureCoords(red, green, blue));
				}
			}
		}

		stbi_image_free(Pixels);
	} 

	std::pair<glm::vec4, float> MapRenderer::pixelToTextureCoords(const uint8_t& red, const uint8_t& green, const uint8_t& blue) {
		float cellTexWidth = static_cast<float>(m_sizeCellTexture + 2) / static_cast<float>(m_texture->getWidth());
		float cellTexHeight = static_cast<float>(m_sizeCellTexture + 2) / static_cast<float>(m_texture->getHeight());
		float amortPixelWidth = 1.f / static_cast<float>(m_texture->getWidth());
		float amortPixelHeight = 1.f / static_cast<float>(m_texture->getHeight());

		glm::vec4 coords(0, 0, 0, 0);

		coords.x = red * cellTexWidth + amortPixelWidth;
		coords.y = green * cellTexHeight + amortPixelHeight;
		coords.z = (red + 1) * cellTexWidth - amortPixelWidth;
		coords.w = (green + 1) * cellTexHeight - amortPixelHeight;

		if (blue & 1) {
			float temp = coords.x;
			coords.x = coords.z;
			coords.z = temp;
		}
		if (blue & 2) {
			float temp = coords.y;
			coords.y = coords.w;
			coords.w = temp;
		}

		return std::pair<glm::vec4, float>(coords, static_cast<float> (blue & 4));
	}
}
