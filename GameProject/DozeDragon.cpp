#include "pch.h"
#include "DozeDragon.h"
#include "Animation.h"
#include "Player.h"
#include "DozeDragonBubbles.h"
#include "TextureManager.h"
#include "DragonExplosion.h"

// Constructors / Destructors
DozeDragon::DozeDragon(const Point2f& dragonPos, TextureManager* textureManager)
	: m_DrawRect{ dragonPos, 0.f, 0.f }
	, m_pTextureManager{ textureManager }
	, m_PlayerKnockUpHead{ 1500.f }
	, m_PlayerKnockUpBody{ 900.f }
	, m_Scale{ 3.5f }
	, m_IsOnScreen{ false }
	, m_CanBeDeleted{ false }
	, m_IsDead{ false }
	, m_DragonCanMove{ false }
	, m_HP{ 7 }
	, m_IsHitTime{ 0 }
	, m_DragonState{ DragonState::idle }
	, m_DragonSteps{ DragonSteps::stepOne }
	, m_StartCycle{ true }
	, m_GotAttacked{ false }
	, m_ExplosionsAreDone{ false }
{
	InitVariables();
	InitAnimations();
	InitExplosions();
}

DozeDragon::~DozeDragon()
{
	std::cout << "DozeDragone gone\n";
	DestroyAnimations();
	DestroyBubbles();
	DestroyExplosions();
}

// Update / Draw
void DozeDragon::Update(float elapsedSec, Player* player, const Rectf& cameraRect)
{
	if (!m_IsOnScreen)
	{
		return;
	}
	UpdateIsHit(elapsedSec);
	UpdateCheckHit(player);
	UpdateAnimations(elapsedSec, cameraRect);
	UpdateHitboxes();
	UpdateDrawRect(elapsedSec);
	UpdatePlayerHit(player);
	UpdateBubbles(elapsedSec, player);
}

void DozeDragon::Draw() const
{
	if (!m_IsOnScreen)
	{
		return;
	}
	DrawDragonTexture();
	DrawExplosions();
	DrawBubbles();
	/*DrawDrawRect();
	DrawHitboxes();*/
}

// Getters / Setters
const Rectf& DozeDragon::GetOnCameraHitbox() const
{
	return m_DrawRect;
}

const Rectf& DozeDragon::GetHeadHitbox() const
{
	return m_HeadHitbox;
}

const Rectf& DozeDragon::GetBodyHitbox() const
{
	return m_BodyHitbox;
}

bool DozeDragon::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

void DozeDragon::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool DozeDragon::GetCanBeDeleted() const
{
	return m_CanBeDeleted;
}

// private functions //
// Init
void DozeDragon::InitVariables()
{
	// moving 1.5 tile per step = 56 * 1.5 = 84 pixels per step
	m_pDragonBubbles.reserve(4);

	// srcRect
	m_SrcRect.left = 0.f;
	m_SrcRect.bottom = 87.f;
	m_SrcRect.width = 181.f;
	m_SrcRect.height = 87.f;

	// drawRect
	m_DrawRect.width = m_SrcRect.width * m_Scale;
	m_DrawRect.height = m_SrcRect.height * m_Scale;

	// bodyHitbox
	m_BodyHitbox.width = m_DrawRect.width * 0.6f;
	m_BodyHitbox.height = m_DrawRect.height * 0.8f;
	m_BodyHitbox.bottom = m_DrawRect.bottom;

	// headHitbox
	m_HeadHitbox.width = m_DrawRect.width * 0.2f;
	m_HeadHitbox.height = m_DrawRect.height * 0.3f;
	m_HeadHitbox.bottom = m_BodyHitbox.bottom;
}

