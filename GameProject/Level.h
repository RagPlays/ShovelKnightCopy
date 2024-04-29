#pragma once

class TileMap;
class Player;
class Texture;
class TextureManager;

class Level final
{
public:
	// Constructors / Destructors
	explicit Level(const Rectf& windowBorder, const std::string& levelKey, TextureManager* textureManager);
	~Level();

	Level(const Level& level) = delete;
	Level& operator=(const Level& level)  = delete;
	Level(Level&& level) noexcept = delete;
	Level& operator=(Level&& level) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, const Point2f& cameraPos);
	void DrawBack(const Point2f& cameraPos) const;
	void DrawFront(const Point2f& cameraPos) const;

	// Getters / Setters
	const std::vector< std::vector< Point2f > >& GetVertices();
	const std::vector< LadderRectf* >& GetLadderRects();
	const std::vector< Rectf >& GetDeadRects();

private:

	const Rectf m_WindowBorder;
	const std::string& m_LevelKey;

	TextureManager* m_pTextureManager;

	// Maps //
	// Tilemaps // in the right order from background to foreground
	Rectf m_CloudDestRect;						// where the background needs to be
	TileMap* m_pCastleMap;						// the castle
	TileMap* m_pLightForestMap;					// the light forest
	TileMap* m_pDarkForestOneMap;				// the first dark forest
	TileMap* m_pDarkForestTwoMap;				// the second dark forest
	std::vector < TileMap* > m_pWaterTileMaps;  // water maps (need to be updated!)
	TileMap* m_pCaveMap;						// cave
	TileMap* m_pBaseMap;						// map where you walk on
	TileMap* m_pLadderMap;						// ladders in game
	TileMap* m_pForeGroundMap;					// draw this before the player

	// water
	enum class WaterViewMode
	{
		one = 0,
		two,
		three
	};
	WaterViewMode m_WaterViewMode;
	float m_WaterViewTime;   // to update the water

	// Vertices
	std::vector< std::vector< Point2f > > m_Vertices; 

	// Ladders
	std::vector< LadderRectf* > m_pLadderRects;

	// DeadRects
	std::vector< Rectf > m_DeadRects;

private:

	// Init
	void InitVariables();
	void InitMaps();
	void LoadMaps();
	void InitVertices();
	void InitLadderRects();
	void InitDeadRects();

	// Destroy
	void DestroyMaps();
	void DestroyLadderRects();

	// Update
	void UpdateWaterView(float elapsedSec);
	void UpdateMapRenderAreas(const Point2f& cameraPos);

	// Draw
	void DrawVertices() const;
	void DrawLadderRects() const;
	void DrawDeadRects() const;
};