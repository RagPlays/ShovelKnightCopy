#pragma once

class Texture;
class PlayerHUD;
class Level;
class ObjectManager;
class SoundManager;
class Player;
class TextureManager;

class GemsManager final
{
public:
	// Constructors / Destructors
	explicit GemsManager(const std::string& levelKey, SoundManager* soundManager, Player* player, TextureManager* textureManager);
	~GemsManager();

	GemsManager(const GemsManager& lootManager) = delete;
	GemsManager& operator=(const GemsManager& lootManager) = delete;
	GemsManager(GemsManager&& lootManager) noexcept = delete;
	GemsManager& operator=(GemsManager&& lootManager) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, const Rectf& cameraRect, PlayerHUD* playerHUD, Level* level, ObjectManager* objectManager);
	void Draw() const;

	// other
	void AddGems(const Rectf& target, int totalValue, const Vector2f& gemDir = Vector2f{ 0.f, 1.f }, bool timeDelete = true);

private:

	const std::string& m_LevelKey;
	const float m_Scale;
	const float m_GemsGravityForce;
	const float m_DecelerationForce;

	TextureManager* m_pTextureManager;

	Player* m_pPlayer;

	SoundManager* m_pSoundManager;

	struct Gem
	{
		Gem(int value, const Rectf& hitbox, const Rectf& srcRect, const Vector2f& velocity, bool timeDelete, bool gravity = true)
			: value{ value }
			, hitbox{ hitbox }
			, srcRect{ srcRect }
			, velocity{ velocity }
			, timeDelete{ timeDelete }
			, existTime{ 0.f }
			, isOnScreen{ false }
			, hasGravity{ gravity }
		{
		}
	 	bool isOnScreen;
		bool timeDelete;	// determines if the gem will be deleted if not picked up
		bool hasGravity;
		float existTime;
		int value;
		Vector2f velocity;
		Rectf srcRect;	// the value will determine the srcRect
		Rectf hitbox;
	};

	std::vector< Gem* > m_pGems;
	std::vector< Rectf > m_GemSrcRects;

private:

	// Init
	void InitGemSrcRects();
	void InitGemsFromFile();

	// Destroy
	void DestroyGems();

	// Update
	void UpdateIsOnScreen(const Rectf& cameraRect);
	void UpdateGems(float elapsedSec, const std::vector<std::vector<Point2f>>& vertices, const std::vector<std::vector<Point2f>>& rockVertices);
	void UpdateGemsVelocity(int idx, float elapsedSec);
	void UpdateExistTime(float elapsedSec);
	void UpdatePlayerCollision(PlayerHUD* playerHUD);
	void UpdateGemVerticesColl(const Point2f& pointTop, const Point2f& pointBot, int gem, const std::vector< std::vector< Point2f > >& vertices);

	// Other
	void AddSingleGem(const Point2f& gemPos, int value, const Vector2f& velocity, bool timeDelete, bool gravity = true);
	void DeleteGem(int idx);
	void GetCloseGemPos(Point2f& gemPos, float offset);
};

