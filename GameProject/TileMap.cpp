#include "pch.h"
#include "TileMap.h"
#include "Camera.h"
#include "TextureManager.h"

// Constructor / Destructor
TileMap::TileMap(TextureManager* textureManager)
	: m_pTextureManager{ textureManager }
	, m_TileMapTextureKey{ }
	, m_MaxWorldSize{ }
	, m_GridSizeWorld{ 0 }
	, m_Scale{ 3.5f }
	, m_ParallaxScale{ 1.f }
	, m_StartX{}
	, m_EndX{}
	, m_StartY{}
	, m_EndY{}
{
	std::cout << "Creating TileMap...\n";
}

TileMap::~TileMap()
{
	std::cout << "Clearing TileMap...\n";

	DestroyTileMap();
}

// Draw
void TileMap::Draw() const
{
	const Texture* texture{ m_pTextureManager->GetTexture(m_TileMapTextureKey) };

	for (int x{ m_StartX }; x < m_EndX; ++x)
	{
		for (int y{ m_StartY }; y < m_EndY; ++y)
		{
			for (size_t idx{}; idx < m_pTileMap[x][y].size(); ++idx)
			{
				m_pTileMap[x][y][idx]->Draw(texture);
			}
		}
	}
}

// Getters / Setters
float TileMap::GetParallaxScale() const
{
	return m_ParallaxScale;
}

const Point2i& TileMap::GetMapDimentions() const
{
	return m_MaxWorldSize;
}

void TileMap::SetDrawArea(const Point2f& cameraPos, const Rectf& windowBorder, bool testing)
{
	const int offset{ 2 }; // to be certainted that everything that you need to see will be drawn
	// x
	m_StartX = int(cameraPos.x / m_GridSizeWorld) - offset;
	m_EndX = int((cameraPos.x + windowBorder.width) / m_GridSizeWorld) + int(offset * 1.5f);

	// y
	m_StartY = int(cameraPos.y / m_GridSizeWorld) - offset;
	m_EndY = int((cameraPos.y + windowBorder.height) / m_GridSizeWorld) + int(offset * 1.5f);

	if (testing)
	{
		// to see this in action
		m_StartX += int(offset * 1.5f);
		m_EndX -= int(offset * 1.5f);
		m_StartY += int(offset * 1.5f);
		m_EndY -= int(offset * 1.5f);
	}

	// checkers -> can go lower than 0 and higher than max tilemap Size
	// x
	if (m_StartX < 0)
	{
		m_StartX = 0;
	}
	else if (m_StartX > m_MaxWorldSize.x)
	{
		m_StartX = m_MaxWorldSize.x;
	}

	if (m_EndX < 0)
	{
		m_EndX = 0;
	}
	else if (m_EndX > m_MaxWorldSize.x)
	{
		m_EndX = m_MaxWorldSize.x;
	}

	// y
	if (m_StartY < 0)
	{
		m_StartY = 0;
	}
	else if (m_StartY > m_MaxWorldSize.y)
	{
		m_StartY = m_MaxWorldSize.y;
	}

	if (m_EndY < 0)
	{
		m_EndY = 0;
	}
	else if (m_EndY > m_MaxWorldSize.y)
	{
		m_EndY = m_MaxWorldSize.y;
	}
}

void TileMap::SetDefaultDrawArea()
{
	m_StartX = 0;
	m_EndX = m_MaxWorldSize.x;
	m_StartY = 0;
	m_EndY = m_MaxWorldSize.y;
}

// Other
void TileMap::AddTile(const Point2i& gridPos, const Rectf& srcRect)
{
	if (gridPos.x < m_MaxWorldSize.x && gridPos.x >= 0)
	{
		if (gridPos.y < m_MaxWorldSize.y && gridPos.y >= 0)
		{
			// Then I Can Add A Tile
			m_pTileMap[gridPos.x][gridPos.y].emplace_back(new Tile{ gridPos, m_GridSizeWorld, srcRect });
			std::cout << "INPUT: Added Tile\n";
			return;
		}
	}
	std::cerr << "ERROR::TILEMAP::CPP::COULD_NOT_ADD_TILE_TO_THIS_LOCATION\n";
}

