#include "pch.h"
#include "DirtPile.h"
#include "Animation.h"
#include "GemsManager.h"
#include "TextureManager.h"

// constructors / Destructors
DirtPile::DirtPile(const Point2f& dirtPilePos, TextureManager* textureManager)
	: m_Hitbox{ dirtPilePos, 0.f, 0.f }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_IsOnScreen{ false }
	, m_CanBeDeleted{ false }
	, m_SrcRect{}
	, m_HitState{}
	, m_MaxHitState{ 4 }
	, m_GravityForce{ -2000 }
{
	InitVariables();
	InitAnimations();
}

DirtPile::~DirtPile()
{
	DestroyAnimations();
	DestroyDirt();
}

// Update / Draw
void DirtPile::Update(float elapsedSec)
{
	UpdateAnimations(elapsedSec);
	UpdateDirt(elapsedSec);
	UpdateDeleteDirt(elapsedSec);

	if (m_IsHit)
	{
		if (utils::GetClockTime(m_HitTime, elapsedSec, 0.5f))
		{
			m_IsHit = false;
		}
	}
}

void DirtPile::Draw() const
{
	if (m_IsOnScreen)
	{
		DrawDirt();
		DrawDirtPile();
	}
}

// Getters / Setters
const Rectf& DirtPile::GetHitbox() const
{
	return m_Hitbox;
}

void DirtPile::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool DirtPile::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

bool DirtPile::GetCanBeDeleted() const
{
	if (m_CanBeDeleted && m_pDirtPieces.size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Other
void DirtPile::Hit(bool playerDir, GemsManager* gemsManager)
{
	if (m_IsHit)
	{
		return;
	}
	else
	{
		m_IsHit = true;
	}

	++m_HitState;

	if (m_HitState > 5)
	{
		return;
	}

	int gemsValue{};

	switch (m_HitState)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		gemsValue = rand() % 2 + 8;
		break;
	case 5:
		gemsValue = rand() % 30 + 50;
		break;
	default:
		break;
	}

	if (playerDir)
	{
		gemsManager->AddGems(m_Hitbox, gemsValue, Vector2f{ 0.6f, 1.f }, true);
	}
	else
	{
		gemsManager->AddGems(m_Hitbox, gemsValue, Vector2f{ -0.6f, 1.f }, true);
	}

	HitDirt(playerDir);

	if (m_HitState > m_MaxHitState)
	{
		m_CanBeDeleted = true;
	}
}


// Private Functions //
// Init
void DirtPile::InitVariables()
{
	m_SrcRect.width = m_pTextureManager->GetTexture("dirtPile")->GetWidth() / 5.f;
	m_SrcRect.height = (m_pTextureManager->GetTexture("dirtPile")->GetHeight() - 6.f) / 5.f;
	m_SrcRect.left = 0.f;
	m_SrcRect.bottom = m_SrcRect.height;
	
	m_Hitbox.width = m_SrcRect.width * m_Scale;
	m_Hitbox.height = m_SrcRect.height * m_Scale;

	m_HitTime = 0.f;
	m_IsHit = false;
}

void DirtPile::InitAnimations()
{
	Rectf startSrcRect{};

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 15.f;
	startSrcRect.width = m_SrcRect.width;
	startSrcRect.height = m_SrcRect.height;
	m_pBlinkOneAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 9.f, 5.f};

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 30.f;
	startSrcRect.width = m_SrcRect.width;
	startSrcRect.height = m_SrcRect.height;
	m_pBlinkTwoAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 9.f, 5.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 45.f;
	startSrcRect.width = m_SrcRect.width;
	startSrcRect.height = m_SrcRect.height;
	m_pBlinkThreeAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 9.f, 5.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 60.f;
	startSrcRect.width = m_SrcRect.width;
	startSrcRect.height = m_SrcRect.height;
	m_pBlinkFourAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 9.f, 5.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 75.f;
	startSrcRect.width = m_SrcRect.width;
	startSrcRect.height = m_SrcRect.height;
	m_pBlinkFiveAnimation = new Animation{ m_SrcRect, startSrcRect, 5, 9.f, 5.f };

	// dirt piles
	startSrcRect.left = 0.f;
	startSrcRect.bottom = m_pTextureManager->GetTexture("dirtPile")->GetHeight();
	startSrcRect.width = 6.f;
	startSrcRect.height = 6.f;
	m_DirtOneSrcRect = startSrcRect;

	startSrcRect.left = 6.f;
	startSrcRect.bottom = m_pTextureManager->GetTexture("dirtPile")->GetHeight();
	startSrcRect.width = 6.f;
	startSrcRect.height = 6.f;
	m_DirtTwoSrcRect = startSrcRect;
}

