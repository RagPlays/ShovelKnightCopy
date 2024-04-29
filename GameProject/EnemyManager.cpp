#include "pch.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Beeto.h"
#include "Blorp.h"
#include "Player.h"
#include "GemsManager.h"
#include "Level.h"
#include "ObjectManager.h"
#include "DozeDragon.h"

// Contructors / Destructors
EnemyManager::EnemyManager(Level* level, const std::string& levelKey, GemsManager* gemsManager, Player* player, ObjectManager* objectManager, TextureManager* textureManager)
	: m_pLevel{ level }
	, m_LevelKey{ levelKey }
	, m_pGemsManager{ gemsManager }
	, m_pPlayer{ player }
	, m_pObjectManager{ objectManager }
	, m_pTextureManager{ textureManager }
{
	std::cout << "\nCreating EnemyManager...\n";

	InitVariables();
	InitEnemies();
}

EnemyManager::~EnemyManager()
{
	std::cout << "\nDestroying EnemyManager...\n";

	DestroyEnemies();
	DestroyMiniBosses();
}

// Update / Draw
void EnemyManager::Update(float elapsedSec, const Rectf& cameraRect)
{
	UpdateIsOnScreen(cameraRect);
	UpdateEnemies(elapsedSec);
	UpdateMiniBosses(elapsedSec, cameraRect);
	UpdateDeletions();
}

void EnemyManager::Draw() const
{
	DrawEnemies();
	DrawMiniBosses();
}

// Getters / Setters
float EnemyManager::GetKnockUpVelocity() const
{
	return m_CurrentKnockUpVelocity;
}

bool EnemyManager::DidLastHitKill() const
{
	return m_IsDeadAfterHit;
}

// Private functions // 
// Init
void EnemyManager::InitVariables()
{
	m_CurrentKnockUpVelocity = 0.f;
	m_IsDeadAfterHit = false;
}

void EnemyManager::InitEnemies()
{
	std::string pathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/enemyConfig.morrog" };

	std::ifstream inFile{};
	inFile.open(pathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::GAMESTATE::CPP::COULD_NOT_LOAD_TILEMAP_FROM_FILE: " << pathName << "\n";
	}
	else
	{
		std::string enemyName{};
		float x{};
		float y{};
		bool enemyDir{};

		while (inFile >> enemyName)
		{
			if (enemyName == "beeto")
			{
				inFile >> x >> y >> enemyDir;
				m_pEnemies.emplace_back(new Beeto{ Point2f{x, y}, enemyDir, m_pTextureManager});
			}
			else if (enemyName == "blorp")
			{
				inFile >> x >> y >> enemyDir;
				m_pEnemies.emplace_back(new Blorp{Point2f{x, y}, enemyDir , m_pTextureManager });
			}
			// Bosses
			else if (enemyName == "dozedragon")
			{
				inFile >> x >> y;
				m_pMiniBosses.emplace_back(new DozeDragon{ Point2f{x, y }, m_pTextureManager });
			}
			else
			{
				std::cerr << "ENEMYMANAGER::CPP::COULD_NOT_ADD_INPUTTED_ENEMY\n";
			}
		}
	}
	inFile.close();

	std::cout << "All Enemies Loaded...\n";
}

// Destroy
void EnemyManager::DestroyEnemies()
{
	for (size_t idx{}; idx < m_pEnemies.size(); ++idx)
	{
		delete m_pEnemies[idx];
	}
	m_pEnemies.clear();
}

void EnemyManager::DestroyMiniBosses()
{
	for (size_t idx{}; idx < m_pMiniBosses.size(); ++idx)
	{
		delete m_pMiniBosses[idx];
	}
	m_pMiniBosses.clear();
}

void EnemyManager::DestroyEnemy(int idx)
{
	delete m_pEnemies[idx];
	if (idx < m_pEnemies.size() - 1)
	{
		m_pEnemies[idx] = m_pEnemies.back();
	}
	m_pEnemies.pop_back();
}

void EnemyManager::DestroyMiniBoss(int idx)
{
	delete m_pMiniBosses[idx];
	if (idx < m_pMiniBosses.size() - 1)
	{
		m_pMiniBosses[idx] = m_pMiniBosses.back();
	}
	m_pMiniBosses.pop_back();
}

// Update
void EnemyManager::UpdateIsOnScreen(const Rectf& cameraRect)
{
	// Enemies
	for (size_t idx{}; idx < m_pEnemies.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pEnemies[idx]->GetHitBox()))
		{
			m_pEnemies[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pEnemies[idx]->SetIsOnScreen(false);
		}
	}

	// MiniBosses
	for (size_t idx{}; idx < m_pMiniBosses.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pMiniBosses[idx]->GetOnCameraHitbox()))
		{
			m_pMiniBosses[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pMiniBosses[idx]->SetIsOnScreen(false);
		}
	}
}

