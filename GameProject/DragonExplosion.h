#pragma once

class Texture;
class Animation;

class DragonExplosion final
{
public:
	// Constructors / Destructors
	explicit DragonExplosion(const Point2f& pos, const Texture* texture, const float delay);
	~DragonExplosion();

	DragonExplosion(const DragonExplosion& dragonExplosion) = delete;
	DragonExplosion& operator=(const DragonExplosion& dragonExplosion) = delete;
	DragonExplosion(DragonExplosion&& dragonExplosion) noexcept = delete;
	DragonExplosion operator=(DragonExplosion&& dragonExplosion) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec);
	void Draw() const;

	// Getters
	bool GetCanBeDeleted() const;

private:

	const float m_Scale;
	const Texture* m_pTexture;
	const float m_Delay;
	float m_DelayTime;
	bool m_PastDelay;
	Animation* m_pAnimation;
	Rectf m_DrawRect;
	Rectf m_SrcRect;

	bool m_CanBeDeleted;

private:
	
	// Init
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update
	void UpdateAnimations(float elapsedSec);

	// Draw
	void DrawExplosion() const;
};