void TileMap::RemoveTile(const Point2i& gridPos)
{
	if (gridPos.x < m_MaxWorldSize.x && gridPos.x >= 0)
	{
		if (gridPos.y < m_MaxWorldSize.y && gridPos.y >= 0)
		{
			if (!m_pTileMap[gridPos.x][gridPos.y].empty())
			{
				delete m_pTileMap[gridPos.x][gridPos.y][m_pTileMap[gridPos.x][gridPos.y].size() - 1];
				m_pTileMap[gridPos.x][gridPos.y].pop_back();
				std::cout << "INPUT: Removed Tile\n";
				return;
			}
		}
	}
	std::cerr << "ERROR::TILEMAP::CPP::COULD_NOT_REMOVE_TILE_ON_THIS_LOCATION\n";
}

void TileMap::SaveToFile(const std::string& pathName)
{
	std::string savePathName{};
	savePathName += "../../Resources/" + pathName;

	std::ofstream outFile{};
	outFile.open(savePathName);

	if (!outFile.is_open())
	{
		std::cerr << "ERROR::TILEMAP::CPP::COULD_NOT_SAVE_TILEMAP_FROM_FILE: " << savePathName << "\n";
	}
	else
	{
		outFile
			<< m_MaxWorldSize.x << " " << m_MaxWorldSize.y << "\n"
			<< m_GridSizeWorld << "\n"
			<< m_ParallaxScale << "\n"
			<< m_TileMapTextureKey << "\n"
			;

		for (int x{}; x < m_MaxWorldSize.x; ++x)
		{
			for (int y{}; y < m_MaxWorldSize.y; ++y)
			{
				if (!m_pTileMap.empty())	// If there are tiles in this vector
				{
					for (size_t idx{}; idx < m_pTileMap[x][y].size(); ++idx)
					{
						// strugles to make it possible to save and load this....(google is my best friend now)
						outFile << x << " " << y << " " << m_pTileMap[x][y][idx]->GetTileInfo() << "\n";
					}
				}
			}
		}
	}
	outFile.close();

	std::cout << "SAVED_TILEMAP!\n";
}

bool TileMap::LoadFromFile(const std::string& pathName)
{
	// Load all variables from a file
	std::string loadPathName{};
	loadPathName += "../../Resources/Saves/" + pathName;

	std::ifstream inFile{};
	inFile.open(loadPathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::TILEMAP::CPP::COULD_NOT_LOAD_TILEMAP_FROM_FILE: " << loadPathName << "\n";
		return false;
	}
	else
	{
		// Load EveryThing In Here!
		Point2i worldSize{};
		float gridSize{};
		float parallaxScale;
		std::string tileMapTextureKey{};
		int x{};
		int y{};
		float srcLeft{};
		float srcBottom{};

		//	//basics
		inFile >> worldSize.x >> worldSize.y >> gridSize >> parallaxScale >> tileMapTextureKey;
		m_MaxWorldSize.x = worldSize.x;
		m_MaxWorldSize.y = worldSize.y;
		m_GridSizeWorld = gridSize;
		m_ParallaxScale = parallaxScale;
		m_TileMapTextureKey = tileMapTextureKey;

		// Tiles // 

		// Clear and reinit the tilemap with right worldSize
		DestroyTileMap();

		m_pTileMap.reserve(m_MaxWorldSize.x);
		m_pTileMap.resize(m_MaxWorldSize.x, std::vector< std::vector< Tile* > >{});
		for (int x{}; x < m_MaxWorldSize.x; ++x)
		{
			m_pTileMap[y].reserve(m_MaxWorldSize.y);
			m_pTileMap[x].resize(m_MaxWorldSize.y, std::vector< Tile* >{});
		}

		// Load the tiles
		while (inFile >> x >> y >> srcLeft >> srcBottom)
		{
			m_pTileMap[x][y].emplace_back
			(
				new Tile
				{ 
					Point2i{x, y},
					m_GridSizeWorld,
					Rectf{srcLeft, srcBottom, m_GridSizeWorld / m_Scale, m_GridSizeWorld / m_Scale }
				}
			);
		}
	}
	inFile.close();

	std::cout << "LOADED_TILEMAP!\n";

	return true;
}

// Private Functions //
// Destroy
void TileMap::DestroyTileMap()
{
	for (size_t x{}; x < m_pTileMap.size(); ++x)
	{
		for (size_t y{}; y < m_pTileMap[x].size(); ++y)
		{
			for (size_t idx{}; idx < m_pTileMap[x][y].size(); ++idx)
			{
				delete m_pTileMap[x][y][idx];
			}
			m_pTileMap[x][y].clear();
		}
		m_pTileMap[x].clear();
	}
	m_pTileMap.clear();
}