#include "pch.h"
#include "GameState.h"
#include "PauseMenu.h"
#include "Level.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "PlayerHUD.h"
#include "ObjectManager.h"
#include "GemsManager.h"
#include "Settings.h"
#include "SoundManager.h"

// Constructors / Destructor
GameState::GameState(const Rectf& windowBorder, const std::string& levelKey, Settings* settings, SoundManager* soundManager, TextureManager* textureManager)
	: State{}
	, m_WindowBorder{ windowBorder }
	, m_LevelKey{ levelKey }
	, m_pSettings{ settings }
	, m_pSoundManager{ soundManager }
	, m_pTextureManager{ textureManager }
{
	std::cout << "Creating GameState...\n";
	// Best gridSize = 56 = 3.5f * 16 pxls

	InitSounds();
	InitClampRects();
	InitCamera();
	InitLevel();
	InitPlayer();
	InitGemManager();
	InitPauseMenu();
	InitObjectManager();
	InitEnemies();
	InitHUD();
}

GameState::~GameState()
{
	std::cout << "Clearing Gamestate...\n";

	DestroyLevel();
	DestroyGemManager();
	DestroyPlayer();
	DestroyPauseMenu();
	DestroyCamera();
	DestroyObjectManager();
	DestroyEnemies();
	DestroyHUD();
}

// Update / Draw
void GameState::Update(float elapsedSec)
{
	UpdateSounds();

	if (m_Paused)
	{
		m_pPauseMenu->Update(elapsedSec);
	}
	else
	{
		UpdateGame(elapsedSec);
	}
}

void GameState::Draw() const
{
	// draw game
	DrawGame();

	// draw pausemenu
	if (m_Paused)
	{
		m_pPauseMenu->Draw();
	}
}

// Inputs
void GameState::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (!m_Paused)
	{
		if (e.keysym.sym == SDLK_k || e.keysym.sym == SDLK_SPACE)	// jump
		{
			m_pPlayer->Jump();
			m_pPlayer->SetUnPressedJump(false);
		}
	}
}

void GameState::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_pPauseMenu->GetIsInPauseMenu())
	{
		if (e.keysym.sym == SDLK_ESCAPE)
		{
			m_Paused = m_Paused ? false : true;
		}
	}

	if (!m_Paused)
	{
		if (e.keysym.sym == SDLK_k || e.keysym.sym == SDLK_SPACE)
		{
			m_pPlayer->SetCanBoostJump(false);
			m_pPlayer->SetUnPressedJump(true);
		}
		if (e.keysym.sym == SDLK_s)
		{
			m_pPlayer->SetIsCrouching(false);
		}
	}

	if (m_Paused)
	{
		UpdatePauseMenuInput(e);
	}
}

void GameState::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void GameState::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void GameState::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

// Private Functions //
// Init
void GameState::InitSounds()
{
	m_pSoundManager->GetMusic("intromusic")->Play(false);
}

void GameState::InitClampRects()
{
	m_CurrentClampRectIdx = 0;

	std::string loadPathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/clampRects.morrog" };

	std::ifstream inFile{};
	inFile.open(loadPathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::GAMESTATE::CPP::COULD_NOT_LOAD_CLAMP_RECTS_FROM_FILE: " << loadPathName << "\n";
	}
	else
	{
		float x{};
		float y{};
		float width{};
		float height{};

		while (inFile >> x >> y >> width >> height)
		{
			m_CameraClampRects.emplace_back(Rectf{ x, y, width, height });
		}
	}
	inFile.close();

	std::cout << "\nLOADED_CLAMP_RECTS!\n";
}

void GameState::InitPauseMenu()
{
	m_pPauseMenu = new PauseMenu{ m_WindowBorder, m_pSettings, m_pTextureManager};

	m_pPauseMenu->AddButton(0);
	m_pPauseMenu->AddButton(1);
	m_pPauseMenu->AddButton(2);
	m_pPauseMenu->AddButton(3);
	m_pPauseMenu->AddButton(4);
	m_pPauseMenu->AddButton(5);
	m_pPauseMenu->AddButton(6);
}

void GameState::InitCamera()
{
	m_pCamera = new Camera{ Point2f{m_WindowBorder.width, m_WindowBorder.height} };

	m_pCamera->SetPosition(Point2f{ 0.f, 2128.f });

	if (!m_CameraClampRects.empty() && m_CurrentClampRectIdx < m_CameraClampRects.size())
	{
		m_pCamera->SetTileMapBounds(m_CameraClampRects[m_CurrentClampRectIdx]);
	}
}

