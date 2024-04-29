#include "pch.h"
#include "GemsManager.h"
#include "Player.h"
#include "PlayerHUD.h"
#include "Level.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Constructors / Destructors
GemsManager::GemsManager(const std::string& levelKey, SoundManager* soundManager, Player* player, TextureManager* textureManager)
	: m_LevelKey{ levelKey }
	, m_pSoundManager{ soundManager }
	, m_pPlayer{ player}
	, m_pTextureManager{ textureManager }
	, m_GemsGravityForce{ -1500 }
	, m_DecelerationForce{ 250 }
	, m_Scale{ 3.5f }
{
	std::cout << "\nCreating GemManager...\n";
	InitGemSrcRects();
	InitGemsFromFile();
}

GemsManager::~GemsManager()
{
	std::cout << "\nDestroying GemManager...\n";
	DestroyGems();
}

// Update / Draw
void GemsManager::Update(float elapsedSec, const Rectf& cameraRect, PlayerHUD* playerHUD, Level* level, ObjectManager* objectManager)
{
	UpdateIsOnScreen(cameraRect);
	UpdateGems(elapsedSec, level->GetVertices(), objectManager->GetRockVertices());
	UpdatePlayerCollision(playerHUD);
	UpdateExistTime(elapsedSec);
}

void GemsManager::Draw() const
{
	const Texture* gemTexture{ m_pTextureManager->GetTexture("gems") };

	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		if (m_pGems[idx]->isOnScreen)
		{
			gemTexture->Draw(m_pGems[idx]->hitbox, m_pGems[idx]->srcRect);
		}
	}
}

// Ohter
void GemsManager::AddGems(const Rectf& target, int totalValue, const Vector2f& gemDir, bool timeDelete)	// no const & because i change value of gemPos (certainly need a copy)
{
	Point2f gemPos{ target.left + target.width / 2.f, target.bottom + target.height / 2.f };
	// setup
	const float speed{ 300 };
	const float offset{ 15.f };
	Vector2f velocity{ gemDir.x * speed, gemDir.y * speed };

	int amount{};

	// 200
	amount = totalValue / 200;
	totalValue -= amount * 200;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 200, velocity, timeDelete);
	}

	// 50
	amount = totalValue / 50;
	totalValue -= amount * 50;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 50, velocity, timeDelete);
	}

	// 20
	amount = totalValue / 20;
	totalValue -= amount * 20;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 20, velocity, timeDelete);
	}

	// 10
	amount = totalValue / 10;
	totalValue -= amount * 10;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 10, velocity, timeDelete);
	}
	
	// 5
	amount = totalValue / 5;
	totalValue -= amount * 5;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 5, velocity, timeDelete);
	}
	
	// 1
	amount = totalValue;
	for (int idx{}; idx < amount; ++idx)
	{
		GetCloseGemPos(gemPos, offset);
		AddSingleGem(gemPos, 1, velocity, timeDelete);
	}
}

// Private Functions //
// Init
void GemsManager::InitGemSrcRects()
{
	m_GemSrcRects.reserve(6);

	Rectf srcRect{};

	// 1 value (gray gem)
	srcRect.left = 0.f;
	srcRect.bottom = 16.f;
	srcRect.width = 6.f;
	srcRect.height = 6.f;
	m_GemSrcRects.emplace_back(srcRect);

	// 5 value (blue gem)
	srcRect.left = 6.f;
	srcRect.width = 8.f;
	srcRect.height = 8.f;
	m_GemSrcRects.emplace_back(srcRect);

	// 10 value (green gem)
	srcRect.left = 14.f;
	srcRect.width = 9.f;
	srcRect.height = 12.f;
	m_GemSrcRects.emplace_back(srcRect);

	// 20 value (gold coin gem)
	srcRect.left = 23.f;
	srcRect.width = 13.f;
	srcRect.height = 9.f;
	m_GemSrcRects.emplace_back(srcRect);

	// 50 value (red diamond gem)
	srcRect.left = 36.f;
	srcRect.width = 17.f;
	srcRect.height = 13.f;
	m_GemSrcRects.emplace_back(srcRect);

	// 200 value (pink diamond gem)
	srcRect.left = 53.f;
	srcRect.width = 21.f;
	srcRect.height = 16.f;
	m_GemSrcRects.emplace_back(srcRect);
}

void GemsManager::InitGemsFromFile()
{
	std::cout << "Loading all gems...\n";

	m_pGems.reserve(50);

	std::string pathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/gemsConfig.morrog" };
	// load from file
	std::ifstream inFile{};
	inFile.open(pathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::GEMSMANAGER::CPP::COULD_NOT_LOAD_GEMS_FROM_FILE: " << pathName << "\n";
	}
	else
	{
		float x{};
		float y{};
		Point2f gemPos{};
		int value{};

		while (inFile >> x >> y >> value)
		{
			gemPos.x = x;
			gemPos.y = y;

			AddSingleGem(gemPos, value, Vector2f{0.f, 0.f}, false, false);
		}
	}
	inFile.close();

	std::cout << "All gems loaded!\n";
}

// Destroy
void GemsManager::DestroyGems()
{
	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		delete m_pGems[idx];
	}
	m_pGems.clear();
}

// Update
void GemsManager::UpdateIsOnScreen(const Rectf& cameraRect)
{
	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		if (utils::IsOverlapping(m_pGems[idx]->hitbox, cameraRect))
		{
			m_pGems[idx]->isOnScreen = true;
		}
		else
		{
			m_pGems[idx]->isOnScreen = false;
		}
	}
}

