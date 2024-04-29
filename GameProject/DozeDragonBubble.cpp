#include "pch.h"
#include "DozeDragonBubble.h"
#include "Animation.h"
#include "Player.h"

// Constructors / Destructors
DozeDragonBubble::DozeDragonBubble(const Point2f& startPos, const Point2f& endPos)
	: m_StartPos{ startPos }
	, m_EndPos{ endPos }
	, m_Hitbox{ startPos, 0.f, 0.f }
	, m_GoingUp{ false }
	, m_Velocity{ -500.f, 100.f }
	, m_CanBeDeleted{ false }
	, m_IsHitTime{ 0.f }
	, m_IsHit{ false }
	, m_IsVisible{ false }
	, m_Scale{ 3.5f }
	, m_PlayerKnockUpSpeed{ 1000.f }
{
	InitAnimations();
}

DozeDragonBubble::~DozeDragonBubble()
{
	DestroyAnimations();
}

// Update / Draw
void DozeDragonBubble::Update(float elapsedSec, Player* player)
{
	if (m_IsVisible)
	{
		UpdateHitCollision(elapsedSec, player);
		UpdateHitbox();
		UpdateBubble(elapsedSec);
		UpdateGoingUp();
		UpdateAnimations(elapsedSec);
		UpdatePlayerCollision(elapsedSec, player);
	}
}

void DozeDragonBubble::Draw(const Texture* bubbleTexture)
{
	if (m_IsVisible)
	{
		bubbleTexture->Draw(m_Hitbox, m_SrcRect);
	}
}

// Getters / Setters
bool DozeDragonBubble::GetCanBeDeleted() const
{
	return m_CanBeDeleted;
}

void DozeDragonBubble::SetVisible()
{
	m_IsVisible = true;
}

bool DozeDragonBubble::GetIsOnScreen(const Rectf& cameraRect) const
{
	if (utils::IsOverlapping(m_Hitbox, cameraRect))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Private Functions //
// Init
void DozeDragonBubble::InitAnimations()
{
	Rectf startSrcRect{};

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 27.f;
	startSrcRect.width = 27.f;
	startSrcRect.height = 27.f;

	m_pBubbleAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 5.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 59.f;
	startSrcRect.width = 32.f;
	startSrcRect.height = 32.f;

	m_PopAnimation = startSrcRect;
}

// Destroy
void DozeDragonBubble::DestroyAnimations()
{
	delete m_pBubbleAnimation;
	m_pBubbleAnimation = nullptr;
}

// Update
void DozeDragonBubble::UpdateHitCollision(float elapsedSec, Player* player)
{
	if (m_IsHit)
	{
		return;
	}

	if (utils::IsOverlapping(m_Hitbox, player->GetAttackHitbox()))
	{
		if (player->GetIsJumpAttacking())
		{
			player->Knock(true, m_PlayerKnockUpSpeed);
		}
		m_IsHit = true;
	}
}

void DozeDragonBubble::UpdateGoingUp()
{
	if (!m_GoingUp)
	{
		if (m_Hitbox.left < m_EndPos.x)
		{
			m_GoingUp = true;
		}
	}
}

void DozeDragonBubble::UpdateBubble(float elapsedSec)
{
	if (m_GoingUp)
	{
		m_Hitbox.bottom += m_Velocity.y * elapsedSec;
	}
	else
	{
		m_Hitbox.left += m_Velocity.x * elapsedSec;
	}
}

void DozeDragonBubble::UpdateAnimations(float elapsedSec)
{
	if (m_IsHit)
	{
		m_SrcRect = m_PopAnimation;
		if (utils::GetClockTime(m_IsHitTime, elapsedSec, 0.5f))
		{
			m_CanBeDeleted = true;
		}
	}
	else
	{
		m_pBubbleAnimation->Play(elapsedSec);
	}
}

void DozeDragonBubble::UpdateHitbox()
{
	m_Hitbox.width = m_SrcRect.width * m_Scale;
	m_Hitbox.height = m_SrcRect.height * m_Scale;
}

void DozeDragonBubble::UpdatePlayerCollision(float elapsedSec, Player* player)
{
	if (m_IsHit || player->GetIsHit())
	{
		return;
	}

	if (utils::IsOverlapping(player->GetHitBox(), m_Hitbox))
	{
		m_IsHit = true;
		player->Hit();

		player->Knock(false, 500.f);
		player->Knock(true, 1000.f);
	}
}