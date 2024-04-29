#include "pch.h"
#include "ObjectManager.h"
#include "Bubble.h"
#include "DirtPile.h"
#include "CheckPoint.h"
#include "GemsManager.h"
#include "Player.h"
#include "Platform.h"
#include "Rock.h"
#include "Platter.h"
#include "SoundManager.h"

// Constructors / Destructors
ObjectManager::ObjectManager(const std::string& levelKey, GemsManager* gemsManager, Player* player, SoundManager* soundManager, TextureManager* textureManager)
	: m_LevelKey{ levelKey }
	, m_pGemsManager{ gemsManager }
	, m_pPlayer{ player }
	, m_pSoundManager{ soundManager }
	, m_pTextureManager{ textureManager }
{
	std::cout << "\nCreating ObjectManager...\n";

	InitVariables();
	InitObjects();
}

ObjectManager::~ObjectManager()
{
	std::cout << "\nDestroying ObjectManager...\n";

	DestroyBubbles();
	DestroyDirtPiles();
	DestroyCheckPoints();
	DestroyPlatforms();
	DestroyRocks();
	DestroyPlatters();
}

// Update
void ObjectManager::Update(float elapsedSec, const Rectf& cameraRect)
{
	UpdateIsOnScreen(cameraRect);
	UpdateObjects(elapsedSec);
	UpdateCanBeDeleted();
}

// Draws
void ObjectManager::DrawDirtPiles() const
{
	for (size_t idx{}; idx < m_pDirtPiles.size(); ++idx)
	{
		m_pDirtPiles[idx]->Draw();
	}
}

void ObjectManager::DrawBubbles(const Point2f& cameraPos) const
{
	glPushMatrix();
	{
		glTranslatef(-cameraPos.x, -cameraPos.y, 0);

		for (size_t idx{}; idx < m_pBubbles.size(); ++idx)
		{
			m_pBubbles[idx]->Draw();
		}
	}
	glPopMatrix();
}

void ObjectManager::DrawCheckPoints() const
{
	for (size_t idx{}; idx < m_pCheckPoints.size(); ++idx)
	{
		m_pCheckPoints[idx]->Draw();
	}
}

void ObjectManager::DrawPlatform() const
{
	for (size_t idx{}; idx < m_pPlatforms.size(); ++idx)
	{
		m_pPlatforms[idx]->Draw();
	}
}

void ObjectManager::DrawRocks() const
{
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		m_pRocks[idx]->Draw();
	}
}

void ObjectManager::DrawPlatters() const
{
	for (size_t idx{}; idx < m_pPlatters.size(); ++idx)
	{
		m_pPlatters[idx]->Draw();
	}
}

// Getters
const std::vector<std::vector<Point2f>>& ObjectManager::GetRockVertices()
{
	return m_RockVertices;
}

// private funtions //
// Init
void ObjectManager::InitVariables()
{
	// resize vectors to expected capacity

	// Bubbles
	m_pBubbles.reserve(10);

	// DirtPiles
	m_pDirtPiles.reserve(10);

	// CheckPoints
	m_pCheckPoints.reserve(4);

	// Platforms
	m_pPlatforms.reserve(5);

	// Rocks
	m_pRocks.reserve(120);
	// Rocks Vertices
	m_RockVertices.reserve(120);

	// Platters
	m_pPlatters.reserve(2);
}

