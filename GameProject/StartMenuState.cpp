#include "pch.h"
#include "StartMenuState.h"
#include "EditorState.h"
#include "GameState.h"
#include "Settings.h"
#include "SoundManager.h"
#include "OptionsMenu.h"
#include "TextureManager.h"

// Constructors / Destructors
StartMenuState::StartMenuState(std::vector<State*>& states, const Rectf& windowBorder)
	: State{}
	, m_pStates{states}
	, m_WindowBorder{windowBorder}
	, m_TitleScreenScale{ 3.5f }
	, m_ButtonsScale{ 3.5f }
	, m_SelectShovelScale{3.5f}
	, m_ButtonHoverState{ ButtonHoverState::startGame }
	, m_LevelKey{ "Level1" }
{
	std::cout << "Creating StartMenu...\n";

	InitSettings();
	InitTextureManager();
	InitSoundManger();
	InitOptionsMenu();
	InitVariables();
	InitTitleScreen();
	InitButtons();
	InitSelectShovel();
}

StartMenuState::~StartMenuState()
{
	std::cout << "Clearing StartMenu...\n";

	DestroyVariables();
	DestroyTextureManager();
	DestroySettings();
	DestroySoundManager();
	DestroyOptionsMenu();
}

// Inputs
void StartMenuState::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
}

void StartMenuState::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		UpdateOptionsInput(e);
		return;
	}
	if (m_KeyIsPressed)
	{
		return;
	}

	switch (e.keysym.sym)
	{
	case SDLK_ESCAPE:
		EndState();
		break;
	case SDLK_UP:
		SetOtherButtonHover(1); // ButtonHover goes up by one
		break;
	case SDLK_z:
		SetOtherButtonHover(1); // ButtonHover goes up by one
		break;
		break;
	case SDLK_DOWN:
		SetOtherButtonHover(0); // ButtonHover goes down by one
		break;
	case SDLK_s:
		SetOtherButtonHover(0); // ButtonHover goes down by one
		break;
	case SDLK_k:
		m_KeyIsPressed = true;
		break;
	case SDLK_0:
		m_pSoundManager->StopAllMusic();
		m_pStates.emplace_back(new EditorState{ m_WindowBorder, m_LevelKey, m_pSettings, m_pTextureManager });
		break;
	default:
		break;
	}

}

void StartMenuState::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{

}

void StartMenuState::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void StartMenuState::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}

// Update / Draw
void StartMenuState::Update(float elapsedSec)
{
	m_pSoundManager->Update();

	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		m_pOptionsMenu->Update(elapsedSec);
	}
	else
	{
		if (!m_KeyIsPressed)
		{
			UpdateSelectShovel(elapsedSec);
		}
		if (m_KeyIsPressed)
		{
			UpdateStateTranstion(elapsedSec);
		}
	}
}

void StartMenuState::Draw() const
{
	DrawTitleScreen();
	DrawButtons();
	DrawSelectShovel();

	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		m_pOptionsMenu->Draw();
	}
}


// Private Functions //
// Init
void StartMenuState::InitVariables()
{
	// for general variables out of the constructor initializer list
	m_KeyIsPressed = false;
	m_BlackBoxButton = false;
	m_TransitionTime = 0.f;
}

void StartMenuState::InitTextureManager()
{
	m_pTextureManager = new TextureManager{ "../../Resources/Saves/" + m_LevelKey + "/Other/textureConfig.morrog" };
}

void StartMenuState::InitTitleScreen()
{
	// Setting up the whole titleScreen Dest Rect
	// 
	// Setting the right dimentions
	m_ScreenTitleDest.width = m_pTextureManager->GetTexture("titleScreen")->GetWidth() * m_TitleScreenScale;
	m_ScreenTitleDest.height = m_pTextureManager->GetTexture("titleScreen")->GetHeight() * m_TitleScreenScale;

	// Setting the right positions
	m_ScreenTitleDest.left = (m_WindowBorder.width - m_ScreenTitleDest.width) / 2.f;
	m_ScreenTitleDest.bottom = (m_WindowBorder.height - m_ScreenTitleDest.height) / 1.02f;
}

void StartMenuState::InitButtons()
{
	// Setting up the buttons Dest Rect
	// 
	// Setting the right dimentions
	m_ButtonsDest.width = m_pTextureManager->GetTexture("menuButtons")->GetWidth() * m_ButtonsScale;
	m_ButtonsDest.height = m_pTextureManager->GetTexture("menuButtons")->GetHeight() * m_ButtonsScale;

	// Setting the right positions
	m_ButtonsDest.left = (m_WindowBorder.width - m_ButtonsDest.width) / 2.f;
	m_ButtonsDest.bottom = m_WindowBorder.height / 5.2f;

	// Other
	m_ButtonBlickTime = 0.f;
}

