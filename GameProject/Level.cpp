#include "pch.h"
#include "Level.h"
#include "TileMap.h"
#include "SVGParser.h"
#include "Player.h"
#include "TextureManager.h"

// Constructors / Destructors
Level::Level(const Rectf& windowBorder, const std::string& levelKey, TextureManager* textureManager)
	: m_WindowBorder{ windowBorder }
	, m_LevelKey{ levelKey }
	, m_pTextureManager{ textureManager }
	, m_WaterViewMode{ WaterViewMode::one }
	, m_WaterViewTime{ 0.f }
{
	InitVariables();
	InitMaps();
	InitVertices();
	InitLadderRects();
	InitDeadRects();
}

Level::~Level()
{
	DestroyMaps();
	DestroyLadderRects();
}

// Update
void Level::Update(float elapsedSec, const Point2f& cameraPos)
{	
	UpdateWaterView(elapsedSec);
	UpdateMapRenderAreas(cameraPos);
}

// Draw
void Level::DrawBack(const Point2f& cameraPos) const
{
	m_pTextureManager->GetTexture("back" + m_LevelKey)->Draw(m_CloudDestRect);

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pCastleMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pCastleMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pLightForestMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pLightForestMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pDarkForestOneMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pDarkForestOneMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pDarkForestTwoMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pDarkForestTwoMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pWaterTileMaps[0]->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pWaterTileMaps[int(m_WaterViewMode)]->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pCaveMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pCaveMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pLadderMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pLadderMap->Draw();
	}
	glPopMatrix();
}

void Level::DrawFront(const Point2f& cameraPos) const
{
	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pBaseMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pBaseMap->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x * m_pForeGroundMap->GetParallaxScale())), -cameraPos.y, 0.f);
		m_pForeGroundMap->Draw();
	}
	glPopMatrix();

	/*glPushMatrix();
	{
		glTranslatef(float(int(-cameraPos.x)), float(int(-cameraPos.y)), 0.f);

		DrawVertices();
		DrawLadderRects();
		DrawDeadRects();
	}
	glPopMatrix();*/
}

// Getters
const std::vector< std::vector< Point2f > >& Level::GetVertices()
{
	return m_Vertices;
}

const std::vector<LadderRectf*>& Level::GetLadderRects()
{
	return m_pLadderRects;
}

const std::vector<Rectf>& Level::GetDeadRects()
{
	return m_DeadRects;
}

// Private Functions //
// Init
void Level::InitVariables()
{
	m_CloudDestRect.width = m_WindowBorder.width;
	m_CloudDestRect.height = m_pTextureManager->GetTexture("back" + m_LevelKey)->GetHeight() * (m_CloudDestRect.width / m_pTextureManager->GetTexture("back" + m_LevelKey)->GetWidth());
	m_CloudDestRect.left = 0.f;
	m_CloudDestRect.bottom = 0.f;
}

void Level::InitMaps()
{
	// Best gridSize = 56 = 3.5f * 16 pxls //
	std::cout << "\n";

	/// Init all tileMaps ///
	
	// background -> is loaded

	// castle
	m_pCastleMap = new TileMap{m_pTextureManager};

	// light forest
	m_pLightForestMap = new TileMap{ m_pTextureManager };

	// dark forest one
	m_pDarkForestOneMap = new TileMap{ m_pTextureManager };

	// dark forest two
	m_pDarkForestTwoMap = new TileMap{ m_pTextureManager };

	// water
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });

	// cave 
	m_pCaveMap = new TileMap{ m_pTextureManager };

	// Base map
	m_pBaseMap = new TileMap{ m_pTextureManager };

	// ladders
	m_pLadderMap = new TileMap{ m_pTextureManager };

	// foreground
	m_pForeGroundMap = new TileMap{ m_pTextureManager };
	
	// Load all maps
	LoadMaps();
}

void Level::LoadMaps()
{
	/// LOADING MAPS ///

	std::cout << "\n";
	const std::string defautlPath{ m_LevelKey + "/Map/" };

	// background --> doesnt need any loading

	// castle
	m_pCastleMap->LoadFromFile(defautlPath + "castleMap.morrog");

	// light forest
	m_pLightForestMap->LoadFromFile(defautlPath + "lightForest.morrog");

	// dark forest one
	m_pDarkForestOneMap->LoadFromFile(defautlPath + "darkForestOne.morrog");

	// dark forest two
	m_pDarkForestTwoMap->LoadFromFile(defautlPath + "darkForestTwo.morrog");

	// water
	m_pWaterTileMaps[0]->LoadFromFile(defautlPath + "waterOne.morrog");
	m_pWaterTileMaps[1]->LoadFromFile(defautlPath + "waterTwo.morrog");
	m_pWaterTileMaps[2]->LoadFromFile(defautlPath + "waterThree.morrog");

	// cave
	m_pCaveMap->LoadFromFile(defautlPath + "caveMap.morrog");

	// baseMap
	m_pBaseMap->LoadFromFile(defautlPath + "baseMap.morrog");

	// ladders
	m_pLadderMap->LoadFromFile(defautlPath + "ladderMap.morrog");

	// foreground
	m_pForeGroundMap->LoadFromFile(defautlPath + "foreGround.morrog");

	std::cout << "\nAll Maps Loaded!\n";
}

void Level::InitVertices()
{
	const std::string filePath{ "Saves/" + m_LevelKey + "/Other/collision.svg" };

	SVGParser::GetVerticesFromSvgFile(filePath, m_Vertices);
}