void ObjectManager::InitObjects()
{
	std::cout << "Creating all objects...\n";

	std::string pathName{"../../Resources/Saves/" + m_LevelKey + "/Other/objectsConfig.morrog"};
	// load from file
	std::ifstream inFile{};
	inFile.open(pathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::OBJECTMANGAGER::CPP::COULD_NOT_LOAD_OBJECTS_FROM_FILE: " << pathName << "\n";
	}
	else
	{
		std::string objectName{};
		Point2f pointOne{};
		Point2f pointTwo{};
		bool boolean{};
		float floatingPoint{};
		int integer{};

		while (inFile >> objectName)
		{
			if (objectName == "bubble")
			{
				inFile >> pointOne.x >> pointOne.y >> pointTwo.x >> pointTwo.y >> floatingPoint;
				AddBubble(pointOne, pointTwo, floatingPoint);
			}
			else if (objectName == "dirtPile")
			{
				inFile >> pointOne.x >> pointOne.y;
				AddDirtPile(pointOne);
			}
			else if (objectName == "checkpoint")
			{
				inFile >> pointOne.x >> pointOne.y;
				AddCheckPoint(pointOne);
			}
			else if (objectName == "platform")
			{
				inFile >> pointOne.x >> pointOne.y >> pointTwo.x >> pointTwo.y >> boolean >> floatingPoint;
				AddPlatform(pointOne, pointTwo, boolean, floatingPoint);
			}
			else if (objectName == "rock")
			{
				inFile >> pointOne.x >> pointOne.y >> boolean >> integer;
				AddRock(pointOne, boolean, integer);
			}
			else if (objectName == "platter")
			{
				inFile >> pointOne.x >> pointOne.y;
				AddPlatter(pointOne);
			}
			else
			{
				std::cerr << "ERROR::OBJECTMANAGER::CPP::COULD_NOT_FIND_SPECIFIED_OBJECTNAME\n";
			}
		}
	}
	inFile.close();

	InitRockVertices();

	std::cout << "All objects fully loaded!\n";
}

void ObjectManager::InitRockVertices()
{
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		m_RockVertices.push_back(m_pRocks[idx]->GetVertices());
	}
}

// Destroy
void ObjectManager::DestroyBubbles()
{
	for (size_t idx{}; idx < m_pBubbles.size(); ++idx)
	{
		delete m_pBubbles[idx];
	}
	m_pBubbles.clear();
}

void ObjectManager::DestroyDirtPiles()
{
	for (size_t idx{}; idx < m_pDirtPiles.size(); ++idx)
	{
		delete m_pDirtPiles[idx];
	}
	m_pDirtPiles.clear();
}

void ObjectManager::DestroyCheckPoints()
{
	for (size_t idx{}; idx < m_pCheckPoints.size(); ++idx)
	{
		delete m_pCheckPoints[idx];
	}
	m_pCheckPoints.clear();
}

void ObjectManager::DestroyPlatforms()
{
	for (size_t idx{}; idx < m_pPlatforms.size(); ++idx)
	{
		delete m_pPlatforms[idx];
	}
	m_pPlatforms.clear();
}

void ObjectManager::DestroyRocks()
{
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		delete m_pRocks[idx];
	}
	m_pRocks.clear();
}

void ObjectManager::DestroyPlatters()
{
	for (size_t idx{}; idx < m_pPlatters.size(); ++idx)
	{
		delete m_pPlatters[idx];
	}
	m_pPlatters.clear();
}

// Update
void ObjectManager::UpdateCanBeDeleted()
{
	// Bubbles	-> Cant be deleted
	
	// DirtPiles
	for (size_t idx{}; idx < m_pDirtPiles.size(); ++idx)
	{
		if (m_pDirtPiles[idx]->GetCanBeDeleted())
		{
			delete m_pDirtPiles[idx];
			if (idx != (m_pDirtPiles.size() - 1))
			{
				m_pDirtPiles[idx] = m_pDirtPiles.back();
			}
			m_pDirtPiles.pop_back();
		}
	}

	// CheckPoints	-> cant be deleted

	// Platforms	-> cant be deleted

	// rocks
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		if (m_pRocks[idx]->GetCanBeDeleted())
		{
			// rock vertice
			m_RockVertices[idx] = m_RockVertices.back();
			m_RockVertices.pop_back();

			// rock itself
			delete m_pRocks[idx];
			if (idx != (m_pRocks.size() - 1))
			{
				m_pRocks[idx] = m_pRocks.back();
			}
			m_pRocks.pop_back();
		}
	}

	// Platers
	for (size_t idx{}; idx < m_pPlatters.size(); ++idx)
	{
		if (m_pPlatters[idx]->GetCanBeDeleted())
		{
			delete m_pPlatters[idx];
			if (idx != (m_pPlatters.size() - 1))
			{
				m_pPlatters[idx] = m_pPlatters.back();
			}
			m_pPlatters.pop_back();
		}
	}
}

