#include "pch.h"
#include "Blorp.h"
#include "Animation.h"
#include "TextureManager.h"

// Constructors
Blorp::Blorp(const Point2f& blorpDest, bool enemyDir, TextureManager* textureManager)
	: Enemy{ blorpDest, enemyDir, 2, textureManager }
{
	InitVariables();
	InitAnimations();
}

Blorp::~Blorp()
{
	DestroyAnimations();
}

// Update / Draw
void Blorp::Update(float elapsedSec, const std::vector< std::vector< Point2f > >& vertices, const Rectf& playerHitbox, const std::vector< std::vector< Point2f > >& rockVertices)
{
	HandleCollision(vertices);
	HandleRockCollision(rockVertices);
	UpdateMovement(elapsedSec, playerHitbox);
	UpdateAnimations(elapsedSec);
	UpdateIsHit(elapsedSec);
	UpdateDrawRect();
}

void Blorp::Draw() const
{
	DrawTexture();
	/*DrawDrawRect();
	DrawHitbox();
	DrawRays();*/
}

// Other
void Blorp::Hit(bool playerDir)
{
	if (m_IsHit || m_IsDead)
	{
		return;
	}

	m_IsHit = true;
	m_PlayingHitAnimation = true;

	if (playerDir) // right
	{
		m_Velocity.x = m_KnockBackVelocity;
	}
	else // left
	{
		m_Velocity.x = -m_KnockBackVelocity;
	}

	--m_HP;
	if (m_HP < 1)
	{
		m_IsDead = true;
	}
}

float Blorp::JumpHit()
{
	if (m_IsHit || m_IsDead)
	{
		return 0.0f;
	}

	m_IsHit = true;
	m_PlayingHitAnimation = true;

	--m_HP;
	if (m_HP < 1)
	{
		m_IsDead = true;
	}
	return m_PlayerKnockUpVelocity;
}


// private functions //
// Init
void Blorp::InitVariables()
{
	m_HitBox.width = m_pTextureManager->GetTexture("blorp")->GetWidth() / 4.f * m_Scale;
	m_HitBox.height = m_pTextureManager->GetTexture("blorp")->GetHeight() * m_Scale;

	m_WalkSpeed = 50.f;

	m_GetHitVelocity = 0.f;
	m_KnockBackVelocity = 90.f;
	m_PlayerKnockUpVelocity = 1500.f;

	// set velocity
	if (m_EnemyDir == EnemyDir::left)
	{
		m_Velocity.x = -m_WalkSpeed;
	}
	else
	{
		m_Velocity.x = m_WalkSpeed;
	}


	m_SmallJumpSpeedX = m_WalkSpeed;
	m_SmallJumpSpeedY = 400.f;

	m_JumpSpeedX = 300.f;
	m_JumpSpeedY = 500.f;

	m_Jumping = false;
	m_JumpTimer = 0.f;

	m_KillValue = 15;
}

void Blorp::InitAnimations()
{
	Rectf startSrcRect{};

	// idle
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 11.f;
	startSrcRect.width = 20.f;
	startSrcRect.height = 11.f;
	m_pWalkAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 6.f };
}

// Destroy
void Blorp::DestroyAnimations()
{
	// walk animation
	delete m_pWalkAnimation;
	m_pWalkAnimation = nullptr;
}

void Blorp::UpdateMovement(float elapsedSec, const Rectf& playerHitbox)
{
	if (m_IsDead)
	{
		return;
	}

	if (m_Jumping)
	{
		if (m_IsOnGround)
		{
			m_Jumping = false;

			// do big jump
			m_Velocity.y = m_JumpSpeedY;
			m_HitBox.bottom += 5.f;

			if (m_EnemyDir == EnemyDir::left)
			{
				m_Velocity.x = -m_JumpSpeedX;
			}
			else
			{
				m_Velocity.x = m_JumpSpeedX;
			}
			
		}
	}
	else
	{
		if (m_IsOnGround)
		{
			if (m_EnemyDir == EnemyDir::left)
			{
				m_Velocity.x = -m_WalkSpeed;
			}
			else
			{
				m_Velocity.x = m_WalkSpeed;
			}
		}
	}

	if (!m_IsHit && !m_Jumping)
	{
		if (m_IsOnGround)
		{
			// move the blorp
			if (utils::GetClockTime(m_JumpTimer, elapsedSec, 5.f))
			{
				m_Jumping = true;
				m_Velocity.y = m_SmallJumpSpeedY;
				m_HitBox.bottom += 5.f;
				if(m_HitBox.left > (playerHitbox.left + playerHitbox.width / 2.f))	// if enemy is at the right of the player
				{
					m_Velocity.x = -m_SmallJumpSpeedX;
					m_EnemyDir = EnemyDir::left;
				}
				else
				{
					m_Velocity.x = m_SmallJumpSpeedX;
					m_EnemyDir = EnemyDir::right;
				}
			}
		}
		else
		{
			m_JumpTimer = 0.f;
		}
	}

	// gravity
	m_Velocity.y += m_Gravity * elapsedSec;

	// Update Pos
	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
}

// Update
void Blorp::UpdateIsHit(float elapsedSec)
{
	if (!m_IsHit)
	{
		return;
	}

	if (utils::GetClockTime(m_IsHitTime, elapsedSec, 0.4f))
	{
		m_IsHit = false;
	}
}

