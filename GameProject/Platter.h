#pragma once

class Texture;
class Player;
class SoundManager;
class TextureManager;

class Platter final
{
public:
	// Constructors / Destructors
	explicit Platter(const Point2f& platerPos, TextureManager* textureManager);
	~Platter();

	Platter(const Platter& platter) = delete;
	Platter& operator=(const Platter& platter) = delete;
	Platter(Platter&& platter) noexcept = delete;
	Platter& operator=(Platter&& platter) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, Player* player, SoundManager* soundManager);
	void Draw() const;

	// Other
	void Hit(Player* player, SoundManager* soundManager);

	// Getters / Setters
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	bool GetCanBeDeleted() const;
	const Rectf& GetHitbox() const;

private:

	const float m_Scale;
	const float m_GravityForce;

	bool m_ChickenPickedUp;

	TextureManager* m_pTextureManager;

	struct Chicken
	{
		Chicken(const Rectf& hitbox)
			: hitbox{ hitbox }
			, existTime{ 0.f }
		{
		}
		Rectf hitbox;
		float existTime;
	};
	Chicken* m_pChicken;

	Rectf m_StartHitbox;

	Rectf m_TopHitbox;
	Rectf m_PlateHitbox;

	Rectf m_TopSrcRect;
	Rectf m_PlateSrcRect;

	Vector2f m_TopVector;
	Vector2f m_PlateVector;

	bool m_IsOnScreen;
	bool m_IsBroken;
	bool m_CanBeDeleted;

private:

	// Init
	void InitPlatter();
	void InitChicken();

	// Destroy
	void DestroyChicken();

	// Update
	void UpdateMovement(float elapsedSec);
	void UpdatePlayerInteraction(float elapsedSec, Player* player, SoundManager* soundManager);
};