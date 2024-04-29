#include "pch.h"
#include "Player.h"
#include "Animation.h"
#include "Level.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Constructors / Destructors
Player::Player(const Point2f& startPos, SoundManager* soundManager, TextureManager* textureManager)
	: m_Hitbox{ startPos, 0.f, 0.f }
	, m_pSoundManager{ soundManager }
	, m_pTextureManager{ textureManager }
	, m_PlayerDirX{ PlayerDirectionX::right }
	, m_Acceleration{ 2400.f, /*jumping is instant, no elapsedSec*/900.f }
	, m_Deceleration{ /*walking*/ 1100.f, /*gravity*/ -4000.f }
	, m_MaxSpeed{ 365.f/*normal: 320*/, 5000.f}
	, m_Scale{ 3.5f }
	, m_CurrentLadderRectIdx{ MAXUINT }
	, m_MaxHP{ 8 }
	, m_CanAttack{ true }
	, m_UnPressedJump{ true }
	, m_IsOnPlatform{ false }
	, m_StandOnLadder{ false }
	, m_NeedToHandelLadderCol{ true }
	, m_IsOnRock{ false }
{
	std::cout << "\nCreating Player...\n";
	InitVariables();
	InitAnimations();
	InitRays();
}

Player::~Player()
{
	std::cout << "\nDestroying Player...\n";

	DestroyAnimations();
	DestroyRays();
}

// Update / Draw
void Player::Update(float elapsedSec, Level* level, const Rectf& cameraRect)
{
	UpdateDeadRects(level->GetDeadRects());
	UpdateIsOnGround(level->GetVertices());
	UpdateIsHit(elapsedSec);
	UpdateKnockBack(elapsedSec);
	UpdateMovement(elapsedSec, level->GetLadderRects());
	UpdateRays();
	UpdateJumpHitbox();
	HandleCollision(level->GetVertices());
	HandleLadderRects(level->GetLadderRects(), cameraRect);
	HandleLadderCollision(level->GetLadderRects());
	UpdateAnimations(elapsedSec, level->GetLadderRects()); 
	UpdateAttackRects();
	UpdateDrawRect();

	//std::cout << "IsOnGround: " << std::boolalpha << IsOnGround() << "\n";
	//std::cout << "CanJump: " << std::boolalpha << m_CanJump << "\n";
	//std::cout << "CanBoostJump: " << std::boolalpha << m_CanBoostJump << "\n";
	//std::cout << "OnLadderRect: " << std::boolalpha << m_IsOnLadderRect << "\n";
	//std::cout << "BoostJumpTime: " << m_BoostJumpTime << "\n";
	//std::cout << "IsDead: " << std::boolalpha << m_IsDead << "\n";
	//std::cout << "CanAttack: " << std::boolalpha << m_CanAttack << "\n";
	//std::cout << "IsOnPlatform: " << std::boolalpha << m_IsOnPlatform << "\n";
	//std::cout << "IsAttacking: " << std::boolalpha << m_IsAttacking << "\n";
}

void Player::Draw() const
{
	DrawTexture();
	if (false)
	{
		DrawDrawRect();
		DrawHitbox();
		DrawRays();
		DrawAttackRects();
	}
}

// Getters / Setters
const Rectf& Player::GetHitBox()
{
	if (!m_IsJumpAttacking)
	{
		return m_Hitbox;
	}
	else
	{
		return m_PlayerJumpHitbox;
	}
}

void Player::SetIsCrouching(bool isCrouching)
{
	m_IsCrouching = isCrouching;
}

void Player::SetCanBoostJump(bool canBoostJump)
{
	m_CanBoostJump = canBoostJump;
}

void Player::SetUnPressedJump(bool unPressedJump)
{
	m_UnPressedJump = unPressedJump;
}

void Player::SetIsOnLadderRect(bool isOnLadderRect)
{
	m_IsOnLadderRect = isOnLadderRect;
}

void Player::SetRespawnPoint(const Point2f& respawnPoint)
{
	m_RespawnPoint = respawnPoint;
}

void Player::SetLadderLockPosition(float ladderLockPos)
{
	m_LadderLockPos = ladderLockPos;
}

bool Player::GetIsOnLadder() const
{
	return m_IsOnLadder;
}

bool Player::GetIsHit() const
{
	return m_IsHit;
}