// Destroy
void DirtPile::DestroyAnimations()
{
	delete m_pBlinkOneAnimation;
	m_pBlinkOneAnimation = nullptr;

	delete m_pBlinkTwoAnimation;
	m_pBlinkTwoAnimation = nullptr;
	
	delete m_pBlinkThreeAnimation;
	m_pBlinkThreeAnimation = nullptr;

	delete m_pBlinkFourAnimation;
	m_pBlinkFourAnimation = nullptr;

	delete m_pBlinkFiveAnimation;
	m_pBlinkFiveAnimation = nullptr;
}

void DirtPile::DestroyDirt()
{
	for (size_t idx{}; idx < m_pDirtPieces.size(); ++idx)
	{
		delete m_pDirtPieces[idx];
	}
	m_pDirtPieces.clear();
}

// Update
void DirtPile::UpdateAnimations(float elapsedSec)
{
	switch (m_HitState)
	{
	case 0:
		m_pBlinkOneAnimation->Play(elapsedSec);
		break;
	case 1:
		m_pBlinkTwoAnimation->Play(elapsedSec);
		break;
	case 2:
		m_pBlinkThreeAnimation->Play(elapsedSec);
		break;
	case 3:
		m_pBlinkFourAnimation->Play(elapsedSec);
		break;
	case 4:
		m_pBlinkFiveAnimation->Play(elapsedSec);
		break;
	default:
		break;
	}
}

void DirtPile::UpdateDirt(float elapsedSec)
{
	for (size_t idx{}; idx < m_pDirtPieces.size(); ++idx)
	{
		// gravity
		m_pDirtPieces[idx]->velocity.y += m_GravityForce * elapsedSec;

		// movement
		m_pDirtPieces[idx]->drawRect.left += m_pDirtPieces[idx]->velocity.x * elapsedSec;
		m_pDirtPieces[idx]->drawRect.bottom += m_pDirtPieces[idx]->velocity.y * elapsedSec;
	}
}

void DirtPile::UpdateDeleteDirt(float elapsedSec)
{
	for (size_t idx{}; idx < m_pDirtPieces.size(); ++idx)
	{
		if (utils::GetClockTime(m_pDirtPieces[idx]->existTime, elapsedSec, 5.f))
		{
			delete m_pDirtPieces[idx];
			if (idx < m_pDirtPieces.size())
			{
				m_pDirtPieces[idx] = m_pDirtPieces.back();
			}
			m_pDirtPieces.pop_back();
		}
	}
}

// Draw
void DirtPile::DrawDirtPile() const
{
	if (!m_CanBeDeleted)
	{
		m_pTextureManager->GetTexture("dirtPile")->Draw(m_Hitbox, m_SrcRect);
	}
}

void DirtPile::DrawDirt() const
{
	for (size_t idx{}; idx < m_pDirtPieces.size(); ++idx)
	{
		m_pTextureManager->GetTexture("dirtPile")->Draw(m_pDirtPieces[idx]->drawRect, m_pDirtPieces[idx]->srcRect);
	}
}

// Other
void DirtPile::HitDirt(bool playerDir)
{
	const Vector2f dirtSpeed{ 200.f, 700.f };
	const float speedOffset{ 50.f };

	Rectf drawRect{ m_Hitbox };
	drawRect.width = m_DirtOneSrcRect.width * m_Scale;
	drawRect.height = m_DirtOneSrcRect.height * m_Scale;
	drawRect.left = m_Hitbox.left + m_Hitbox.width / 2.f - drawRect.width / 2.f;
	drawRect.bottom = m_Hitbox.bottom + m_Hitbox.height / 2.f - drawRect.height / 2.f;

	// 4 dirtPieces
	const int nrOfDirt{ 4 };
	for (int idx{}; idx < nrOfDirt; ++idx)
	{
		if (playerDir)	// right
		{
			if (rand() % 2)
			{
				m_pDirtPieces.emplace_back(new Dirt{ drawRect, m_DirtOneSrcRect, Vector2f{ dirtSpeed.x + speedOffset * idx, dirtSpeed.y } });
			}
			else
			{
				m_pDirtPieces.emplace_back(new Dirt{ drawRect, m_DirtTwoSrcRect, Vector2f{ dirtSpeed.x + speedOffset * idx, dirtSpeed.y } });
			}
		}
		else // left
		{
			if (rand() % 2)
			{
				m_pDirtPieces.emplace_back(new Dirt{ drawRect, m_DirtOneSrcRect, Vector2f{ -(dirtSpeed.x + speedOffset * idx), dirtSpeed.y } });
			}
			else
			{
				m_pDirtPieces.emplace_back(new Dirt{ drawRect, m_DirtTwoSrcRect, Vector2f{ -(dirtSpeed.x + speedOffset * idx), dirtSpeed.y } });
			}
		}

	}
}