#include "Collider.h"

Collider::Collider(const Collider& collider){
	m_position = collider.m_position;
	m_points = collider.m_points;
	m_circumradius = collider.m_circumradius;
}

Collider::Collider(Collider&& collider) noexcept{
	m_position = std::move(collider.m_position);
	m_points = std::move(collider.m_points);
	m_circumradius = std::move(collider.m_circumradius);
}

Collider Collider::operator=(const Collider& collider){
	m_position = collider.m_position;
	m_points = collider.m_points;
	m_circumradius = collider.m_circumradius;
	return *this;
}

Collider Collider::operator=(Collider&& collider){
	m_position = std::move(collider.m_position);
	m_points = std::move(collider.m_points);
	m_circumradius = std::move(collider.m_circumradius);
	return *this;
}
