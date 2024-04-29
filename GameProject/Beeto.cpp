#include "pch.h"
#include "Beeto.h"
#include "Animation.h"
#include "TextureManager.h"

// constructors / Destructors
Beeto::Beeto(const Point2f& startPos, bool enemyDir, TextureManager* textureManager)
	: Enemy{ startPos , enemyDir, 1, textureManager }
{
	InitVariables();
	InitAnimations();
}

Beeto::~Beeto()
{
	DestroyAnimations();
}

// Update / Draw
void Beeto::Update(float elapsedSec, const std::vector< std::vector< Point2f > >& vertices, const Rectf& playerHitbox, const std::vector< std::vector< Point2f > >& rockVertices)
{
	HandleCollision(vertices);
	HandleRockCollision(rockVertices);
	UpdateMovements(elapsedSec);
	UpdateAnimations(elapsedSec);
	UpdateIsHit(elapsedSec);
	UpdateDrawRect();
}

void Beeto::Draw() const
{
	DrawTexture();
	/*DrawDrawRect();
	DrawHitbox();
	DrawRays();*/
}

void Beeto::Hit(bool playerDir)
{
	if (!m_IsHit)
	{
		m_IsHit = true;
		m_IsDead = true;

		if (!m_PoefAnimationOn)
		{
			m_Velocity.y = m_GetHitVelocity;
			m_HitBox.bottom += 5.f;

			if (playerDir)	// right
			{
				m_Velocity.x = m_KnockBackVelocity;
			}
			else // left
			{
				m_Velocity.x = -m_KnockBackVelocity;
			}
		}
	}
}

float Beeto::JumpHit()
{
	if (!m_PoefAnimationOn)
	{
		m_IsDead = true;
		m_PoefAnimationOn = true;
		m_pPoefAnimation->SetCurrentFrame(1);
		m_SrcRect = m_pPoefAnimation->GetStartRect();
		return m_PlayerKnockUpVelocity;
	}
	return 0.f;
}

// Private functions //
// Init
void Beeto::InitVariables()
{
	// booleans
	m_PoefAnimationOn = false;

	// Set dimentions
	m_HitBox.width = m_pTextureManager->GetTexture("beeto")->GetWidth() / 4.f * m_Scale;
	m_HitBox.height = m_pTextureManager->GetTexture("beeto")->GetHeight() / 2.f * m_Scale;

	const float beetoSpeed{ 70.f };

	m_GetHitVelocity = 550.f;
	m_KnockBackVelocity = beetoSpeed;
	m_PlayerKnockUpVelocity = 1200.f;

	// set velocity
	if (m_EnemyDir == EnemyDir::left)
	{
		m_Velocity.x = -beetoSpeed;
	}
	else
	{
		m_Velocity.x = beetoSpeed;
	}
	
	m_KillValue = 5;
}

void Beeto::InitAnimations()
{
	Rectf startSrcRect{};

	// idle walking
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 15.f;
	startSrcRect.width = 26.f;
	startSrcRect.height = 15.f;
	m_pWalkAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 4.f };

	// dead
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 31.f;
	startSrcRect.width = 24.f;
	startSrcRect.height = 16.f;
	m_pDeadAnimation = new Animation{ m_SrcRect, startSrcRect, 2, 5.f };
}

// Destroy
void Beeto::DestroyAnimations()
{
	// walking
	delete m_pWalkAnimation;
	m_pWalkAnimation = nullptr;

	// dead
	delete m_pDeadAnimation;
	m_pDeadAnimation = nullptr;
}

// Update
void Beeto::UpdateMovements(float elapsedSec)
{
	// Update enemyDir
	if (m_Velocity.x < 0)
	{
		m_EnemyDir = EnemyDir::left;
	}
	else
	{
		m_EnemyDir = EnemyDir::right;
	}

	if (m_PoefAnimationOn)
	{
		m_Velocity.x = 0.f;
		m_Velocity.y = 0.f;
	}

	// gravity
	m_Velocity.y += m_Gravity * 0.9f * elapsedSec;

	// Update Position
	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
}

