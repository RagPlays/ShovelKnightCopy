#include "pch.h"
#include "Platform.h"
#include "Player.h"
#include "TextureManager.h"

// Constructors / Destructors
Platform::Platform(const Point2f& startPos, const Point2f& endPos, bool moveDir /*x = 0, y = 1*/, const float speed, TextureManager* textureManager)
	: m_StartPos{ startPos }
	, m_EndPos{ endPos }
	, m_MoveDir{ moveDir }
	, m_Speed{ speed }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_Velocity{ 0.f }
	, m_PlatformMoveDir{}
	, m_IsOnScreen{ false }
{
	InitVariables();
}

// Update / Draw
bool Platform::Update(float elapsedSec, Player* player)
{
	UpdateMovement(elapsedSec);
	UpdateVertices();
	UpdateCollision(player);
	UpdatePlayerOnPlatform(player);
	UpdatePlayerMovement(player, elapsedSec);

	return m_IsOnPlatform;
}

void Platform::Draw() const
{
	if (!m_IsOnScreen)
	{
		return;
	}

	DrawPlatform();
	/*DrawHitBox();
	DrawVertices();*/
}

// Getters / Setters
const Rectf& Platform::GetHitbox() const
{
	return m_Hitbox;
}

void Platform::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool Platform::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

bool Platform::GetIsOnPlatform() const
{
	return m_IsOnPlatform;
}

// Private Functions //
// Init
void Platform::InitVariables()
{
	m_Hitbox.width = m_pTextureManager->GetTexture("platform")->GetWidth() * m_Scale;
	m_Hitbox.height = m_pTextureManager->GetTexture("platform")->GetHeight() * m_Scale;

	m_Hitbox.left = m_StartPos.x - m_Hitbox.width / 2.f;
	m_Hitbox.bottom = m_StartPos.y - m_Hitbox.height / 2.f;


	if (m_MoveDir)	// y
	{
		if (m_StartPos.y < m_EndPos.y)
		{
			m_PlatformMoveDir = false;
			m_Velocity = m_Speed;
		}
		else
		{
			m_PlatformMoveDir = true;
			m_Velocity = -m_Speed;
		}
	}
	else // x
	{
		if (m_StartPos.x < m_EndPos.x)
		{
			m_PlatformMoveDir = false;
			m_Velocity = m_Speed;
		}
		else
		{
			m_PlatformMoveDir = true;
			m_Velocity = -m_Speed;
		}
	}
}

// Update
void Platform::UpdateVertices()
{
	Point2f pointOne{ m_Hitbox.left, m_Hitbox.bottom };
	Point2f pointTwo{ m_Hitbox.left + m_Hitbox.width, m_Hitbox.bottom };
	Point2f pointThree{ m_Hitbox.left + m_Hitbox.width, m_Hitbox.bottom + m_Hitbox.height };
	Point2f pointFour{ m_Hitbox.left, m_Hitbox.bottom + m_Hitbox.height };

	m_PlatformVertices.clear();

	m_PlatformVertices.emplace_back(pointOne);
	m_PlatformVertices.emplace_back(pointTwo);
	m_PlatformVertices.emplace_back(pointThree);
	m_PlatformVertices.emplace_back(pointFour);
}

void Platform::UpdateMovement(float elapsedSec)
{
	if (m_MoveDir)	// y
	{
		m_Hitbox.bottom += m_Velocity * elapsedSec;
	}
	else // x
	{
		m_Hitbox.left += m_Velocity * elapsedSec;
	}

	if (m_MoveDir) // y
	{
		if (m_PlatformMoveDir)	// goes back
		{
			if (m_Hitbox.bottom + m_Hitbox.height / 2.f < m_StartPos.y)
			{
				m_Velocity *= -1;
				m_PlatformMoveDir = false;
			}
		}
		else // goes to endPos
		{
			if (m_Hitbox.bottom + m_Hitbox.height / 2.f > m_EndPos.y)
			{
				m_Velocity *= -1;
				m_PlatformMoveDir = true;
			}
		}
	}
	else  // x
	{
		if (m_PlatformMoveDir)	// goes back
		{
			if ((m_Hitbox.left + m_Hitbox.width / 2.f) < m_StartPos.x)
			{
				m_Velocity *= -1;
				m_PlatformMoveDir = false;
			}
		}
		else // goes to endPos
		{
			if (m_Hitbox.left + m_Hitbox.width / 2.f > m_EndPos.x)
			{
				m_Velocity *= -1;
				m_PlatformMoveDir = true;
			}
		}
	}
}

void Platform::UpdateCollision(Player* player)
{
	UpdateVerticalCollisions(player);
	UpdateHorizontalCollisions(player);
}

void Platform::UpdateVerticalCollisions(Player* player)
{
	// vertical rays
	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetVerticalRays().size(); ++index)
	{
		if (utils::Raycast(m_PlatformVertices, player->GetVerticalRays()[index]->pointOne, player->GetVerticalRays()[index]->pointTwo, hitInfo))
		{
			player->SetPlayerVelocity(true, 0);

			if (hitInfo.lambda < 0.5f)
			{
				// on ground
				player->SetPlayerBottom(hitInfo.intersectPoint.y + 1.f);
			}
			else
			{
				// hit the your head
				player->SetPlayerBottom(hitInfo.intersectPoint.y - player->GetHitBox().height - 3.f);
			}
			return;
		}
	}
}

void Platform::UpdateHorizontalCollisions(Player* player)
{
	// horizontal rays
	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetHorizontalRays().size(); ++index)
	{
		if (utils::Raycast(m_PlatformVertices, player->GetHorizontalRays()[index]->pointOne, player->GetHorizontalRays()[index]->pointTwo, hitInfo))
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

void Platform::UpdatePlayerOnPlatform(Player* player)
{
	utils::HitInfo hitInfo{};

	for (size_t index{}; index < player->GetVerticalRays().size(); ++index)
	{
		// bottom
		Point2f pointOne{ player->GetVerticalRays()[index]->pointOne.x, player->GetVerticalRays()[index]->pointOne.y - 2.f };
		// top
		Point2f pointTwo{ player->GetVerticalRays()[index]->pointTwo.x, player->GetVerticalRays()[index]->pointTwo.y };

		if (utils::Raycast(m_PlatformVertices, pointOne, pointTwo, hitInfo))
		{
			if (hitInfo.lambda < 0.5f)
			{
				m_IsOnPlatform = true;
				return;
			}
		}
	}
	m_IsOnPlatform = false;
}

void Platform::UpdatePlayerMovement(Player* player, float elapsedSec)
{
	if (m_IsOnPlatform)
	{
		player->ChangePosition(m_MoveDir, m_Velocity * elapsedSec);
	}
}

// Draw
void Platform::DrawPlatform() const
{
	m_pTextureManager->GetTexture("platform")->Draw(m_Hitbox);
}

void Platform::DrawHitBox() const
{
	utils::SetRGBColor(200, 30, 30);
	utils::DrawRect(m_Hitbox);
}

void Platform::DrawVertices() const
{
	const float rad{ 3.f };
	utils::SetRGBColor(255, 255, 255);
	for (size_t idx{}; idx < m_PlatformVertices.size(); ++idx)
	{
		utils::DrawEllipse(m_PlatformVertices[idx], rad, rad, rad);
	}
}