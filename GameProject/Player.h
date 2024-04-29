#pragma once

class Texture;
class Animation;
class Level;
class SoundManager;
class TextureManager;

class Player final
{
public:
	// Constructors / Destructors
	explicit Player(const Point2f& startPos, SoundManager* soundManager, TextureManager* textureManager);
	~Player();
	
	Player(const Player& player) = delete;
	Player& operator=(const Player& player) = delete;
	Player(Player&& player) noexcept = delete;
	Player& operator=(Player&& player) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, Level* level, const Rectf& cameraRect);
	void Draw() const;

	// Getters / Setters
	const Rectf& GetHitBox();
	void SetIsCrouching(bool isCrouching);
	void SetCanBoostJump(bool canBoostJump);
	void SetUnPressedJump(bool unPressedJump);
	void SetIsOnLadderRect(bool isOnLadderRect);
	void SetRespawnPoint(const Point2f& respawnPoint);
	void SetLadderLockPosition(float ladderLockPos);
	bool GetIsOnLadder() const;
	bool GetIsHit() const;
	bool GetDir() const;
	bool GetIsJumpAttacking() const;
	Rectf GetAttackHitbox() const;
	int GetPlayerHP() const;
	void ChangePosition(bool moveDir, float distance);
	void SetPlayerVelocity(bool moveDir, float speed);
	void SetPlayerLeft(float left);
	void SetPlayerBottom(float bottom);
	void SetIsOnPlatform(bool isOnPlatform);
	void SetIsOnRock(bool isOnRock);
	void SetHP(int hp);
	int GetMaxHP() const;
	const std::vector< Rayf* >& GetVerticalRays();
	const std::vector< Rayf* >& GetHorizontalRays();

	// Other
	void Move(const Vector2f& moveDir, float elapsedSec);
	void Jump();
	void Attack();
	/*0 = x, 1 = y*/
	void Knock(bool direction, float knockSpeed);
	void Hit();

private:

	const float m_Scale;
	const int m_MaxHP;
	const Vector2f m_Acceleration;
	const Vector2f m_Deceleration;
	const Point2f m_MaxSpeed;

	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;

	bool m_UnPressedJump;
	bool m_CanJump;
	bool m_CanBoostJump;
	bool m_CanAttack;
	bool m_FallSoundHasPlayed;

	bool m_KnockBack;
	float m_knockBackTime;

	int m_HP;
	bool m_IsDead;
	bool m_IsDirectDead;

	Point2f m_RespawnPoint;
	Rectf m_Hitbox;	  // Hitbox for the player
	Rectf m_PlayerJumpHitbox;	// hitbox when doing jump attack

	Rectf m_DrawRect; // rect where to draw the animation texture
	Rectf m_SrcRect;  // player srcRect getting from animation class

	// Velocity
	Vector2f m_Velocity;

	Rectf m_AttackHitBox;
	Rectf m_JumpAttackHitBox;
	bool m_AttackIsUpdated;		// to make sure that if i use these hitboxes that they are updated
	bool m_JumpAttackIsUpdated;

	bool m_IsHit;
	float m_IsHitTime;

	bool m_IsVisible;
	float m_IsVisibleTime;
	bool m_HitAnimationOn;
	float m_HitAnimationTime;

	// animations //
	// idle
	Animation* m_pIdleAnimation;
	// crouching
	Rectf m_CrouchingAnimation;
	// walking
	Animation* m_pWalkAnimation;
	// Turning
	Rectf m_TurningAnimation;
	// Jump
	Rectf m_JumpUpAnimation;
	Rectf m_JumpDownAnimation;
	Rectf m_JumpAttackAnimation;
	// Attacking
	Animation* m_pAttackingAnimation;
	// Hit
	Rectf m_HitAnimation;
	// Climbing
	Rectf m_ClimbAnimation;
	Rectf m_EndClimbAnimation;
	Animation* m_pClimbAttackAnimation;
	// Dead
	Animation* m_pDeadAnimation;


	/// Collision ///  (yeppie..(sarcasm)) 
	// Ray = 8byte = same as a pointer -> pointer is not handy
	// What is Rayf? is a ray.... i made it in struct.h and struct.cpp!
	utils::HitInfo m_VerticalHitInfo;
	utils::HitInfo m_HorizontalHitInfo;
	std::vector< Rayf* > m_pVerticalRays;
	std::vector< Rayf* > m_pHorizontalRays;

	// look direction
	enum class PlayerDirectionX	// which diriction the player wants to go (wich keyboard press) , or last went
	{
		left,
		right,
	};
	PlayerDirectionX m_PlayerDirX;

	bool m_IsTurning;
	bool m_IsAttacking;
	bool m_IsJumpAttacking;
	bool m_IsCrouching;
	bool m_IsRespawning;

	float m_LadderLockPos;
	bool m_IsOnGround;
	bool m_IsOnPlatform;
	bool m_IsOnLadder;
	bool m_IsOnLadderRect;
	bool m_StandOnLadder;
	bool m_NeedToHandelLadderCol;
	unsigned int m_CurrentLadderRectIdx;
	bool m_IsOnRock;

	float m_TurnTime;
	float m_BoostJumpTime;
	float m_ClimbTurnTime;
	float m_RespawningTime;

private:
	// Init
	void InitVariables();
	void InitRays();
	void InitAnimations();

	// Destroy
	void DestroyAnimations();
	void DestroyRays();

	// Update
	void UpdateMovement(float elapsedSec, const std::vector< LadderRectf* >& ladderRects);
	void UpdateVelocity(float elapsedSec);
	void UpdateAnimations(float elapsedSec, const std::vector< LadderRectf* >& ladderRects);
	void UpdateNormalAnimations(float elapsedSec);
	void UpdateLadderAnimations(float elapsedSec, const std::vector< LadderRectf* >& ladderRects);
	void UpdateAttackRects();
	void UpdateRays();
	void UpdateDrawRect();
	void UpdateIsHit(float elapsedSec);
	void UpdateKnockBack(float elapsedSec);
	void UpdateJumpHitbox();
	void UpdateDeadRects(const std::vector< Rectf > deadRects);

	// Draw
	void DrawTexture() const;
	void DrawDrawRect() const;
	void DrawHitbox() const;
	void DrawRays() const;
	void DrawAttackRects() const;
	
	// Handles
	void HandleCollision(const std::vector< std::vector< Point2f > >& vertices);
	void HandleVerticalCollision(const std::vector< std::vector< Point2f > >& vertices);
	void HandleHorizontalCollision(const std::vector< std::vector< Point2f > >& vertices);
	void UpdateIsOnGround(const std::vector< std::vector< Point2f > >& vertices);
	void HandleLadderRects(const std::vector< LadderRectf* >& ladderRects, const Rectf& cameraRect);
	void HandleLadderCollision(const std::vector< LadderRectf* >& ladderRects);

	// Movement
	void NormalMove(float elapsedSec, const Vector2f& moveDir);
	void LadderMove(float elapsedSec, const Vector2f& moveDir);

	// Other
	bool IsOnAGround();
	void RespawnPlayer();
};