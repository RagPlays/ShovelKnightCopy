#pragma once

class Button;
class Texture;
class OptionsMenu;
class Settings;
class TextureManager;

class PauseMenu final
{
public:
	// Constructors / Destructors
	explicit PauseMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager);
	~PauseMenu();

	PauseMenu(const PauseMenu& pauseMenu) = delete;
	PauseMenu& operator=(const PauseMenu& pauseMenu) = delete;
	PauseMenu(PauseMenu&& pauseMenu) noexcept = delete;
	PauseMenu& operator=(PauseMenu&& pauseMenu) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec);
	void Draw() const;

	// Inputs
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	// Getters / Setters
	int GetActiveButtonID() const;
	void SetActiveButtonID(int activeID);
	bool GetIsPressed() const;
	void SetIsPressed(bool isPressed);
	bool GetIsInPauseMenu() const;

	// Other
	void AddButton(int id);
	void OpenOptions();

private:

	const Rectf m_WindowBorder;
	const float m_Scale;

	Settings* m_pSettings;

	TextureManager* m_pTextureManager;

	OptionsMenu* m_pOptionsMenu;
	
	std::vector< Button* > m_pButtons;

	Rectf m_DefaultPauseDest;

	Rectf m_ButtonDestRect;

	int m_ActiveID;
	int m_NrOfIds;
	bool m_IsInPauseMenu;

	bool m_IsPressed;

private:

	// Init
	void InitVariables();
	void InitPausedWindow();
	void InitOptionsMenu();

	// Destroy
	void DestroyButtons();

	// Update
	void UpdateOptionsMenuInput(const SDL_KeyboardEvent& e);
};