void GemsManager::UpdateGems(float elapsedSec, const std::vector<std::vector<Point2f>>& vertices, const std::vector<std::vector<Point2f>>& rockVertices)
{
	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		if (m_pGems[idx]->isOnScreen)
		{
			if (m_pGems[idx]->hasGravity)
			{
				UpdateGemsVelocity(int(idx), elapsedSec);
				m_pGems[idx]->hitbox.left += m_pGems[idx]->velocity.x * elapsedSec;
				m_pGems[idx]->hitbox.bottom += m_pGems[idx]->velocity.y * elapsedSec;
			}

			Point2f pointTop{ m_pGems[idx]->hitbox.left + m_pGems[idx]->hitbox.width / 2.f, m_pGems[idx]->hitbox.bottom + m_pGems[idx]->hitbox.height };
			Point2f pointBot{ m_pGems[idx]->hitbox.left + m_pGems[idx]->hitbox.width / 2.f, m_pGems[idx]->hitbox.bottom - 1.f };

			UpdateGemVerticesColl(pointTop, pointBot, int(idx), vertices);
			UpdateGemVerticesColl(pointTop, pointBot, int(idx), rockVertices);
		}
	}
}

void GemsManager::UpdateGemsVelocity(int idx, float elapsedSec)
{
	// gravity
	m_pGems[idx]->velocity.y += m_GemsGravityForce * elapsedSec;

	// deceleration
	if (m_pGems[idx]->velocity.x > 0.f)
	{
		// decelerate
		m_pGems[idx]->velocity.x -= m_DecelerationForce * elapsedSec;

		if (m_pGems[idx]->velocity.x < 0.f)
		{
			m_pGems[idx]->velocity.x = 0.f;
		}
	}
	else if (m_pGems[idx]->velocity.x < 0.f)
	{
		// decelerate
		m_pGems[idx]->velocity.x += m_DecelerationForce * elapsedSec;

		if (m_pGems[idx]->velocity.x > 0.f)
		{
			m_pGems[idx]->velocity.x = 0.f;
		}
	}
}

void GemsManager::UpdateExistTime(float elapsedSec)
{
	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		m_pGems[idx]->existTime += elapsedSec;

		if (m_pGems[idx]->timeDelete)
		{
			if (m_pGems[idx]->existTime > 10.f)
			{
				DeleteGem(int(idx));
			}
		}
	}
}

void GemsManager::UpdatePlayerCollision(PlayerHUD* playerHUD)
{
	for (size_t idx{}; idx < m_pGems.size(); ++idx)
	{
		if (m_pGems[idx]->existTime > 0.45f)
		{
			if (utils::IsOverlapping(m_pGems[idx]->hitbox, m_pPlayer->GetHitBox()))
			{
				playerHUD->AddGold(m_pGems[idx]->value);

				if (m_pGems[idx]->value == 1)
				{
					m_pSoundManager->GetSoundEffect("greygem")->Play(false);
				}
				else if (m_pGems[idx]->value > 200)
				{
					m_pSoundManager->GetSoundEffect("redgem")->Play(false);
				}
				else
				{
					m_pSoundManager->GetSoundEffect("gem")->Play(false);
				}
				DeleteGem(int(idx));
			}
		}
	}
}

void GemsManager::UpdateGemVerticesColl(const Point2f& pointTop, const Point2f& pointBot, int gem, const std::vector< std::vector< Point2f > >& vertices)
{
	utils::HitInfo hitInfo{};

	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		if (utils::Raycast(vertices[idx], pointBot, pointTop, hitInfo))
		{
			m_pGems[gem]->velocity.y = 0.f;
			if (hitInfo.lambda < 0.5f)
			{
				m_pGems[gem]->hitbox.bottom = hitInfo.intersectPoint.y + 1.f;
				return;
			}
			m_pGems[gem]->hitbox.bottom = hitInfo.intersectPoint.y - m_pGems[gem]->hitbox.height - 1.5f;
			return;
		}
	}
}

// Other
void GemsManager::AddSingleGem(const Point2f& gemPos, int value, const Vector2f& velocity, bool timeDelete, bool gravity)
{
	Rectf hitbox{ gemPos, 0.f, 0.f };

	switch (value)
	{
	case 1:
		hitbox.width = m_GemSrcRects[0].width * m_Scale;
		hitbox.height = m_GemSrcRects[0].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[0], velocity, timeDelete, gravity });
		break;
	case 5:
		hitbox.width = m_GemSrcRects[1].width * m_Scale;
		hitbox.height = m_GemSrcRects[1].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[1], velocity, timeDelete, gravity });
		break;
	case 10:
		hitbox.width = m_GemSrcRects[2].width * m_Scale;
		hitbox.height = m_GemSrcRects[2].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[2], velocity, timeDelete, gravity });
		break;
	case 20:
		hitbox.width = m_GemSrcRects[3].width * m_Scale;
		hitbox.height = m_GemSrcRects[3].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[3], velocity, timeDelete, gravity });
		break;
	case 50:
		hitbox.width = m_GemSrcRects[4].width * m_Scale;
		hitbox.height = m_GemSrcRects[4].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[4], velocity, timeDelete, gravity });
		break;
	case 200:
		hitbox.width = m_GemSrcRects[5].width * m_Scale;
		hitbox.height = m_GemSrcRects[5].height * m_Scale;
		m_pGems.emplace_back(new Gem{ value, hitbox, m_GemSrcRects[5], velocity, timeDelete, gravity });
		break;
	default:
		break;
	}
}

void GemsManager::DeleteGem(int idx)
{
	if (idx < m_pGems.size())
	{
		delete m_pGems[idx];
		m_pGems[idx] = m_pGems.back();
		m_pGems.pop_back();
	}
}

void GemsManager::GetCloseGemPos(Point2f& gemPos, float offset)
{
	gemPos.x -= offset;

	gemPos.x += float(rand() % int(offset * 2.f));
	gemPos.y += float(rand() % int(offset));
}