void GameState::InitLevel()
{
	m_pLevel = new Level{ m_WindowBorder, m_LevelKey , m_pTextureManager};
}

void GameState::InitObjectManager()
{
	m_pObjectManager = new ObjectManager{ m_LevelKey, m_pGemsManager, m_pPlayer, m_pSoundManager, m_pTextureManager };
}

void GameState::InitPlayer()
{
	// normal start Point2f{ 150.f, 2400.f }
	// first ladder Point2f{ 7950.f, 2620.f }
	// up by spikes Point2f{ 8225.f, 3367.f }
	// long thing Point2f{ 11979.f, 851.f }
	// platforms Point2f{ 15189.f, 1518.f }
	// first drake Point2f{ 9700.f, 3100.f}
	// second drake Point2f{ 18038.f, 3417.f }
	m_pPlayer = new Player{ Point2f{ 150.f, 2400.f }, m_pSoundManager , m_pTextureManager };
}

void GameState::InitGemManager()
{
	m_pGemsManager = new GemsManager{ m_LevelKey , m_pSoundManager, m_pPlayer, m_pTextureManager };
}

void GameState::InitEnemies()
{
	m_pEnemies = new EnemyManager{ m_pLevel, m_LevelKey, m_pGemsManager, m_pPlayer, m_pObjectManager, m_pTextureManager };
}

void GameState::InitHUD()
{
	const int bossStartHP{ 12 };
	m_pHUD = new PlayerHUD{ m_WindowBorder, m_pPlayer->GetMaxHP(), bossStartHP , m_pTextureManager };
}

// Destroy
void GameState::DestroyPauseMenu()
{
	delete m_pPauseMenu;
	m_pPauseMenu = nullptr;
}

void GameState::DestroyCamera()
{
	delete m_pCamera;
	m_pCamera = nullptr;
}

void GameState::DestroyLevel()
{
	delete m_pLevel;
	m_pLevel = nullptr;
}

void GameState::DestroyObjectManager()
{
	delete m_pObjectManager;
	m_pObjectManager = nullptr;
}

void GameState::DestroyPlayer()
{
	delete m_pPlayer;
	m_pPlayer = nullptr;
}

void GameState::DestroyGemManager()
{
	delete m_pGemsManager;
	m_pGemsManager = nullptr;
}

void GameState::DestroyEnemies()
{
	delete m_pEnemies;
	m_pEnemies = nullptr;
}

void GameState::DestroyHUD()
{
	delete m_pHUD;
	m_pHUD = nullptr;
}

// Update
void GameState::UpdateSounds()
{
	m_pSoundManager->Update();

	if (!m_pSoundManager->GetMusic("loopmusic")->IsPlaying())
	{
		if (!m_pSoundManager->GetMusic("intromusic")->IsPlaying())
		{
			m_pSoundManager->GetMusic("intromusic")->Stop();
			m_pSoundManager->GetMusic("loopmusic")->Play(true);
		}
	}
}

void GameState::UpdateInputs(float elapsedSec)
{
	// Get keyboard state
	const Uint8* pKeyBoardState{ SDL_GetKeyboardState(nullptr) };

	// Walking
	if (pKeyBoardState[SDL_SCANCODE_A])	// going left
	{
		m_pPlayer->Move(Vector2f{ -1.f, 0.f }, elapsedSec);
	}
	if (pKeyBoardState[SDL_SCANCODE_D])	// going right
	{
		m_pPlayer->Move(Vector2f{ 1.f, 0.f }, elapsedSec);
	}
	
	// Crouching or going down on ladder
	if (pKeyBoardState[SDL_SCANCODE_S]) // crouching
	{
		m_pPlayer->Move(Vector2f{ 0.f, -1.f }, elapsedSec);
	}
	
	// Going up on ladder
	if (pKeyBoardState[SDL_SCANCODE_W])
	{
		m_pPlayer->Move(Vector2f{0.f, 1.f}, elapsedSec);
	}

	// attacking
	if (pKeyBoardState[SDL_SCANCODE_J] || pKeyBoardState[SDL_SCANCODE_L])
	{
		m_pPlayer->Attack();
	}
}

