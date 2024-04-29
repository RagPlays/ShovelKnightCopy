#pragma once

class Texture;
class Number;
class TextureManager;

class PlayerHUD final
{
public:
	// Constructors / Destructors
	explicit PlayerHUD(const Rectf& windowBorder, int playerMaxHP, int bossMaxHP, TextureManager* textureManager);
	~PlayerHUD();

	PlayerHUD(const PlayerHUD& playerHUD) = delete;
	PlayerHUD& operator=(const PlayerHUD& playerHUD) = delete;
	PlayerHUD(PlayerHUD&& playerHUD) noexcept = delete;
	PlayerHUD& operator=(PlayerHUD&& playerHUD) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, int playerHp, int bossHp);
	void Draw() const;

	// Other
	void AddGold(int addGoldValue);

private:

	const float m_Scale;
	const Rectf m_WindowBorder;

	TextureManager* m_pTextureManager;

	Rectf m_PlayerHpRect;
	Rectf m_BossHpRect;
	Point2f m_GoldNumberPos;
	
	Rectf m_HudDrawRect;
	Rectf m_HpSrcRect;

	Number* m_pGoldNumber;

	int m_PlayerHp;
	int m_PlayerMaxHp;
	int m_BossHp;
	int m_BossMaxHp;
	int m_MagicValue;

private:

	// Init
	void InitVariables();

	// Draw
	void DrawPlayerHp() const;
	void DrawBossHp() const;
};