bool Player::GetDir() const
{
	if (m_PlayerDirX == PlayerDirectionX::left)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Player::GetIsJumpAttacking() const
{
	return m_IsJumpAttacking;
}

Rectf Player::GetAttackHitbox() const
{
	if (!m_HitAnimationOn)
	{
		if (m_IsAttacking)
		{
			return m_AttackHitBox;
		}
		else if (m_IsJumpAttacking)
		{
			return m_JumpAttackHitBox;
		}
	}
	return Rectf{ 0.f, 0.f, 0.f, 0.f };
}

int Player::GetPlayerHP() const
{
	return m_HP;
}

void Player::ChangePosition(bool moveDir, float distance)
{
	if (moveDir) // y
	{
		m_Hitbox.bottom += distance;
	}
	else // x
	{
		m_Hitbox.left += distance;
	}

}

void Player::SetPlayerVelocity(bool moveDir, float speed)
{
	if (moveDir) // y
	{
		m_Velocity.y = speed;
	}
	else // x
	{
		m_Velocity.x = speed;
	}
}

void Player::SetPlayerLeft(float left)
{
	m_Hitbox.left = left;
}

void Player::SetPlayerBottom(float bottom)
{
	m_Hitbox.bottom = bottom;
}

void Player::SetIsOnPlatform(bool isOnPlatform)
{
	m_IsOnPlatform = isOnPlatform;
}

void Player::SetIsOnRock(bool isOnRock)
{
	m_IsOnRock = isOnRock;
}

void Player::SetHP(int hp)
{
	m_HP = hp;
}

int Player::GetMaxHP() const
{
	return m_MaxHP;
}

const std::vector<Rayf*>& Player::GetVerticalRays()
{
	return m_pVerticalRays;
}

const std::vector<Rayf*>& Player::GetHorizontalRays()
{
	return m_pHorizontalRays;
}

// Move
void Player::Move(const Vector2f& moveDir, float elapsedSec)
{
	if (moveDir.y < 0.f)
	{
		m_NeedToHandelLadderCol = false;
	}
	else
	{
		m_NeedToHandelLadderCol = true;
	}

	if (m_KnockBack || m_IsDead)
	{
		return;
	}

	if (!m_IsOnLadder)
	{
		NormalMove(elapsedSec, moveDir);
	}
	else
	{
		LadderMove(elapsedSec, moveDir);
	}
}

void Player::Jump()
{
	if (m_IsDead)
	{
		return;
	}

	if (IsOnAGround() && m_UnPressedJump)
	{
		m_pSoundManager->GetSoundEffect("jump")->Play(false);

		m_CanBoostJump = true;
		m_BoostJumpTime = 0.f;
		m_Velocity.y = m_Acceleration.y;
		if (m_IsOnPlatform)
		{
			m_Hitbox.bottom += 5.f;
		}
		else
		{
			m_Hitbox.bottom += 2.f;
		}
	}

	if (m_IsOnLadder)
	{
		m_IsOnLadder = false;
		Knock(true, 200);
	}
}

void Player::Attack()
{
	if (!m_IsAttacking)
	{
		m_pSoundManager->GetSoundEffect("attack")->Play(false);
		m_IsJumpAttacking = false;
		m_IsAttacking = true;
	}
}

void Player::Knock(bool direction /*0 = x, 1 = y*/, float knockSpeed)
{
	if (m_IsDead)
	{
		return;
	}

	if (direction)	// knockBack in y
	{
		if (knockSpeed)
		{
			m_Velocity.y = knockSpeed;
			m_Hitbox.bottom += 5.f;
		}	
	}
	else		// knockBack in x
	{
		m_KnockBack = true;
		if (m_PlayerDirX == PlayerDirectionX::right)
		{
			m_Velocity.x = -knockSpeed;
		}
		else
		{
			m_Velocity.x = knockSpeed;
		}
	}
}

void Player::Hit()
{
	if (!m_IsDead && !m_IsHit)
	{
		m_IsHit = true;
		--m_HP;
		m_HitAnimationOn = true;

		if (m_HP <= 0)
		{
			m_IsDead = true;
		}

		m_pSoundManager->GetSoundEffect("gethit")->Play(false);
	}
}

// Private functions //
// Init
void Player::InitVariables()
{
	m_Velocity.x = 0;
	m_Velocity.y = 0;

	m_SrcRect.left = 0.f;
	m_SrcRect.bottom = 0.f;
	m_SrcRect.width = 0.f;
	m_SrcRect.height = 0.f;

	// Hitbox
	m_Hitbox.width = 14.f * m_Scale;
	m_Hitbox.height = 26.f * m_Scale;

	// AttackHitbox
	m_AttackHitBox.width = m_Hitbox.width * 2.f;
	m_AttackHitBox.height = m_Hitbox.height * 0.95f;

	// JumpAttackHitbox
	const int shovelpixelHeight{ 9 };
	m_JumpAttackHitBox.width = m_Hitbox.width * 0.7f;
	m_JumpAttackHitBox.height = shovelpixelHeight * m_Scale;

	// respawn point
	m_RespawnPoint.x = m_Hitbox.left;
	m_RespawnPoint.y = m_Hitbox.bottom;
	m_IsRespawning = false;
	m_RespawningTime = 0.f;
	
	// stats
	m_HP = m_MaxHP;
	m_IsDead = false;
	m_IsDirectDead = false;

	// Defaults
	m_CanBoostJump = false;
	m_CanJump = true;
	m_UnPressedJump = true;

	m_IsHit = false;
	m_IsHitTime = 0.f;

	m_IsVisible = true;
	m_IsVisibleTime = 0.f;

	m_KnockBack = false;
	m_knockBackTime = 0.f;

	m_IsOnLadder = false;
	m_IsOnLadderRect = false;

	m_IsTurning = false;
	m_IsJumpAttacking = false;
	m_IsAttacking = false;
	m_IsCrouching = false;
	m_HitAnimationOn = false;

	m_TurnTime = 0.f;
	m_BoostJumpTime = 0.f;
	m_HitAnimationTime = 0.f;
	m_ClimbTurnTime = 0.f;
}

void Player::InitRays()
{
	m_pVerticalRays.resize(3);
	m_pHorizontalRays.resize(3);

	// vertical
	m_pVerticalRays[0] = new Rayf{};
	m_pVerticalRays[1] = new Rayf{};
	m_pVerticalRays[2] = new Rayf{};

	// horizontal
	m_pHorizontalRays[0] = new Rayf{};
	m_pHorizontalRays[1] = new Rayf{};
	m_pHorizontalRays[2] = new Rayf{};
}

void Player::InitAnimations()
{
	// maybe loading this from file?
	Rectf startSrcRect{};

	// idle animation
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_SrcRect = startSrcRect;
	m_pIdleAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 15.f, 8.f };

	// Crouching
	startSrcRect.left = 160.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_CrouchingAnimation = startSrcRect;

	// walking animation
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 80.f;
	startSrcRect.width = 50.f;
	startSrcRect.height = 40.f;
	m_pWalkAnimation = new Animation{ m_SrcRect, startSrcRect, 6, 12.f };

	// turning
	startSrcRect.left = 300.f;
	startSrcRect.bottom = 80.f;
	startSrcRect.width = 50.f;
	startSrcRect.height = 40.f;
	m_TurningAnimation = startSrcRect;

	// Jumping //
	
	// Up
	startSrcRect.left = 240.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_JumpUpAnimation = startSrcRect;

	// Down
	startSrcRect.left = 280.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_JumpDownAnimation = startSrcRect;
	
	// Attacks //
	
	// Shovel down
	startSrcRect.left = 320.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_JumpAttackAnimation = startSrcRect;

	// normal
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 120.f;
	startSrcRect.width = 80.f;
	startSrcRect.height = 40.f;
	m_pAttackingAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 12.f };

	// Hit Animation //
	startSrcRect.left = 360.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_HitAnimation = startSrcRect;

	// Ladder animations //
	// idle
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 160.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_ClimbAnimation = startSrcRect;

	// endClimb
	startSrcRect.left = 40.f;
	startSrcRect.bottom = 160.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_EndClimbAnimation = startSrcRect;

	// climbAttack
	startSrcRect.left = 80.f;
	startSrcRect.bottom = 160.f;
	startSrcRect.width = 80.f;
	startSrcRect.height = 40.f;
	m_pClimbAttackAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 10.f };

	// dead //
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 200.f;
	startSrcRect.width = 40.f;
	startSrcRect.height = 40.f;
	m_pDeadAnimation = new Animation{ m_SrcRect, startSrcRect, 3, 1.f, 0.f, 3.f };

}

