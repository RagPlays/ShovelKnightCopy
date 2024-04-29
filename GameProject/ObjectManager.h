#pragma once

class Bubble;
class DirtPile;
class CheckPoint;
class Platform;
class Player;
class Texture;
class GemsManager;
class Rock;
class Platter;
class SoundManager;
class TextureManager;

class ObjectManager final
{
public:
	// Constructors / Destructors
	explicit ObjectManager(const std::string& levelKey, GemsManager* gemsManager, Player* player, SoundManager* soundManager, TextureManager* textureManager);
	~ObjectManager();

	ObjectManager(const ObjectManager& objectManager) = delete;
	ObjectManager& operator=(const ObjectManager& objectManager) = delete;
	ObjectManager(ObjectManager&& objectManger) noexcept = delete;
	ObjectManager& operator=(ObjectManager&& ObjectManager) noexcept = delete;

	// Update
	void Update(float elapsedSec, const Rectf& cameraRect);

	// Draw
	void DrawDirtPiles() const;
	void DrawBubbles(const Point2f& cameraPos) const;
	void DrawCheckPoints() const;
	void DrawPlatform() const;
	void DrawRocks() const;
	void DrawPlatters() const;

	// Getters
	const std::vector< std::vector< Point2f > >& GetRockVertices();

private:

	const std::string& m_LevelKey;

	TextureManager* m_pTextureManager;

	GemsManager* m_pGemsManager;

	Player* m_pPlayer;

	SoundManager* m_pSoundManager;

	std::vector< Bubble* > m_pBubbles;
	std::vector< DirtPile* > m_pDirtPiles;
	std::vector< CheckPoint* > m_pCheckPoints;
	std::vector< Platform* > m_pPlatforms;
	std::vector< Rock* > m_pRocks;
	std::vector< Platter* > m_pPlatters;

	std::vector< std::vector< Point2f > > m_RockVertices;

private:

	// Init
	void InitVariables();
	void InitObjects();
	void InitRockVertices();

	// Destroy
	void DestroyBubbles();
	void DestroyDirtPiles();
	void DestroyCheckPoints();
	void DestroyPlatforms();
	void DestroyRocks();
	void DestroyPlatters();

	// Update
	void UpdateIsOnScreen(const Rectf& cameraRect);
	bool IsOnScreen(const Rectf& cameraRect, const Rectf& objectHitBox, float border = 100.f);
	void UpdateObjects(float elapsedSec);
	void UpdateCanBeDeleted();
	void UpdateRocksVertices();
	void UpdateBubbles(float elapsedSec);
	void UpdateDirtPiles(float elapsedSec);
	void UpdateCheckPoints(float elapsedSec);
	void UpdatePlatforms(float elapsedSec);
	void UpdateRocks(float elapsedSec);
	void UpdatePlattes(float elapsedSec);

	// Other
	void AddBubble(const Point2f& startPos, const Point2f& endPos, const float speed);
	void AddDirtPile(const Point2f& dirtPilePos);
	void AddCheckPoint(const Point2f& checkPointPos);
	void AddPlatform(const Point2f& platformStartPos, const Point2f& platformEndPos, bool moveDir, const float speed);
	void AddRock(const Point2f& rockPos, bool rockType, const int breakValue);
	void AddPlatter(const Point2f& platerPos);
};