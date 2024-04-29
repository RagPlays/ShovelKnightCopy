#include "pch.h"
#include "Rock.h"
#include "Animation.h"
#include "Player.h"
#include "GemsManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Constructors / Destructors
Rock::Rock(const Point2f& rockPos, const int breakValue, bool rockType, TextureManager* textureManager)
	: m_Hitbox{rockPos, 0.f, 0.f}
	, m_BreakValue{ breakValue }
	, m_RockType{ rockType }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_CanBeDeleted{ false }
	, m_IsBroken{ false }
	, m_PlayerKnockUp{ 950.f }
	, m_IsOnRock{ false }
{
	InitVertices();
	InitAnimations();
}

Rock::~Rock()
{
	DestroyAnimations();
}

// Update / Draw
bool Rock::Update(float elapsedSec, Player* player)
{
	if (!m_IsBroken)
	{
		UpdateIsOnRock(player);
		UpdateCollision(player);
	}
	UpdateAnimation(elapsedSec);

	return m_IsOnRock;
}

void Rock::Draw() const
{
	if (!m_IsOnScreen)
	{
		return;
	}
	DrawTexture();
	//DrawVertices();
}

// Getters / Setters
void Rock::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool Rock::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

bool Rock::GetIsBroken() const
{
	return m_IsBroken;
}

const Rectf& Rock::GetHitbox() const
{
	return m_Hitbox;
}

bool Rock::GetCanBeDeleted() const
{
	return m_CanBeDeleted;
}

const std::vector<Point2f>& Rock::GetVertices() const
{
	return m_Vertices;
}

// Other
void Rock::Hit(GemsManager* gemsManager, Player* player, SoundManager* soundManager)
{
	if (m_IsBroken)
	{
		return;
	}
	else
	{
		m_IsBroken = true;
	}

	if (player->GetIsJumpAttacking())
	{
		player->Knock(true, m_PlayerKnockUp);

		soundManager->GetSoundEffect("rockjumpbreak")->Play(false);
	}
	else
	{
		soundManager->GetSoundEffect("rockbreak")->Play(false);
	}

	if (m_BreakValue)
	{
		gemsManager->AddGems(m_Hitbox, m_BreakValue, Vector2f{ 0.f, 2.f });
	}
}


// Private Functions //
// Init
void Rock::InitAnimations()
{
	Rectf startSrcRect{};

	startSrcRect.left = 0.f;
	if (m_RockType)
	{
		startSrcRect.bottom = m_pTextureManager->GetTexture("smallRockBreak")->GetHeight();
		startSrcRect.width = m_pTextureManager->GetTexture("smallRockBreak")->GetWidth() / 3.f;
		startSrcRect.height = m_pTextureManager->GetTexture("smallRockBreak")->GetHeight();
	}
	else
	{
		startSrcRect.bottom = m_pTextureManager->GetTexture("bigRockBreak")->GetHeight();
		startSrcRect.width = m_pTextureManager->GetTexture("bigRockBreak")->GetWidth() / 3.f;
		startSrcRect.height = m_pTextureManager->GetTexture("bigRockBreak")->GetHeight();
	}
	
	m_pBreakAnimation = new Animation{ m_BreakSrcRect, startSrcRect, 3, 6.f };
}

void Rock::InitVertices()
{
	if (m_RockType)
	{
		m_Hitbox.width = m_pTextureManager->GetTexture("smallRock")->GetWidth() * m_Scale;
		m_Hitbox.height = m_pTextureManager->GetTexture("smallRock")->GetHeight() * m_Scale;
	}
	else
	{
		m_Hitbox.width = m_pTextureManager->GetTexture("bigRock")->GetWidth() * m_Scale;
		m_Hitbox.height = m_pTextureManager->GetTexture("bigRock")->GetHeight() * m_Scale;
	}
	

	const float offset{1.f};

	m_Vertices.emplace_back(Point2f{ m_Hitbox.left + offset, m_Hitbox.bottom + offset});
	m_Vertices.emplace_back(Point2f{ m_Hitbox.left + m_Hitbox.width - offset, m_Hitbox.bottom + offset});
	m_Vertices.emplace_back(Point2f{ m_Hitbox.left + m_Hitbox.width - offset, m_Hitbox.bottom + m_Hitbox.height - offset});
	m_Vertices.emplace_back(Point2f{ m_Hitbox.left + offset, m_Hitbox.bottom + m_Hitbox.height - offset });
}

