#pragma once

class Animation;
class GemsManager;
class Texture;
class TextureManager;

class DirtPile final
{
public:
	// constructors / Destructors
	explicit DirtPile(const Point2f& dirtPilePos, TextureManager* textureManager);
	~DirtPile();

	DirtPile(const DirtPile& dirtPile) = delete;
	DirtPile& operator=(const DirtPile& dirtPile) = delete;
	DirtPile(DirtPile&& dirtPile) noexcept = delete;
	DirtPile& operator=(DirtPile&& dirtPile) noexcept = delete;

	// Update
	void Update(float elapsedSec);
	void Draw() const;

	// Getters / Setters
	const Rectf& GetHitbox() const;
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	bool GetCanBeDeleted() const;

	// Ohter
	void Hit(bool playerDir, GemsManager* gemsManager);

private:

	const float m_Scale;
	bool m_IsOnScreen;
	bool m_CanBeDeleted;

	TextureManager* m_pTextureManager;

	struct Dirt
	{
		Dirt()
			: drawRect{ Rectf{} }
			, srcRect{ Rectf{} }
			, velocity{ Vector2f{} }
			, existTime{ 0.f }
		{
		}
		Dirt(const Rectf& drawRect, const Rectf& srcRect, const Vector2f& velocity)
			: drawRect{ drawRect }
			, srcRect{ srcRect }
			, velocity{ velocity }
			, existTime{ 0.f }
		{
		}

		Rectf drawRect;
		Rectf srcRect;
		Vector2f velocity;
		float existTime;
	};

	int m_HitState;
	const int m_MaxHitState;

	const float m_GravityForce;

	bool m_IsHit;
	float m_HitTime;

	Animation* m_pBlinkOneAnimation;
	Animation* m_pBlinkTwoAnimation;
	Animation* m_pBlinkThreeAnimation;
	Animation* m_pBlinkFourAnimation;
	Animation* m_pBlinkFiveAnimation;

	Rectf m_Hitbox;
	Rectf m_SrcRect;

	Rectf m_DirtOneSrcRect;
	Rectf m_DirtTwoSrcRect;

	std::vector< Dirt* > m_pDirtPieces;

private:

	// Init
	void InitVariables();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();
	void DestroyDirt();

	// Update
	void UpdateAnimations(float elapsedSec);
	void UpdateDirt(float elapsedSec);
	void UpdateDeleteDirt(float elapsedSec);

	// Draw
	void DrawDirtPile() const;
	void DrawDirt() const;

	// Other
	void HitDirt(bool playerDir);
};