void StartMenuState::InitSelectShovel()
{
	// Stetting up the selectShovel Dest Rect
	//
	// Setting the right offset
	m_SelectShovelOffsetY = m_ButtonsDest.height / 3.5f;

	// Setting the right dimensions
	m_SelectShoverDest.width = m_pTextureManager->GetTexture("selectShovel")->GetWidth() * m_SelectShovelScale;
	m_SelectShoverDest.height = m_pTextureManager->GetTexture("selectShovel")->GetHeight() * m_SelectShovelScale;

	// Setting the right positions
	m_SelectShoverDest.left = m_ButtonsDest.left - m_SelectShoverDest.width * 1.175f;
	m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height;

	// Other
	m_ShovelIsCloseToButton = true;
	m_ShovelMoveTime = 0.f;
}

void StartMenuState::InitSettings()
{
	m_pSettings = new Settings{ "../../Resources/Saves/Other/settings.morrog" };
}

void StartMenuState::InitSoundManger()
{
	const std::string path{ "../../Resources/Saves/" + m_LevelKey + "/Other/soundConfig.morrog" };
	m_pSoundManager = new SoundManager{ m_pSettings, path };

	m_pSoundManager->GetMusic("menumusic")->Play(true);
}

void StartMenuState::InitOptionsMenu()
{
	m_pOptionsMenu = new OptionsMenu{ m_WindowBorder, m_pSettings, m_pTextureManager };

	// add buttons
	m_pOptionsMenu->AddButton(10);
	m_pOptionsMenu->AddButton(11);
	m_pOptionsMenu->AddButton(12);
	m_pOptionsMenu->AddButton(13);
	m_pOptionsMenu->AddButton(14);
	m_pOptionsMenu->AddButton(15);
	m_pOptionsMenu->AddButton(16);
}

// Destroy
void StartMenuState::DestroyVariables()
{
	// To clear general variables
}

void StartMenuState::DestroyTextureManager()
{
	delete m_pTextureManager;
	m_pTextureManager = nullptr;
}

void StartMenuState::DestroySettings()
{
	delete m_pSettings;
	m_pSettings = nullptr;
}

void StartMenuState::DestroySoundManager()
{
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}

void StartMenuState::DestroyOptionsMenu()
{
	delete m_pOptionsMenu;
	m_pOptionsMenu = nullptr;
}

// Update
void StartMenuState::UpdateSelectShovel(float elapsedSec)
{
	const float maxSec{ 0.3f };  // is the time it take to go for left to right and the other way around

	// Update Select Shovel Animation
	if (m_ShovelIsCloseToButton)
	{
		if (utils::GetClockTime(m_ShovelMoveTime, elapsedSec, maxSec))     // Wait for (maxSec) long before entering the if statement
		{
			m_SelectShoverDest.left -= m_SelectShoverDest.width / 2.5f;  // Move Shovel to the left
			m_ShovelIsCloseToButton = false;							 // Letting it know that it moved
		}
	}
	else
	{
		if (utils::GetClockTime(m_ShovelMoveTime, elapsedSec, maxSec))     // Wait for (maxSec) long before entering the if statement
		{
			m_SelectShoverDest.left += m_SelectShoverDest.width / 2.5f;  // Move Shovel to the right
			m_ShovelIsCloseToButton = true;								 // Letting it know that it moved
		}
	}

	// Update Position
	switch (m_ButtonHoverState)
	{
	case StartMenuState::ButtonHoverState::startGame:
		m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height;
		break;
	case StartMenuState::ButtonHoverState::feats:
		m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height - m_SelectShovelOffsetY;
		break;
	case StartMenuState::ButtonHoverState::options:
		m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height - m_SelectShovelOffsetY * 2.f;
		break;
	case StartMenuState::ButtonHoverState::exit:
		m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height - m_SelectShovelOffsetY * 3.f;
		break;
	default:
		break;
	}
}

void StartMenuState::UpdateStateTranstion(float elapsedSec)
{
	// check if legal press
	if (m_ButtonHoverState == ButtonHoverState::feats)
	{
		m_KeyIsPressed = false;
		std::cout << "This is still being developed...Forever...\n";
		return;
	}
	else if (m_ButtonHoverState == ButtonHoverState::options)
	{
		m_KeyIsPressed = false;
		m_pOptionsMenu->SetOptionsIsOpen(true);
		return;
	}

	// Change blinking state
	if (utils::GetClockTime(m_ButtonBlickTime, elapsedSec, 0.075f))
	{
		if (m_BlackBoxButton)
		{
			m_BlackBoxButton = false;
		}
		else
		{
			m_BlackBoxButton = true;
		}
	}

	const float shovelSpeed{ 900.f };
	m_SelectShoverDest.left += shovelSpeed * elapsedSec;

	if (utils::GetClockTime(m_TransitionTime, elapsedSec, 1.5f))
	{
		GoNextState();
	}
}

