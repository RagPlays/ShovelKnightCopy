#include "pch.h"
#include "Bubble.h"
#include "Animation.h"
#include "TextureManager.h"

// constructors / desturtors
Bubble::Bubble(const Point2f& startPos, const Point2f& endPos, const float speed, TextureManager* textureManager)
	: m_StartPos{ startPos }
	, m_EndPos{ endPos }
	, m_Velocity{ 0.f, speed }
	, m_pTextureManager{ textureManager }
	, m_Scale { 3.5f }
	, m_Angle{ 0.f }
	, m_IsOnScreen{ false }
	, m_Hitbox{ startPos, 0.f, 0.f }
	, m_IsHit{ false }
	, m_PlayerKnockUpVelocity{ 1400 }
	, m_IsHitTime{ 0.f }
{
	InitAnimations();
	InitVariables();
}

Bubble::~Bubble()
{
	DestroyAnimations();
}

// Update / Draw
void Bubble::Update(float elapsedSec)
{
	if (!m_IsHit)
	{
		UpdateMovement(elapsedSec);
	}
	else
	{
		m_Hitbox.width = m_SrcRect.width * m_Scale;
		m_Hitbox.height = m_SrcRect.height * m_Scale;

		if (utils::GetClockTime(m_IsHitTime, elapsedSec, 0.3f))
		{
			// reset the position
			m_Hitbox.left = m_StartPos.x;
			m_Hitbox.bottom = m_StartPos.y;

			m_Hitbox.width = m_pBubblyAnimation->GetStartRect().width * m_Scale;
			m_Hitbox.height = m_pBubblyAnimation->GetStartRect().height * m_Scale;
			m_IsHit = false;
		}
	}
	UpdateAnimations(elapsedSec);
}

void Bubble::Draw() const
{
	m_pTextureManager->GetTexture("bubble")->Draw(m_Hitbox, m_SrcRect);
}

// Getters / Setters
const Rectf& Bubble::GetHitbox() const
{
	return m_Hitbox;
}

void Bubble::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool Bubble::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

float Bubble::GetPlayerKnockUp() const
{
	return m_PlayerKnockUpVelocity;
}

bool Bubble::GetIsHit() const
{
	return m_IsHit;
}

// Other
void Bubble::Hit()
{
	m_IsHit = true;
}

// Private Functions //
// Init
void Bubble::InitVariables()
{
	m_SrcRect.width = m_pBubblyAnimation->GetStartRect().width;
	m_SrcRect.height = m_pBubblyAnimation->GetStartRect().height;

	// drawRect
	m_Hitbox.width = m_SrcRect.width * m_Scale;
	m_Hitbox.height = m_SrcRect.height * m_Scale;

	// velocity
	m_Velocity.y = 100.f;
}

void Bubble::InitAnimations()
{
	Rectf startSrcRect{};

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 27.f;
	startSrcRect.width = 27.f;
	startSrcRect.height = 27.f;

	m_pBubblyAnimation = new Animation{ m_SrcRect, startSrcRect, 4, 5.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 59.f;
	startSrcRect.width = 32.f;
	startSrcRect.height = 32.f;

	m_PlopAnimation = startSrcRect;
}

// Destroy
void Bubble::DestroyAnimations()
{
	delete m_pBubblyAnimation;
	m_pBubblyAnimation = nullptr;
}

// Update
void Bubble::UpdateMovement(float elapsedSec)
{
	const float offsetX{ 20.f };
	// x
	//sin function
	m_Hitbox.left = m_StartPos.x + (sinf(m_Angle) * offsetX);
	m_Angle += 1.5f * elapsedSec;

	//y
	m_Hitbox.bottom += m_Velocity.y * elapsedSec;	// go up

	// checks
	if (m_Hitbox.bottom > m_EndPos.y)
	{
		m_Hitbox.bottom = m_EndPos.y;
	}
}

void Bubble::UpdateAnimations(float elapsedSec)
{
	if (m_IsHit)
	{
		m_SrcRect = m_PlopAnimation;
		m_pBubblyAnimation->SetCurrentFrame(1);
	}
	else
	{
		m_pBubblyAnimation->Play(elapsedSec);
	}
	
}