void DozeDragon::InitAnimations()
{
	Rectf startSrcRect{};

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 87.f;
	startSrcRect.width = 181.f;
	startSrcRect.height = 87.f;
	m_pIdleAnimation = new Animation{ m_SrcRect , startSrcRect, 12, 8.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 174.f;
	startSrcRect.width = 181.f;
	startSrcRect.height = 87.f;
	m_pWalkBackwardAnimation = new Animation{ m_SrcRect, startSrcRect, 6, 7.f };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 261.f;
	startSrcRect.width = 181.f;
	startSrcRect.height = 87.f;
	m_pWalkForwardAnimation = new Animation{ m_SrcRect, startSrcRect, m_pWalkBackwardAnimation->GetNrOfFrames(), m_pWalkBackwardAnimation->GetFPS() };

	startSrcRect.left = 0.f;
	startSrcRect.bottom = 348.f;
	startSrcRect.width = 181.f;
	startSrcRect.height = 87.f;
	m_pBubbleAttackAnimation = new Animation{ m_SrcRect, startSrcRect, 3, 5.f, 0.f, 1.f };

	const float pixelsStep{ 1.5 * 56.f };
	m_MoveSpeed = pixelsStep * (m_pWalkBackwardAnimation->GetFPS() / m_pWalkBackwardAnimation->GetNrOfFrames());
}

void DozeDragon::InitExplosions()
{
	const Texture* bigExplosion{ m_pTextureManager->GetTexture("bigExplosion") };
	const Texture* smallExplosion{ m_pTextureManager->GetTexture("smallExplosion") };

	Rectf explosionRect{ m_DrawRect.left, m_DrawRect.bottom, m_DrawRect.width / 2.f, m_DrawRect.height / 2.f };
	explosionRect.left += m_DrawRect.width / 2.f - explosionRect.width / 2.f - 50.f;
	explosionRect.bottom += m_DrawRect.height / 2.f - explosionRect.height / 2.f - 50.f;

	int rndmNr{ rand() % 10 + 10 };
	float delay{};

	m_pDragonExplosions.reserve(rndmNr);

	for (int idx{}; idx < rndmNr; ++idx)
	{
		const bool rndmType{ bool(rand() % 2)};
		const float rndmDelay{ float((rand() % 10) / 100.f) };
		delay += rndmDelay;

		if (rndmType)
		{
			m_pDragonExplosions.emplace_back(new DragonExplosion{ utils::GetRandomPos(explosionRect, 20.f), bigExplosion, delay});
		}
		else
		{
			m_pDragonExplosions.emplace_back(new DragonExplosion{ utils::GetRandomPos(explosionRect, 20.f) , smallExplosion, delay });
		}
	}
}

// Destroy
void DozeDragon::DestroyAnimations()
{
	delete m_pIdleAnimation;
	m_pIdleAnimation = nullptr;

	delete m_pWalkBackwardAnimation;
	m_pWalkBackwardAnimation = nullptr;

	delete m_pWalkForwardAnimation;
	m_pWalkForwardAnimation = nullptr;

	delete m_pBubbleAttackAnimation;
	m_pBubbleAttackAnimation = nullptr;
}

void DozeDragon::DestroyBubbles()
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		delete m_pDragonBubbles[idx];
	}
	m_pDragonBubbles.clear();
}

void DozeDragon::DestroyExplosions()
{
	for (size_t idx{}; idx < m_pDragonExplosions.size(); ++idx)
	{
		delete m_pDragonExplosions[idx];
	}
	m_pDragonExplosions.clear();
}

// Update
void DozeDragon::UpdateDrawRect(float elapsedSec)
{
	if (!(m_DragonState == DragonState::movingLeft || m_DragonState == DragonState::movingRight))
	{
		return;
	}

	if (m_DragonState == DragonState::movingLeft)
	{
		m_DrawRect.left -= m_MoveSpeed * elapsedSec;
	}
	else // moving right
	{
		m_DrawRect.left += m_MoveSpeed * elapsedSec;
	}
}

void DozeDragon::UpdateAnimations(float elapsedSec, const Rectf& cameraRect)
{
	if (m_IsDead)
	{
		if (m_ExplosionsAreDone)
		{
			bool canBeDeleted{ true };
			for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
			{
				if (m_pDragonBubbles[idx]->GetIsOnScreen(cameraRect))
				{
					canBeDeleted = false;
				}
			}
			m_CanBeDeleted = canBeDeleted;
			return;
		}
		for (size_t idx{}; idx < m_pDragonExplosions.size(); ++idx)
		{
			m_pDragonExplosions[idx]->Update(elapsedSec);

			if (m_pDragonExplosions[idx]->GetCanBeDeleted())
			{
				delete m_pDragonExplosions[idx];
				m_pDragonExplosions[idx] = m_pDragonExplosions.back();
				m_pDragonExplosions.pop_back();
			}
		}

		if (m_pDragonExplosions.empty())
		{
			m_ExplosionsAreDone = true;
		}
		return;
	}

	if (m_DragonState == DragonState::idle)
	{
		if (m_pIdleAnimation->Play(elapsedSec))
		{
			if (m_StartCycle)
			{
				m_DragonState = DragonState::attacking;
				m_StartCycle = false;
			}
			else
			{
				if (m_GotAttacked)
				{
					if (m_DragonSteps != DragonSteps::stepOne)
					{
						m_DragonState = DragonState::movingRight;
						m_DragonSteps = DragonSteps((int(m_DragonSteps) - 1));
					}
					else if (m_DragonSteps != DragonSteps::stepThree)
					{
						m_DragonState = DragonState::movingLeft;
						m_DragonSteps = DragonSteps((int(m_DragonSteps) + 1));
					}
					else
					{
						m_DragonState = DragonState::attacking;
					}
					
				}
				else
				{
					if (m_DragonSteps != DragonSteps::stepThree)
					{
						m_DragonState = DragonState::movingLeft;
						m_DragonSteps = DragonSteps((int(m_DragonSteps) + 1));
					}
					else
					{
						m_DragonState = DragonState::attacking;
					}
				}
				m_GotAttacked = false;
			}
		}
	}
	else if (m_DragonState == DragonState::movingLeft)
	{
		if (m_pWalkForwardAnimation->Play(elapsedSec))
		{
			m_DragonState = DragonState::attacking;
		}
	}
	else if (m_DragonState == DragonState::movingRight)
	{
		if (m_pWalkBackwardAnimation->Play(elapsedSec))
		{
			m_DragonState = DragonState::attacking;
		}
	}
	else // attacking
	{
		if (m_pBubbleAttackAnimation->Play(elapsedSec))
		{
			m_DragonState = DragonState::idle;
		}

		if (m_pBubbleAttackAnimation->GetCurrentFrame() == m_pBubbleAttackAnimation->GetNrOfFrames())
		{
			if (!m_HasAttacked)
			{
				DragonAttack();
				m_HasAttacked = true;
			}
		}
		else
		{
			m_HasAttacked = false;
		}
	}
}