// Destroy
void Player::DestroyAnimations()
{
	// idle
	delete m_pIdleAnimation;
	m_pIdleAnimation = nullptr;

	// crouching

	// walking
	delete m_pWalkAnimation;
	m_pWalkAnimation = nullptr;

	// attacks
	delete m_pAttackingAnimation;
	m_pAttackingAnimation = nullptr;

	// climbing
	delete m_pClimbAttackAnimation;
	m_pClimbAttackAnimation = nullptr;

	// dead
	delete m_pDeadAnimation;
	m_pDeadAnimation = nullptr;
}

void Player::DestroyRays()
{
	// vertical
	for (size_t idx{}; idx < m_pVerticalRays.size(); ++idx)
	{
		delete m_pVerticalRays[idx];
	}
	m_pVerticalRays.clear();

	// horizontal
	for (size_t idx{}; idx < m_pHorizontalRays.size(); ++idx)
	{
		delete m_pHorizontalRays[idx];
	}
	m_pHorizontalRays.clear();
}

// Update
void Player::UpdateMovement(float elapsedSec, const std::vector< LadderRectf* >& ladderRects)
{
	if (m_IsDead && IsOnAGround())
	{
		return;
	}

	// Update BoostJump
	if (m_CanBoostJump)
	{
		if (utils::GetClockTime(m_BoostJumpTime, elapsedSec, 0.13f))
		{
			m_CanBoostJump = false;
		}
	}
	else
	{
		m_BoostJumpTime = 0.f;
	}

	// Update the velocity
	UpdateVelocity(elapsedSec);

	// Update Pos
	const float offset{ 30.f };
	// x
	if (!utils::AreFloatsEqual(m_Velocity.x, 0.f, offset))
	{
		m_Hitbox.left += m_Velocity.x * elapsedSec;
	}
	// y
	if (!utils::AreFloatsEqual(m_Velocity.y, 0.f, offset))
	{
		m_Hitbox.bottom += m_Velocity.y * elapsedSec;
	}
	
	if (m_IsOnLadder)
	{
		m_Hitbox.left = m_LadderLockPos;
		m_Velocity.y = 0.f;
		if (!m_IsOnLadderRect)
		{
			m_IsOnLadder = false;
		}
	}

	if (IsOnAGround())
	{
		if (!m_FallSoundHasPlayed)
		{
			m_FallSoundHasPlayed = true;
			m_pSoundManager->GetSoundEffect("fall")->Play(false);
		}
	}
	else
	{
		m_FallSoundHasPlayed = false;
	}
}