void ObjectManager::UpdateRocksVertices()
{
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		if (m_pRocks[idx]->GetIsOnScreen())
		{
			if (m_pRocks[idx]->GetIsBroken())
			{
				m_RockVertices[idx] = std::vector< Point2f >{};
			}
		}
	}
}

void ObjectManager::UpdateBubbles(float elapsedSec)
{
	for (size_t idx{}; idx < m_pBubbles.size(); ++idx)
	{
		if (!m_pBubbles[idx]->GetIsOnScreen())
		{
			continue;
		}

		if (utils::IsOverlapping(m_pPlayer->GetAttackHitbox(), m_pBubbles[idx]->GetHitbox()))
		{
			if (m_pPlayer->GetIsJumpAttacking())
			{
				m_pPlayer->Knock(true, m_pBubbles[idx]->GetPlayerKnockUp());
			}
			m_pBubbles[idx]->Hit();
		}

		m_pBubbles[idx]->Update(elapsedSec);

		if (!m_pBubbles[idx]->GetIsHit())
		{
			if (utils::IsOverlapping(m_pPlayer->GetHitBox(), m_pBubbles[idx]->GetHitbox()))
			{
				m_pBubbles[idx]->Hit();
				m_pPlayer->Hit();

				m_pPlayer->Knock(false, 500.f);
				m_pPlayer->Knock(true, 1000.f);
			}
		}
	}
}

void ObjectManager::UpdateDirtPiles(float elapsedSec)
{
	for (size_t idx{}; idx < m_pDirtPiles.size(); ++idx)
	{
		if (!m_pDirtPiles[idx]->GetIsOnScreen())
		{
			continue;
		}

		if (!m_pPlayer->GetIsJumpAttacking())
		{
			if (utils::IsOverlapping(m_pPlayer->GetAttackHitbox(), m_pDirtPiles[idx]->GetHitbox()))
			{
				m_pDirtPiles[idx]->Hit(m_pPlayer->GetDir(), m_pGemsManager);
			}
		}

		m_pDirtPiles[idx]->Update(elapsedSec);
	}
}

void ObjectManager::UpdateCheckPoints(float elapsedSec)
{
	for (size_t idx{}; idx < m_pCheckPoints.size(); ++idx)
	{
		if (!m_pCheckPoints[idx]->GetIsOnScreen())
		{
			continue;
		}

		m_pCheckPoints[idx]->Update(elapsedSec, m_pPlayer, m_pSoundManager);
	}
}

void ObjectManager::UpdatePlatforms(float elapsedSec)
{
	bool isOnPlatform{ false };

	for (size_t idx{}; idx < m_pPlatforms.size(); ++idx)
	{
		if (!m_pPlatforms[idx]->GetIsOnScreen())
		{
			continue;
		}

		if (m_pPlatforms[idx]->Update(elapsedSec, m_pPlayer))
		{
			isOnPlatform = true;
		}
	}
	m_pPlayer->SetIsOnPlatform(isOnPlatform);
}

void ObjectManager::UpdateRocks(float elapsedSec)
{
	bool isOnRock{ false };

	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		if (!m_pRocks[idx]->GetIsOnScreen())
		{
			continue;
		}

		if (utils::IsOverlapping(m_pPlayer->GetAttackHitbox(), m_pRocks[idx]->GetHitbox()))
		{
			m_pRocks[idx]->Hit(m_pGemsManager, m_pPlayer, m_pSoundManager);
		}
		UpdateRocksVertices();

		if (m_pRocks[idx]->Update(elapsedSec, m_pPlayer))
		{
			isOnRock = true;
		}
	}
	m_pPlayer->SetIsOnRock(isOnRock);
}

