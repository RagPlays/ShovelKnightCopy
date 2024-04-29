#pragma once

#include "State.h"

class State;
class Texture;
class Settings;
class SoundManager;
class OptionsMenu;
class TextureManager;

class StartMenuState final : public State
{
public:
	// Constructors / Destructors
	explicit StartMenuState(std::vector<State*>& states, const Rectf& windowBorder);
	virtual ~StartMenuState() override;

	StartMenuState(const StartMenuState& startMenuState) = delete;
	StartMenuState& operator=(const StartMenuState& startMenuState) = delete;
	StartMenuState(StartMenuState&& startMenuState) noexcept = delete;
	StartMenuState& operator=(StartMenuState&& startMenuState) noexcept = delete;
	
	// Inputs
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

	// Update / Draw
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:

	const Rectf m_WindowBorder;
	const std::string m_LevelKey;

	std::vector <State*>& m_pStates;

	// TitleScreen
	float m_TitleScreenScale;
	Rectf m_ScreenTitleDest;

	// Buttons
	enum class ButtonHoverState
	{
		startGame = 1,
		feats = 2,
		options = 3,
		exit = 4
	};

	ButtonHoverState m_ButtonHoverState;
	float m_ButtonsScale;
	Rectf m_ButtonsDest;
	float m_ButtonBlickTime;
	bool m_BlackBoxButton;

	// SelectShovel
	float m_SelectShovelScale;
	Rectf m_SelectShoverDest;
	float m_SelectShovelOffsetY;
	float m_ShovelMoveTime;
	bool m_ShovelIsCloseToButton;

	// keys
	bool m_KeyIsPressed;    // bool that checks of a button has been pressed
	float m_TransitionTime;

	// TextureManager
	TextureManager* m_pTextureManager;

	// Settings
	Settings* m_pSettings;

	// SoundManagar
	SoundManager* m_pSoundManager;

	// Options
	OptionsMenu* m_pOptionsMenu;
	
private:

	// Init
	void InitVariables();
	void InitTextureManager();
	void InitSettings();
	void InitSoundManger();
	void InitTitleScreen();
	void InitButtons();
	void InitSelectShovel();
	void InitOptionsMenu();

	// Destroy
	void DestroyVariables();
	void DestroyTextureManager();
	void DestroySettings();
	void DestroySoundManager();
	void DestroyOptionsMenu();

	// Update / Draw
	void UpdateSelectShovel(float elapsedSec);
	void UpdateStateTranstion(float elapsedSec);

	void UpdateOptionsInput(const SDL_KeyboardEvent& e);

	void DrawTitleScreen() const;
	void DrawButtons() const;
	void DrawSelectShovel() const;

	// Others
	void GoNextState();
	void SetOtherButtonHover(bool up);
	void ResetMainMenu();
};