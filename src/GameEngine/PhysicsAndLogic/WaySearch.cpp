#include "WaySearch.h"

#include <glm/glm.hpp>

WayCell** WaySearch::m_mapWay=nullptr;
glm::ivec2 WaySearch::m_sizeMap;
float WaySearch::m_sizeCell;
uint32_t WaySearch::seed=0;

namespace glm {
	bool operator<(const glm::ivec2& lhs, const glm::ivec2& rhs) {
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		return lhs.y < rhs.y;
	}
}

void WaySearch::init(const float& cellSize, const glm::ivec2& sizeMap, uint32_t* pixels){
	if (m_mapWay != nullptr) {
		for (int i = 0; i < m_sizeMap.y; i++) {
			delete[] m_mapWay[i];
		}
		delete[] m_mapWay;
	}

	m_sizeMap = sizeMap;
	m_sizeCell = cellSize;

	m_mapWay = new WayCell * [m_sizeMap.y];
	for (int i = 0; i < m_sizeMap.y; i++) {
		m_mapWay[i] = new WayCell[m_sizeMap.x];
		for (int j = 0; j < m_sizeMap.x; j++) {
			int index = j*m_sizeMap.x + i;
			if (pixels[index] & 8) {
				m_mapWay[i][j].open = true;
			}
		}
	}
}

std::vector<glm::vec2> WaySearch::FindWay(const glm::vec2& start, const glm::vec2& end){
	glm::ivec2 startIndex = positionToIndex(start);
	glm::ivec2 endIndex = positionToIndex(end);

	if (!(m_mapWay[startIndex.x][startIndex.y].open && m_mapWay[endIndex.x][endIndex.y].open)) {
		return std::vector<glm::vec2>();
	}
	 
	seed++;

	m_mapWay[startIndex.x][startIndex.y].seedCell = seed;
	m_mapWay[startIndex.x][startIndex.y].distanceNow = 0;
	m_mapWay[startIndex.x][startIndex.y].helpDistance = 0;
	m_mapWay[startIndex.x][startIndex.y].indexLastCell = startIndex;

	std::set<glm::ivec2> buffer;
	buffer.insert(startIndex);

	while (!buffer.empty()) { 
		glm::ivec2 MinIndex = *buffer.begin();
		for (auto& index : buffer) {
			if (m_mapWay[MinIndex.x][MinIndex.y].helpDistance > m_mapWay[index.x][index.y].helpDistance) {
				MinIndex = index;
			}
		}

		if (MinIndex == endIndex) {
			return Way(end);
		}

		auto a = updateCellAround(MinIndex, endIndex);
		buffer.insert(a.begin(), a.end());

		buffer.erase(MinIndex);
	}
	
	return std::vector<glm::vec2>();
}

float WaySearch::getLengthError(){
	return m_sizeCell/8.f;
}

std::vector<glm::vec2> WaySearch::Way(const glm::vec2& end){
	glm::ivec2 iteratorIndex = positionToIndex(end);
	std::vector<glm::vec2> result;
	result.push_back(end);
	glm::ivec2 direction = glm::ivec2(0, 0);

	while (iteratorIndex != m_mapWay[iteratorIndex.x][iteratorIndex.y].indexLastCell) {
		glm::ivec2 directionNew = iteratorIndex - m_mapWay[iteratorIndex.x][iteratorIndex.y].indexLastCell;
		if (direction != directionNew) {
			result.push_back(indexToPosition(iteratorIndex));
			direction = directionNew;
		}
		iteratorIndex = m_mapWay[iteratorIndex.x][iteratorIndex.y].indexLastCell;
	}

	if (result.size() > 1) {
		result.erase(result.begin() + 1);
	}

	return result;
}

glm::vec2 WaySearch::indexToPosition(const glm::ivec2& index){
	return glm::vec2(index.x * m_sizeCell + m_sizeCell / 2.f, index.y * m_sizeCell + m_sizeCell / 2.f);
}

glm::ivec2 WaySearch::positionToIndex(const glm::vec2& position){
	return glm::ivec2(position.x / m_sizeCell, position.y / m_sizeCell);
}

std::set<glm::ivec2> WaySearch::updateCellAround(const glm::ivec2& coords, const glm::ivec2& cellEnd){
	std::set<glm::ivec2> finalSet;

	if (coords.x > 0) {
		finalSet.insert(updateCell(coords + glm::ivec2(-1, 0), coords, cellEnd));
		if (coords.y > 0)
			finalSet.insert(updateCell(coords + glm::ivec2(-1, -1), coords, cellEnd));
		if (coords.y < m_sizeMap.y - 1)
			finalSet.insert(updateCell(coords + glm::ivec2(-1, 1), coords, cellEnd));
	}

	if (coords.y > 0) 
		finalSet.insert(updateCell(coords + glm::ivec2(0, -1), coords, cellEnd));
	if (coords.y < m_sizeMap.y - 1) 
		finalSet.insert(updateCell(coords + glm::ivec2(0, 1), coords, cellEnd));

	if (coords.x < m_sizeMap.x - 1) {
		finalSet.insert(updateCell(coords + glm::ivec2(1, 0), coords, cellEnd));
		if (coords.y > 0)
			finalSet.insert(updateCell(coords + glm::ivec2(1, -1), coords, cellEnd));
		if (coords.y < m_sizeMap.y - 1)
			finalSet.insert(updateCell(coords + glm::ivec2(1, 1), coords, cellEnd));
	}

	finalSet.erase(glm::ivec2(-1, -1));

	return finalSet;
}

glm::ivec2 WaySearch::updateCell(const glm::ivec2& cell, const glm::ivec2& cellWhere, const glm::ivec2& cellEnd) {
	if (m_mapWay[cell.x][cell.y].seedCell == seed || !m_mapWay[cell.x][cell.y].open) return glm::ivec2(-1, -1);
	m_mapWay[cell.x][cell.y].seedCell = seed;
	m_mapWay[cell.x][cell.y].distanceNow = glm::length(static_cast<glm::vec2>(cell - cellWhere)) + m_mapWay[cellWhere.x][cellWhere.y].distanceNow;
	m_mapWay[cell.x][cell.y].helpDistance = m_mapWay[cell.x][cell.y].distanceNow + std::abs(cell.x - cellEnd.x) + std::abs(cell.y - cellEnd.y);
	m_mapWay[cell.x][cell.y].indexLastCell = cellWhere;
	return cell;
}
