#pragma once
#include "BaseGame.h"

class State;

class Game final: public BaseGame
{
public:
	explicit Game(const Window& window);
	virtual ~Game() override;
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) noexcept = delete;
	Game& operator=(Game&& other) noexcept = delete;

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

	std::vector<State*> m_pStates;

private:

	// FrameWork
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	// Update 
	void UpdateStates(float elapsedSec);

	// Draw
	void DrawStates() const;

	// Other
	void ShowKeyBordInfo();
};