void Player::UpdateVelocity(float elapsedSec)
{
	// BoostJump
	if (m_CanBoostJump)
	{
		m_Velocity.y += 5000.f * elapsedSec;
	}

	// Update gravity //
	if (!m_IsOnLadder)
	{
		m_Velocity.y += m_Deceleration.y * elapsedSec;
	}

	// deceleration
	if (m_Velocity.x > 0.f)
	{
		// decelerate
		m_Velocity.x -= m_Deceleration.x * elapsedSec;

		if (m_Velocity.x < 0.f)
		{
			m_Velocity.x = 0.f;
		}
	}
	else if (m_Velocity.x < 0.f)
	{
		// decelerate
		m_Velocity.x += m_Deceleration.x * elapsedSec;

		if (m_Velocity.x > 0.f)
		{
			m_Velocity.x = 0.f;
		}
	}

	if (!m_KnockBack)
	{
		// check max speed
		// x
		if (m_Velocity.x < -m_MaxSpeed.x)
		{
			m_Velocity.x = -m_MaxSpeed.x;
		}
		else if (m_Velocity.x > m_MaxSpeed.x)
		{
			m_Velocity.x = m_MaxSpeed.x;
		}
		// y
		if (m_Velocity.y < -m_MaxSpeed.y)
		{
			m_Velocity.y = -m_MaxSpeed.y;
		}
		else if (m_Velocity.y > m_MaxSpeed.y)
		{
			m_Velocity.y = m_MaxSpeed.y;
		}
	}
}

void Player::UpdateAnimations(float elapsedSec, const std::vector< LadderRectf* >& ladderRects)
{
	if (m_IsRespawning)
	{
		if (utils::GetClockTime(m_RespawningTime, elapsedSec, 3.f))
		{
			RespawnPlayer();
		}
		return;
	}
	if (m_IsDead)
	{
		if (m_IsDirectDead)
		{
			m_IsRespawning = true;
			m_IsVisible = false;
		}
		else
		{
			if (m_pDeadAnimation->Play(elapsedSec))
			{
				RespawnPlayer();
			}
		}
		return;
	}

	if (m_IsOnLadder)
	{
		UpdateLadderAnimations(elapsedSec, ladderRects);
	}
	else
	{
		UpdateNormalAnimations(elapsedSec);
	}
}

void Player::UpdateNormalAnimations(float elapsedSec)
{
	// offsets to avoid jittering
	const float offsetVelocityX{ 80.f };

	// update turnings
	if (!m_IsTurning)
	{
		if (m_PlayerDirX == PlayerDirectionX::left && m_Velocity.x > 0.f)
		{
			m_IsTurning = true;
		}
		else if (m_PlayerDirX == PlayerDirectionX::right && m_Velocity.x < 0.f)
		{
			m_IsTurning = true;
		}
	}
	else
	{
		if (utils::GetClockTime(m_TurnTime, elapsedSec, 0.16f))
		{
			m_IsTurning = false;
		}
	}

	// reset crouching
	if (m_IsCrouching)
	{
		m_IsJumpAttacking = false;
		// if player is moving in y
		if (!IsOnAGround())
		{
			m_IsCrouching = false;
		}
	}

	// reset jumpattacking
	if (IsOnAGround())
	{
		m_IsJumpAttacking = false;
	}

	// settting variables // 
	if (m_HitAnimationOn)
	{
		if (utils::GetClockTime(m_HitAnimationTime, elapsedSec, 0.47f))
		{
			m_HitAnimationOn = false;
		}
		m_SrcRect = m_HitAnimation;
		m_pIdleAnimation->SetCurrentFrame(1);
		m_pWalkAnimation->SetCurrentFrame(1);
		return;
	}

	// actual animations // 
	if (m_IsAttacking)
	{
		m_pIdleAnimation->SetCurrentFrame(1);
		m_pWalkAnimation->SetCurrentFrame(1);

		if (m_pAttackingAnimation->Play(elapsedSec))
		{
			m_CanAttack = true;
			m_pAttackingAnimation->SetCurrentFrame(1);
			m_IsAttacking = false;
		}
	}
	else if (m_IsJumpAttacking)
	{
		m_CanAttack = true;
		m_pAttackingAnimation->SetCurrentFrame(1);
		m_pIdleAnimation->SetCurrentFrame(1);
		m_pWalkAnimation->SetCurrentFrame(1);

		m_SrcRect = m_JumpAttackAnimation;
	}
	else if (IsOnAGround())	// if on ground
	{
		if (m_IsTurning)
		{
			m_pAttackingAnimation->SetCurrentFrame(1);
			m_pIdleAnimation->SetCurrentFrame(1);
			m_pWalkAnimation->SetCurrentFrame(1);

			m_SrcRect = m_TurningAnimation;
		}
		else if (!utils::AreFloatsEqual(m_Velocity.x, 0.f, offsetVelocityX))  // walk animation
		{
			m_pAttackingAnimation->SetCurrentFrame(1);
			m_pIdleAnimation->SetCurrentFrame(1);

			m_pWalkAnimation->Play(elapsedSec);
		}
		else  // idle animation
		{
			m_pAttackingAnimation->SetCurrentFrame(1);
			m_pWalkAnimation->SetCurrentFrame(1);

			if (m_IsCrouching)
			{
				m_SrcRect = m_CrouchingAnimation;
			}
			else
			{
				m_pIdleAnimation->Play(elapsedSec);
			}
		}
	}
	else
	{
		m_pAttackingAnimation->SetCurrentFrame(1);
		m_pIdleAnimation->SetCurrentFrame(1);
		m_pWalkAnimation->SetCurrentFrame(1);

		if (m_Velocity.y < 0.f)
		{
			m_SrcRect = m_JumpDownAnimation;
		}
		else
		{
			m_SrcRect = m_JumpUpAnimation;
		}
	}
}

