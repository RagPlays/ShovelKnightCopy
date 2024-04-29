#pragma once

class Texture;
class Animation;
class Player;
class DozeDragonBubbles;
class DragonExplosion;
class TextureManager;

class DozeDragon final
{
public:
	explicit DozeDragon(const Point2f& dragonPos, TextureManager* textureManager);
	~DozeDragon();

	DozeDragon(const DozeDragon& dragon) = delete;
	DozeDragon& operator=(const DozeDragon& dragon) = delete;
	DozeDragon(DozeDragon&& dragon) noexcept = delete;
	DozeDragon& operator=(DozeDragon&& dragon) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, Player* player, const Rectf& cameraRect);
	void Draw() const;

	// Getters / Setters
	const Rectf& GetOnCameraHitbox() const;
	const Rectf& GetHeadHitbox() const;
	const Rectf& GetBodyHitbox() const;
	bool GetIsOnScreen() const;
	void SetIsOnScreen(bool isOnScreen);
	bool GetCanBeDeleted() const;

private:
	const float m_Scale;
	const float m_PlayerKnockUpHead;
	const float m_PlayerKnockUpBody;

	TextureManager* m_pTextureManager;

	float m_MoveSpeed;
	bool m_StartCycle;
	bool m_IsOnScreen;
	bool m_CanBeDeleted;
	bool m_IsDead;
	bool m_DragonCanMove;
	bool m_IsHit;
	bool m_GotAttacked;
	bool m_HasAttacked;
	bool m_ExplosionsAreDone;

	int m_HP;
	float m_IsHitTime;

	enum class DragonSteps
	{
		stepOne = 0,
		stepTwo,
		stepThree,
	};
	DragonSteps m_DragonSteps;

	enum class DragonState
	{
		idle = 0,
		movingLeft,
		movingRight,
		attacking
	};
	DragonState m_DragonState;

	Animation* m_pIdleAnimation;
	Animation* m_pWalkBackwardAnimation;
	Animation* m_pWalkForwardAnimation;
	Animation* m_pBubbleAttackAnimation;

	Rectf m_SrcRect;
	Rectf m_DrawRect;

	Rectf m_HeadHitbox;
	Rectf m_BodyHitbox;

	std::vector< DozeDragonBubbles* > m_pDragonBubbles;

	std::vector < DragonExplosion* > m_pDragonExplosions;

private:
	// Init
	void InitVariables();
	void InitAnimations();
	void InitExplosions();

	// Destroy
	void DestroyAnimations();
	void DestroyBubbles();
	void DestroyExplosions();

	// Update
	void UpdateAnimations(float elapsedSec, const Rectf& cameraRect);
	void UpdateHitboxes();
	void UpdateDrawRect(float elapsedSec);
	void UpdatePlayerHit(Player* player);
	void UpdateCheckHit(Player* player);
	void UpdateIsHit(float elapsedSec);
	void UpdateBubbles(float elapsedSec, Player* player);

	// Draw
	void DrawDragonTexture() const;
	void DrawBubbles() const;
	void DrawDrawRect() const;
	void DrawHitboxes() const;
	void DrawExplosions() const;

	// Other
	void HitDragon();
	void DragonAttack();
};