void Level::InitLadderRects()
{
	std::cout << "Loading LadderRects...\n";

	const std::string loadPathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/ladderRects.morrog" };

	std::ifstream inFile{};
	inFile.open(loadPathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::LEVEL::CPP::COULD_NOT_LOAD_LADDERRECTS\n";
	}
	else
	{
		float left{};
		float bottom{};
		float width{};
		float height{};
		bool endRect{};

		while (inFile >> left >> bottom >> width >> height >> endRect)
		{
			m_pLadderRects.emplace_back( new LadderRectf{left, bottom, width, height, endRect });
		}
	}
	inFile.close();

	std::cout << "All ladderRects loaded!\n";
}

void Level::InitDeadRects()
{
	std::cout << "Loading DeadRects...\n";

	const std::string loadPathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/deadRects.morrog" };

	std::ifstream inFile{};
	inFile.open(loadPathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::LEVEL::CPP::COULD_NOT_LOAD_DEADRECTS\n";
	}
	else
	{
		float left{};
		float bottom{};
		float width{};
		float height{};
		bool endRect{};

		while (inFile >> left >> bottom >> width >> height)
		{
			m_DeadRects.emplace_back( Rectf{ left, bottom, width, height });
		}
	}
	inFile.close();

	std::cout << "All DeadRects loaded!\n";
}

// Destroy
void Level::DestroyMaps()
{
	std::cout << "\n";

	// background doenst need to be deleted

	// castle
	delete m_pCastleMap;
	m_pCastleMap = nullptr;

	// light forest
	delete m_pLightForestMap;
	m_pLightForestMap = nullptr;

	// dark forests
	delete m_pDarkForestOneMap;
	m_pDarkForestOneMap = nullptr;

	delete m_pDarkForestTwoMap;
	m_pDarkForestTwoMap = nullptr;

	// waterTileMaps
	for (size_t idx{}; idx < m_pWaterTileMaps.size(); ++idx)
	{
		delete m_pWaterTileMaps[idx];
	}
	m_pWaterTileMaps.clear();

	// Cave
	delete m_pCaveMap;
	m_pCaveMap = nullptr;

	// baseMap
	delete m_pBaseMap;
	m_pBaseMap = nullptr;

	// ladders
	delete m_pLadderMap;
	m_pLadderMap = nullptr;

	// gametilemap
	delete m_pForeGroundMap;
	m_pForeGroundMap = nullptr;
}

void Level::DestroyLadderRects()
{
	for (size_t idx{}; idx < m_pLadderRects.size(); ++idx)
	{
		delete m_pLadderRects[idx];
	}
	m_pLadderRects.clear();
}

// Update
void Level::UpdateWaterView(float elapsedSec)
{
	if (utils::GetClockTime(m_WaterViewTime, elapsedSec, 0.3f))
	{
		if (m_WaterViewMode == WaterViewMode::three)
		{
			m_WaterViewMode = WaterViewMode::one;
		}
		else
		{
			m_WaterViewMode = WaterViewMode(int(m_WaterViewMode) + 1);
		}
	}
}

void Level::UpdateMapRenderAreas(const Point2f& cameraPos)
{
	// background -> doesnt move -> just png

	// castle
	m_pCastleMap->SetDrawArea(Point2f{ cameraPos.x * m_pCastleMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// lightForest
	m_pLightForestMap->SetDrawArea(Point2f{ cameraPos.x * m_pLightForestMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// DarkForestOne
	m_pDarkForestOneMap->SetDrawArea(Point2f{ cameraPos.x * m_pDarkForestOneMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// DrakForestTwo
	m_pDarkForestTwoMap->SetDrawArea(Point2f{ cameraPos.x * m_pDarkForestTwoMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// Water
	m_pWaterTileMaps[int(m_WaterViewMode)]->SetDrawArea(Point2f{ cameraPos.x * m_pWaterTileMaps[int(m_WaterViewMode)]->GetParallaxScale(), cameraPos.y }, m_WindowBorder);

	// Cave
	m_pCaveMap->SetDrawArea(Point2f{ cameraPos.x * m_pCaveMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// BaseMap
	m_pBaseMap->SetDrawArea(Point2f{ cameraPos.x * m_pBaseMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// ladders
	m_pLadderMap->SetDrawArea(Point2f{ cameraPos.x * m_pLadderMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);

	// foreGround
	m_pForeGroundMap->SetDrawArea(Point2f{ cameraPos.x * m_pForeGroundMap->GetParallaxScale(), cameraPos.y}, m_WindowBorder);
}

// Draw
void Level::DrawVertices() const
{
	utils::SetRGBColor(255, 25, 200);
	for (size_t idx{}; idx < m_Vertices.size(); ++idx)
	{
		utils::DrawPolygon(m_Vertices[idx], true, 2.f);
	}
}

void Level::DrawLadderRects() const
{
	utils::SetRGBColor(230, 20, 100);
	for (size_t idx{}; idx < m_pLadderRects.size(); ++idx)
	{
		utils::DrawRect(m_pLadderRects[idx]->rect, 2.f);
	}
}

void Level::DrawDeadRects() const
{
	utils::SetRGBColor(250, 20, 20);
	for (size_t idx{}; idx < m_DeadRects.size(); ++idx)
	{
		utils::DrawRect(m_DeadRects[idx], 2.f);
	}
}