#pragma once

class Player;
class Texture;
class TextureManager;

class Platform final
{
public:
	// Constructors / Destructors
	explicit Platform(const Point2f& startPos, const Point2f& endPos, bool moveDir, const float speed, TextureManager* textureManager);
	~Platform() = default;

	// Update / draw
	bool Update(float elapsedSec, Player* player);
	void Draw() const;

	// Getters / Setters
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	const Rectf& GetHitbox() const;
	bool GetIsOnPlatform() const;

private:

	const float m_Speed;
	const bool m_MoveDir;
	const float m_Scale;

	TextureManager* m_pTextureManager;

	Point2f m_StartPos;
	Point2f m_EndPos;

	Rectf m_Hitbox;

	bool m_IsOnScreen;

	std::vector< Point2f > m_PlatformVertices;

	bool m_IsOnPlatform;
	bool m_PlatformMoveDir;

	float m_Velocity;

private:

	// Init
	void InitVariables();

	// Update
	void UpdateVertices();
	void UpdateMovement(float elapsedSec);
	void UpdateCollision(Player* player);
	void UpdateVerticalCollisions(Player* player);
	void UpdateHorizontalCollisions(Player* player);
	void UpdatePlayerOnPlatform(Player* player);
	void UpdatePlayerMovement(Player* player, float elapsedSec);

	// Draw
	void DrawPlatform() const;
	void DrawHitBox() const;
	void DrawVertices() const;
};