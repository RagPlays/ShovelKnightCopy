#include "pch.h"
#include "PauseMenu.h"
#include "Button.h"
#include "OptionsMenu.h"
#include "TextureManager.h"

// Constructors / Destructors
PauseMenu::PauseMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager)
	: m_WindowBorder{ windowBorder } 
	, m_pSettings{ settings }
	, m_pTextureManager{ textureManager }
	, m_ActiveID{}
	, m_Scale{ 3.5f }
	, m_IsInPauseMenu{ false }
{
	std::cout << "\nCreating PauseMenu...\n";
	InitVariables();
	InitPausedWindow();
	InitOptionsMenu();
}

PauseMenu::~PauseMenu()
{
	std::cout << "\nDestroying PauseMenu...\n";

	DestroyButtons();

	delete m_pOptionsMenu;
	m_pOptionsMenu = nullptr;
}

// Update / Draw
void PauseMenu::Update(float elapsedSec)
{
	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		m_pOptionsMenu->Update(elapsedSec);
	}
	else
	{
		// Update the buttons (for the animation)
		for (Button* ptr : m_pButtons)
		{
			ptr->Update(elapsedSec, m_ActiveID);
		}
	}
}

void PauseMenu::Draw() const
{
	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		m_pOptionsMenu->Draw();
	}
	else
	{
		m_pTextureManager->GetTexture("pauseMenu")->Draw(m_DefaultPauseDest);

		// draw the buttons
		for (Button* ptr : m_pButtons)
		{
			ptr->Draw();
		}
	}
}

// Inputs
void PauseMenu::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_pOptionsMenu->GetOptionsIsOpen())
	{
		UpdateOptionsMenuInput(e);
	}
	else
	{
		switch (e.keysym.sym)
		{
		case SDLK_UP: // go up
			if (!m_ActiveID)
			{
				m_ActiveID = m_NrOfIds - 1;
			}
			else
			{
				--m_ActiveID;	// going up -> id goes down
			}
			break;
		case SDLK_DOWN: // Go down
			if (m_ActiveID == m_NrOfIds - 1)
			{
				m_ActiveID = 0;
			}
			else
			{
				++m_ActiveID;	// going down -> id goes up
			}
			break;
		case SDLK_z:
			if (!m_ActiveID)
			{
				m_ActiveID = m_NrOfIds - 1;
			}
			else
			{
				--m_ActiveID;	// going up -> id goes down
			}
			break;
		case SDLK_s:
			if (m_ActiveID == m_NrOfIds - 1)
			{
				m_ActiveID = 0;
			}
			else
			{
				++m_ActiveID;	// going down -> id goes up
			}
			break;
		case SDLK_k: // confirm
			m_IsPressed = true;
			break;
		default:
			break;
		}
	}
}

// Getters / Setters
int PauseMenu::GetActiveButtonID() const
{
	return m_ActiveID;
}

void PauseMenu::SetActiveButtonID(int activeID)
{
	m_ActiveID = activeID;
}

bool PauseMenu::GetIsPressed() const
{
	return m_IsPressed;
}

void PauseMenu::SetIsPressed(bool isPressed)
{
	m_IsPressed = isPressed;
}

bool PauseMenu::GetIsInPauseMenu() const
{
	return !(m_pOptionsMenu->GetOptionsIsOpen() /* || other menu is open*/);
}

// Other
void PauseMenu::AddButton(int id)
{
	if (id < m_pTextureManager->GetTextureVector("buttons").size())
	{
		// set right dimentiions
		m_ButtonDestRect.width = m_pTextureManager->GetTextureVector("buttons")[id]->GetWidth() * m_Scale;
		m_ButtonDestRect.height = m_pTextureManager->GetTextureVector("buttons")[id]->GetHeight() * m_Scale;

		m_pButtons.emplace_back(new Button{ id, m_ButtonDestRect, m_NrOfIds, m_pTextureManager });

		++m_NrOfIds;

		// Set Position for next button
		const float offset{ m_ButtonDestRect.height * 2.4f };
		m_ButtonDestRect.bottom -= offset;
	}
	else
	{
		std::cout << "Cant add a button with addID: " << id << "\n";
	}
}

void PauseMenu::OpenOptions()
{
	m_pOptionsMenu->SetOptionsIsOpen(true);
}

// Private Functions //
// Init
void PauseMenu::InitVariables()
{
	// setting defaults
	m_IsPressed = false;
	m_ActiveID = 0;
	m_NrOfIds = 0;
	m_pButtons.clear();
}

void PauseMenu::InitPausedWindow()
{
	// set pause menu dimentions
	m_DefaultPauseDest.width = m_pTextureManager->GetTexture("pauseMenu")->GetWidth() * m_Scale;
	m_DefaultPauseDest.height = m_pTextureManager->GetTexture("pauseMenu")->GetHeight() * m_Scale;

	// Set it in the middle..
	m_DefaultPauseDest.left = (m_WindowBorder.width - m_DefaultPauseDest.width) / 2.f;
	m_DefaultPauseDest.bottom = (m_WindowBorder.height - m_DefaultPauseDest.height) / 2.f;


	// start Pos buttonDestRect
	m_ButtonDestRect.left = m_DefaultPauseDest.left + m_DefaultPauseDest.width / 20.f;
	m_ButtonDestRect.bottom = m_DefaultPauseDest.bottom + m_DefaultPauseDest.height - ((m_pTextureManager->GetTextureVector("buttons")[0]->GetHeight() * m_Scale) * 5.5f);
}

void PauseMenu::InitOptionsMenu()
{
	m_pOptionsMenu = new OptionsMenu{ m_WindowBorder, m_pSettings, m_pTextureManager };

	m_pOptionsMenu->AddButton(10);
	m_pOptionsMenu->AddButton(11);
	m_pOptionsMenu->AddButton(12);
	m_pOptionsMenu->AddButton(13);
	m_pOptionsMenu->AddButton(14);
	m_pOptionsMenu->AddButton(15);
}

// Destroy
void PauseMenu::DestroyButtons()
{
	// Clear the buttons
	for (Button* ptr : m_pButtons)
	{
		delete ptr;
	}
	m_pButtons.clear();
}

// Update
void PauseMenu::UpdateOptionsMenuInput(const SDL_KeyboardEvent& e)
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
		case 2:		// CONTROLS
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
			std::cerr << "ERROR::PAUSEMENU::CPP::COULD_NOT_FIND_CORRECT_BUTTONID\n";
			break;
		}
		m_pOptionsMenu->SetIsPressed(false);
	}
}