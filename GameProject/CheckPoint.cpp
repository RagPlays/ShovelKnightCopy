#include "pch.h"
#include "CheckPoint.h"
#include "Animation.h"
#include "Player.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Constructors / Destructors
CheckPoint::CheckPoint(const Point2f& position, TextureManager* textureManager)
	: m_BasicHitbox{ position, 0.f, 0.f }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_CheckPointState{ CheckPointState::idle }
	, m_IsActive{ false }
	, m_IsOnScreen{ false }
{
	InitAnimations();
	InitVariables();
}

CheckPoint::~CheckPoint()
{
	DestroyAnimations();
}

// Update / Draw
void CheckPoint::Update(float elapsedSec, Player* player, SoundManager* soundManager)
{
	if (!m_IsActive)
	{
		if (utils::IsOverlapping(player->GetHitBox(), m_BasicHitbox))
		{
			m_IsActive = true;
			soundManager->GetSoundEffect("checkpoint")->Play(false);
			m_CheckPointState = CheckPointState::lighting;
			player->SetRespawnPoint(Point2f{ m_BasicHitbox.left, m_BasicHitbox.bottom });
		}
	}
	UpdateAnimations(elapsedSec);
}

void CheckPoint::Draw() const
{
	if (!m_IsOnScreen)
	{
		return;
	}

	DrawGlass();
	DrawBasics();
}

// Getters / Setters
void CheckPoint::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool CheckPoint::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

const Rectf& CheckPoint::GetHitBox() const
{
	return m_BasicHitbox;
}

// Private Functions //
// Init
void CheckPoint::InitVariables()
{
	m_BasicHitbox.width = m_BasicSrcRect.width * m_Scale;
	m_BasicHitbox.height = m_BasicSrcRect.height * m_Scale;

	m_GlassHitBox.left = m_BasicHitbox.left + m_BasicHitbox.width * 0.2f;
	m_GlassHitBox.bottom = m_BasicHitbox.bottom + m_BasicHitbox.height * 0.55f;
	m_GlassHitBox.width = m_GlassSrcRect.width * m_Scale;
	m_GlassHitBox.height = m_GlassSrcRect.height * m_Scale;
}

void CheckPoint::InitAnimations()
{
	Rectf startSrcRect{};

	// checkpointsrcRect
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 94.f;
	startSrcRect.width = 35.f;
	startSrcRect.height = 54.f;
	m_BasicSrcRect = startSrcRect;

	// idle
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 20.f;
	startSrcRect.width = 21.f;
	startSrcRect.height = 20.f;
	m_GlassSrcRect = startSrcRect;
	m_IdleAnimation = startSrcRect;

	// lighting
	startSrcRect.left = 0.f;
	startSrcRect.bottom = 40.f;
	startSrcRect.width = 21.f;
	startSrcRect.height = 20.f;
	m_pLightingAnimation = new Animation{ m_GlassSrcRect, startSrcRect, 5, 5.f };

	// flaming
	startSrcRect.left = 21.f;
	startSrcRect.bottom = 20.f;
	startSrcRect.width = 21.f;
	startSrcRect.height = 20.f;
	m_pFlamingAnimation = new Animation{ m_GlassSrcRect, startSrcRect, 3, 5.f };
}

// Destroy
void CheckPoint::DestroyAnimations()
{
	// lighting
	delete m_pLightingAnimation;
	m_pLightingAnimation = nullptr;

	// flaming
	delete m_pFlamingAnimation;
	m_pFlamingAnimation = nullptr;

}

// Update
void CheckPoint::UpdateAnimations(float elapsedSec)
{
	if (m_CheckPointState == CheckPointState::idle)
	{
		m_GlassSrcRect = m_IdleAnimation;
	}
	else if (m_CheckPointState == CheckPointState::lighting)
	{
		if(m_pLightingAnimation->Play(elapsedSec))
		{
			m_CheckPointState = CheckPointState::flaming;
		}
	}
	else
	{
		m_pFlamingAnimation->Play(elapsedSec);
	}
}

// Draw
void CheckPoint::DrawBasics() const
{
	m_pTextureManager->GetTexture("checkPoint")->Draw(m_BasicHitbox, m_BasicSrcRect);
}

void CheckPoint::DrawGlass() const
{
	m_pTextureManager->GetTexture("checkPoint")->Draw(m_GlassHitBox, m_GlassSrcRect);
}