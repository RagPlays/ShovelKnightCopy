#pragma once

#include "Enemy.h"

class Animation;
class Texture;
class TextureManager;

class Blorp final: public Enemy
{
public:
	// Constructors / Destructors
	explicit Blorp(const Point2f& blorpDest, bool enemyDir, TextureManager* textureManager);
	virtual ~Blorp() override;

	Blorp(const Blorp& blorp) = delete;
	Blorp& operator=(const Blorp& blorp) = delete;
	Blorp(Blorp&& blorp) noexcept = delete;
	Blorp& operator=(Blorp&& blorp) noexcept = delete;

	// Update / Draw
	virtual void Update(float elapsedSec, const std::vector< std::vector< Point2f > >& vertices, const Rectf& playerHitbox, const std::vector< std::vector< Point2f > >& rockVertices) override;
	virtual void Draw() const override;
	
	// Other
	virtual void Hit(bool playerDir) override;
	virtual float JumpHit() override;

private:

	float m_JumpTimer;

	bool m_Jumping;
	float m_JumpSpeedX;
	float m_JumpSpeedY;

	bool m_SmallJumpingOver;
	float m_SmallJumpSpeedX;
	float m_SmallJumpSpeedY;

	float m_WalkSpeed;

	bool m_PlayingHitAnimation;

	Animation* m_pWalkAnimation;

private:
	// Init
	void InitVariables();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update
	void UpdateMovement(float elapsedSec, const Rectf& playerHitbox);
	void UpdateIsHit(float elapsedSec);
	void UpdateAnimations(float elapsedSec);
	void UpdateDrawRect();

	// Draw
	void DrawTexture() const;
	void DrawDrawRect() const;
	void DrawHitbox() const;
	void DrawRays() const;

	// Handle
	void HandleCollision(const std::vector< std::vector< Point2f > >& vertices);
	void HandleVerticalCollision(const std::vector< std::vector< Point2f > >& vertices);
	void HandleHorizontalCollision(const std::vector< std::vector< Point2f > >& vertices);

	void HandleRockCollision(const std::vector< std::vector< Point2f > >& rockVertices);
	void HandleVerticalRockCollision(const std::vector< std::vector< Point2f > >& rockVertices);
	void HandleHorizontalRockCollision(const std::vector< std::vector< Point2f > >& rockVertices);
};