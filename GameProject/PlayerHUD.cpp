#include "pch.h"
#include "PlayerHUD.h"
#include "Number.h"
#include "TextureManager.h"

// Constructors / Destructors
PlayerHUD::PlayerHUD(const Rectf& windowBorder, int playerMaxHP, int bossMaxHP, TextureManager* textureManager)
	: m_WindowBorder{ windowBorder }
	, m_PlayerMaxHp{ playerMaxHP }
	, m_BossMaxHp{ bossMaxHP }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
{
	std::cout << "\nCreating HUD...\n";
	InitVariables();
}

PlayerHUD::~PlayerHUD()
{
	std::cout << "\nDestroying HUD...\n";
	
	delete m_pGoldNumber;
	m_pGoldNumber = nullptr;
}

// Update / Draw
void PlayerHUD::Update(float elapsedSec, int playerHp, int bossHp)
{
	// set the right values
	m_PlayerHp = playerHp;
	m_BossHp = bossHp;

	// update number
	m_pGoldNumber->Update(elapsedSec);
}

void PlayerHUD::Draw() const
{
	// draw hud background (black rect)
	utils::SetRGBColor(0, 0, 0);
	utils::FillRect(m_HudDrawRect);

	// draw main hud
	m_pTextureManager->GetTexture("HUD")->Draw(m_HudDrawRect);

	// draw gold number
	m_pGoldNumber->Draw();

	// draw the player hp
	DrawPlayerHp();

	// draw the boss hp
	DrawBossHp();

}

// Other
void PlayerHUD::AddGold(int addGoldValue)
{
	m_pGoldNumber->AddNumber(addGoldValue);
}

// Private Functions //
// Init
void PlayerHUD::InitVariables()
{
	// set drawRect
	m_HudDrawRect.width = m_WindowBorder.width;
	m_HudDrawRect.height = (m_WindowBorder.width / m_pTextureManager->GetTexture("HUD")->GetWidth()) * m_pTextureManager->GetTexture("HUD")->GetHeight();
	m_HudDrawRect.left = 0.f;
	m_HudDrawRect.bottom = m_WindowBorder.height - m_HudDrawRect.height;

	// init numbers // 
	m_GoldNumberPos.x = m_WindowBorder.width * 0.035f;
	m_GoldNumberPos.y = m_WindowBorder.height * 0.92f;

	// gold
	m_pGoldNumber = new Number{ m_GoldNumberPos, m_pTextureManager, m_Scale };

	// setting start hp
	m_PlayerHp = m_PlayerMaxHp;
	m_BossHp = m_BossMaxHp;

	// setting up HpSrcRect
	m_HpSrcRect.left = 0.f;
	m_HpSrcRect.bottom = m_pTextureManager->GetTexture("playerHP")->GetHeight();
	m_HpSrcRect.width = m_pTextureManager->GetTexture("playerHP")->GetWidth() / 3.f;
	m_HpSrcRect.height = m_pTextureManager->GetTexture("playerHP")->GetHeight();

	// setting up drawRects
	// player hp
	m_PlayerHpRect.left = m_WindowBorder.width * 0.426f;
	m_PlayerHpRect.bottom = m_WindowBorder.height * 0.92f;
	m_PlayerHpRect.width = m_HpSrcRect.width * m_Scale;
	m_PlayerHpRect.height = m_HpSrcRect.height * m_Scale;

	// boss hp
	m_BossHpRect.left = m_WindowBorder.width * 0.965f;
	m_BossHpRect.bottom = m_WindowBorder.height * 0.92f;
	m_BossHpRect.width = m_HpSrcRect.width * m_Scale;
	m_BossHpRect.height = m_HpSrcRect.height * m_Scale;
}

// Draw
void PlayerHUD::DrawPlayerHp() const
{
	int hp{ m_PlayerHp };
	Rectf drawRect{ m_PlayerHpRect };
	Rectf srcRect{ m_HpSrcRect };

	for (int idx{}; idx < m_PlayerMaxHp; ++idx)
	{
		if (hp > 1)
		{
			// 2 hp texture
			srcRect.left = 0.f;
		}
		else if (hp > 0)
		{
			// 1 hp texture
			srcRect.left = srcRect.width; 
		}
		else
		{
			// 0 hp texture
			srcRect.left = srcRect.width * 2.f;
		}
		// draw
		m_pTextureManager->GetTexture("playerHP")->Draw(drawRect, srcRect);

		// change values
		drawRect.left += drawRect.width;
		hp -= 2;
		++idx;
	}
}

void PlayerHUD::DrawBossHp() const
{
	int hp{ m_BossHp };
	Rectf drawRect{ m_BossHpRect };
	Rectf srcRect{ m_HpSrcRect };

	for (int idx{}; idx < m_BossMaxHp; ++idx)
	{
		if (hp > 1)
		{
			// 2 hp texture
			srcRect.left = 0.f;
		}
		else if (hp > 0)
		{
			// 1 hp texture
			srcRect.left = srcRect.width;
		}
		else
		{
			// 0 hp texture
			srcRect.left = srcRect.width * 2.f;
		}
		// draw
		m_pTextureManager->GetTexture("bossHP")->Draw(drawRect, srcRect);

		// change values
		drawRect.left -= drawRect.width;
		hp -= 2;
		++idx;
	}
}