void ObjectManager::UpdatePlattes(float elapsedSec)
{
	for (size_t idx{}; idx < m_pPlatters.size(); ++idx)
	{
		if (!m_pPlatters[idx]->GetIsOnScreen())
		{
			continue;
		}

		if (utils::IsOverlapping(m_pPlayer->GetAttackHitbox(), m_pPlatters[idx]->GetHitbox()))
		{
			m_pPlatters[idx]->Hit(m_pPlayer, m_pSoundManager);
		}

		m_pPlatters[idx]->Update(elapsedSec, m_pPlayer, m_pSoundManager);
	}
}

void ObjectManager::UpdateIsOnScreen(const Rectf& cameraRect)
{
	// bubbles
	for (size_t idx{}; idx < m_pBubbles.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pBubbles[idx]->GetHitbox()))
		{
			m_pBubbles[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pBubbles[idx]->SetIsOnScreen(false);
		}
	}

	// dirtPiles
	for (size_t idx{}; idx < m_pDirtPiles.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pDirtPiles[idx]->GetHitbox()))
		{
			m_pDirtPiles[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pDirtPiles[idx]->SetIsOnScreen(false);
		}
	}

	// checkPoints
	for (size_t idx{}; idx < m_pCheckPoints.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pCheckPoints[idx]->GetHitBox()))
		{
			m_pCheckPoints[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pCheckPoints[idx]->SetIsOnScreen(false);
		}
	}

	// platforms
	for (size_t idx{}; idx < m_pPlatforms.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pPlatforms[idx]->GetHitbox()))
		{
			m_pPlatforms[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pPlatforms[idx]->SetIsOnScreen(false);
		}
	}

	// rocks
	for (size_t idx{}; idx < m_pRocks.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pRocks[idx]->GetHitbox()))
		{
			m_pRocks[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pRocks[idx]->SetIsOnScreen(false);
		}
	}

	// platers
	for (size_t idx{}; idx < m_pPlatters.size(); ++idx)
	{
		if (IsOnScreen(cameraRect, m_pPlatters[idx]->GetHitbox(), 250.f))
		{
			m_pPlatters[idx]->SetIsOnScreen(true);
		}
		else
		{
			m_pPlatters[idx]->SetIsOnScreen(false);
		}
	}
}

bool ObjectManager::IsOnScreen(const Rectf& cameraRect, const Rectf& objectHitBox, float border)
{
	if (utils::IsOverlapping(objectHitBox, Rectf{ cameraRect.left - border, cameraRect.bottom - border, cameraRect.width + border * 2.f, cameraRect.height + border * 2.f }))
	{
		return true;
	}
	return false;
}

void ObjectManager::UpdateObjects(float elapsedSec)
{
	UpdateBubbles(elapsedSec);
	UpdateDirtPiles(elapsedSec);
	UpdateCheckPoints(elapsedSec);
	UpdatePlatforms(elapsedSec);
	UpdateRocks(elapsedSec);
	UpdatePlattes(elapsedSec);
}

// Other
void ObjectManager::AddBubble(const Point2f& startPos, const Point2f& endPos, const float speed)
{
	m_pBubbles.emplace_back(new Bubble{ startPos, endPos, speed, m_pTextureManager });
}

void ObjectManager::AddDirtPile(const Point2f& dirtPilePos)
{
	m_pDirtPiles.emplace_back(new DirtPile{ dirtPilePos, m_pTextureManager });
}

void ObjectManager::AddCheckPoint(const Point2f& checkPointPos)
{
	m_pCheckPoints.emplace_back(new CheckPoint{ checkPointPos, m_pTextureManager });
}

void ObjectManager::AddPlatform(const Point2f& platformStartPos, const Point2f& platformEndPos, bool moveDir, const float speed)
{
	m_pPlatforms.emplace_back(new Platform{ platformStartPos, platformEndPos, moveDir, speed, m_pTextureManager });
}

void ObjectManager::AddRock(const Point2f& rockPos, bool rockType, const int breakValue)
{
	m_pRocks.emplace_back(new Rock{ rockPos, breakValue, rockType, m_pTextureManager });
}

void ObjectManager::AddPlatter(const Point2f& platerPos)
{
	m_pPlatters.emplace_back(new Platter{ platerPos, m_pTextureManager });
}