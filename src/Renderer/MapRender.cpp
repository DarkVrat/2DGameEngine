#include "MapRender.h"

#include "../Managers/ResourceManager.h"
#include "../Managers/stb_image.h"
#include "../GameEngine/Objects/Camera.h"
#include "RenderEngine.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cmath"

std::shared_ptr<Renderer::Texture2D> Renderer::MapRenderer::m_texture;
int Renderer::MapRenderer::m_sizeCellTexture;

std::map<uint32_t, glm::vec4> Renderer::MapRenderer::m_coordCells;
float Renderer::MapRenderer::m_sizeCellMap;
float Renderer::MapRenderer::m_layer;

uint32_t* Renderer::MapRenderer::m_mapPixels;
glm::ivec2 Renderer::MapRenderer::m_sizeMap;
    
namespace Renderer {
	void MapRenderer::init(const std::shared_ptr<Texture2D>& texture, const int& sizeCellTexture){
		m_texture = texture;
		m_sizeCellTexture = sizeCellTexture;
	}

	void MapRenderer::setLayer(const float& layer){
		m_layer = layer;
	}

	void MapRenderer::render(){
		glm::vec2 posCamera = CAMERA::getCoords();
		glm::vec2 sizeCamera = CAMERA::getSize();

		glm::ivec2 leftButtonBorderCamera = glm::ivec2(	std::round((posCamera.x - (sizeCamera.x + m_sizeCellMap) / 2) / m_sizeCellMap),
														std::round((posCamera.y - (sizeCamera.y + m_sizeCellMap) / 2) / m_sizeCellMap));
		glm::ivec2 rightTopBorderCamera = glm::ivec2(	std::round((posCamera.x + (sizeCamera.x + m_sizeCellMap) / 2) / m_sizeCellMap),
														std::round((posCamera.y + (sizeCamera.y + m_sizeCellMap) / 2) / m_sizeCellMap)); 

		if (leftButtonBorderCamera.x < 0) { leftButtonBorderCamera.x = 0; }
		if (leftButtonBorderCamera.y < 0) { leftButtonBorderCamera.y = 0; }
		if (rightTopBorderCamera.x > m_sizeMap.x ) { rightTopBorderCamera.x = m_sizeMap.x; } 
		if (rightTopBorderCamera.y > m_sizeMap.y ) { rightTopBorderCamera.y = m_sizeMap.y; }

		int countPixels = (rightTopBorderCamera.x - leftButtonBorderCamera.x) * (rightTopBorderCamera.y - leftButtonBorderCamera.y);

		float startXCellPosition = leftButtonBorderCamera.x * m_sizeCellMap;
		float offsetXCell = startXCellPosition;
		float offsetYCell = leftButtonBorderCamera.y * m_sizeCellMap;

		for (int h = leftButtonBorderCamera.y; h < rightTopBorderCamera.y; h++) {
			for (int w = leftButtonBorderCamera.x; w < rightTopBorderCamera.x; w++) {
				uint32_t pixel = m_mapPixels[h * m_sizeMap.x + w];

				glm::mat4 model(1.f);

				float rotation = 90 * (pixel & 3); 

				model = glm::translate(model, glm::vec3(offsetXCell, offsetYCell, 0.f));
				model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
				model = glm::translate(model, glm::vec3(-0.5f * m_sizeCellMap, -0.5f * m_sizeCellMap, 0.f));
				model = glm::scale(model, glm::vec3(m_sizeCellMap, m_sizeCellMap, m_layer));

				if (pixel & 4) {model = glm::scale(model, glm::vec3(-1.f, 1.f, 1.f));}
				if (pixel & 8) {model = glm::scale(model, glm::vec3(1.f, -1.f, 1.f));}

				RENDER_ENGINE::draw(m_texture, m_coordCells[pixel >> 8], model);

				offsetXCell += m_sizeCellMap;
			}
			offsetXCell = startXCellPosition;
			offsetYCell += m_sizeCellMap;
		}
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

				if (m_coordCells.count(m_mapPixels[pixelIndex]>>8) < 1) {
					m_coordCells.emplace(m_mapPixels[pixelIndex]>>8, glm::vec4(pixelToTextureCoords(red, green)));
				}
			}
		}

		stbi_image_free(Pixels);
	} 

	glm::vec4 MapRenderer::pixelToTextureCoords(const uint8_t& red, const uint8_t& green){
		float cellTexWidth = static_cast<float>(m_sizeCellTexture) / static_cast<float>(m_texture->getWidth());
		float cellTexHeight = static_cast<float>(m_sizeCellTexture) / static_cast<float>(m_texture->getHeight());

		glm::vec4 coords(0,0,0,0);

		coords.x = red * cellTexWidth + cellTexWidth/1000.0;
		coords.y = green * cellTexHeight + cellTexHeight / 1000.0;
		coords.z = coords.x + cellTexWidth - cellTexWidth / 500.0;
		coords.w = coords.y + cellTexHeight - cellTexHeight / 500.0;

		return coords;
	}
}