void Player::UpdateLadderAnimations(float elapsedSec, const std::vector< LadderRectf* >& ladderRects)
{
	m_IsJumpAttacking = false;
	
	if (m_IsAttacking)
	{
		if (m_pClimbAttackAnimation->Play(elapsedSec))
		{
			m_IsAttacking = false;
		}
	}
	else
	{
		Rectf ladder{ ladderRects[m_CurrentLadderRectIdx]->rect };
		ladder.bottom += ladder.height;
		ladder.height = ladder.width / 8.f;
		ladder.bottom += ladder.height * 8.f;

		if (utils::IsOverlapping(m_Hitbox, ladder))
		{
			m_IsAttacking = false;
			m_SrcRect = m_EndClimbAnimation;
		}
		else
		{
			m_SrcRect = m_ClimbAnimation;
		}
	}
}

void Player::UpdateAttackRects()
{
	m_AttackIsUpdated = false;
	m_JumpAttackIsUpdated = false;

	if (m_IsAttacking)
	{
		// update the attack rect
		if (m_PlayerDirX == PlayerDirectionX::left)
		{
			m_AttackHitBox.left = m_Hitbox.left - m_AttackHitBox.width;
			m_AttackHitBox.bottom = m_Hitbox.bottom;
		}
		else // right
		{
			m_AttackHitBox.left = m_Hitbox.left + m_Hitbox.width;
			m_AttackHitBox.bottom = m_Hitbox.bottom;
		}

		m_AttackIsUpdated = true;
	}
	else if (m_IsJumpAttacking)
	{
		if (m_PlayerDirX == PlayerDirectionX::left)	// left
		{
			// update jump attack shovel rect
			m_JumpAttackHitBox.left = m_Hitbox.left + m_Hitbox.width - m_JumpAttackHitBox.width;
			m_JumpAttackHitBox.bottom = m_Hitbox.bottom;
		}
		else  // right
		{
			// update jump attack shovel rect
			m_JumpAttackHitBox.left = m_Hitbox.left;
			m_JumpAttackHitBox.bottom = m_Hitbox.bottom;
		}
		m_JumpAttackIsUpdated = true;
	}
}

