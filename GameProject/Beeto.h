#pragma once

#include "Enemy.h"

class Animation;
class Texture;
class TextureManager;

class Beeto final : public Enemy
{
public:
	// Constructors / Destructors
	explicit Beeto(const Point2f& startPos, bool enemyDir, TextureManager* textureManager);
	virtual ~Beeto() override;

	Beeto(const Beeto& beeto) = delete;
	Beeto& operator=(const Beeto& beeto) = delete;
	Beeto(Beeto&& beeto) noexcept = delete;
	Beeto& operator=(Beeto&& beeto) noexcept = delete;

	// Update / Draw
	virtual void Update(float elapsedSec, const std::vector< std::vector< Point2f > >& vertices, const Rectf& playerHitbox, const std::vector< std::vector< Point2f > >& rockVertices ) override;
	virtual void Draw() const override;

	// Other
	virtual void Hit(bool playerDir) override;
	virtual float JumpHit() override;

private:

	Animation* m_pWalkAnimation;
	Animation* m_pDeadAnimation;

private:

	// Init
	void InitVariables();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update
	void UpdateMovements(float elapsedSec);
	void UpdateAnimations(float elapsedSec);
	void UpdateDrawRect();
	void UpdateIsHit(float elapsedSec);

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