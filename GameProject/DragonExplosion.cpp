#include "pch.h"
#include "DragonExplosion.h"
#include "Animation.h"

// Constructors / Destructors
DragonExplosion::DragonExplosion(const Point2f& pos, const Texture* texture, const float delay)
	: m_DrawRect{ pos, 0.f, 0.f}
	, m_pTexture{ texture }
	, m_Scale{ 3.5f }
	, m_CanBeDeleted{ false }
	, m_Delay{ delay }
	, m_PastDelay{ false }
	, m_DelayTime{ }
{
	InitAnimations();
}

DragonExplosion::~DragonExplosion()
{
	DestroyAnimations();
}

// Update / Draw
void DragonExplosion::Update(float elapsedSec)
{
	UpdateAnimations(elapsedSec);
}

void DragonExplosion::Draw() const
{
	if (!m_CanBeDeleted)
	{
		DrawExplosion();
	}
}

// Getters
bool DragonExplosion::GetCanBeDeleted() const
{
	return m_CanBeDeleted;
}

// Private Functions //
// Init
void DragonExplosion::InitAnimations()
{
	Rectf startSrcRectf{ 0.f, m_pTexture->GetHeight(), m_pTexture->GetWidth() / 7.f, m_pTexture->GetHeight() };
	m_SrcRect = startSrcRectf;
	m_pAnimation = new Animation{ m_SrcRect, startSrcRectf, 7, 10.f };

	m_DrawRect.width = m_SrcRect.width * m_Scale;
	m_DrawRect.height = m_SrcRect.height * m_Scale;
}

// Destroy
void DragonExplosion::DestroyAnimations()
{
	delete m_pAnimation;
	m_pAnimation = nullptr;
}

// Update
void DragonExplosion::UpdateAnimations(float elapsedSec)
{
	if (!m_PastDelay)
	{
		if (utils::GetClockTime(m_DelayTime, elapsedSec, m_Delay))
		{
			m_PastDelay = true;
		}
		return;
	}

	if (!m_CanBeDeleted)
	{
		if (m_pAnimation->Play(elapsedSec))
		{
			m_CanBeDeleted = true;
		}
	}
}

// Draw
void DragonExplosion::DrawExplosion() const
{
	if (m_PastDelay)
	{
		m_pTexture->Draw(m_DrawRect, m_SrcRect);
	}
}