void GameState::UpdatePauseMenuInput(const SDL_KeyboardEvent& e)
{
	m_pPauseMenu->ProcessKeyUpEvent(e);

	if (m_pPauseMenu->GetIsPressed())
	{
		switch (m_pPauseMenu->GetActiveButtonID())
		{
		case 0:		// resume
			m_Paused = false;
			m_pPauseMenu->SetActiveButtonID(0);
			break;
		case 1:		// options
			m_pPauseMenu->SetActiveButtonID(0);
			m_pPauseMenu->OpenOptions();
			break;
		case 2:		// Body Swap
			std::cout << "This is still in development...\n";
			break;
		case 3:		// Feats
			std::cout << "This is still in development...\n";
			break;
		case 4:		// Return to map
			std::cout << "This is still in development...\n";
			break;
		case 5:		// Back to title menu
			m_Quit = true;
			m_pSoundManager->StopAllMusic();
			m_pSoundManager->GetMusic("menumusic")->Play(true);
			break;
		case 6:		// Quit to desktop
			m_Quit = true;
			m_pSoundManager->StopAllMusic();
			m_pSoundManager->GetMusic("menumusic")->Play(true);
			break;
		default:
			std::cerr << "ERROR::GAMESTATE::CPP::COULD_NOT_FIND_CORRECT_BUTTONID\n";
			break;
		}
		m_pPauseMenu->SetIsPressed(false);
	}
}

void GameState::UpdateClampRects()
{
	if (CheckClampRects(m_pPlayer->GetHitBox()))
	{
		if (!m_CameraClampRects.empty() && m_CurrentClampRectIdx < m_CameraClampRects.size())
		{
			m_pCamera->SetTileMapBounds(m_CameraClampRects[m_CurrentClampRectIdx]);
		}
	}
}

bool GameState::CheckClampRects(const Rectf& playerHitbox)
{
	if (!utils::IsOverlapping(playerHitbox, m_CameraClampRects[m_CurrentClampRectIdx]))
	{
		for (size_t idx{}; idx < m_CameraClampRects.size(); ++idx)
		{
			if (utils::IsOverlapping(playerHitbox, m_CameraClampRects[idx]))
			{
				m_CurrentClampRectIdx = int(idx);
				return true;
			}
		}
	}
	return false;
}

void GameState::UpdateGame(float elapsedSec)
{
	// Update the clampRect
	UpdateClampRects();

	// Update the camera
	m_pCamera->Update(m_pPlayer->GetHitBox());

	// Update Inputs
	UpdateInputs(elapsedSec);

	// Update Level (water and renderAreas)
	m_pLevel->Update(elapsedSec, m_pCamera->GetPosition());

	// Update the player
	m_pPlayer->Update(elapsedSec, m_pLevel, m_pCamera->GetCameraRect());

	// Update the enemies
	m_pEnemies->Update(elapsedSec, m_pCamera->GetCameraRect());

	// Update objects
	m_pObjectManager->Update(elapsedSec, m_pCamera->GetCameraRect());

	// Update Gems
	m_pGemsManager->Update(elapsedSec, m_pCamera->GetCameraRect(), m_pHUD, m_pLevel, m_pObjectManager);

	// Update the hud
	m_pHUD->Update(elapsedSec, m_pPlayer->GetPlayerHP(), 0);		// boss hp needs to change (first need to make boss)
}

// Draw
void GameState::DrawGame() const
{
	// Get the camera Pos
	Point2f cameraPos{ m_pCamera->GetPosition() };

	// Background
	m_pLevel->DrawBack(cameraPos);

	glPushMatrix();
	{
		//translate
		glTranslatef(-cameraPos.x, -cameraPos.y, 0);

		//CheckPoints
		m_pObjectManager->DrawCheckPoints();

		// DirtPiles
		m_pObjectManager->DrawDirtPiles();

		// Platters
		m_pObjectManager->DrawPlatters();

		// Enemies
		m_pEnemies->Draw();

		// Rocks
		m_pObjectManager->DrawRocks();

		// Player
		m_pPlayer->Draw();

		// Platforms
		m_pObjectManager->DrawPlatform();

		// Gems
		m_pGemsManager->Draw();
	}
	glPopMatrix();

	// ForeGround
	m_pLevel->DrawFront(cameraPos);

	// Bubbles
	m_pObjectManager->DrawBubbles(cameraPos);

	// HUD
	m_pHUD->Draw();
}