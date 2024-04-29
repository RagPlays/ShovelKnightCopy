#include "pch.h"
#include "AudioMenu.h"
#include "Button.h"
#include "Settings.h"
#include "TextureManager.h"

// Constructors / Destructors
AudioMenu::AudioMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager)
	: m_WindowBorder{ windowBorder }
	, m_pSettings{ settings }
	, m_pTextureManager{ textureManager }
	, m_Scale{ 3.5f }
	, m_AudioIsOpen{ false }
	, m_ActiveID{ 0 }
	, m_NrOfIds{ 0 }
{
	InitAudioMenu();
	InitButtons();
}

AudioMenu::~AudioMenu()
{
	DestroyButtons();
}

// Update / Draw
void AudioMenu::Update(float elapsedSec)
{
	UpdateButtons(elapsedSec);
}

void AudioMenu::Draw() const
{
	DrawDefaultMenu();
	DrawButtons();
	DrawSliders();
}

// Inputs
void AudioMenu::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_ESCAPE)
	{
		m_pSettings->SaveSettingsToFile();
		m_AudioIsOpen = false;
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
	case SDLK_d:	// volume to left
		if (m_ActiveID == 1)	// soundVolum
		{
			m_pSettings->IncreaseSoundEffectVolume();
		}
		else if (m_ActiveID == 2)	// musicVolume
		{
			m_pSettings->IncreaseMusicVolume();
		}
		break;
	case SDLK_q:
		if (m_ActiveID == 1)	// soundVolum
		{
			m_pSettings->LowerSoundEffectVolume();
		}
		else if (m_ActiveID == 2)	// musicVolume
		{
			m_pSettings->LowerMusicVolume();
		}
		break;
	default:
		break;
	}
}

// Getters / Setters
bool AudioMenu::GetIsPressed() const
{
	return m_IsPressed;
}

void AudioMenu::SetIsPressed(bool isPressed)
{
	m_IsPressed = isPressed;
}

int AudioMenu::GetActiveID() const
{
	return m_ActiveID;
}

void AudioMenu::SetActiveID(int activeID)
{
	m_ActiveID = activeID;
}

bool AudioMenu::GetAudioIsOpen() const
{
	return m_AudioIsOpen;
}

void AudioMenu::SetAudioIsOpen(bool audioIsOpen)
{
	m_AudioIsOpen = audioIsOpen;
}

void AudioMenu::SetDefaultAudio()
{
	m_pSettings->SetSoundsToDefault();
}


// Private Functions //
// Init
void AudioMenu::InitAudioMenu()
{
	
	// set pause menu dimentions
	m_DefaultAudioDest.width = m_pTextureManager->GetTexture("audioMenu")->GetWidth() * m_Scale;
	m_DefaultAudioDest.height = m_pTextureManager->GetTexture("audioMenu")->GetHeight() * m_Scale;

	// Set it in the middle..
	m_DefaultAudioDest.left = (m_WindowBorder.width - m_DefaultAudioDest.width) / 2.f;
	m_DefaultAudioDest.bottom = (m_WindowBorder.height - m_DefaultAudioDest.height) / 2.f;

	// start Pos buttonDestRect
	m_ButtonDestRect.left = m_DefaultAudioDest.left + m_DefaultAudioDest.width / 20.f;
	m_ButtonDestRect.bottom = m_DefaultAudioDest.bottom + m_DefaultAudioDest.height - ((m_pTextureManager->GetTextureVector("buttons")[0]->GetHeight() * m_Scale) * 5.5f);
}

void AudioMenu::InitButtons()
{
	// back
	AddButton(10);

	// sound Volume
	AddButton(16);

	// music Volume
	AddButton(17);

	// set to default
	AddButton(18);
}

// Destroy
void AudioMenu::DestroyButtons()
{
	// delete buttons
	for (size_t idx{}; idx < m_pButtons.size(); ++idx)
	{
		delete m_pButtons[idx];
	}
	m_pButtons.clear();
}

// Update
void AudioMenu::UpdateButtons(float elapsedSec)
{
	// update buttons
	for (size_t idx{}; idx < m_pButtons.size(); ++idx)
	{
		m_pButtons[idx]->Update(elapsedSec, m_ActiveID);
	}
}

// Draw
void AudioMenu::DrawDefaultMenu() const
{
	m_pTextureManager->GetTexture("audioMenu")->Draw(m_DefaultAudioDest);
}

void AudioMenu::DrawButtons() const
{
	// buttons
	for (size_t idx{}; idx < m_pButtons.size(); ++idx)
	{
		m_pButtons[idx]->Draw();
	}
}

void AudioMenu::DrawSliders() const
{
	Point2f drawPos{ m_DefaultAudioDest.left + m_DefaultAudioDest.width * 0.7f, 0.f};

	// sound slider
	drawPos.y = m_pButtons[1]->GetDrawRect().bottom;
	DrawASlider(drawPos, false);

	drawPos.y = m_pButtons[2]->GetDrawRect().bottom;
	DrawASlider(drawPos, true);
}

void AudioMenu::DrawASlider(const Point2f& pos, bool music) const
{
	Rectf sliderBase{ pos.x, pos.y, 200.f, 2.f * m_Scale};
	sliderBase.bottom += m_pButtons[0]->GetDrawRect().height / 2.f - (sliderBase.height / 2.f);

	utils::SetRGBColor(255, 255, 255);
	utils::FillRect(sliderBase);

	const float offset{ sliderBase.width / 20.f };
	Rectf srcRect{ 0.f, m_pTextureManager->GetTexture("slider")->GetHeight(), 3, 7};
	Rectf slider{ pos.x, pos.y, srcRect.width * m_Scale,srcRect.height * m_Scale };

	if (music)
	{
		slider.left += m_pSettings->GetMusicVolumePos() * offset;
	}
	else
	{
		slider.left += m_pSettings->GetSoundEffectVolumePos() * offset;
	}
	m_pTextureManager->GetTexture("slider")->Draw(slider, srcRect);
}

// Other
void AudioMenu::AddButton(int id)
{
	if (id < m_pTextureManager->GetTextureVector("buttons").size())
	{
		// set right dimentiions
		m_ButtonDestRect.width = m_pTextureManager->GetTextureVector("buttons")[id]->GetWidth() * m_Scale;
		m_ButtonDestRect.height = m_pTextureManager->GetTextureVector("buttons")[id]->GetHeight() * m_Scale;

		m_pButtons.emplace_back(new Button{ id, m_ButtonDestRect, m_NrOfIds , m_pTextureManager});

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