void DozeDragon::UpdateHitboxes()
{
	// body hitbox
	m_BodyHitbox.left = m_DrawRect.left + m_DrawRect.width - m_BodyHitbox.width;

	// head hitbox
	m_HeadHitbox.left = m_BodyHitbox.left - m_HeadHitbox.width;
}

void DozeDragon::UpdatePlayerHit(Player* player)
{
	if (m_IsDead)
	{
		return;
	}

	if (utils::IsOverlapping(player->GetHitBox(), m_HeadHitbox) || utils::IsOverlapping(player->GetHitBox(), m_BodyHitbox))
	{
		if (!player->GetIsHit())
		{
			// set Hit Knocks
			// in x
			player->Knock(false, 500.f);
			// in y
			player->Knock(true, 1000.f);
			player->Hit();
		}
	}
}

void DozeDragon::UpdateCheckHit(Player* player)
{
	if (m_IsDead)
	{
		return;
	}

	if (utils::IsOverlapping(m_HeadHitbox, player->GetAttackHitbox()))
	{
		if (player->GetIsJumpAttacking())
		{
			player->Knock(true, m_PlayerKnockUpHead);
		}

		HitDragon();
	}
	else if (utils::IsOverlapping(m_BodyHitbox, player->GetAttackHitbox()))
	{
		if (player->GetIsJumpAttacking())
		{
			// knock up
			player->Knock(true, m_PlayerKnockUpBody);
		}
	}
}

void DozeDragon::UpdateIsHit(float elapsedSec)
{
	if (m_IsHit)
	{
		if (utils::GetClockTime(m_IsHitTime, elapsedSec, 0.6f))
		{
			m_IsHit = false;
		}
	}
}

void DozeDragon::UpdateBubbles(float elapsedSec, Player* player)
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		m_pDragonBubbles[idx]->Update(elapsedSec, player);
	}
}

// Draw
void DozeDragon::DrawDragonTexture() const
{
	if (m_IsDead)
	{
		return;
	}

	m_pTextureManager->GetTexture("dozeDragon")->Draw(m_DrawRect, m_SrcRect);
}

void DozeDragon::DrawBubbles() const
{
	const Texture* texture{ m_pTextureManager->GetTexture("bubble") };

	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		m_pDragonBubbles[idx]->Draw(texture);
	}
}

void DozeDragon::DrawDrawRect() const
{
	utils::SetRGBColor(0, 250, 0);
	utils::DrawRect(m_DrawRect, 2.f);
}

void DozeDragon::DrawHitboxes() const
{
	utils::SetRGBColor(250, 0, 0);
	utils::DrawRect(m_HeadHitbox, 2.f);
	utils::DrawRect(m_BodyHitbox, 2.f);
}

void DozeDragon::DrawExplosions() const
{
	if (m_IsDead)
	{
		for (size_t idx{}; idx < m_pDragonExplosions.size(); ++idx)
		{
			m_pDragonExplosions[idx]->Draw();
		}
	}
}

// Other
void DozeDragon::HitDragon()
{
	if (!m_IsDead && !m_IsHit)
	{
		m_GotAttacked = true;
		m_IsHit = true;
		--m_HP;

		if (m_HP < 1)
		{
			m_IsDead = true;
		}
	}
}

void DozeDragon::DragonAttack()
{
	m_pDragonBubbles.emplace_back(new DozeDragonBubbles{ Point2f{m_HeadHitbox.left - m_pTextureManager->GetTexture("bubble")->GetWidth() , m_HeadHitbox.bottom}});
}