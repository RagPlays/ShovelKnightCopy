#include "pch.h"
#include "Camera.h"

// Constructors / Destructors
Camera::Camera(const Point2f& cameraDimentions)
	: m_Dimentions{ cameraDimentions }
	, m_Position{ Point2f{} }
	, m_Bounds{ Rectf{} }
{
}

// Update / Draw
void Camera::Draw() const
{
	utils::SetRGBColor(20, 20, 255);
	utils::DrawRect(m_Position.x, m_Position.y, m_Dimentions.x, m_Dimentions.y);
}

void Camera::Update(const Rectf& target)
{
	Track(target);
	Clamp();
}

// Getters / Setters
const Point2f& Camera::GetPosition() const
{
	return m_Position;
}

void Camera::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

void Camera::ChangePosition(float xChange, float yChange)
{
	m_Position.x += xChange;
	m_Position.y += yChange;
}

const Point2f& Camera::GetDimentions() const
{
	return m_Dimentions;
}

Rectf Camera::GetCameraRect() const
{
	return Rectf{ m_Position, m_Dimentions.x , m_Dimentions.y };
}

// Other
void Camera::SetTileMapBounds(const Rectf& bounds)
{
	m_Bounds = bounds;
}

// private funtions //
void Camera::Track(const Rectf& target)
{
	// A helper function that positions the camera around the given rectangle target and returns this new position.
	m_Position =  
		Point2f{ 
		float(int(target.left - m_Dimentions.x / 2.f + target.width / 2.f)), 
		float(int(target.bottom - m_Dimentions.y / 2.f + target.height / 2.f))
	};
}

void Camera::Clamp()
{
	// A helper function that corrects the given camera position so that it does not leave the level boundaries.
	if (m_Position.x < m_Bounds.left)
	{
		m_Position.x = m_Bounds.left;
	}
	if (m_Position.x + m_Dimentions.x > m_Bounds.left + m_Bounds.width)
	{
		m_Position.x = m_Bounds.left + m_Bounds.width - m_Dimentions.x;
	}
	if (m_Position.y < m_Bounds.bottom)
	{
		m_Position.y = m_Bounds.bottom;
	}
	if (m_Position.y + m_Dimentions.y > m_Bounds.bottom + m_Bounds.height)
	{
		m_Position.y = m_Bounds.bottom + m_Bounds.height - m_Dimentions.y;
	}
}