void StartMenuState::UpdateOptionsInput(const SDL_KeyboardEvent& e)
{
	m_pOptionsMenu->ProcessKeyUpEvent(e);

	if (m_pOptionsMenu->GetIsPressed())
	{
		switch (m_pOptionsMenu->GetActiveID())
		{
		case 0:		// BACK
			m_pOptionsMenu->SetActiveID(0);
			m_pOptionsMenu->SetOptionsIsOpen(false);
			break;
		case 1:		// GAME
			std::cout << "This is still in development...\n";
			break;
		case 2:		// CONTROLES
			std::cout << "This is still in development...\n";
			break;
		case 3:		// AUDIO
			m_pOptionsMenu->SetActiveID(0);
			m_pOptionsMenu->OpenAudioSettings();
			break;
		case 4:		// VIDEO
			std::cout << "This is still in development...\n";
			break;
		case 5:		// ACCESSIBILITY
			std::cout << "This is still in development...\n";
			break;
		case 6:		// DELETE DATA
			std::cout << "This is still in development...\n";
			break;
		default:
			break;
		}

		m_pOptionsMenu->SetIsPressed(false);
	}
}

// Draw
void StartMenuState::DrawTitleScreen() const
{
	m_pTextureManager->GetTexture("titleScreen")->Draw(m_ScreenTitleDest);
}

void StartMenuState::DrawButtons() const
{
	m_pTextureManager->GetTexture("menuButtons")->Draw(m_ButtonsDest);

	if (m_BlackBoxButton)
	{
		//float bottom{ m_ButtonsDest.bottom + m_ButtonsDest.height - m_ButtonsDest.height / 3.f - m_SelectShovelOffsetY * int(m_ButtonHoverState) };
		Rectf drawRect{m_ButtonsDest.left, m_SelectShoverDest.bottom - m_ButtonsDest.height / 20.f, m_ButtonsDest.width * 1.f, m_ButtonsDest.height * 0.25f };
		utils::SetRGBColor(0, 0, 0);
		utils::FillRect(drawRect);
	}
}

void StartMenuState::DrawSelectShovel() const
{
	m_pTextureManager->GetTexture("selectShovel")->Draw(m_SelectShoverDest);
}

// Other
void StartMenuState::GoNextState()
{
	switch (m_ButtonHoverState)
	{
	case StartMenuState::ButtonHoverState::startGame:
		m_pSoundManager->StopAllMusic();
		m_pStates.emplace_back(new GameState{ m_WindowBorder, m_LevelKey, m_pSettings, m_pSoundManager, m_pTextureManager});
		break;
	case StartMenuState::ButtonHoverState::feats:
		std::cout << "This Can't be right!\n";
		break;
	case StartMenuState::ButtonHoverState::options:
		std::cout << "This Can't be right!\n";
		break;
	case StartMenuState::ButtonHoverState::exit:
		EndState();
		break;
	default:
		break;
	}

	ResetMainMenu();
}

void StartMenuState::SetOtherButtonHover(bool up)
{
	if (up)
	{
		if (m_ButtonHoverState == ButtonHoverState::startGame)
		{
			m_ButtonHoverState = ButtonHoverState::exit;
		}
		else
		{
			m_ButtonHoverState = ButtonHoverState(int(m_ButtonHoverState) - 1);
		}
	}
	else
	{
		if (m_ButtonHoverState == ButtonHoverState::exit)
		{
			m_ButtonHoverState = ButtonHoverState::startGame;
		}
		else
		{
			m_ButtonHoverState = ButtonHoverState(int(m_ButtonHoverState) + 1);
		}
	}
}

void StartMenuState::ResetMainMenu()
{
	m_ButtonHoverState = ButtonHoverState::startGame;
	m_KeyIsPressed = false;
	m_BlackBoxButton = false;
	m_TransitionTime = 0.f;
	m_ShovelIsCloseToButton = true;
	m_ShovelMoveTime = 0.f;
	m_SelectShoverDest.left = m_ButtonsDest.left - m_SelectShoverDest.width * 1.175f;
	m_SelectShoverDest.bottom = m_ButtonsDest.bottom + m_ButtonsDest.height - m_SelectShoverDest.height;
}