void Blorp::UpdateAnimations(float elapsedSec)
{
	if (m_IsDead)
	{
		m_PoefAnimationOn = true;
	}

	if (!m_PoefAnimationOn)
	{
		m_pWalkAnimation->Play(elapsedSec);
	}
	else
	{
		if (m_pPoefAnimation->Play(elapsedSec))
		{
			m_CanBeDelete = true;
		}
	}
}

void Blorp::UpdateDrawRect()
{
	m_DrawRect = Rectf{ m_HitBox.left - ((m_SrcRect.width * m_Scale - m_HitBox.width) / 2.f), m_HitBox.bottom, m_SrcRect.width * m_Scale, m_SrcRect.height * m_Scale };
}

// Draw
void Blorp::DrawTexture() const
{
	if (m_PoefAnimationOn)
	{
		m_pTextureManager->GetTexture("poef")->Draw(m_DrawRect, m_SrcRect);
	}
	else
	{
		m_pTextureManager->GetTexture("blorp")->Draw(m_DrawRect, m_SrcRect);
	}
}

void Blorp::DrawDrawRect() const
{
	// Draw DrawRect
	utils::SetRGBColor(30, 200, 30);
	utils::DrawRect(m_DrawRect);
}

void Blorp::DrawHitbox() const
{
	// Draw the hitbox
	utils::SetRGBColor(200, 30, 30);
	utils::DrawRect(m_HitBox);
}

void Blorp::DrawRays() const
{
	// Draw Rays
	utils::SetRGBColor(100, 100, 240);

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
}

// Other
void Blorp::HandleCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	HandleVerticalCollision(vertices);
	HandleHorizontalCollision(vertices);
}

void Blorp::HandleVerticalCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	bool didCollision{ false };
	m_VerticalHitInfo = utils::HitInfo{};

	// vertical rays
	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			if (utils::Raycast(vertices[idx], m_pVerticalRays[index]->pointOne, m_pVerticalRays[index]->pointTwo, m_VerticalHitInfo))
			{
				if (!didCollision)	// so it does it only once
				{
					didCollision = true;
					if (m_VerticalHitInfo.lambda < 0.5f)
					{
						// on ground
						m_Velocity.y = 0.f;
						m_HitBox.bottom = m_VerticalHitInfo.intersectPoint.y;
					}
					else
					{
						// hit the your head
						m_Velocity.y = 0.f;
						m_HitBox.bottom = m_VerticalHitInfo.intersectPoint.y - m_HitBox.height - 2.f;
					}
				}
			}
			else
			{
				m_Velocity.x *= -1;
			}
		}
	}
}

void Blorp::HandleHorizontalCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	m_HorizontalHitInfo = utils::HitInfo{};	// reset hitinfo

	// horizontal rays
	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pHorizontalRays.size(); ++index)
		{
			if (utils::Raycast(vertices[idx], m_pHorizontalRays[index]->pointOne, m_pHorizontalRays[index]->pointTwo, m_HorizontalHitInfo))
			{
				// turn //
				const float offset{ 1.f };
				if (m_HorizontalHitInfo.lambda < 0.5f)
				{
					// turn to right
					//std::cout << "Turn to Right!\n";
					m_HitBox.left += offset;
					m_Velocity.x = abs(m_Velocity.x);
				}

				if (m_HorizontalHitInfo.lambda > 0.5f)
				{
					// turn to left
					//std::cout << "Turn to Left!\n";
					m_HitBox.left -= offset;
					m_Velocity.x = -abs(m_Velocity.x);
				}
				return;
			}
		}
	}
}

void Blorp::HandleRockCollision(const std::vector< std::vector< Point2f > >& rockVertices)
{
	HandleVerticalRockCollision(rockVertices);
	HandleHorizontalRockCollision(rockVertices);
}

void Blorp::HandleVerticalRockCollision(const std::vector< std::vector< Point2f > >& rockVertices)
{
	m_VerticalHitInfo = utils::HitInfo{};

	// vertical rays
	for (size_t idx{}; idx < rockVertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			if (utils::Raycast(rockVertices[idx], m_pVerticalRays[index]->pointOne, m_pVerticalRays[index]->pointTwo, m_VerticalHitInfo))
			{
				if (m_VerticalHitInfo.lambda < 0.5f)
				{
					// on ground
					m_Velocity.y = 0.f;
					m_HitBox.bottom = m_VerticalHitInfo.intersectPoint.y;
				}
				else
				{
					// hit the your head
					m_Velocity.y = 0.f;
					m_HitBox.bottom = m_VerticalHitInfo.intersectPoint.y - m_HitBox.height - 2.f;
				}
				return;
			}
		}
	}
}

void Blorp::HandleHorizontalRockCollision(const std::vector< std::vector< Point2f > >& rockVertices)
{
	m_HorizontalHitInfo = utils::HitInfo{};	// reset hitinfo
	const float offset{ 1.f };

	// horizontal rays
	for (size_t idx{}; idx < rockVertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pHorizontalRays.size(); ++index)
		{
			if (utils::Raycast(rockVertices[idx], m_pHorizontalRays[index]->pointOne, m_pHorizontalRays[index]->pointTwo, m_HorizontalHitInfo))
			{
				// turn //
				if (m_HorizontalHitInfo.lambda < 0.5f)
				{
					// turn to right
					//std::cout << "Turn to Right!\n";
					m_HitBox.left += offset;
					m_Velocity.x = abs(m_Velocity.x);
				}

				if (m_HorizontalHitInfo.lambda > 0.5f)
				{
					// turn to left
					//std::cout << "Turn to Left!\n";
					m_HitBox.left -= offset;
					m_Velocity.x = -abs(m_Velocity.x);
				}
				return;
			}
		}
	}
}