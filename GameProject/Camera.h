#pragma once

class Camera final
{
public:
	explicit Camera(const Point2f& cameraDimentions);
	~Camera() = default;

	// Updates / Draw
	void Draw() const;	// for testing
	void Update(const Rectf& target);

	// Getters / Setters
	const Point2f& GetPosition() const;
	const Point2f& GetDimentions() const;
	Rectf GetCameraRect() const;
	void SetPosition(const Point2f& pos);
	void ChangePosition(float xChange, float yChange);

	// Other
	void SetTileMapBounds(const Rectf& bounds);
	
private:

	Point2f m_Position;
	Point2f m_Dimentions;
	Rectf m_Bounds;

private:

	void Track(const Rectf& target);
	void Clamp();
};