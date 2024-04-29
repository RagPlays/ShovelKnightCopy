#pragma once

class Animation;
class Texture;
class Player;
class GemsManager;
class SoundManager;
class TextureManager;

class Rock final
{
public:
	// Constructors / Destructors
	explicit Rock(const Point2f& rockPos, const int breakValue, bool rockType, TextureManager* textureManager);
	~Rock();

	Rock(const Rock& rock) = delete;
	Rock& operator=(const Rock& rock) = delete;
	Rock(Rock&& rock) noexcept  = delete;
	Rock& operator=(Rock&& rock) noexcept = delete;
	
	// Update / Draw
	bool Update(float elapsedSec, Player* player);
	void Draw() const;

	// Getters / Setters
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	bool GetIsBroken() const;
	const Rectf& GetHitbox() const;
	bool GetCanBeDeleted() const;
	const std::vector< Point2f >& GetVertices() const;

	// Other
	void Hit(GemsManager* gemsManager, Player* player, SoundManager* soundManager);

private:

	const float m_Scale;
	const bool m_RockType;
	const int m_BreakValue;
	const float m_PlayerKnockUp;

	TextureManager* m_pTextureManager;
	
	bool m_IsOnScreen;
	bool m_CanBeDeleted;
	bool m_IsBroken;
	bool m_IsOnRock;

	std::vector< Point2f > m_Vertices;
	Rectf m_Hitbox;

	Rectf m_BreakSrcRect;
	
	Animation* m_pBreakAnimation;

private:

	// Init 
	void InitAnimations();
	void InitVertices();

	// Destroy
	void DestroyAnimations();
	
	// Update
	void UpdateAnimation(float elapsedSec);
	void UpdateCollision(Player* player);
	void UpdateVerticalCollision(Player* player);
	void UpdateHorizontalCollision(Player* player);
	void UpdateIsOnRock(Player* player);

	// Draw
	void DrawTexture() const;
	void DrawVertices() const;
};