void EnemyManager::UpdateEnemies(float elapsedSec)
{
	// Update the ones that are on screen
	for (size_t enemyNr{}; enemyNr < m_pEnemies.size(); ++enemyNr)
	{
		if (m_pEnemies[enemyNr]->GetIsOnScreen())
		{
			if (utils::IsOverlapping(m_pEnemies[enemyNr]->GetHitBox(), m_pPlayer->GetAttackHitbox()))
			{
				if (m_pPlayer->GetIsJumpAttacking())
				{
					m_pPlayer->Knock(true, m_pEnemies[enemyNr]->JumpHit());
				}
				else
				{
					m_pEnemies[enemyNr]->Hit(m_pPlayer->GetDir());

					if (!m_pEnemies[enemyNr]->GetIsDead())
					{
						m_pPlayer->Knock(false, 280.f);
					}
				}
			}

			// default updates
			m_pEnemies[enemyNr]->UpdateRays();
			m_pEnemies[enemyNr]->HandleIsOnGround(m_pLevel->GetVertices());
			m_pEnemies[enemyNr]->Update(elapsedSec, m_pLevel->GetVertices(), m_pPlayer->GetHitBox(), m_pObjectManager->GetRockVertices());

			if (m_pEnemies[enemyNr]->GetIsPoefAnimationOn())
			{
				if (!m_pEnemies[enemyNr]->GetValueIsGiven())
				{
					m_pEnemies[enemyNr]->SetValueIsGiven(true);
					m_pGemsManager->AddGems(m_pEnemies[enemyNr]->GetHitBox(), m_pEnemies[enemyNr]->GetKillValue());
				}
			}

			// UpdateCheckHit
			if (!m_pEnemies[enemyNr]->GetIsDead())
			{
				if (utils::IsOverlapping(m_pEnemies[enemyNr]->GetHitBox(), m_pPlayer->GetHitBox()))
				{
					if (!m_pPlayer->GetIsHit())
					{
						// set Hit Knocks

						// in x
						m_pPlayer->Knock(false, 500.f);
						// in y
						m_pPlayer->Knock(true, 1000.f);

						// lower the hp
						m_pPlayer->Hit();
					}
				}
			}
		}
	}

	// resets variables
	m_CurrentKnockUpVelocity = 0.f;
	m_IsDeadAfterHit = false;
}

void EnemyManager::UpdateMiniBosses(float elapsedSec, const Rectf& cameraRect)
{
	for (size_t idx{}; idx < m_pMiniBosses.size(); ++idx)
	{
		if (m_pMiniBosses[idx]->GetIsOnScreen())
		{
			m_pMiniBosses[idx]->Update(elapsedSec, m_pPlayer, cameraRect);
		}
	}
}

void EnemyManager::UpdateDeletions()
{
	for (size_t enemyNr{}; enemyNr < m_pEnemies.size(); ++enemyNr)
	{
		if (m_pEnemies[enemyNr]->GetCanBeDeleted())
		{
			DestroyEnemy(int(enemyNr));
		}
	}

	for (size_t idx{}; idx < m_pMiniBosses.size(); ++idx)
	{
		if (m_pMiniBosses[idx]->GetCanBeDeleted())
		{
			DestroyMiniBoss(int(idx));
		}
	}
}

// Draw
void EnemyManager::DrawEnemies() const
{
	for (size_t idx{}; idx < m_pEnemies.size(); ++idx)
	{
		if (m_pEnemies[idx]->GetIsOnScreen())
		{
			m_pEnemies[idx]->Draw();
		}
	}
}

void EnemyManager::DrawMiniBosses() const
{
	for (size_t idx{}; idx < m_pMiniBosses.size(); ++idx)
	{
		if (m_pMiniBosses[idx]->GetIsOnScreen())
		{
			m_pMiniBosses[idx]->Draw();
		}
	}
}

// Other
bool EnemyManager::IsOnScreen(const Rectf cameraRect, const Rectf& enemyHitbox, const float border)
{
	// This function will be called so only the enemies that are almoste in/in the cameraview will be drawn and updated

	if (utils::IsOverlapping(enemyHitbox, Rectf{ cameraRect.left - border, cameraRect.bottom - border, cameraRect.width + border * 2, cameraRect.height + border * 2 }))
	{
		return true;
	}
	return false;
}