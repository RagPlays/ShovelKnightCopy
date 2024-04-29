#pragma once

class Animation;
class Texture;
class TextureManager;

class Enemy
{
public:
	// Constructors / Destructors
	explicit Enemy(const Point2f& enemyPos, bool enemyDir, int hp, TextureManager* textureManager);
	virtual ~Enemy();

	Enemy(const Enemy& enemy) = delete;
	Enemy& operator=(const Enemy& enemy) = delete;
	Enemy(Enemy&& enemy) noexcept = delete;
	Enemy& operator=(Enemy&& enemy) noexcept = delete;

	// Update / Draw
	virtual void Update(float elapsedSec, const std::vector< std::vector< Point2f > >& vertices, const Rectf& playerHitbox, const std::vector< std::vector< Point2f > >& rockVertices) = 0;
	void UpdateRays();
	virtual void Draw() const = 0;

	// Getters / Setters
	const Rectf& GetHitBox() const;
	bool GetIsOnScreen() const;
	void SetIsOnScreen(bool isOnScreen);
	bool GetIsDead() const;
	bool GetCanBeDeleted() const;
	int GetKillValue() const;
	bool GetIsPoefAnimationOn() const;
	bool GetValueIsGiven() const;
	void SetValueIsGiven(bool valueIsGiven);

	// Other
	virtual void Hit(bool playerDir) = 0;
	virtual float JumpHit() = 0;

	// Handling
	void HandleIsOnGround(const std::vector< std::vector< Point2f > >& vertices);

protected:
	const float m_Gravity;
	const float m_Scale;

	TextureManager* m_pTextureManager;

	enum class EnemyDir
	{
		left = 0,
		right
	};
	EnemyDir m_EnemyDir;

	int m_KillValue;
	bool m_ValueIsGiven;

	// Enemy stats
	bool m_IsOnScreen;		// indictaes if the enemy is visible on the gamescreen
	bool m_IsOnGround;		// indicates if the enemy is on the ground or not
	bool m_IsHit;			// indictates if the enemy is hit by player
	float m_IsHitTime;		// time after that enemy is hit by player
	bool m_IsDead;			// indicates if the enemy is dead or not
	bool m_CanBeDelete;		// indicates that everything is done and the obj can be deleted
	int m_HP;				// the remaining hp of the enemy
	Vector2f m_Velocity;	// the velocity of the enemy

	float m_GetHitVelocity;		
	float m_KnockBackVelocity;
	float m_PlayerKnockUpVelocity;

	Animation* m_pPoefAnimation;
	bool m_PoefAnimationOn;

	Rectf m_HitBox;
	Rectf m_DrawRect;
	Rectf m_SrcRect;

	utils::HitInfo m_VerticalHitInfo;
	utils::HitInfo m_HorizontalHitInfo;
	std::vector< Rayf* > m_pVerticalRays;
	std::vector< Rayf* > m_pHorizontalRays;

private:
	void InitRays();
	void InitAnimations();
	void DestroyRays();
};