void Player::UpdateRays()
{
	const float offset{ 1.f };

	Rectf currentHitbox{ GetHitBox() };

	// vertical rays
	if (m_pVerticalRays.size() != 3)
	{
		std::cerr << "ERROR::PLAYER::CPP::VERTICAL_PLAYER_RAYS_ARE_NOT_INIT_RIGHT!\n";
	}
	else
	{
		/// Ray 1 ///	LEFT
		// left under
		m_pVerticalRays[0]->pointOne = Point2f{ currentHitbox.left + offset * 5.f, currentHitbox.bottom  - offset };
		// left upper
		m_pVerticalRays[0]->pointTwo = Point2f{ currentHitbox.left + offset * 5.f, currentHitbox.bottom + currentHitbox.height + offset };

		/// Ray 2 ///	RIGHT
		// right under
		m_pVerticalRays[1]->pointOne = Point2f{ currentHitbox.left + currentHitbox.width - offset * 5.f, currentHitbox.bottom - offset };
		// right upper
		m_pVerticalRays[1]->pointTwo = Point2f{ currentHitbox.left + currentHitbox.width - offset * 5.f, currentHitbox.bottom + currentHitbox.height + offset };

		/// Ray 3 ///	MID
		// middle under
		m_pVerticalRays[2]->pointOne = Point2f{ currentHitbox.left + currentHitbox.width / 2.f , currentHitbox.bottom - offset };
		//middle upper
		m_pVerticalRays[2]->pointTwo = Point2f{ currentHitbox.left + currentHitbox.width / 2.f, currentHitbox.bottom + currentHitbox.height + offset };
	}

	// horizontal rays
	if (m_pHorizontalRays.size() != 3)
	{
		std::cerr << "ERROR::PLAYER::CPP::HORIZONTAL_PLAYER_RAYS_ARE_NOT_INIT_RIGHT!\n";
	}
	else
	{
		/// Ray 1 ///	TOP
		// left upper
		m_pHorizontalRays[0]->pointOne = Point2f{ currentHitbox.left - offset, currentHitbox.bottom + currentHitbox.height - offset * 5.f };
		// right upper
		m_pHorizontalRays[0]->pointTwo = Point2f{ currentHitbox.left + currentHitbox.width + offset , currentHitbox.bottom + currentHitbox.height - offset * 5.f };

		/// Ray 2 ///	BOT
		// left under
		m_pHorizontalRays[1]->pointOne = Point2f{ currentHitbox.left - offset, currentHitbox.bottom + offset * 5.f };
		// right under
		m_pHorizontalRays[1]->pointTwo = Point2f{ currentHitbox.left + currentHitbox.width + offset , currentHitbox.bottom + offset * 5.f };

		/// Ray 2 ///	MID
		// left middle
		m_pHorizontalRays[2]->pointOne = Point2f{ currentHitbox.left - offset, currentHitbox.bottom + currentHitbox.height / 2.f };
		// right middle
		m_pHorizontalRays[2]->pointTwo = Point2f{ currentHitbox.left + currentHitbox.width + offset, currentHitbox.bottom + currentHitbox.height / 2.f };
	}
}

void Player::UpdateDrawRect()
{
	m_DrawRect = Rectf{ m_Hitbox.left - ((m_SrcRect.width * m_Scale - m_Hitbox.width) / 2.f), m_Hitbox.bottom, m_SrcRect.width * m_Scale, m_SrcRect.height * m_Scale };

	if (m_IsAttacking)
	{
		m_DrawRect.bottom -= 3.f * m_Scale; // -3 pixels * m_Scale;
	}
	else if (m_IsDead)
	{
		m_DrawRect.bottom -= 5.f * m_Scale; // -5 pixels * m_Scale;
	}
}

void Player::UpdateIsHit(float elapsedSec)
{
	if (!m_IsHit || m_IsDead)
	{
		return;
	}

	if (utils::GetClockTime(m_IsVisibleTime, elapsedSec, 0.05f))
	{
		m_IsVisible = m_IsVisible ? false : true;
	}

	if (utils::GetClockTime(m_IsHitTime, elapsedSec, 2.f))
	{
		m_IsHit = false;
		m_IsVisible = true;
		m_IsVisibleTime = 0.f;
	}
}

void Player::UpdateKnockBack(float elapsedSec)
{
	if (!m_KnockBack)
	{
		return;
	}

	if (utils::GetClockTime(m_knockBackTime, elapsedSec, 0.5f))
	{
		m_KnockBack = false;
	}
}

void Player::UpdateJumpHitbox()
{
	const float offset{ m_Hitbox.height / 2.8f };
	m_PlayerJumpHitbox = Rectf{ m_Hitbox.left, m_Hitbox.bottom + offset, m_Hitbox.width, m_Hitbox.height - offset };
}

void Player::UpdateDeadRects(const std::vector<Rectf> deadRects)
{
	if (m_IsDirectDead || m_IsRespawning)
	{
		return;
	}

	for(size_t idx{}; idx < deadRects.size(); ++idx)
	{
		if (utils::IsOverlapping(GetHitBox(), deadRects[idx]))
		{
			m_IsDead = true;
			m_IsDirectDead = true;
			m_IsVisible = false;
			m_HP = 0;
		}
	}
}

// Draw
void Player::DrawTexture() const
{
	if (!m_IsVisible)
	{
		return;
	}

	glPushMatrix();
	{
		if (m_PlayerDirX == PlayerDirectionX::left)
		{
			glScalef(-1, 1, 1);
			glTranslatef((-m_DrawRect.left * 2.f) - m_DrawRect.width, 0.f, 0.f);
		}
		m_pTextureManager->GetTexture("player")->Draw(m_DrawRect, m_SrcRect);
	}
	glPopMatrix();
}

void Player::DrawDrawRect() const
{
	// draw TextureDrawRect
	utils::SetRGBColor(30, 200, 30);
	utils::DrawRect(m_DrawRect);
}

void Player::DrawHitbox() const
{
	utils::SetRGBColor(200, 30, 30);
	if (!m_IsJumpAttacking)
	{
		// draw hitBox
		utils::DrawRect(m_Hitbox);
	}
	else
	{
		// draw jumpattackhitbox
		utils::DrawRect(m_PlayerJumpHitbox);
	}
}

