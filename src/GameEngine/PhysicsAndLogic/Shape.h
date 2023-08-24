#pragma once

#include <vector>
#include <glm/vec2.hpp>

class Shape {
public:
	Shape(const glm::vec2& position=glm::vec2(0,0));
	Shape(const glm::vec2& position, const std::vector<glm::vec2>& points);
	Shape(const Shape& shape);
	Shape(Shape&& shape) noexcept;

	Shape operator=(const Shape& shape);
	Shape operator=(Shape&& shape) noexcept;

	Shape copyShape() const;

	void ShapeIsPoint();
	void ShapeIsPolygon(const std::vector<glm::vec2>& points);
	void ShapeIsCircle(const float& radius, const uint8_t& numSegments = 16);

	void Rotate(const float& gradus);
	void Scale(const float& factor);
	void Scale(const float& factor, const float& direction);
	void Move(const glm::vec2& vec);

	void updateCircumradius();

	void SetPosition(const glm::vec2& position);
	glm::vec2 GetPosition() const;
	void SetPoints(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> GetPoints() const;

	glm::vec2 Support(const glm::vec2& direction) const;
	std::vector<glm::vec2> Render() const;
protected:
	glm::vec2 m_position = glm::vec2(0, 0);
	std::vector<glm::vec2> m_points;
	float m_circumradius=0.f;
};