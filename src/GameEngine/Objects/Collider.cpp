#include "Collider.h"

Collider::Collider(Collider&& collider) noexcept{
	m_position = std::move(collider.m_position);
	m_points = std::move(collider.m_points);
}

Collider Collider::operator=(const Collider& collider){
	m_position = collider.m_position;
	m_points = collider.m_points;
	return *this;
}

Collider Collider::operator=(Collider&& collider){
	m_position = std::move(collider.m_position);
	m_points = std::move(collider.m_points);
	return *this;
}
