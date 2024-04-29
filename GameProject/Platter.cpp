#include "pch.h"
#include "Platter.h"
#include "Player.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Constructors / Destructors
Platter::Platter(const Point2f& platerPos, TextureManager* textureManager)
	: m_TopHitbox{ platerPos, 0.f, 0.f}
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_CanBeDeleted{ false }
	, m_IsOnScreen{ false }
	, m_IsBroken{ false }
	, m_GravityForce{ -2500.f }
	, m_TopVector{ 150.f, 820.f }
	, m_PlateVector{ -150.f, 850.f }
	, m_ChickenPickedUp{ false }
{
	InitPlatter();
	InitChicken();
}

Platter::~Platter()
{
	DestroyChicken();
}

// Update / Draw
void Platter::Update(float elapsedSec, Player* player, SoundManager* soundManager)
{
	if (m_IsBroken)
	{
		if (utils::GetClockTime(m_pChicken->existTime, elapsedSec, 7.f))
		{
			m_ChickenPickedUp = true;
		}
	}

	UpdateMovement(elapsedSec);
	UpdatePlayerInteraction(elapsedSec, player, soundManager);

	if(m_IsBroken && m_ChickenPickedUp)
	{
		m_CanBeDeleted = true;
	}
}

void Platter::Draw() const
{
	if (!m_IsOnScreen)
	{
		return;
	}

	if (m_IsBroken)
	{
		m_pTextureManager->GetTexture("platterBreak")->Draw(m_TopHitbox, m_TopSrcRect);
		m_pTextureManager->GetTexture("platterBreak")->Draw(m_PlateHitbox, m_PlateSrcRect);

		if (!m_ChickenPickedUp)
		{
			m_pTextureManager->GetTexture("chicken")->Draw(m_pChicken->hitbox);
		}
	}
	else
	{
		m_pTextureManager->GetTexture("platter")->Draw(m_StartHitbox);
	}
}

// Other
void Platter::Hit(Player* player, SoundManager* soundManager)
{
	if (m_IsBroken)
	{
		return;
	}

	if (player->GetIsJumpAttacking())
	{
		player->Knock(true, 1500.f);
	}
	else
	{
		m_IsBroken = true;
		
		soundManager->GetSoundEffect("platter")->Play(false);
	}
}

// Getters / Setters
void Platter::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool Platter::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

bool Platter::GetCanBeDeleted() const
{
	return m_CanBeDeleted;
}

const Rectf& Platter::GetHitbox() const
{
	return m_StartHitbox;
}

// Private functions //
// Init
void Platter::InitPlatter()
{
	m_TopHitbox.width = m_pTextureManager->GetTexture("platter")->GetWidth() * m_Scale;
	m_TopHitbox.height = m_pTextureManager->GetTexture("platter")->GetHeight() * m_Scale;

	m_StartHitbox = m_TopHitbox;
	m_PlateHitbox = m_TopHitbox;

	m_TopSrcRect.width = m_pTextureManager->GetTexture("platterBreak")->GetWidth();
	m_TopSrcRect.height = m_pTextureManager->GetTexture("platterBreak")->GetHeight() / 2.f;
	m_TopSrcRect.left = 0.f;
	m_TopSrcRect.bottom = m_TopSrcRect.height;

	m_PlateSrcRect.width = m_TopSrcRect.width;
	m_PlateSrcRect.height = m_TopSrcRect.height;
	m_PlateSrcRect.left = 0.f;
	m_PlateSrcRect.bottom = m_PlateSrcRect.height * 2.f;

	m_TopHitbox.width = m_TopSrcRect.width * m_Scale;
	m_TopHitbox.height = m_TopSrcRect.height * m_Scale;

	m_PlateHitbox.width = m_PlateSrcRect.width * m_Scale;
	m_PlateHitbox.height = m_PlateSrcRect.height * m_Scale;
}

void Platter::InitChicken()
{
	// initChicken
	Rectf chickenRect{ 0.f, m_TopHitbox.bottom, m_pTextureManager->GetTexture("chicken")->GetWidth() * m_Scale,  m_pTextureManager->GetTexture("chicken")->GetHeight() * m_Scale};
	chickenRect.left = m_TopHitbox.left + m_TopHitbox.width / 2.f - chickenRect.width / 2.f;

	m_pChicken = new Chicken{ chickenRect };
}

// Destroy
void Platter::DestroyChicken()
{
	if (m_pChicken != nullptr)
	{
		delete m_pChicken;
		m_pChicken = nullptr;
	}
}

// Update
void Platter::UpdateMovement(float elapsedSec)
{
	if (!m_IsBroken)
	{
		return;
	}

	// gravity
	m_TopVector.y += m_GravityForce * elapsedSec;
	m_PlateVector.y += m_GravityForce * elapsedSec;

	// update pos
	m_TopHitbox.left += m_TopVector.x * elapsedSec;
	m_TopHitbox.bottom += m_TopVector.y * elapsedSec;

	m_PlateHitbox.left += m_PlateVector.x * elapsedSec;
	m_PlateHitbox.bottom += m_PlateVector.y * elapsedSec;
}

void Platter::UpdatePlayerInteraction(float elapsedSec, Player* player, SoundManager* soundManager)
{
	// function that checks if player picks up chicken
	if (!m_IsBroken || m_ChickenPickedUp)
	{
		return;
	}

	if (m_pChicken->existTime > 0.5f)
	{
		if (utils::IsOverlapping(player->GetHitBox(), m_pChicken->hitbox))
		{
			m_ChickenPickedUp = true;

			// delete the chicken
			delete m_pChicken;
			m_pChicken = nullptr;

			player->SetHP(player->GetMaxHP());

			// sound picking up chicken
			soundManager->GetSoundEffect("chickenpickup")->Play(false);
		}
	}
}