// Destroy
void Rock::DestroyAnimations()
{
	delete m_pBreakAnimation;
	m_pBreakAnimation = nullptr;
}

// Update
void Rock::UpdateAnimation(float elapsedSec)
{
	if (m_IsBroken)
	{
		if (m_pBreakAnimation->Play(elapsedSec))
		{
			m_CanBeDeleted = true;
		}
	}
}

void Rock::UpdateCollision(Player* player)
{
	UpdateVerticalCollision(player);
	UpdateHorizontalCollision(player);
}

void Rock::UpdateVerticalCollision(Player* player)
{
	// vertical rays
	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetVerticalRays().size(); ++index)
	{
		if (utils::Raycast(m_Vertices, player->GetVerticalRays()[index]->pointOne, player->GetVerticalRays()[index]->pointTwo, hitInfo))
		{
			if (hitInfo.lambda < 0.5f)
			{
				if (!player->GetIsJumpAttacking())
				{
					// on ground
					player->SetPlayerVelocity(true, 0);
					player->SetPlayerBottom(hitInfo.intersectPoint.y + 1.f);
				}
			}
			else
			{
				// hit the your head
				player->SetPlayerVelocity(true, 0);
				player->SetPlayerBottom(hitInfo.intersectPoint.y - player->GetHitBox().height - 2.f);
			}
			return;
		}
	}
}

void Rock::UpdateHorizontalCollision(Player* player)
{
	// horizontal rays
	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetHorizontalRays().size(); ++index)
	{
		if (utils::Raycast(m_Vertices, player->GetHorizontalRays()[index]->pointOne, player->GetHorizontalRays()[index]->pointTwo, hitInfo))
		{
			const float offset{ 1.75f };

			player->SetPlayerVelocity(false, 0);

			if (hitInfo.lambda < 0.5f)	 // hitting wall with left of player
			{
				player->SetPlayerLeft(hitInfo.intersectPoint.x + offset);
			}
			else   // hitting wall with right of player
			{
				player->SetPlayerLeft(hitInfo.intersectPoint.x - player->GetHitBox().width - offset);
			}
			return;
		}
	}
}

void Rock::UpdateIsOnRock(Player* player)
{
	m_IsOnRock = false;

	if (player->GetIsJumpAttacking())
	{
		return;
	}

	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetVerticalRays().size(); ++index)
	{
		// bottom
		Point2f pointOne{ player->GetVerticalRays()[index]->pointOne.x, player->GetVerticalRays()[index]->pointOne.y - 2.f };
		// top
		Point2f pointTwo{ player->GetVerticalRays()[index]->pointTwo.x, player->GetVerticalRays()[index]->pointTwo.y };

		if (utils::Raycast(m_Vertices, pointOne, pointTwo, hitInfo))
		{
			if (hitInfo.lambda < 0.5f)
			{
				m_IsOnRock = true;
			}
		}
	}
}

// Draw
void Rock::DrawTexture() const
{
	if (m_IsBroken)
	{
		if (m_RockType)
		{
			m_pTextureManager->GetTexture("smallRockBreak")->Draw(m_Hitbox, m_BreakSrcRect);
		}
		else
		{
			m_pTextureManager->GetTexture("bigRockBreak")->Draw(m_Hitbox, m_BreakSrcRect);
		}
	}
	else
	{
		if (m_RockType)
		{
			m_pTextureManager->GetTexture("smallRock")->Draw(m_Hitbox);
		}
		else
		{
			m_pTextureManager->GetTexture("bigRock")->Draw(m_Hitbox);
		}
	}
}

void Rock::DrawVertices() const
{
	utils::SetRGBColor(255, 255, 255);
	for (size_t idx{}; idx < m_Vertices.size(); ++idx)
	{
		utils::DrawEllipse(m_Vertices[idx], 3.f, 3.f, 3.f);
	}
}