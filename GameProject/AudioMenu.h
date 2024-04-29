#pragma once

class Texture;
class Settings;
class Button;
class TextureManager;

class AudioMenu final
{
public:
	// Constructors / Destructors
	explicit AudioMenu(const Rectf& windowBorder, Settings* settings, TextureManager* textureManager);
	~AudioMenu();

	AudioMenu(const AudioMenu& audioMenu) = delete;
	AudioMenu& operator=(const AudioMenu& audioMenu) = delete;
	AudioMenu(AudioMenu&& audioMenu) noexcept = delete;
	AudioMenu& operator=(AudioMenu&& audioMenu) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec);
	void Draw() const;

	// Inputs
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	// Getters / Setters
	bool GetIsPressed() const;
	void SetIsPressed(bool isPressed);
	int GetActiveID() const;
	void SetActiveID(int activeID);
	bool GetAudioIsOpen() const;
	void SetAudioIsOpen(bool audioIsOpen);
	void SetDefaultAudio();

private:

	const Rectf m_WindowBorder;
	const float m_Scale;

	TextureManager* m_pTextureManager;

	bool m_AudioIsOpen;

	int m_ActiveID;
	int m_NrOfIds;

	bool m_IsPressed;

	Settings* m_pSettings;

	Rectf m_ButtonDestRect;

	Rectf m_DefaultAudioDest;

	std::vector< Button* > m_pButtons;

private:

	// Init
	void InitAudioMenu(); 
	void InitButtons();

	// Destroy
	void DestroyButtons();

	// Update
	void UpdateButtons(float elapsedSec);

	// Draw
	void DrawDefaultMenu() const;
	void DrawButtons() const;
	void DrawSliders() const;
	void DrawASlider(const Point2f& pos, bool music) const;

	// Other
	void AddButton(int id);
};

