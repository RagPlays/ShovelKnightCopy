#pragma once

class Animation;
class Player;
class Texture;
class SoundManager;
class TextureManager;

class CheckPoint final
{
public:
	// Constructors / Destructors
	explicit CheckPoint(const Point2f& position, TextureManager* textureManager);
	~CheckPoint();

	CheckPoint(const CheckPoint& checkPoint) = delete;
	CheckPoint& operator=(const CheckPoint& checkPoint) = delete;
	CheckPoint(CheckPoint&& checkPoint) noexcept = delete;
	CheckPoint& operator=(CheckPoint&& checkPoint) noexcept = delete;

	// Update/ Draw
	void Update(float elapsedSec, Player* player, SoundManager* soundManager);
	void Draw() const;

	// Getters / Setters
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsOnScreen() const;
	const Rectf& GetHitBox() const;

private:

	const float m_Scale;
	bool m_IsOnScreen;
	bool m_IsActive;

	TextureManager* m_pTextureManager;

	Rectf m_BasicHitbox;
	Rectf m_GlassHitBox;

	Rectf m_BasicSrcRect;
	Rectf m_GlassSrcRect;

	// Animations
	Rectf m_IdleAnimation;
	Animation* m_pLightingAnimation;
	Animation* m_pFlamingAnimation;

	enum class CheckPointState
	{
		idle = 0,
		lighting,
		flaming
	};
	CheckPointState m_CheckPointState;

private:

	// Init
	void InitVariables();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();

	// Update
	void UpdateAnimations(float elapsedSec);

	// Draw
	void DrawBasics() const;
	void DrawGlass() const;
};