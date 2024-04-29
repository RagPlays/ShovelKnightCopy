#pragma once

class Texture;
class Settings;
class Button;
class AudioMenu;
class TextureManager;

class OptionsMenu final
{
public:
	// Constructors / Destructors
	explicit OptionsMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager);
	~OptionsMenu();

	OptionsMenu(const OptionsMenu& optionsMenu) = delete;
	OptionsMenu& operator=(const OptionsMenu& optionsMenu) = delete;
	OptionsMenu(OptionsMenu&& optionsMenu) noexcept = delete;
	OptionsMenu& operator=(OptionsMenu&& optionsMenu) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec);
	void Draw() const;

	// Inputs
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	// Getters / Setters
	bool GetIsPressed() const;
	void SetIsPressed(bool isPressed);
	bool GetOptionsIsOpen() const;
	void SetOptionsIsOpen(bool isOpen);
	int GetActiveID() const;
	void SetActiveID(int activeID);

	// Other
	void AddButton(int id);
	void OpenAudioSettings();

private:

	const Rectf m_WindowBorder;
	const float m_Scale;

	TextureManager* m_pTextureManager;
	Settings* m_pSettings;

	AudioMenu* m_pAudioMenu;
	
	// defaults
	int m_ActiveID;
	int m_NrOfIds;
	bool m_OptionsIsOpen;

	bool m_IsPressed; // when k is pressend on a button

	Rectf m_ButtonDestRect;

	// background
	Rectf m_DefaultOptionsDestRect;

	// buttons
	std::vector< Button* > m_pButtons;

private:

	// Init
	void InitOptionsMenu();
	void InitAudioMenu();

	// Update
	void UpdateAudioMenuInput(const SDL_KeyboardEvent& e);
};