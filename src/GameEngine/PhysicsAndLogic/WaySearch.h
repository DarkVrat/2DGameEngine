#pragma once 

#include "glm/vec2.hpp"
#include <set>
#include <vector>

struct WayCell {
	bool open = false;
	float distanceNow = 0.f;
	float helpDistance = 0.f;
	glm::ivec2 indexLastCell = glm::ivec2(0, 0);
	uint32_t seedCell = 0;
};

class WaySearch {
public:
	static void init(const float& cellSize, const glm::ivec2& sizeMap, uint32_t* pixels);
	static std::vector<glm::vec2> FindWay(const glm::vec2& start, const glm::vec2& end);
	static float getLengthError();
private:
	static std::vector<glm::vec2> Way(const glm::vec2& end);

	static glm::vec2 indexToPosition(const glm::ivec2& index);
	static glm::ivec2 positionToIndex(const glm::vec2& position);

	static std::set<glm::ivec2> updateCellAround(const glm::ivec2& coords, const glm::ivec2& cellEnd);
	static glm::ivec2 updateCell(const glm::ivec2& cell, const glm::ivec2& cellWhere, const glm::ivec2& cellEnd);

	static WayCell** m_mapWay;
	static glm::ivec2 m_sizeMap;
	static float m_sizeCell;
	static uint32_t seed;
};
