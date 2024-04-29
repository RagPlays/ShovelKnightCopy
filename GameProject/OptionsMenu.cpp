#include "pch.h"
#include "OptionsMenu.h"
#include "Button.h"
#include "AudioMenu.h"
#include "Settings.h"
#include "TextureManager.h"

// Constructors / Destructors
OptionsMenu::OptionsMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager)
	: m_OptionsIsOpen{ false }
	, m_WindowBorder{ windowBorder }
	, m_pSettings{ settings }
	, m_ActiveID{ 0 }
	, m_NrOfIds{ 0 }
	, m_Scale{ 3.5f }
	, m_IsPressed{ false }
	, m_pTextureManager{ textureManager }
{
	InitOptionsMenu();
	InitAudioMenu();
}

OptionsMenu::~OptionsMenu()
{
	for (size_t idx{}; idx < m_pButtons.size(); ++idx)
	{
		delete m_pButtons[idx];
	}
	m_pButtons.clear();

	delete m_pAudioMenu;
	m_pAudioMenu = nullptr;
}

// Update / Draw
void OptionsMenu::Update(float elapsedSec)
{
	if (!m_OptionsIsOpen)
	{
		return;
	}

	if (m_pAudioMenu->GetAudioIsOpen())
	{
		m_pAudioMenu->Update(elapsedSec);
	}
	else
	{
		for (size_t idx{}; idx < m_pButtons.size(); ++idx)
		{
			m_pButtons[idx]->Update(elapsedSec, m_ActiveID);
		}
	}
}

void OptionsMenu::Draw() const
{
	if (!m_OptionsIsOpen)
	{
		return;
	}

	if (m_pAudioMenu->GetAudioIsOpen())
	{
		m_pAudioMenu->Draw();
	}
	else
	{
		m_pTextureManager->GetTexture("optionsScreen")->Draw(m_DefaultOptionsDestRect);

		// draw buttons
		// draw the buttons
		for (size_t idx{}; idx < m_pButtons.size(); ++idx)
		{
			m_pButtons[idx]->Draw();
		}
	}
}

// Inputs
void OptionsMenu::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_pAudioMenu->GetAudioIsOpen())
	{
		UpdateAudioMenuInput(e);
		return;
	}

	if (e.keysym.sym == SDLK_ESCAPE)
	{
		m_OptionsIsOpen = false;
		m_ActiveID = 0;
	}

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

// Getters / Setters
bool OptionsMenu::GetIsPressed() const
{
	return m_IsPressed;
}

void OptionsMenu::SetIsPressed(bool isPressed)
{
	m_IsPressed = isPressed;
}

bool OptionsMenu::GetOptionsIsOpen() const
{
	return m_OptionsIsOpen;
}

void OptionsMenu::SetOptionsIsOpen(bool isOpen)
{
	m_OptionsIsOpen = isOpen;
}

int OptionsMenu::GetActiveID() const
{
	return m_ActiveID;
}

void OptionsMenu::SetActiveID(int activeID)
{
	m_ActiveID = activeID;
}

// Other
void OptionsMenu::AddButton(int id)
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

void OptionsMenu::OpenAudioSettings()
{
	m_pAudioMenu->SetAudioIsOpen(true);
}

// Private Functions //
// Init
void OptionsMenu::InitOptionsMenu()
{
	// set pause menu dimentions
	m_DefaultOptionsDestRect.width = m_pTextureManager->GetTexture("optionsScreen")->GetWidth() * m_Scale;
	m_DefaultOptionsDestRect.height = m_pTextureManager->GetTexture("optionsScreen")->GetHeight() * m_Scale;

	// Set it in the middle..
	m_DefaultOptionsDestRect.left = (m_WindowBorder.width - m_DefaultOptionsDestRect.width) / 2.f;
	m_DefaultOptionsDestRect.bottom = (m_WindowBorder.height - m_DefaultOptionsDestRect.height) / 2.f;

	// start Pos buttonDestRect
	m_ButtonDestRect.left = m_DefaultOptionsDestRect.left + m_DefaultOptionsDestRect.width / 20.f;
	m_ButtonDestRect.bottom = m_DefaultOptionsDestRect.bottom + m_DefaultOptionsDestRect.height - ((m_pTextureManager->GetTextureVector("buttons")[0]->GetHeight() * m_Scale) * 5.5f);
}

void OptionsMenu::InitAudioMenu()
{
	m_pAudioMenu = new AudioMenu{ m_WindowBorder, m_pSettings, m_pTextureManager };
}

// Update
void OptionsMenu::UpdateAudioMenuInput(const SDL_KeyboardEvent& e)
{
	m_pAudioMenu->ProcessKeyUpEvent(e);

	if (m_pAudioMenu->GetIsPressed())
	{
		switch (m_pAudioMenu->GetActiveID())
		{
		case 0:		// back
			m_pSettings->SaveSettingsToFile();
			m_pAudioMenu->SetActiveID(0);
			m_pAudioMenu->SetAudioIsOpen(false);
			break;
		case 1:		// Sounds
			std::cout << "There cant be clicked on this!\n";
			break;
		case 2:		// music
			std::cout << "There cant be clicked on this!\n";
			break;
		case 3:		// default sounds
			m_pAudioMenu->SetDefaultAudio();
			break;
		default:
			break;
		}

		m_pAudioMenu->SetIsPressed(false);
	}
	
}