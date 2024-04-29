#pragma once

class Texture;
class Animation;
class Player;

class DozeDragonBubble final
{
public:
	// Constructors / Destructors
	explicit DozeDragonBubble(const Point2f& startPos, const Point2f& endPos);
	~DozeDragonBubble();
	
	DozeDragonBubble(const DozeDragonBubble& dozeDragonBubble) = delete;
	DozeDragonBubble& operator=(const DozeDragonBubble& dozeDragonBubble) = delete;
	DozeDragonBubble(DozeDragonBubble&& dozeDragonBubble) noexcept = delete;
	DozeDragonBubble& operator=(DozeDragonBubble&& dozeDragonBubble) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, Player* player);
	void Draw(const Texture* bubbleTexture);

	// Getters / Setters
	bool GetCanBeDeleted() const;
	void SetVisible();
	bool GetIsOnScreen(const Rectf& cameraRect) const;

private:

	const float m_Scale;
	bool m_GoingUp;
	bool m_CanBeDeleted;
	bool m_IsHit;
	float m_IsHitTime;
	bool m_IsVisible;

	const Point2f m_StartPos;
	const Point2f m_EndPos;
	const Vector2f m_Velocity;

	Rectf m_SrcRect;
	Rectf m_Hitbox;
	
	Animation* m_pBubbleAnimation;
	Rectf m_PopAnimation;

	const float m_PlayerKnockUpSpeed;

private:
	// Init
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update
	void UpdateHitCollision(float elapsedSec, Player* player);
	void UpdateGoingUp();
	void UpdateBubble(float elapsedSec);
	void UpdateAnimations(float elapsedSec);
	void UpdateHitbox();
	void UpdatePlayerCollision(float elapsedSec, Player* player);
};