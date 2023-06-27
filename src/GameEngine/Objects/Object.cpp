#include"Object.h"

Object::Object(){
	m_objectData = ObjectData();
	m_position = glm::vec2(0, 0);
	ShapeIsPoint();
}

Object::Object(Object&& object) noexcept{
	m_position = std::move(object.m_position);
	m_points = std::move(object.m_points);
	m_objectData = std::move(object.m_objectData);
}

Object Object::operator=(const Object& object){
	m_position = object.m_position;
	m_points = object.m_points;
	m_objectData = object.m_objectData;
	return *this;
}

Object Object::operator=(Object&& object)noexcept {
	m_position = std::move(object.m_position);
	m_points = std::move(object.m_points);
	m_objectData = std::move(object.m_objectData);
	return *this;
}
