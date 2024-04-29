#pragma once

class Texture;
class Animation;
class TextureManager;

class Bubble final
{
public:
	// Constructors / Destructors
	explicit Bubble(const Point2f& startPos, const Point2f& endPos, const float speed, TextureManager* textureManager);
	~Bubble();

	Bubble(const Bubble& bubble) = delete;
	Bubble& operator=(const Bubble& bubble) = delete;
	Bubble(Bubble&& bubble) noexcept = delete;
	Bubble& operator=(Bubble&& bubble) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec);
	void Draw() const;

	// Getters / Setters
	const Rectf& GetHitbox() const;
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	float GetPlayerKnockUp() const;
	bool GetIsHit() const;

	// other
	void Hit();

private:

	const float m_Scale;
	const float m_PlayerKnockUpVelocity;

	bool m_IsOnScreen;

	TextureManager* m_pTextureManager;

	Animation* m_pBubblyAnimation;
	Rectf m_PlopAnimation;

	Rectf m_Hitbox;
	Rectf m_SrcRect;

	Point2f m_StartPos;
	Point2f m_EndPos;

	float m_Angle;
	bool m_IsHit;
	bool m_IsVisible;
	float m_IsHitTime;

	Vector2f m_Velocity;

private:

	// Init
	void InitVariables();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update 
	void UpdateMovement(float elapsedSec);
	void UpdateAnimations(float elapsedSec);
};