void Beeto::UpdateAnimations(float elapsedSec)
{
	if (m_IsDead && m_IsOnGround)
	{
		m_PoefAnimationOn = true;
	}

	if (m_PoefAnimationOn)
	{
		// Poef
		if (m_pPoefAnimation->Play(elapsedSec))
		{
			// after this animation this enemy can be deleted
			m_CanBeDelete = true;
		}
	}
	else if (m_IsDead && !m_IsOnGround)
	{
		m_pDeadAnimation->Play(elapsedSec);
		m_pPoefAnimation->SetCurrentFrame(1);
	}
	else
	{
		m_pWalkAnimation->Play(elapsedSec);
		m_pPoefAnimation->SetCurrentFrame(1);
		m_pDeadAnimation->SetCurrentFrame(1);
	}
}

void Beeto::UpdateDrawRect()
{
	m_DrawRect = Rectf{ m_HitBox.left - ((m_SrcRect.width * m_Scale - m_HitBox.width) / 2.f), m_HitBox.bottom, m_SrcRect.width * m_Scale, m_SrcRect.height * m_Scale };
}

void Beeto::UpdateIsHit(float elapsedSec)
{
	if (m_IsHit && utils::GetClockTime(m_IsHitTime, elapsedSec, 0.55f))
	{
		m_IsHit = false;
	}
}

// Draw
void Beeto::DrawTexture() const
{
	if (m_PoefAnimationOn)
	{
		m_pTextureManager->GetTexture("poef")->Draw(m_DrawRect, m_SrcRect);
	}
	else
	{
		glPushMatrix();
		{
			if (m_EnemyDir == EnemyDir::left)
			{
				glScalef(-1, 1, 1);
				glTranslatef((-m_DrawRect.left * 2.f) - m_DrawRect.width, 0.f, 0.f);
			}
			m_pTextureManager->GetTexture("beeto")->Draw(m_DrawRect, m_SrcRect);
		}
		glPopMatrix();
	}
}

void Beeto::DrawDrawRect() const
{
	// Draw DrawRect
	utils::SetRGBColor(30, 200, 30);
	utils::DrawRect(m_DrawRect);
}

void Beeto::DrawHitbox() const
{
	// Draw the hitbox
	utils::SetRGBColor(200, 30, 30);
	utils::DrawRect(m_HitBox);
}

void Beeto::DrawRays() const
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

// Handle
void Beeto::HandleCollision(const std::vector< std::vector< Point2f > >& vertices)
{
	HandleVerticalCollision(vertices);
	HandleHorizontalCollision(vertices);
}

void Beeto::HandleVerticalCollision(const std::vector< std::vector< Point2f > >& vertices)
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

void Beeto::HandleHorizontalCollision(const std::vector< std::vector< Point2f > >& vertices)
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

void Beeto::HandleRockCollision(const std::vector<std::vector<Point2f>>& rockVertices)
{
	if (!m_IsOnGround)
	{
		HandleVerticalCollision(rockVertices);
	}
	HandleHorizontalCollision(rockVertices);
}

void Beeto::HandleVerticalRockCollision(const std::vector<std::vector<Point2f>>& rockVertices)
{
	bool didCollision{ false };
	m_VerticalHitInfo = utils::HitInfo{};

	// vertical rays
	for (size_t idx{}; idx < rockVertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			if (utils::Raycast(rockVertices[idx], m_pVerticalRays[index]->pointOne, m_pVerticalRays[index]->pointTwo, m_VerticalHitInfo))
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
		}
	}
}

void Beeto::HandleHorizontalRockCollision(const std::vector<std::vector<Point2f>>& rockVertices)
{
	m_HorizontalHitInfo = utils::HitInfo{};	// reset hitinfo

	// horizontal rays
	for (size_t idx{}; idx < rockVertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pHorizontalRays.size(); ++index)
		{
			if (utils::Raycast(rockVertices[idx], m_pHorizontalRays[index]->pointOne, m_pHorizontalRays[index]->pointTwo, m_HorizontalHitInfo))
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