void Player::DrawRays() const
{
	// lines
	utils::SetRGBColor(100, 100, 240); // light blue

	// vertical
	for (size_t idx{}; idx < m_pVerticalRays.size(); ++idx)
	{
		utils::DrawLine(m_pVerticalRays[idx]->pointOne, m_pVerticalRays[idx]->pointTwo);
	}
	// horizontal
	for (size_t idx{}; idx < m_pHorizontalRays.size(); ++idx)
	{
		utils::DrawLine(m_pHorizontalRays[idx]->pointOne, m_pHorizontalRays[idx]->pointTwo);
	}

	// intersetionPoints
}

void Player::DrawAttackRects() const
{
	if (!m_HitAnimationOn)
	{
		if (m_IsAttacking)
		{
			utils::SetRGBColor(252, 181, 3);
			utils::DrawRect(m_AttackHitBox, 2.f);
		}
		else if (m_IsJumpAttacking)
		{
			utils::SetRGBColor(252, 181, 3);
			utils::DrawRect(m_JumpAttackHitBox, 2.f);
		}
	}
}

// Other
void Player::HandleCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	HandleVerticalCollision(vertices);
	HandleHorizontalCollision(vertices);
}

void Player::HandleVerticalCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	// vertical rays
	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			if (utils::Raycast(vertices[idx], m_pVerticalRays[index]->pointOne, m_pVerticalRays[index]->pointTwo, m_VerticalHitInfo))
			{
				m_Velocity.y = 0.f;
				if (m_VerticalHitInfo.lambda < 0.5f)
				{
					m_Hitbox.bottom = m_VerticalHitInfo.intersectPoint.y + 1.f;
					return;
				}
				m_CanBoostJump = false;
				m_Hitbox.bottom = m_VerticalHitInfo.intersectPoint.y - m_Hitbox.height - 1.5f;
				return;
			}
		}
	}
}

void Player::HandleHorizontalCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	const float offset{ 1.75f };
	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pHorizontalRays.size(); ++index)
		{
			if (utils::Raycast(vertices[idx], m_pHorizontalRays[index]->pointOne, m_pHorizontalRays[index]->pointTwo, m_HorizontalHitInfo))
			{
				m_Velocity.x = 0.f;
				if (m_HorizontalHitInfo.lambda < 0.5f)
				{
					m_Hitbox.left = m_HorizontalHitInfo.intersectPoint.x + offset;
					return;
				}
				m_Hitbox.left = m_HorizontalHitInfo.intersectPoint.x - m_Hitbox.width - offset;
				return;
			}
		}
	}
}

void Player::UpdateIsOnGround(const std::vector< std::vector< Point2f > >& vertices)
{
	for(size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			// bottom
			Point2f pointOne{ m_pVerticalRays[index]->pointOne.x, m_pVerticalRays[index]->pointOne.y - 2.f };
			// top
			Point2f pointTwo{ m_pVerticalRays[index]->pointTwo.x, m_pVerticalRays[index]->pointTwo.y };

			if (utils::Raycast(vertices[idx], pointOne, pointTwo, m_VerticalHitInfo))
			{
				if (m_VerticalHitInfo.lambda < 0.5f)
				{
					m_IsOnGround = true;
					return;
				}
			}
		}
	}
	m_IsOnGround = false;
}

void Player::HandleLadderRects(const std::vector< LadderRectf* >& ladderRects, const Rectf& cameraRect)
{
	// UpdateIsOnScreen
	for (size_t idx{}; idx < ladderRects.size(); ++idx)
	{
		if (utils::IsOverlapping(ladderRects[idx]->rect, cameraRect))
		{
			ladderRects[idx]->isOnScreen = true;
		}
		else
		{
			ladderRects[idx]->isOnScreen = false;
		}
	}

	m_IsOnLadderRect = false;

	for (size_t idx{}; idx < ladderRects.size(); ++idx)
	{
		if (!ladderRects[idx]->isOnScreen)
		{
			continue;
		}
		if (utils::IsOverlapping(ladderRects[idx]->rect, GetHitBox()))
		{
			m_CurrentLadderRectIdx = unsigned int(idx);
			m_IsOnLadderRect = true;
			m_LadderLockPos = ladderRects[idx]->rect.left + ladderRects[idx]->rect.width / 2.f - m_Hitbox.width / 2.f;
			return;
		}
	}
}

bool Player::IsOnAGround()
{
	return m_IsOnGround || m_IsOnPlatform || m_StandOnLadder || m_IsOnRock;
}

void Player::RespawnPlayer()
{
	m_IsRespawning = false;
	m_IsDead = false;
	m_IsDirectDead = false;
	m_IsVisible = true;
	m_IsAttacking = false;
	m_IsJumpAttacking = false;
	m_HP = m_MaxHP;
	m_Hitbox.left = m_RespawnPoint.x;
	m_Hitbox.bottom = m_RespawnPoint.y;
	m_Velocity.x = m_Velocity.y = 0.f;
	m_IsHit = false;
	m_HitAnimationOn = false;
}

