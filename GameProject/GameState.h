#pragma once

#include "State.h"

class State;
class Texture;
class GemsManager;
class PauseMenu;
class Camera;
class Level;
class ObjectManager;
class PlayerHUD;
class Player;
class EnemyManager;
class Settings;
class SoundManager;
class TextureManager;

class GameState final : public State
{
public:
	// Contructors / Destructors
	explicit GameState(const Rectf& windowBorder, const std::string& levelKey, Settings* settings, SoundManager* soundManager, TextureManager* textureManager);
	virtual ~GameState() override;

	GameState(const GameState& gameState) = delete;
	GameState& operator=(const GameState& gameState) = delete;
	GameState(GameState&& gameState) noexcept = delete;
	GameState& operator=(GameState&& gameState) noexcept = delete;

	// Update / Draw
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	 
	// Inputs
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	const Rectf m_WindowBorder;
	const std::string& m_LevelKey;

	// Camera Clamp Rects
	std::vector< Rectf > m_CameraClampRects;
	int m_CurrentClampRectIdx;

	// Textures
	TextureManager* m_pTextureManager;

	// Settings
	Settings* m_pSettings;

	// Sound
	SoundManager* m_pSoundManager;

	// Level
	Level* m_pLevel;

	// GemManager
	GemsManager* m_pGemsManager;

	// PauseMenu
	PauseMenu* m_pPauseMenu;

	//Camera
	Camera* m_pCamera;

	// Objects
	ObjectManager* m_pObjectManager;

	// Enemies
	EnemyManager* m_pEnemies;

	// Player
	Player* m_pPlayer;

	// HUD
	PlayerHUD* m_pHUD;


private:

	// Init
	void InitSounds();
	void InitClampRects();
	void InitPauseMenu();
	void InitCamera();
	void InitLevel();
	void InitObjectManager();
	void InitEnemies();
	void InitPlayer();
	void InitGemManager();
	void InitHUD();

	// Destroy
	void DestroyPauseMenu();
	void DestroyCamera();
	void DestroyLevel();
	void DestroyObjectManager();
	void DestroyEnemies();
	void DestroyPlayer();
	void DestroyGemManager();
	void DestroyHUD();

	// Update
	void UpdateSounds();
	void UpdateInputs(float elapsedSec);
	void UpdatePauseMenuInput(const SDL_KeyboardEvent& e);
	void UpdateClampRects();
	bool CheckClampRects(const Rectf& playerHitbox);
	void UpdateGame(float elapsedSec);

	// Draw
	void DrawGame() const;
};