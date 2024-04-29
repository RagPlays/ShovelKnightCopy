#pragma once

class GemsManager;
class Player;
class ObjectManager;
class Enemy;
class Texture;
class Level;
class DozeDragon;
class TextureManager;

class EnemyManager final
{
public:
	// Constructors / Destructors
	explicit EnemyManager(Level* level, const std::string& levelKey, GemsManager* gemsManager, Player* player, ObjectManager* objectManager, TextureManager* textureManager);
	~EnemyManager();
	EnemyManager(const EnemyManager& enemyManager) = delete;
	EnemyManager& operator=(const EnemyManager& enemyManager) = delete;
	EnemyManager(EnemyManager&& enemyManager) noexcept = delete;
	EnemyManager& operator=(EnemyManager&& enemyManager) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, const Rectf& cameraRect);
	void Draw() const;

	// Getters / Setters
	float GetKnockUpVelocity() const;
	bool DidLastHitKill() const;

private:

	const std::string& m_LevelKey;

	TextureManager* m_pTextureManager;
	GemsManager* m_pGemsManager;
	ObjectManager* m_pObjectManager;
	Player* m_pPlayer;
	Level* m_pLevel;

	std::vector< Enemy* > m_pEnemies;
	std::vector< DozeDragon* > m_pMiniBosses;

	float m_CurrentKnockUpVelocity;
	bool m_IsDeadAfterHit;

private:
	// Init
	void InitVariables();
	void InitEnemies();

	// Destroy
	void DestroyEnemies();
	void DestroyMiniBosses();
	void DestroyEnemy(int idx);
	void DestroyMiniBoss(int idx);

	// Update
	void UpdateIsOnScreen(const Rectf& cameraRect);
	void UpdateEnemies(float elapsedSec);
	void UpdateMiniBosses(float elapsedSec, const Rectf& cameraRect);
	void UpdateDeletions();

	// Draw
	void DrawEnemies() const;
	void DrawMiniBosses() const;

	// Other
	bool IsOnScreen(const Rectf cameraRect, const Rectf& enemyHitbox, const float border = 100.f);
};