void Player::NormalMove(float elapsedSec, const Vector2f& moveDir)
{
	// move in X
	if (m_IsAttacking)
	{
		if (!IsOnAGround())
		{
			if (moveDir.x < 0.f)	// want to go left
			{
				m_Velocity.x = -m_MaxSpeed.x;
			}
			else if (moveDir.x > 0.f)	// want to go right
			{
				m_Velocity.x = m_MaxSpeed.x;
			}
		}	
	}
	else
	{
		if (moveDir.x < 0.f)	// want to go left
		{
			m_PlayerDirX = PlayerDirectionX::left;
			if (IsOnAGround())
			{
				m_Velocity.x -= m_Acceleration.x * elapsedSec;
			}
			else
			{
				m_Velocity.x = -m_MaxSpeed.x;
			}
		}
		else if (moveDir.x > 0.f)	// want to go right
		{
			m_PlayerDirX = PlayerDirectionX::right;
			if (IsOnAGround())
			{
				m_Velocity.x += m_Acceleration.x * elapsedSec;
			}
			else
			{
				m_Velocity.x = m_MaxSpeed.x;
			}
		}
	}

	// Move in y
	if (moveDir.y < 0.f) // want to crouch
	{
		if (IsOnAGround())
		{
			if (!m_IsAttacking)
			{
				if (IsOnAGround())	// because is on ground not reliable
				{
					m_IsCrouching = true;
				}
			}
		}
		else
		{
			if (!utils::AreFloatsEqual(m_Velocity.y, 0.f, 60.f))	// needs to have a decent veloccity before player can attack
			{
				m_IsJumpAttacking = true;
			}
		}
	}


	if (m_IsOnLadderRect)
	{
		if (moveDir.y > 0.f)
		{
			if (IsOnAGround())
			{
				m_Hitbox.bottom += 5.f;
			}
			m_IsOnLadder = true;
		}

		if (moveDir.y < 0.f)
		{
			if (!IsOnAGround())
			{
				m_IsOnLadder = true;
			}
		}
	}
}

void Player::LadderMove(float elapsedSec, const Vector2f& moveDir)
{
	// in y
	if (moveDir.y > 0.f) // going up
	{
		m_Velocity.y = m_MaxSpeed.x * 0.6f;
	}
	else if (moveDir.y < 0.f) // going down
	{
		m_Velocity.y = -m_MaxSpeed.x * 0.6f;
	}
	// in x
	if (moveDir.x < 0.f) // going left
	{
		m_PlayerDirX = PlayerDirectionX::left;
	}
	else if (moveDir.x > 0.f) // going right
	{
		m_PlayerDirX = PlayerDirectionX::right;
	}

	if (moveDir.y != 0.f)	// update direction on ladder
	{
		if (utils::GetClockTime(m_ClimbTurnTime, elapsedSec, 0.24f))
		{
			if (m_PlayerDirX == PlayerDirectionX::left)
			{
				m_PlayerDirX = PlayerDirectionX::right;
			}
			else
			{
				m_PlayerDirX = PlayerDirectionX::left;
			}
		}
	}

	if (IsOnAGround())
	{
		m_IsOnLadder = false;
	}
}

void Player::HandleLadderCollision(const std::vector< LadderRectf* >& ladderRects)
{
	m_StandOnLadder = false;

	if (!m_NeedToHandelLadderCol || m_IsOnLadder || m_Velocity.y > 0.f)
	{
		return;
	}

	for (size_t idx{}; idx < ladderRects.size(); ++idx)
	{
		if (ladderRects[idx]->isOnScreen)
		{
			std::vector< Point2f > ladderVertices{};
			const Rectf collisionRectf{ ladderRects[idx]->rect };
			const float offsetHeight{ 56.f };

			ladderVertices.emplace_back(Point2f{collisionRectf.left, collisionRectf.bottom + collisionRectf.height - offsetHeight});
			ladderVertices.emplace_back(Point2f{collisionRectf.left + collisionRectf.width, collisionRectf.bottom + collisionRectf.height - offsetHeight});
			ladderVertices.emplace_back(Point2f{collisionRectf.left + collisionRectf.width, collisionRectf.bottom + collisionRectf.height });
			ladderVertices.emplace_back(Point2f{collisionRectf.left, collisionRectf.bottom + collisionRectf.height });
			
			utils::HitInfo hitInfo{};

			for (size_t index{}; index < m_pVerticalRays.size(); ++index)
			{
				if (utils::Raycast(ladderVertices, m_pVerticalRays[index]->pointOne, m_pVerticalRays[index]->pointTwo, hitInfo))
				{
					if (hitInfo.lambda < 0.5f)
					{
						m_Hitbox.bottom = hitInfo.intersectPoint.y + 1.f;
						m_StandOnLadder = true;
						return;
					}
				}
			}
		}
	}
}